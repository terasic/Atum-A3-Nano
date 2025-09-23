module sd_control(
		input					clk,
		input					reset_n,
		// AVMM Slave
		input					
		input					avs_chipselect_n,
		input					avs_read_n,
		input					avs_write_n,
		input		[3:0]		avs_address,
		output	[31:0]	avs_writedata,
		input		[31:0]	avs_readdata,
		
		
		// SD interface
		output					SD_CLK,
		inout						SD_CMD,
		inout		[3:0]			SD_DAT,
);

reg [47:0]	reg_cmd;
reg [47:0]	reg_response;

always @(posedge clk or negedge reset_n)
begin
	if (~reset_n)
	begin
	end
end

endmodule

