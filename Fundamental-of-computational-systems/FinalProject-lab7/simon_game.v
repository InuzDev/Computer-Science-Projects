module simon_game (
    input        clk,
    input        rst,
    input        start_pulse,
    input  [3:0] btn_edge,
    output [3:0] leds,
    output [2:0] rgb0,
    output [2:0] rgb1,
    output [3:0] level_bcd
);

   localparam DUR_SHOW = 2;
   localparam DUR_GAP = 1;
   localparam DUR_FEEDBACK = 1;
   localparam DUR_WIN = 3;

   localparam MAX_LEVEL = 10;

   localparam S_IDLE = 4'd0;
   localparam S_NEW_LEVEL = 4'd1;
   localparam S_SHOW_ELEM = 4'd2;
   localparam S_SHOW_GAP = 4'd3;
   localparam S_WAIT_INPUT = 4'd4;
   localparam S_INPUT_FEEDBACK = 4'd5;
   localparam S_WIN_FLASH = 4'd6;
   localparam S_LOSE = 4'd7;

   reg  [3:0] state;
   reg  [3:0] level;
   reg  [3:0] show_idx;
   reg  [3:0] input_idx;
   reg  [2:0] dur_cnt;
   reg  [1:0] feedback_val;

   reg  [1:0] seq_mem      [0:MAX_LEVEL-1];

   wire       tick;
   tick_gen tickgen_inst (
       .clk (clk),
       .rst (rst),
       .tick(tick)
   );

   wire [3:0] rand_val;
   lfsr4 lfsr_inst (
       .clk  (clk),
       .rst  (rst),
       .value(rand_val)
   );


   wire have_press = |btn_edge;
   reg [1:0] pressed_idx;
   always @(*) begin
      casez (btn_edge)
         4'b???1: pressed_idx = 2'd0;
         4'b??10: pressed_idx = 2'd1;
         4'b?100: pressed_idx = 2'd2;
         4'b1000: pressed_idx = 2'd3;
         default: pressed_idx = 2'd0;
      endcase
   end

   always @(posedge clk) begin
      if (rst) begin
         state     <= S_IDLE;
         level     <= 4'd0;
         show_idx  <= 4'd0;
         input_idx <= 4'd0;
         dur_cnt   <= 3'd0;
      end else begin
         case (state)

            S_IDLE: begin
               if (start_pulse) begin
                  level <= 4'd0;
                  state <= S_NEW_LEVEL;
               end
            end

            S_NEW_LEVEL: begin
               seq_mem[level] <= rand_val[1:0];
               level          <= level + 1'b1;
               show_idx       <= 4'd0;
               dur_cnt        <= DUR_SHOW - 1;
               state          <= S_SHOW_ELEM;
            end

            S_SHOW_ELEM: begin
               if (tick) begin
                  if (dur_cnt == 0) begin
                     dur_cnt <= DUR_GAP - 1;
                     state   <= S_SHOW_GAP;
                  end else begin
                     dur_cnt <= dur_cnt - 1'b1;
                  end
               end
            end

            S_SHOW_GAP: begin
               if (tick) begin
                  if (dur_cnt == 0) begin
                     if (show_idx == level - 1'b1) begin
                        input_idx <= 4'd0;
                        state     <= S_WAIT_INPUT;
                     end else begin
                        show_idx <= show_idx + 1'b1;
                        dur_cnt  <= DUR_SHOW - 1;
                        state    <= S_SHOW_ELEM;
                     end
                  end else begin
                     dur_cnt <= dur_cnt - 1'b1;
                  end
               end
            end

            S_WAIT_INPUT: begin
               if (have_press) begin
                  if (pressed_idx == seq_mem[input_idx]) begin
                     if (input_idx == level - 1'b1) begin
                        dur_cnt <= DUR_WIN - 1;
                        state   <= S_WIN_FLASH;
                     end else begin
                        input_idx    <= input_idx + 1'b1;
                        feedback_val <= pressed_idx;
                        dur_cnt      <= DUR_FEEDBACK - 1;
                        state        <= S_INPUT_FEEDBACK;
                     end
                  end else begin
                     state <= S_LOSE;
                  end
               end
            end

            S_INPUT_FEEDBACK: begin
               if (tick) begin
                  if (dur_cnt == 0) state <= S_WAIT_INPUT;
                  else dur_cnt <= dur_cnt - 1'b1;
               end
            end

            S_WIN_FLASH: begin
               if (tick) begin
                  if (dur_cnt == 0) begin
                     if (level >= MAX_LEVEL) state <= S_LOSE;
                     else state <= S_NEW_LEVEL;
                  end else begin
                     dur_cnt <= dur_cnt - 1'b1;
                  end
               end
            end

            S_LOSE: begin
               if (start_pulse) begin
                  level <= 4'd0;
                  state <= S_NEW_LEVEL;
               end
            end

            default: state <= S_IDLE;
         endcase
      end
   end


   function [3:0] one_hot;
      input [1:0] idx;
      begin
         case (idx)
            2'd0: one_hot = 4'b0001;
            2'd1: one_hot = 4'b0010;
            2'd2: one_hot = 4'b0100;
            2'd3: one_hot = 4'b1000;
         endcase
      end
   endfunction

   assign leds = (state == S_SHOW_ELEM) ? one_hot(
       seq_mem[show_idx]
   ) : (state == S_INPUT_FEEDBACK) ? one_hot(
       feedback_val
   ) : 4'b0000;

   assign rgb0 = (state == S_WIN_FLASH) ? 3'b010 : 3'b000;
   assign rgb1 = (state == S_LOSE) ? 3'b100 : 3'b000;

   assign level_bcd = level;

endmodule
