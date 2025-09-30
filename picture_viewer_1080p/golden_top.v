// ============================================================================
// Copyright (c) 2025 by Terasic Technologies Inc.
// ============================================================================
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development
//   Kits made by Terasic.  Other use of this code, including the selling
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use
//   or functionality of this code.
//
// ============================================================================
//
//  Terasic Technologies Inc
//  No.80, Fenggong Rd., Hukou Township, Hsinchu County 303035. Taiwan
//
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// ============================================================================
//Date:  Mon May 12 17:38:06 2025
// ============================================================================

//`define ENABLE_CAM

module golden_top(

      ///////// CLOCK /////////
      input              CLOCK0_50,
      input              CLOCK1_50,
      input              CLOCK2_50,
      input              CLOCK3_50,

      ///////// KEY /////////
      input    [ 1: 0]   KEY, //KEY is Low-Active

      ///////// FPGA /////////
      output             FPGA_UART_TX,
      input              FPGA_UART_RX,

      ///////// SW /////////
      input    [ 1: 0]   SW,

      ///////// LED /////////
      output   [ 3: 0]   LED, //LED is Low-Active

      ///////// SDRAM /////////
      output             DRAM_CLK,
      output             DRAM_CKE,
      output   [12: 0]   DRAM_ADDR,
      output   [ 1: 0]   DRAM_BA,
      inout    [31: 0]   DRAM_DQ,
      output             DRAM_CS_n,
      output             DRAM_WE_n,
      output             DRAM_CAS_n,
      output             DRAM_RAS_n,
      output   [ 3: 0]   DRAM_DQM,

      ///////// SD /////////
      output             SD_CLK,
      inout    [ 3: 0]   SD_DATA,
      inout              SD_CMD,

      ///////// HDMI /////////
      inout              HDMI_I2C_SCL,
      inout              HDMI_I2C_SDA,
      output             HDMI_TX_HS,
      output             HDMI_TX_VS,
      output   [23: 0]   HDMI_TX_D,
      output             HDMI_TX_DE,
      output             HDMI_TX_CLK_p,
      output             HDMI_ISEL,
      output             HDMI_PD_n,
      inout              DDC_I2C_SCL,
      inout              DDC_I2C_SDA,

      ///////// NET /////////
      output             NET_TX_CLK,
      output   [ 3: 0]   NET_TX_DATA,
      output             NET_TX_CTRL,
      input              NET_RX_CLK,
      input    [ 3: 0]   NET_RX_DATA,
      input              NET_RX_CTRL,
      output             NET_MDC,
      inout              NET_MDIO,
      output             NET_RESET_n,

`ifdef ENABLE_CAM
      ///////// CAM /////////
      input              CAM_CLK_p,
      input              CAM_CLK_n,
      input    [ 3: 0]   CAM_D_p,
      input    [ 3: 0]   CAM_D_n,
      inout              CAM_I2C_SCL,
      inout              CAM_I2C_SDA,
      inout              CAM_GPIO,
      input              CAM_RZQ1,
`endif /*ENABLE_CAM*/

      ///////// GPIO /////////
      inout    [35: 0]   GPIO_D,

      ///////// TMD0 /////////
      inout    [ 7: 0]   TMD0_D,

      ///////// TMD1 /////////
      inout    [ 7: 0]   TMD1_D

);


//=======================================================
//  REG/WIRE declarations
//=======================================================


wire    [ 3: 0]   led;

wire    [31: 0]   dram_data_out;
wire    [31: 0]   dram_data_in;
wire              dram_data_oe;

wire clk_hdmi_tx;



wire [23: 0] disp_data;
wire disp_clk;
wire disp_hs;
wire disp_vs;
wire disp_de;

//=======================================================
//  Structural coding
//=======================================================
assign reset_n = !ninit_done & KEY[0];
wire ninit_done;
agilex_reset_release reset_release_Inst (
	.ninit_done(ninit_done)
	);
	
assign HDMI_ISEL = reset_n; // I2C mode. If ISEL is brought low and then back high, the I2C state machine is reset.
assign HDMI_PD_n = 1'b0; // In I2C mode, pin should be tied to GND
	 
//HDMI I2C
I2C_HDMI_Config u_I2C_HDMI_Config (
	.iCLK(CLOCK0_50),
	.iRST_N(reset_n),
	.I2C_SCLK(HDMI_I2C_SCL),
	.I2C_SDAT(HDMI_I2C_SDA)
	 );
	 


	 
