
module sdio (
    input  		clk,
    input  		reset_n,
	 
	 // SD BUS interface
    inout	[3:0] 	sd_data,
    inout				sd_cmd,
    output	reg		sd_clk,
	 
	 
	 // command control
    input 					txn_start, // rise edge trigger
    input		[2:0] 	txn_type,
    output	reg			txn_done,// 1: done
    output	reg [3:0]	txn_error_code, //0: success
	 
    input	[47:0] 	cmd, // 48 bits (only first 40 bit is used)

	// onchip memory, 512 Byte
    output	reg	[7:0]		mem_writedata,
    input		[7:0]		   mem_readdata,
    output	reg	[8:0]		mem_addr,
    output	reg				mem_write

   
);

parameter CLK_FREQ = 64'd5000_0000;


parameter DATA_WRITE_TIMEOUT = 64'd50 * CLK_FREQ / 64'd1000; // 50 ms
parameter DATA_READ_START_TIMEOUT = 64'd20 * CLK_FREQ / 64'd1000;  // 20 ms




reg 		sd_cmd_oe; // output enable
reg		sd_cmd_out;

assign sd_cmd = sd_cmd_oe?sd_cmd_out:1'bz;

reg 		sd_data_oe; // output enable
reg [3:0]	sd_data_out;



assign sd_data = sd_data_oe?sd_data_out:4'bzzzz;

// transaction type
`define TXN_NONE				3'd0
`define TXN_DUMMY_CYCLE		3'd1
`define TXN_CMD				3'd2
`define TXN_CMD_RESPONE48	3'd3
`define TXN_CMD_RESPONE136	3'd4
`define TXN_BLOCK_READ		3'd5
`define TXN_BLOCK_WRITE		3'd6

`define ERR_SUCCESS    			4'd0
`define ERR_TIMEOUT    			4'd1
`define ERR_CRC    				4'd2
`define ERR_WRITE    			4'd3
`define ERR_INVALID_END_BIT  	4'd4
`define ERR_OTHER    			4'd5




//////////////////////////
// start detect

reg txn_start_pre;
wire start_rise_detected;


always @(posedge clk or negedge reset_n) 
begin
	if (~reset_n)
		txn_start_pre <= 1'b0;
	else
		txn_start_pre <= txn_start;
end

assign 	start_rise_detected = ~txn_start_pre & txn_start;




//////////////////////////
// state
reg [23:0]	tick_cnt;
//reg [7:0]   tick_cycle_cnt;
reg [9:0]	cmd_tx_tick_cnt_remained;	
reg [9:0]	cmd_rx_tick_cnt_remained /*synthesis noprune*/;	
reg [135:0] 	response /*synthesis noprune*/;
reg [9:0]	dummy_clk_cnt_remained;

`define ST_STANDBY				4'd0		
`define ST_CMD_TX					4'd1
`define ST_CMD_RX					4'd2
`define ST_DATA_READ_START		4'd3
`define ST_DATA_READ_512B 		4'd4
`define ST_DATA_READ_CRC16		4'd5
`define ST_DATA_READ_END		4'd6
`define ST_DATA_WRITE_START	4'd7
`define ST_DATA_WRITE_512B		4'd8
`define ST_DATA_WRITE_CRC16	4'd9
`define ST_DATA_WRITE_RESPONSE	4'd10
`define ST_DATA_WRITE_BUSY		4'd11
`define ST_DUMMY_CLK			4'd12
`define ST_DONE				4'd13
//`define ST_ERROR			4'd14

reg [3:0]	state  /*synthesis noprune*/;
reg         crc7_valid;
reg [7:0]	data8  /*synthesis noprune*/;
reg [7:0]	data_response8 /*synthesis noprune*/;
reg [2:0]	executed_txn_type;
reg			wait_0;
reg [7:0]   response_bit_index;

// crc16
//reg				crc16_cal_reset;
//reg				crc16_cal_enable;
//reg	[3:0]		crc16_cal_data;

reg            crc16_cal_enable_extra;
reg				crc16_valid;
wire [3:0] 		expcted_crc16;




