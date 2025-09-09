module vga_controller(iRST_n,
                      iVGA_CLK,
                      oBLANK_n,
                      oHS,
                      oVS,
                      b_data,
                      g_data,
                      r_data);
input iRST_n;
input iVGA_CLK;
output  oBLANK_n;
output  oHS;
output  oVS;
output [7:0] b_data;
output [7:0] g_data;  
output [7:0] r_data;                        
///////// ////                     
reg [20:0] ADDR;
reg [23:0] bgr_data;
wire VGA_CLK_n;
wire [7:0] index;
wire [23:0] bgr_data_raw;
wire cBLANK_n,cHS,cVS,rst;
////
assign rst = ~iRST_n;
video_sync_generator LTM_ins (.vga_clk(iVGA_CLK),
                              .reset(rst),
                              .blank_n(cBLANK_n),
                              .HS(cHS),
                              .VS(cVS));
////
////Addresss generator
always@(posedge iVGA_CLK,negedge iRST_n)
begin
  if (!iRST_n)
     ADDR<=21'd0;
  else if (cHS==1'b0 && cVS==1'b0)
     ADDR<=21'd0;
  else if (cBLANK_n==1'b1)
     ADDR<=ADDR+1;
end

//////////////////////////
//////INDEX addr.
assign VGA_CLK_n = ~iVGA_CLK;

wire [23:0] m20k_index;
img_data	img_data_inst (
	.address ( ADDR ),
	.clock ( iVGA_CLK ),
	.q ( m20k_index )
	);
//////Color table output
img_index	img_index_inst (
	.address ( index ),
	.clock ( iVGA_CLK ),
	.q ( bgr_data_raw)
	);	

wire [23:0] mlab_index;
////Color table output
img_data_mlab	img_data_mlab_inst (
	.address ( ADDR-21'd896000),
	.clock ( iVGA_CLK ),
	.q ( mlab_index)
	);	

	assign index  = (ADDR <= 21'd896000)? m20k_index: mlab_index;

//////
//////latch valid data at falling edge; mlab_bgr_data_raw
//always@(posedge VGA_CLK_n) bgr_data <= bgr_data_raw;
always@(posedge VGA_CLK_n)bgr_data <= bgr_data_raw;

assign b_data = bgr_data[23:16];
assign g_data = bgr_data[15:8];
assign r_data = bgr_data[7:0];

assign oHS = cHS;
assign oVS = cVS;
assign oBLANK_n = cBLANK_n;
			

endmodule
 	
