assign DRAM_DQ = dram_data_oe ? dram_data_out : 32'hZZZZ;
assign dram_data_in = DRAM_DQ;
//assign LED = ~led;

    nios_system nios_system_Inst (
         .clk_clk                                    (CLOCK2_50),                                    //   input,   width = 1,                        clk.clk
			
			// sdcard
        .terasic_sdcard_conduit_end_sd_clk                  (SD_CLK),                  //  output,   width = 1,                sdcard_conduit_end.sd_clk
        .terasic_sdcard_conduit_end_sd_cmd                  (SD_CMD),                  //   inout,   width = 1,                                  .sd_cmd
        .terasic_sdcard_conduit_end_sd_data                 (SD_DATA),                 //   inout,   width = 4,                                  .sd_data
			
			// vvp frame reader
         .intel_vvp_cvo_vid_status_vid_locked        (),        //  output,   width = 1,   intel_vvp_cvo_vid_status.vid_locked
         .intel_vvp_cvo_vid_status_vid_is_full       (),       //  output,   width = 1,                           .vid_is_full
         .intel_vvp_cvo_vid_status_vid_stall_error   (),   //  output,   width = 1,                           .vid_stall_error
         .intel_vvp_cvo_vid_status_vid_size_mismatch (), //  output,   width = 1,                           .vid_size_mismatch
         .intel_vvp_cvo_frame_start_conduit          (1'b1),          //   input,   width = 1,  intel_vvp_cvo_frame_start.conduit
         .intel_vvp_fr2cv_cv_clk_out_clk             (),             //  output,   width = 1, intel_vvp_fr2cv_cv_clk_out.clk
         .intel_vvp_fr2cv_cv_vid_out_vid_data        (disp_data),        //  output,  width = 24, intel_vvp_fr2cv_cv_vid_out.vid_data
         .intel_vvp_fr2cv_cv_vid_out_underflow       (),       //  output,   width = 1,                           .underflow
         .intel_vvp_fr2cv_cv_vid_out_vid_datavalid   (disp_de),   //  output,   width = 1,                           .vid_datavalid
         .intel_vvp_fr2cv_cv_vid_out_vid_v_sync      (disp_vs),      //  output,   width = 1,                           .vid_v_sync
         .intel_vvp_fr2cv_cv_vid_out_vid_h_sync      (disp_hs),      //  output,   width = 1,                           .vid_h_sync
         .intel_vvp_fr2cv_cv_vid_out_vid_f           (),           //  output,   width = 1,                           .vid_f
         .intel_vvp_fr2cv_cv_vid_out_vid_h           (),           //  output,   width = 1,                           .vid_h
         .intel_vvp_fr2cv_cv_vid_out_vid_v           (),           //  output,   width = 1,                           .vid_v
         .intel_vvp_vfr_fsync_in_read                (vfr_fsync),                //   input,   width = 1,     intel_vvp_vfr_fsync_in.read
//         .hdmi_tx_clk_clk                       (clk_hdmi_tx),                       //  output,   width = 1,           hdmi_tx_clk_640p.clk
			.iopll_vga_outclk1_clk (clk_hdmi_tx),
			
			.key_external_connection_export             (KEY),             //   input,   width = 2,    key_external_connection.export
         .led_external_connection_export             (led),             //  output,   width = 4,    led_external_connection.export
         .sw_external_connection_export              (SW),              //   input,   width = 2,     sw_external_connection.export
         .reset_reset_n                              (1'b1),                              //   input,   width = 1,                      reset.reset_n
		   .pio_fsync_out_external_connection_export   (vfr_fsync),   //  output,   width = 1, pio_fsync_out_external_connection.export

			.sdram_clk_clk                  (DRAM_CLK),                     //               sdram_clk.clk
			.sdram_cke                      (DRAM_CKE),                     //                   sdram.cke
			.sdram_cs_n                     (DRAM_CS_n),                    //                        .cs_n
			.sdram_ras_n                    (DRAM_RAS_n),                   //                        .ras_n
			.sdram_cas_n                    (DRAM_CAS_n),                   //                        .cas_n
			.sdram_we_n                     (DRAM_WE_n),                    //                        .we_n
			.sdram_dqm                      (DRAM_DQM[3: 0]),               //                        .dqm
			.sdram_addr                     (DRAM_ADDR),                    //                        .addr
			.sdram_ba                       (DRAM_BA),                      //                        .ba
			.sdram_data_out                 (dram_data_out[31: 0]),         //                        .data_out
			.sdram_data_oe                  (dram_data_oe),                 //                        .data_oe
			.sdram_data_in                  (dram_data_in[31: 0])           //                        .data_in
    );
	 
	 
	 
/////////////////////////	 
// VVP to HDMI OUTPUT	 

assign disp_clk = clk_hdmi_tx; 

assign HDMI_TX_D = disp_data;	 
assign HDMI_TX_CLK_p = disp_clk;
assign HDMI_TX_HS = disp_hs;
assign HDMI_TX_VS = disp_vs;
assign HDMI_TX_DE = disp_de;		 


/////////////////////////	
// breath LED

assign LED = (!KEY[0])?4'h0:
				 (!KEY[1])?4'hf:{~PWM_width[6],~PWM_width[6],~PWM_width[6],~PWM_width[6]};


reg 	[27:0] 	counter;
reg 	[5:0]   PWM_adj;
reg   [6:0]   PWM_width;
always @(posedge CLOCK1_50 or negedge reset_n)
begin
	if(!reset_n)
		begin
			counter <= 0;
		end
	else 
	begin
		counter   <= counter+1;
		PWM_width <= PWM_width[5:0]+ PWM_adj;
		if(counter[26])
		begin
			PWM_adj <= counter[25:20];
		end
		else 
		begin
			PWM_adj <= ~ counter[25:20];
		end
	end
end	 

	 
endmodule
