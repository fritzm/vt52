`timescale 1ns / 1ps

module chargenDump(
    input  logic CLK,
    output logic [9:0] ROM_ADDR,
    input  logic [6:0] ROM_DATA,
    output logic UART_TXD
);

//
//----- 9600 baud serial clock.  Fractional frequency divider counts upward from negative,
//      generating a pulse and subtracting down when crossing zero.  Error/remainder is
//      carried forward so clock may have some jitter but should not drift.  The 3200
//      divisor in the constants below is gcd(9600Hz, 100Mhz), and reduces the necessary
//      register sizes.
//

logic [15:0] ser_cnt = 0;
logic [15:0] ser_nxt;
logic ser_clk;

assign ser_nxt = ser_cnt + (ser_cnt[15] ? 16'(9600/3200) : 16'(9600/3200 - 100000000/3200));
assign ser_clk = ~ser_nxt[15];

always_ff @(posedge CLK) begin
    ser_cnt <= ser_nxt;
end

//
//----- ROM_ADDR, data_strobe, and halt are derived from a 17-bit counter driven (enabled) by 
//      the serial clock.
//

logic [16:0] addr_cnt = 0;
logic [16:0] addr_nxt;
logic data_strobe;
logic halt;

assign addr_nxt = addr_cnt + 1;

assign ROM_ADDR = addr_nxt[15:6];
assign data_strobe = &addr_nxt[5:0];
assign halt = addr_nxt[16];

always_ff @(posedge CLK) begin
    if (ser_clk & ~halt) begin
        addr_cnt <= addr_nxt;
    end
end

//
//----- RS-232 serial shifter, 1 start bit, 8 data bits, two stop bits.  Runs continuously,
//      shifting in 1 bits on the high end, so settles to RS-232 idle (which is logic high).
//      Loads four ASCII characters worth of data on data_strobe, representing ROM_DATA
//      as three octal digits followed by a space.  Stop bits of the final character are
//      accounted for by the logic high idle condition.
//

logic [40:0] shifter = 41'h1FFFFFFFFFF;
logic txd = 1;

assign UART_TXD = txd;

always_ff @(posedge CLK) begin
    if (ser_clk) begin
        { shifter, txd } <= data_strobe ? {
            8'h20, 1'b0,
            2'b11, 4'h3, 1'b0, ROM_DATA[2:0], 1'b0,
            2'b11, 4'h3, 1'b0, ROM_DATA[5:3], 1'b0,
            2'b11, 4'h3, 3'b000, ROM_DATA[6], 1'b0
        } : { 
            1'b1, shifter 
        };
    end
end

endmodule
