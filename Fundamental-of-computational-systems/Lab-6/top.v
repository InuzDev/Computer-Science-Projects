// Top-level wrapper for the Traffic Light project on the Boolean Board.
// Wires together: clk_divider -> TrafficLight -> output_decoder,
// with ped_request_handler debouncing the pedestrian button.
//
// Board pin usage:
//   clk       -> 100 MHz onboard oscillator
//   btn[0]    -> system reset (this board has no dedicated reset pin)
//   btn[1]    -> pedestrian crossing request button
//   RGB0, RGB1 -> the two onboard RGB LEDs (main light / secondary light)
//   D1_AN/D1_SEG -> seven-segment countdown display (single digit, static)

module top(
    input clk,
    input [3:0] btn,
    output [2:0] RGB0,
    output [2:0] RGB1,
    output [3:0] D1_AN,
    output [7:0] D1_SEG
);

    wire rst = btn[0];
    wire ped_btn = btn[1];

    wire tick;
    wire ped_request;
    wire ped_clear;
    wire [2:0] state;
    wire [3:0] countdown;

    clk_divider #(.CLK_FREQ(100_000_000)) clkdiv_inst (
        .clk (clk),
        .rst (rst),
        .tick(tick)
    );

    ped_request_handler ped_handler_inst (
        .clk        (clk),
        .rst        (rst),
        .btn        (ped_btn),
        .ped_clear  (ped_clear),
        .ped_request(ped_request)
    );

    TrafficLight traffic_light_inst (
        .clk        (clk),
        .rst        (rst),
        .tick       (tick),
        .ped_request(ped_request),
        .state      (state),
        .countdown  (countdown),
        .ped_clear  (ped_clear)
    );

    output_decoder decoder_inst (
        .state(state),
        .rgb0 (RGB0),
        .rgb1 (RGB1)
    );

    // Show the countdown value as a hex digit on one 7-seg digit.
    // Static display (no multiplexing needed for a single digit).
    function [7:0] hex_to_seg;
        input [3:0] val;
        begin
            case (val)
                4'h0: hex_to_seg = 8'b1100_0000;
                4'h1: hex_to_seg = 8'b1111_1001;
                4'h2: hex_to_seg = 8'b1010_0100;
                4'h3: hex_to_seg = 8'b1011_0000;
                4'h4: hex_to_seg = 8'b1001_1001;
                4'h5: hex_to_seg = 8'b1001_0010;
                4'h6: hex_to_seg = 8'b1000_0010;
                4'h7: hex_to_seg = 8'b1111_1000;
                4'h8: hex_to_seg = 8'b1000_0000;
                4'h9: hex_to_seg = 8'b1001_0000;
                default: hex_to_seg = 8'b1111_1111;
            endcase
        end
    endfunction

    assign D1_AN  = 4'b1110;               // enable rightmost digit only
    assign D1_SEG = hex_to_seg(countdown);

endmodule