always @(posedge clk or negedge reset_n) 
begin
	if (~reset_n)
	begin
		state <= `ST_STANDBY;
		sd_clk <= 1'b1;
		txn_done <= 1'b0;
		txn_error_code <= `ERR_SUCCESS;
		sd_cmd_oe <= 1'b0;
		sd_data_oe <= 1'b0;

	end
	else if ((state == `ST_STANDBY) && start_rise_detected) // start, chagne state 
	begin
		txn_done <= 1'b0;
		txn_error_code <= `ERR_SUCCESS;
		sd_clk <= 1'b1;
		mem_write <= 1'b0;
		executed_txn_type <= txn_type;
		sd_cmd_oe <= 1'b0;
		sd_data_oe <= 1'b0;
		crc7_valid <= 1'b1;
		if (txn_type == `TXN_DUMMY_CYCLE)
		begin
			state <= `ST_DUMMY_CLK;
			dummy_clk_cnt_remained <= cmd[47:42]; 
		end
		else if (txn_type == `TXN_CMD)
		begin
			state <= `ST_CMD_TX;
			cmd_tx_tick_cnt_remained <= 48;
			cmd_rx_tick_cnt_remained <= 0;
		end
		else if (txn_type == `TXN_CMD_RESPONE48)
		begin
			state <= `ST_CMD_TX;
			cmd_tx_tick_cnt_remained <= 48;
			cmd_rx_tick_cnt_remained <= 48;
		end
		else if (txn_type == `TXN_CMD_RESPONE136)
		begin
			state <= `ST_CMD_TX;
			cmd_tx_tick_cnt_remained <= 48;
			cmd_rx_tick_cnt_remained <= 136;
		end
		else if (txn_type == `TXN_BLOCK_READ)
		begin
			// read 512 byte data from sdcard, and write to mem
			state <= `ST_DATA_READ_START;
			tick_cnt <= 0;
			//crc16_cal_reset <= 1'b1;
			//crc16_cal_enable <= 1'b0;
		end
		else if (txn_type == `TXN_BLOCK_WRITE)
		begin
			// read 512 byte data from mem, and write to sdcard
			state <= `ST_DATA_WRITE_START;
			tick_cnt <= 0;
			// output start bit (0000) to data bus
			//sd_data_oe <= 1'b1; // data become output port
			//sd_data_out <= 4'b000;
			// point to onchip memory to read data for writing to sdcard
			mem_addr <= 0;
			mem_write <= 0;
			
			//crc16_cal_reset <= 1'b1;
			//crc16_cal_enable <= 1'b0;
		end
	end


	else if (state == `ST_CMD_TX)
	begin
			sd_clk <= ~sd_clk;
			if (sd_clk) // change data when sd_clk falling 
			begin
				cmd_tx_tick_cnt_remained <= cmd_tx_tick_cnt_remained -1;
				if (cmd_tx_tick_cnt_remained == 1)
					sd_cmd_out <= 1'b1; // end bit: 1
				else if (cmd_tx_tick_cnt_remained <= 8)
					sd_cmd_out <= cmd_rsp_crc7[cmd_tx_tick_cnt_remained-2]; // 7-bit crc7
				else
					sd_cmd_out <= cmd[cmd_tx_tick_cnt_remained-1];
				sd_cmd_oe <= 1'b1;
			end
			else if (~sd_clk && (cmd_tx_tick_cnt_remained == 0)) // go to next state at sd_clk rise
			begin
				state <= cmd_rx_tick_cnt_remained?`ST_CMD_RX:`ST_DUMMY_CLK;
				tick_cnt <= 0;
				dummy_clk_cnt_remained <= 8;
				sd_cmd_oe <= 1'b0;
				wait_0 <= 1'b1;
				mem_write <= 1'b0;
				mem_addr <= 0;
				response_bit_index <= 0;
			end
	end
	else if (state == `ST_CMD_RX)
	begin
			sd_clk <= ~sd_clk;
			if (~sd_clk) // latch response when sd_clk rise 
			begin
				// wait start bit 0
				if (wait_0 & sd_cmd)
				begin
					tick_cnt <= tick_cnt + 1;
					if (tick_cnt == 128) // timeout, 4M, 6 tick, 25M, 37 tick...
					begin
						state <= `ST_DONE;
						
					end
				end
				else
				begin
					wait_0 <= 1'b0; 
					cmd_rx_tick_cnt_remained <= cmd_rx_tick_cnt_remained -1;
					response[cmd_rx_tick_cnt_remained - 1] <= sd_cmd; // host need to check the content 
					
					if (cmd_rx_tick_cnt_remained == 1)  // stop
					begin
						crc7_valid <= (data8 != cmd_rsp_crc7)?1'b1:1'b0;
						state <= `ST_DUMMY_CLK;
						//tick_cnt <= 0;
					end
					
					// write respone to onchip memory
					response_bit_index <= response_bit_index + 1;
					if ((response_bit_index+1) % 8 == 0)
					begin
						mem_writedata <= {data8[6:0], sd_cmd}; // current addr is 0
						mem_addr <= response_bit_index/8; // byte addr
						mem_write <= 1'b1;
					end
					else
					begin
						mem_write <= 1'b0;
						data8 = {data8[6:0], sd_cmd};
					end
				end
			end
	end

	else if (state == `ST_DUMMY_CLK)
	begin
		   mem_write <= 1'b0;	
			sd_clk <= ~sd_clk;
			if (~sd_clk)
			begin
				dummy_clk_cnt_remained <= dummy_clk_cnt_remained - 1;
				if (dummy_clk_cnt_remained == 1)
				begin
					state <= `ST_DONE;
				end
			end
	end

	///////////////////////////////////////////////////////////////////////////////////////
	// Read Data 512 Byte
	else if (state == `ST_DATA_READ_START)
	begin
			sd_clk <= ~sd_clk;
			if (~sd_clk) // check data when sd_clk high
			begin
				//crc16_cal_reset <= 1'b0;
				tick_cnt <= tick_cnt + 1;
				if (sd_data == 4'b0000) // check start bit (0)  
				begin
					state <= `ST_DATA_READ_512B;
					tick_cnt <= 0;
					mem_addr <= 0;
				end
//				else if (tick_cnt >= 4095) // timeout. 641 at 4MHz clock
				else if (tick_cnt >= DATA_READ_START_TIMEOUT) // timeout. 641 at 4MHz clock
				begin
					state <= `ST_DONE;
					txn_error_code <= `ERR_TIMEOUT;
				end
			end
	end
	else if (state == `ST_DATA_READ_512B) // read 512 byte data. read 4-bit each cyclone
	begin
			sd_clk <= ~sd_clk;
			if (~sd_clk) // read data when sd_clk high
			begin
				tick_cnt <= tick_cnt + 1;
				data8 <= {data8[3:0], sd_data};
				if (tick_cnt[0] == 1'b1) // second nibble, write data into memory
				begin
					mem_writedata <=  {data8[3:0], sd_data};
					mem_addr <= tick_cnt[9:1];
					mem_write <= 1'b1;
					if ((tick_cnt+1) == 512*2) // last data 512 nibble
					begin
						state <= `ST_DATA_READ_CRC16;
						tick_cnt <= 0;
						crc16_valid <= 1'b1;
						crc16_cal_enable_extra <= 1'b1;
					end
				end
				// crc16
				//crc16_cal_enable <= 1'b1;
				//crc16_cal_data <= sd_data;
				//else
				//	mem_write <= 1'b0;
			end
	end
	else if (state == `ST_DATA_READ_CRC16) // read crc16
	begin
		//crc16_cal_enable <= 1'b0;
		//tick_cnt <= tick_cnt + 1;
		crc16_cal_enable_extra <= 1'b0;
		mem_write <= 1'b0;
		
		sd_clk <= ~sd_clk;
		if (~sd_clk) // read data when sd_clk high
		begin
			if (sd_data != expcted_crc16) // crc16 not match
				crc16_valid <= 1'b0;

			tick_cnt <= tick_cnt + 1;
//			if ((tick_cnt+1) == 16*2)
			if ((tick_cnt+1) == 16)
				state <= `ST_DATA_READ_END;
		end
	end

	else if (state == `ST_DATA_READ_END)  // read end bit. expect 4'b1111
	begin
		sd_clk <= ~sd_clk;
		if (~sd_clk) // read data when sd_clk high
		begin
			state <= `ST_DONE;
			if (sd_data != 4'b1111) // check end bit (1) 
				txn_error_code <= `ERR_INVALID_END_BIT;
			else if (!crc16_valid)
				txn_error_code <= `ERR_CRC;
			
				//state <= crc16_valid?`ST_DONE:`ST_ERROR;
			//else
				//state <= `ST_ERROR;
				//
									//state <= `ST_DONE;
					//txn_error_code <= `ERR_TIMEOUT;
		end
	end

	///////////////////////////////////////////////////////////////////////////////////////
	// Write Data 512 Byte
	else if (state == `ST_DATA_WRITE_START)  //
	begin
		//crc16_cal_reset <= 1'b0;
		sd_clk <= ~sd_clk;
		if (sd_clk) // write data when sd_clk falling
		begin
		   sd_data_oe <= 1'b1; // data become output port
			sd_data_out <= 4'b0000; // output start bits (zero value)
		end
	
		tick_cnt <= tick_cnt + 1;
		if (tick_cnt == 1)
		begin
			state <= `ST_DATA_WRITE_512B;
			tick_cnt <= 0;
			data8 <= mem_readdata; // current addr is 0
			mem_addr <= mem_addr + 1; // read next data
		//	crc16_cal_enable_extra <= 1'b0;
		end

	end

	else if (state == `ST_DATA_WRITE_512B) // write 512 byte data. write 4-bit each cycle
	begin
		sd_clk <= ~sd_clk;
		if (sd_clk) // write data when sd_clk falling (sdcard read data when sd_clk rise)
		begin
			tick_cnt <= tick_cnt + 1;
			sd_data_out <= tick_cnt[0]?(data8 & 4'hF):((data8 >> 4) & 4'hF); // write out data
			if (tick_cnt[0]) // second nibble
			begin
				data8 <= mem_readdata;
				mem_addr <= mem_addr + 1;
				
				//crc16_cal_enable <= 1'b1;
				//crc16_cal_data <= mem_readdata;				
			end
		end
		else if (tick_cnt == 512*2) // ~sd_clk, last data rising clk (512x2 nibble)
		begin
				state <= `ST_DATA_WRITE_CRC16;
				tick_cnt <= 0;
				crc16_cal_enable_extra <= 1'b1;
			   //crc16_cal_enable <= 1'b0;
		end
	end
	else if (state == `ST_DATA_WRITE_CRC16) // write crc16 
	begin
		crc16_cal_enable_extra <= 1'b0;
		sd_clk <= ~sd_clk;
		if (sd_clk)  // write data when sd_clk falling (sdcard read data when sd_clk rise)
		begin
			tick_cnt <= tick_cnt + 1;
			sd_data_out <= expcted_crc16;	
		end
		else if (tick_cnt == 16)  // ~sd_clk
		begin
				state <= `ST_DATA_WRITE_RESPONSE;
				tick_cnt <= 0;
				sd_data_oe <= 1'b0; // data become input port
		end
	end

	else if (state == `ST_DATA_WRITE_RESPONSE)  // read data response from sdcard data0
	begin
		sd_clk <= ~sd_clk;
		if (~sd_clk) // read data when sd_clk rising (sdcard set data when sd_clk fall) 
		begin
			tick_cnt <= tick_cnt + 1;
			data_response8 <= {data_response8[6:0], sd_data[0]}; // read 1 bit each cyclone. 0x05: success, 0x0B: crc error, 0x0D: write error
			if ((tick_cnt+1) == 8)
			begin
				state <= `ST_DATA_WRITE_BUSY;
				tick_cnt <= 0;
				//tick_cycle_cnt <= 0;
			end
		end
	end


	else if (state == `ST_DATA_WRITE_BUSY)  // read data response from sdcard data0
	begin
		sd_clk <= ~sd_clk;
		if (~sd_clk) // read data when sd_clk rising (sdcard set data when sd_clk fall) 
		begin
			tick_cnt <= tick_cnt + 1;
			if (sd_data[0]) // high: not busy
			begin
				state <= `ST_DONE;
				case (data_response8 & 8'h1F) // check LSB 5 bit, 0x05: success, 0x0B: crc error, 0x0D: write error
					8'h05:   txn_error_code <= `ERR_SUCCESS;
					8'h0B:   txn_error_code <= `ERR_CRC;
					8'h0D:   txn_error_code <= `ERR_WRITE;
					default: txn_error_code <= `ERR_OTHER;
				endcase
				
			end 
			else if (tick_cnt == DATA_WRITE_TIMEOUT) // 5395h  timeout
			begin
				state <= `ST_DONE;
				txn_error_code <= `ERR_TIMEOUT;
				//state <= `ST_ERROR; // timeout
				//tick_cycle_cnt <= tick_cycle_cnt + 1;
				//if (tick_cycle_cnt == 8'hFF)
				//	state <= `ST_ERROR; // timeout
				//else
				//	tick_cnt <= 0;
			end
		end
	end


	else if (state == `ST_DONE)
	begin
		state <= `ST_STANDBY;
		txn_done <= 1'b1;
		//txn_error <= (executed_txn_type == `TXN_BLOCK_WRITE)?((data_response8 == 8'h05)?0:1):1'b0;
	end

//	else if (state == `ST_ERROR)
//	begin
//		state <= `ST_STANDBY;
//		txn_done <= 1'b1;
//		txn_error <= 1'b1;
//	end

end



////////////////////////////
// CMD CRC7
wire [6:0] cmd_rsp_crc7;
wire enable_cmd_cal_crc7;
wire enable_rsp_cal_crc7;
wire		cmd_crc7_reset;
wire		rsp_crc7_reset;

assign cmd_crc7_reset = ((state == `ST_CMD_TX) && (cmd_tx_tick_cnt_remained == 48))?1:1'b0;
assign rsp_crc7_reset = ((state == `ST_CMD_RX) && (cmd_rx_tick_cnt_remained == 48))?1:1'b0;
assign enable_cmd_cal_crc7 = ((state == `ST_CMD_TX) && (cmd_tx_tick_cnt_remained >= 8))?1:1'b0;
assign enable_rsp_cal_crc7 = ((state == `ST_CMD_RX) && (cmd_rx_tick_cnt_remained >= 8))?1:1'b0;

crc7_serial crc7_serial_cmd(
    .clk(sd_clk),
    .reset(cmd_crc7_reset | rsp_crc7_reset),
    .data_in(sd_cmd),  // single data input
    .enable(enable_cmd_cal_crc7 | enable_rsp_cal_crc7),   // high to input data
    .crc_out(cmd_rsp_crc7)   // 7-bit CRC
);



////////////////////////////
// DATA CRC16



wire		crc16_reset;
wire [15:0] data0_crc16;
wire [15:0] data1_crc16;
wire [15:0] data2_crc16;
wire [15:0] data3_crc16;
wire enable_cal_crc16;

assign crc16_reset = ((state == `ST_DATA_READ_START) || (state == `ST_DATA_WRITE_START))?1'b1:1'b0;


//assign expcted_crc16 = enable_cal_crc16?{data3_crc16[15-tick_cnt],data2_crc16[15-tick_cnt] ,data1_crc16[15-tick_cnt],data0_crc16[15-tick_cnt]}:4'b0000;
assign expcted_crc16 = {data3_crc16[15-tick_cnt],data2_crc16[15-tick_cnt] ,data1_crc16[15-tick_cnt],data0_crc16[15-tick_cnt]};

//assign enable_cal_crc16 = ((state == `ST_DATA_READ_512B) || ((state == `ST_DATA_READ_CRC16) && (tick_cnt == 0)) || (state == `ST_DATA_WRITE_512B))?1:1'b0;

//assign enable_cal_crc16 = (((state == `ST_DATA_READ_512B) || crc16_cal_enable_extra) || (state == `ST_DATA_WRITE_512B))?1:1'b0;


assign enable_cal_crc16 = (((state == `ST_DATA_READ_512B) || crc16_cal_enable_extra) || ((state == `ST_DATA_WRITE_512B) || crc16_cal_enable_extra))?1:1'b0;






crc16_serial crc16_serial_data0(
    .clk(sd_clk),
    .reset(crc16_reset),
    .data_in(sd_data[0]),  // single data input
    .enable(enable_cal_crc16),   // high to input data
    .crc_out(data0_crc16)   // 16-bit CRC
);

crc16_serial crc16_serial_data1(
    .clk(sd_clk),
    .reset(crc16_reset),
    .data_in(sd_data[1]),  // single data input
    .enable(enable_cal_crc16),   // high to input data
    .crc_out(data1_crc16)   // 16-bit CRC
);

crc16_serial crc16_serial_data2(
    .clk(sd_clk),
    .reset(crc16_reset),
    .data_in(sd_data[2]),  // single data input
    .enable(enable_cal_crc16),   // high to input data
    .crc_out(data2_crc16)   // 16-bit CRC
);

crc16_serial crc16_serial_data3(
    .clk(sd_clk),
    .reset(crc16_reset),
    .data_in(sd_data[3]),  // single data input
    .enable(enable_cal_crc16),   // high to input data
    .crc_out(data3_crc16)   // 16-bit CRC
);



/////////////////////////////
// debug
assign state_out = state;


	 
endmodule

