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
//Date:  Mon May 12 18:04:03 2025
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
//--SDRAM no use
assign DRAM_CS_n =1'b1;//
assign DRAM_WE_n =1'b1;//
assign DRAM_CAS_n=1'b1;//
assign DRAM_RAS_n=1'b1;//

//---reset delay
wire TFP410_I2C_reset_delay_n;
RESET_DELY u_RESET_DELY( 
       .BUTTON (KEY[1]    ), 
	    .OSC_50 (CLOCK0_50  ), 
       .RESET_n(TFP410_I2C_reset_delay_n)  
) ;

//---inital reset 
wire reset_n ;
wire ninit_done  ; 
reset_release u_reset_release(
		.ninit_done(ninit_done) 
	);
assign reset_n =!ninit_done	;
///-----TFP410 pin set
assign HDMI_ISEL   	= reset_n;// 1:enable I2C
assign HDMI_PD_n   	= 1'b1;


//--TFP410_I2C SET /readID by way of I2C -----
wire [15:0] VEN_ID;
wire [15:0] DEV_ID;
wire        TFP410_I2C_READY;
TFP410_I2C u_TFP410_I2C(
     .clk50       (CLOCK0_50    ),
     .reset_n     (TFP410_I2C_reset_delay_n ),
     .i2c_clk     (HDMI_I2C_SCL),
     .i2c_dat     (HDMI_I2C_SDA),
     .SLAVE_ADDR_R(8'h78       ),// TFP410 I2C Slave address
	  .VEN_ID      (VEN_ID      ),
	  .DEV_ID      (DEV_ID      ),
	  .READY       (TFP410_I2C_READY)
);

//-----Color bar generater---------
//-- pll
wire pll_locked;
wire vpg_pclk ; 
V_PLL u_AV_PLL(
    .refclk       (CLOCK0_50      ),
    .outclk_0     (vpg_pclk      ),//148.5MHZ
    .locked       (pll_locked    ),
	 .rst          ( 1'b0         )
);
//---pattern gen
wire [7:0] vpg_r;
wire [7:0] vpg_g;
wire [7:0] vpg_b;				
vpg	u_vpg (
	.vpg_pclk    (vpg_pclk   ),//vedio clock input
	.reset_n     (KEY[0]     ),    
	.vpg_de      (HDMI_TX_DE ),
	.vpg_hs      (HDMI_TX_HS ),
	.vpg_vs      (HDMI_TX_VS ),
	.vpg_pclk_out(HDMI_TX_CLK_p),
	.vpg_r       (vpg_r),
	.vpg_g       (vpg_g),
	.vpg_b       (vpg_b)
	);
								

//--video out
assign HDMI_TX_D[23:16]	=	vpg_r;
assign HDMI_TX_D[15:8] 	=	vpg_g;
assign HDMI_TX_D[7:0]  	=	vpg_b;

//---LED display------
assign LED = ~{1'h0, HZ_vpg_pclk,pll_locked, TFP410_I2C_READY};

//---heart of 148.5MHZ--
wire HZ_vpg_pclk;
CLOCKMEM u_CLOCKMEM( 
    .CLK     ( vpg_pclk    ),
    .CLK_FREQ( 148_500_000 ),
    .CK_1HZ  ( HZ_vpg_pclk )
	 ) ;




endmodule
