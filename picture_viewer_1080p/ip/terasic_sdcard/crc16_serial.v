

module crc16_serial (
    input          clk,
    input          reset,
    input          data_in,  // 1-bit serial input
    input          enable,   // Enable signal
    output   [15:0] crc_out   // 16-bit CRC output
);
    
    reg [15:0] crc_reg;
    
    always @(posedge clk or posedge reset) begin
        if (reset) begin
            crc_reg <= 16'h0000; // Initial value for CRC-16-CCITT
        end else if (enable) begin
            crc_reg[0]  <= data_in ^ crc_reg[15];
            crc_reg[1]  <= crc_reg[0];
            crc_reg[2]  <= crc_reg[1];
            crc_reg[3]  <= crc_reg[2];
            crc_reg[4]  <= crc_reg[3];
            crc_reg[5]  <= crc_reg[4] ^ (data_in ^ crc_reg[15]);
            crc_reg[6]  <= crc_reg[5];
            crc_reg[7]  <= crc_reg[6];
            crc_reg[8]  <= crc_reg[7];
            crc_reg[9]  <= crc_reg[8];
            crc_reg[10] <= crc_reg[9];
            crc_reg[11] <= crc_reg[10];
            crc_reg[12] <= crc_reg[11] ^ (data_in ^ crc_reg[15]);
            crc_reg[13] <= crc_reg[12];
            crc_reg[14] <= crc_reg[13];
            crc_reg[15] <= crc_reg[14];
        end
    end
    
    assign crc_out = crc_reg;
    
endmodule