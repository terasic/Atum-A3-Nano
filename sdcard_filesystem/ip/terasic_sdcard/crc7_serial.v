module crc7_serial (
    input        clk,
    input        reset,
    input        data_in,  // single data input
    input        enable,   // high to input data
    output [6:0] crc_out   // 7-bit CRC
);
    reg [6:0] crc_reg;

    always @(posedge clk or posedge reset) begin
        if (reset)
            crc_reg <= 7'b0000000; // init 0
        else if (enable) begin
            crc_reg[6] <= crc_reg[5];
            crc_reg[5] <= crc_reg[4];
            crc_reg[4] <= crc_reg[3];
            crc_reg[3] <= crc_reg[2] ^ (crc_reg[6] ^ data_in);
            crc_reg[2] <= crc_reg[1];
            crc_reg[1] <= crc_reg[0];
            crc_reg[0] <= crc_reg[6] ^ data_in;
        end
    end

    assign crc_out = crc_reg;

endmodule