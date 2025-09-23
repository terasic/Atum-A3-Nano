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




//=======================================================
//  Structural coding
//=======================================================
	wire ninit_done;

	reset_release reset_release_inst (
		.ninit_done (ninit_done)  //  output,  width = 1, ninit_done.ninit_done
	);
	
	

	wire sys_reset_n;
	assign sys_reset_n = ~ninit_done & KEY[0];
	

	 

    system u0 (
        .clk50_clk                          (CLOCK0_50),                         //   input,  width = 1,                      clk100.clk
        .reset_n_reset_n                    (sys_reset_n),                    //   input,  width = 1,                     reset_n.reset_n
        .pio_led_external_connection_export (LED), //  output,  width = 4, pio_led_external_connection.export
        .pio_key_external_connection_export (KEY),  //   input,  width = 2, pio_key_external_connection.export
		  
        .terasic_sdcard_conduit_end_sd_clk  (SD_CLK),  //  output,  width = 1,  terasic_sdcard_conduit_end.sd_clk
        .terasic_sdcard_conduit_end_sd_cmd  (SD_CMD),  //   inout,  width = 1,                            .sd_cmd
        .terasic_sdcard_conduit_end_sd_data (SD_DATA)  //   inout,  width = 4,                            .sd_data
    );
	 
	 



endmodule
