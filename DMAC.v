module DMAC #(
    // 定義圖像和tile的大小
    parameter IMG_HEIGHT  = 416,
    parameter IMG_WIDTH   = 416,
    parameter TILE_HEIGHT = 52,
    parameter TILE_WIDTH  = 52,
    parameter IMG_DEPTH   = 3,
    parameter TILE_DEPTH  = 1,     // 假設每個tile的深度為1
    parameter DRAM_ADDR_WIDTH = 12,
    parameter DRAM_DATA_LENGTH = 32,
    parameter DRAM_ROW_NUM = 12,
    parameter DRAM_COL_NUM = 10,
    parameter DRAM_ROW_SIZE = (1 << DRAM_ROW_NUM),
    parameter DRAM_COL_SIZE = (1 << DRAM_COL_NUM),
    parameter SRAM_ADDR_WIDTH = 12,
    parameter SRAM_DATA_LENGTH = 32
    // parameter SRAM_SIZE = (1 << SRAM_ADDR_WIDTH)
)(
    input CLK,
    input RST,
    input [1:0] Start,                  // 開始信號，來自CPU
    input wire [DRAM_ROW_NUM + DRAM_COL_NUM - 1:0]  DRAM_START_A,
    input wire [31:0]  DRAM_SIZE,
    input wire [31:0]  SRAM_SIZE,

    output reg DRAM_RST,
    output reg DRAM_CSn,
    output reg DRAM_RASn,
    output reg DRAM_CASn,
    output reg [3:0] DRAM_WEn,
    output reg [DRAM_ADDR_WIDTH-1:0]  DRAM_A,
    output reg [DRAM_DATA_LENGTH-1:0] DRAM_D,
    input wire [DRAM_DATA_LENGTH-1:0] DRAM_Q,

    output reg SRAM_RST,
    output reg SRAM_CSn,
    output reg [3:0] SRAM_WEn,
    output reg [SRAM_ADDR_WIDTH-1:0]  SRAM_Addr,
    output reg [SRAM_DATA_LENGTH-1:0] SRAM_Data,  // 發送到SRAM的數據

    output reg  Valid,             // 有效標誌位
    output reg  DMA_Start,         // 告訴CPU DMA開始搬運數據
    output reg  DMA_Done           // 告訴CPU DMA完成搬運數據
);
    // 計數器
    reg [9:0] row, col;           // 行列計數器
    reg [1:0] rn_cn;
    reg [1:0] pingpong;
    reg [DRAM_ROW_NUM + DRAM_COL_NUM - 1:0] start_send_cnt;

    // 狀態機
    parameter IDLE  = 2'b00;
    parameter START = 2'b01;
    parameter LOAD  = 2'b10;
    parameter DONE  = 2'b11;
    reg [1:0] state, next_state;

    parameter READ_IDLE  = 3'b000;
    parameter SET_ROW    = 3'b001;
    parameter LOW_ROW    = 3'b010;
    parameter SET_COL    = 3'b011;
    parameter LOW_COL    = 3'b100;
    parameter READ_DONE  = 3'b101;
    reg [2:0] r_state, next_r_state;

    // 初始化狀態和計數器
    initial begin
        state = IDLE;
        next_state = IDLE;
        r_state = READ_IDLE;
        next_r_state = READ_IDLE;

        row = 0;
        col = 0;
        rn_cn = 2'b11;
        pingpong = 2'b01;
        start_send_cnt = 0;

        Valid <= 0;
        DMA_Start <= 0;
        DMA_Done <= 0;

        DRAM_RST <= 1;
        DRAM_CSn <= 1;
        DRAM_RASn <= 1;
        DRAM_CASn <= 1;
        DRAM_WEn <= 4'b1111;
        DRAM_A <= 0;
        DRAM_D <= 0;
        
        SRAM_RST <= 1;
        SRAM_CSn <= 1;
        SRAM_WEn <= 4'b1111;
        SRAM_Addr <= 0;
        SRAM_Data <= 0;
    end

    // 狀態轉移邏輯 (時序)
    always @(posedge CLK or posedge RST) begin
        if (RST) begin
            state <= IDLE;
            r_state <= READ_IDLE;
            
            row <= 0;
            col <= 0;

            DRAM_RASn <= 1;
            DRAM_CASn <= 1;
            DRAM_WEn <= 4'b1111;
            DRAM_A <= 0;
            DRAM_D <= 0;

            SRAM_WEn <= 4'b1111;
            SRAM_Addr <= 0;
            SRAM_Data <= 0;
        end else begin
            state <= next_state;
            r_state <= next_r_state;

            if (r_state == SET_COL && rn_cn == 2'b00) begin
                if (start_send_cnt >= 5) begin
                    SRAM_Addr <= SRAM_Addr + 1;
                end
                if (start_send_cnt >= 4) begin // LOW_COL -> SET_COL -> 2 Cycle Latency
                    if (Start == 2'b11) begin // load weight data
                        SRAM_WEn <= 4'b1011;
                    end
                    else begin
                        if (pingpong == 2'b01) begin
                            if (SRAM_WEn == 4'b1101) begin
                                SRAM_Addr <= 0; // switch to another SRAM, address initialize to 0
                            end
                            SRAM_WEn <= 4'b1110;
                        end
                        else if (pingpong == 2'b10) begin
                            if (SRAM_WEn == 4'b1110) begin
                                SRAM_Addr <= 0; // switch to another SRAM, address initialize to 0
                            end
                            SRAM_WEn <= 4'b1101;
                        end
                    end

                    SRAM_Data <= DRAM_Q;
                end

                // 更新 row 和 col
                if (col < DRAM_COL_SIZE - 1) begin // - 1 because col cannot equal to DRAM_COL_SIZE 
                    rn_cn <= 2'b00;
                    col <= col + 1;
                end else begin
                    rn_cn <= 2'b10;
                    col <= 0;
                    if (row < DRAM_ROW_SIZE - 1) begin
                        row <= row + 1;
                    end else begin
                        row <= 0;
                    end
                end

                start_send_cnt <= start_send_cnt + 1;
            end
            else begin
                start_send_cnt <= 0;
            end
        end
    end

    // 狀態機 (組合)
    always @(*) begin
        case (state)
            IDLE: begin
                if (Start == 2'b01 || Start == 2'b10 || Start == 2'b11) begin
                    next_state = START;
                end else begin
                    Valid <= 0;
                    next_state = IDLE;
                    DRAM_RST <= 1;
                    DRAM_CSn <= 1;
                    SRAM_RST <= 1;
                    SRAM_CSn <= 1;
                end
            end
            START: begin
                DMA_Start <= 1;
                next_state = LOAD;

                DRAM_RST <= 0;
                DRAM_CSn <= 0;
                SRAM_RST <= 0;
                SRAM_CSn <= 0;
            end
            LOAD: begin
                DMA_Start <= 0;
                Valid <= 1;

                if (row * DRAM_COL_SIZE + col < DRAM_SIZE - 1) begin
                    if (SRAM_Addr < SRAM_SIZE - 1) begin
                        next_state = LOAD;
                    end
                    else begin
                        if (pingpong == 2'b01) begin
                            pingpong = 2'b10; // switch to another SRAM
                            next_state = LOAD;
                        end
                        else if (pingpong == 2'b10) begin
                            pingpong = 2'b01; // switch to another SRAM
                            next_state = LOAD;
                        end
                    end
                end
                else begin
                    next_state = DONE;
                    next_r_state = READ_DONE;
                end
            end
            DONE: begin
                DMA_Done <= 1;
                Valid <= 0;
                next_state = IDLE;
            end
            default: next_state = IDLE;
        endcase
    end

    // 调试信息
    always @(posedge CLK) begin
        // $display("Time: %0t | State:  %b | Next State:  %b", $time, state, next_state);
        // $display("Time: %0t | RState: %b | Next RState: %b | rn_cn: %b | SRAM_Addr: %h", $time, r_state, next_r_state, rn_cn, SRAM_Addr);
        $display("Time: %0t | RState: %b | DRAM_RASn: %h | DRAM_CASn: %h | DRAM_WEn: %h | DRAM_A: %h | DRAM_Q: %h | DRAM_IDX: %h | start_send_cnt: %h | SRAM_WEn: %h | SRAM_Addr: %h", $time, r_state, DRAM_RASn, DRAM_CASn, DRAM_WEn, DRAM_A, DRAM_Q, row * DRAM_COL_SIZE + col, start_send_cnt, SRAM_WEn, SRAM_Addr);
    end

    // 讀 DRAM 狀態機 (組合)
    always @(*) begin
        case (r_state)
            READ_IDLE: begin
                rn_cn = 2'b11;
                if (Valid) // 啟動 DMA
                    next_r_state = SET_ROW;
                else
                    next_r_state = READ_IDLE;
            end
            SET_ROW: begin
                next_r_state = LOW_ROW;
            end
            LOW_ROW: begin
                rn_cn = 2'b01;
                next_r_state = SET_COL;
            end
            SET_COL: begin
                if (rn_cn == 2'b01) begin
                    next_r_state = LOW_COL;
                end
                else begin
                    if (rn_cn == 2'b10) begin
                        next_r_state = SET_ROW;
                    end
                    else begin
                        next_r_state = SET_COL;
                    end
                end
            end
            LOW_COL: begin
                rn_cn = 2'b00;
                next_r_state = SET_COL;
            end
            READ_DONE: begin
                rn_cn = 2'b11;
                next_r_state = READ_IDLE;
            end
            default: next_r_state = READ_IDLE;
        endcase
    end

    // 數據加載邏輯
    always @(posedge CLK) begin
        if (state == LOAD) begin
            case (r_state)
                READ_IDLE: begin
                    DRAM_RASn <= 1;
                    DRAM_CASn <= 1;
                    DRAM_WEn <= 4'b1111;
                end
                SET_ROW: begin
                    DRAM_A <= (DRAM_START_A >> DRAM_COL_NUM) + row; // DRAM_START_A 若是 'h100000，要 Right Shift DRAM column 的位數，以賦值給 upper address (row address)

                    DRAM_RASn <= 1;
                    DRAM_CASn <= 1;
                    DRAM_WEn <= 4'b1111;
                end
                LOW_ROW: begin
                    DRAM_RASn <= 0;
                    DRAM_CASn <= 1;
                    DRAM_WEn <= 4'b1111;
                end
                SET_COL: begin
                    DRAM_A <= col;
                end
                LOW_COL: begin
                    DRAM_RASn <= 0;
                    DRAM_CASn <= 0;
                    DRAM_WEn <= 4'b1111;
                end
                READ_DONE: begin
                    DRAM_RASn <= 1;
                    DRAM_CASn <= 1;
                    DRAM_WEn <= 4'b1111;
                end
            endcase

        end else begin
            row <= 0;
            col <= 0;

            DRAM_RASn <= 1;
            DRAM_CASn <= 1;
            DRAM_WEn <= 4'b1111;
            DRAM_A <= 0;
            DRAM_D <= 0;

            SRAM_WEn <= 4'b1111;
            SRAM_Addr <= 0;
            SRAM_Data <= 0;
        end

        if (state == DONE) begin
            DMA_Done <= 1;
        end else begin
            DMA_Done <= 0;
        end
    end

endmodule

