    
module terasic_sdio(
	input						clk,
	input 					reset_n,
	
	// avmm slave
    input   [11:0]  		avs_address, // byte address 
    input          		avs_write,
    input          		avs_read,
    input   [7:0] 		avs_writedata,
    output reg  [7:0] 	avs_readdata,
//    output reg         avs_waitrequest
	
	// avmm slave (optional)

	// sdcard conduit
	output		sd_clk,
	inout 		sd_cmd, 
	inout [3:0]	sd_data 	
);  

parameter CLK_FREQ = 64'd5000_0000;

//reg [31:0] memory [0:15];

// registers are 32-bits
reg [7:0]	reg_control; // host write
//reg [7:0]	reg_status; // host read

reg [7:0]   reg_cmd[5:0];


reg [7:0]   reg_data_to_sdcard[511:0] /*synthesis noprune*/;
reg [7:0]   reg_data_from_sdcard[511:0] /*synthesis noprune*/;

//reg [7:0]   reg_data_to_sdcard[16:0] /*synthesis noprune*/;
//reg [7:0]   reg_data_from_sdcard[16:0] /*synthesis noprune*/;


`define REG_CSR   12'd0 // control & status register
`define REG_CRR   12'd4 // command & response register
`define REG_DATA  12'd30 // command & response register

`define REG_CRR_CMD_LEN	 6 // max 17 byte
`define REG_CRR_RES_LEN	 17 // max 17 byte
`define REG_DATA_LEN	 512 // byte

wire [8:0] data_index;
wire [2:0] cmd_index;
wire [4:0] response_index;
//wire [7:0] response_bit_index; // max 135
assign data_index = (avs_address-`REG_DATA); 
assign cmd_index = (avs_address-`REG_CRR); 
assign response_index = (avs_address-`REG_CRR);
//assign response_bit_index = (avs_address-`REG_CRR) << 3; // x8

//wire [7:0] response8;
//assign response8 = response[(response_bit_index+7):response_bit_index];

always @(posedge clk or negedge reset_n) begin
  if (!reset_n) begin
		//waitrequest <= 0;
		avs_readdata <= 8'b0;
  end else begin
		//waitrequest <= 0; // No wait states in this example

		if (avs_write) begin
			if (avs_address == `REG_CSR) begin
				reg_control <= avs_writedata;
				//reg_status <= 0; // reset status
			end
			else if ((avs_address >= `REG_CRR) && (avs_address < (`REG_CRR+`REG_CRR_CMD_LEN))) begin
				reg_cmd[cmd_index] <= avs_writedata;
			end
			else if (avs_address >= `REG_DATA) begin
				reg_data_to_sdcard[data_index] <= avs_writedata;
			end
			
		end

		if (avs_read) begin
			if (avs_address == `REG_CSR) begin
				//avs_readdata <= reg_status;
				avs_readdata <= {3'b000, txn_error_code, txn_done};
			end
			else if ((avs_address >= `REG_CRR) && (avs_address < (`REG_CRR+`REG_CRR_RES_LEN))) begin
				avs_readdata <= reg_data_from_sdcard[response_index];
			end	
			else if (avs_address >= `REG_DATA) begin
				avs_readdata <= reg_data_from_sdcard[data_index];
			end		
		end
  end
end

	 
 


wire       		txn_start;
wire      		txn_done;
wire [3:0] 		txn_error_code;
wire [2:0]     txn_type;


wire [47:0]     cmd;



//reg  [7:0]  	mem[511:0];
wire [7:0] 		mem_readdata; 
wire [7:0] 		mem_writedata; 
wire [8:0] 		mem_addr; 
wire 				mem_write; 


assign txn_start = reg_control[0];
assign txn_type = reg_control[3:1];


assign cmd = {reg_cmd[0], reg_cmd[1], reg_cmd[2], reg_cmd[3], reg_cmd[4], reg_cmd[5]};


// mem_readdata. sdio_inst read data from register reg_data_to_sdcard
assign mem_readdata = reg_data_to_sdcard[mem_addr];

// mem_writedata, sdio_inst write data to register reg_data_from_sdcard 

always @(posedge clk or negedge reset_n)
begin
	if (~reset_n)
	begin
		reg_data_from_sdcard[0] <= 8'hff;
		reg_data_from_sdcard[1] <= 8'hff;
		reg_data_from_sdcard[2] <= 8'hff;
		reg_data_from_sdcard[3] <= 8'hff;
		reg_data_from_sdcard[4] <= 8'hff;
		reg_data_from_sdcard[5] <= 8'hff;
		reg_data_from_sdcard[6] <= 8'hff;
		reg_data_from_sdcard[7] <= 8'hff;
	end
	else if (mem_write)
		reg_data_from_sdcard[mem_addr] <= mem_writedata;
end

    sdio sdio_inst(
		.clk(clk), 
		.reset_n(reset_n), 
		
		.txn_start(txn_start), 
		.txn_type(txn_type), 
		.txn_done(txn_done), 
		.txn_error_code(txn_error_code), 
		
		.cmd(cmd), 
		
		.sd_clk(sd_clk), 
		.sd_cmd(sd_cmd), 
		.sd_data(sd_data),

		.mem_readdata(mem_readdata),
		.mem_writedata(mem_writedata),
		.mem_addr(mem_addr),
		.mem_write(mem_write)

		);
	defparam sdio_inst.CLK_FREQ = CLK_FREQ;	


endmodule