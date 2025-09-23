module sd_controller (
    input  		clk,
    input  		rst_n,
	 
	 // SD BUS interface
    inout	[3:0] sd_data,
    inout			sd_cmd,
    output			sd_clk,
	 
	 
	 // command control
    input 					start,
    input		[5:0] 	cmd,
    input		[31:0] 	arg,
	 input      [5:0]    rep_len, 
	 
    output reg [127:0] 	response, // 48 or 132 bits
    output reg 			busy
);

    reg [47:0] command;
    reg [7:0] bit_cnt;
    reg sending;
    reg receiving;
    reg data_transfer;
    reg [127:0] response_buffer;
    reg [31:0] data_buffer;
    reg [4:0] data_bit_cnt;
    reg data_receiving;
	 
	 // richard add
	 reg sd_cmd_reg
	 assign sd_cmd = sending?sd_cmd_reg:1'bz;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) 
			  begin
					sd_cmd <= 1;
					sd_clk <= 0;
					busy <= 0;
					sending <= 0;
					receiving <= 0;
					data_transfer <= 0;
					data_receiving <= 0;
					bit_cnt <= 0;
					data_bit_cnt <= 0;
			  end 
		  
		  else if (start && !busy)
				begin
                busy <= 1;
                command <= {2'b01, cmd, arg, 7'h00, 1'b1}; // construct a SD command
                sending <= 1;
                bit_cnt <= 0;
					 sd_cmdr_reg <= 1'b0; //richard add
				end
		  
		  else if (sending)
				begin
					 sd_clk <= ~sd_clk; // generate clock
					 if (sd_clk) begin
						  sd_cmdr_reg <= command[47];
						  command <= {command[46:0], 1'b1};
						  bit_cnt <= bit_cnt + 1;
						  if (bit_cnt == 47) begin
								sending <= 0;
								receiving <= rep_len?1:0; // receive response
								bit_cnt <= 0;
						  end
					 end
				end
					
		  else if (receiving)
				begin					
					 sd_clk <= ~sd_clk;
					 if (!sd_clk) begin
						  response_buffer <= {response_buffer[126:0], sd_cmd};
						  bit_cnt <= bit_cnt + 1;
						  if ((bit_cnt+1) == rep_len) begin
								receiving <= 0;
								//data_transfer <= 1;
								data_bit_cnt <= 0;
								
								busy <= 0;
								data_transfer <= 0;
						  end
					 end
				end
					
//		  else if (data_transfer)
//				begin					
//					 sd_clk <= ~sd_clk;
//					 if (!sd_clk) begin
//						  sd_data <= data_buffer[31:28];
//						  data_buffer <= {data_buffer[27:0], 4'b0};
//						  data_bit_cnt <= data_bit_cnt + 1;
//						  if (data_bit_cnt == 7) begin
//								data_transfer <= 0;
//								busy <= 0;
//						  end
//					 end
//				end
//				
//        end
    end //always end
	 
endmodule
