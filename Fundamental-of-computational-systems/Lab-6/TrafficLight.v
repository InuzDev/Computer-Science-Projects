module TrafficLight (
    input clk,
    input rst,
    input tick,
    input ped_request,
    output reg [2:0] state,
    output reg [3:0] countdown,
    output reg ped_clear
);

   localparam S_MAIN_GREEN = 3'd0;
   localparam S_MAIN_YELLOW = 3'd1;
   localparam S_SEC_GREEN = 3'd2;
   localparam S_SEC_YELLOW = 3'd3;
   localparam S_PED_CROSS = 3'd4;


   localparam T_GREEN = 4'd9;
   localparam T_YELLOW = 4'd3;
   localparam T_PED = 4'd5;
   localparam T_MIN_SAFETY = 4'd3;

   always @(posedge clk) begin
      if (rst) begin
         state <= S_MAIN_GREEN;
         countdown <= T_GREEN;
         ped_clear <= 1'b0;
      end else if (tick) begin
         if (state == S_MAIN_GREEN && ped_request && countdown <= (T_GREEN - T_MIN_SAFETY)) begin
            state <= S_MAIN_YELLOW;
            countdown <= T_YELLOW;
            ped_clear <= 1'b0;
         end else if (countdown > 0) begin
            countdown <= countdown - 1;
            ped_clear <= 1'b0;
         end else begin
            case (state)
               S_MAIN_GREEN: begin
                  state     <= S_MAIN_YELLOW;
                  countdown <= T_YELLOW;
                  ped_clear <= 1'b0;
               end
               S_MAIN_YELLOW: begin
                  if (ped_request) begin
                     state     <= S_PED_CROSS;
                     countdown <= T_PED;
                     ped_clear <= 1'b1;
                  end else begin
                     state     <= S_SEC_GREEN;
                     countdown <= T_GREEN;
                     ped_clear <= 1'b0;
                  end
               end
               S_SEC_GREEN: begin
                  state     <= S_SEC_YELLOW;
                  countdown <= T_YELLOW;
                  ped_clear <= 1'b0;
               end
               S_SEC_YELLOW: begin
                  state     <= S_MAIN_GREEN;
                  countdown <= T_GREEN;
                  ped_clear <= 1'b0;
               end
               S_PED_CROSS: begin
                  state     <= S_MAIN_GREEN;
                  countdown <= T_GREEN;
                  ped_clear <= 1'b0;
               end
               default: begin
                  state     <= S_MAIN_GREEN;
                  countdown <= T_GREEN;
                  ped_clear <= 1'b0;
               end
            endcase
         end
      end else begin
         ped_clear <= 1'b0;
      end
   end

endmodule

module ped_request_handler (
    input clk,
    input rst,
    input btn,
    input ped_clear,
    output reg ped_request
);
   reg btn_sync1, btn_sync2, btn_prev;

   always @(posedge clk) begin
      btn_sync1 <= btn;
      btn_sync2 <= btn_sync1;
      btn_prev  <= btn_sync2;
   end

   wire btn_pressed_edge = btn_sync2 & ~btn_prev;

   always @(posedge clk) begin
      if (rst) ped_request <= 1'b0;
      else if (ped_clear) ped_request <= 1'b0;
      else if (btn_pressed_edge) ped_request <= 1'b1;
   end
endmodule

module output_decoder (
    input [2:0] state,
    output reg [2:0] rgb0,
    output reg [2:0] rgb1
);

   localparam S_MAIN_GREEN = 3'd0;
   localparam S_MAIN_YELLOW = 3'd1;
   localparam S_SEC_GREEN = 3'd2;
   localparam S_SEC_YELLOW = 3'd3;
   localparam S_PED_CROSS = 3'd4;

   always @(*) begin
      case (state)
         S_MAIN_GREEN: begin
            rgb0 = 3'b010;
            rgb1 = 3'b100;
         end
         S_MAIN_YELLOW: begin
            rgb0 = 3'b110;
            rgb1 = 3'b100;
         end
         S_SEC_GREEN: begin
            rgb0 = 3'b100;
            rgb1 = 3'b010;
         end
         S_SEC_YELLOW: begin
            rgb0 = 3'b100;
            rgb1 = 3'b110;
         end
         S_PED_CROSS: begin
            rgb0 = 3'b100;
            rgb1 = 3'b100;
         end
         default: begin
            rgb0 = 3'b100;
            rgb1 = 3'b100;
         end
      endcase
   end

endmodule

module clk_divider #(
    parameter CLK_FREQ = 100_000_000
) (
    input clk,
    input rst,
    output reg tick
);
   reg [26:0] counter;

   always @(posedge clk) begin
      if (rst) begin
         counter <= 0;
         tick <= 0;
      end else if (counter == CLK_FREQ - 1) begin
         counter <= 0;
         tick <= 1'b1;
      end else begin
         counter <= counter + 1;
         tick <= 1'b0;
      end
   end
endmodule
