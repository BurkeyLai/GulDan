`timescale 1ns/10ps
`define CYCLE 10
`include "AI-on-Chip/project1/RTL_Dram/DRAM.v"
`include "SRAM.v"
`define MAX 10000
`define DRAM_START 'h0000
`define SRAM_START 'h0000

`define dram_mem_word(addr) {M1.Memory_byte3[addr], M1.Memory_byte2[addr], M1.Memory_byte1[addr], M1.Memory_byte0[addr]}
`define sram_mem_data1(addr) {M2.Memory0[addr]}
`define sram_mem_data2(addr) {M2.Memory1[addr]}
`define sram_mem_weight(addr) {M2.Memory2[addr]}

module tb_DMAC;

    parameter dram_word_size = 32;       // Word Size
    parameter sram_word_size = 32;       // Word Size
    parameter addr_size = 12;       // Address Size    
    reg CK;
    reg RST;

    wire DRAM_RST;
    wire DRAM_CSn;                        // Chip Select
    wire [3:0] DRAM_WEn;                  // Write Enable
    wire DRAM_RASn;                       // Row Address Select
    wire DRAM_CASn;                       // Column Address Select
    wire [addr_size-1:0] DRAM_A;          // Address
    wire [dram_word_size-1:0] DRAM_D;          // Data Input
    wire [dram_word_size-1:0] DRAM_Q;         // Data Output

    wire SRAM_RST;
    wire SRAM_CSn;                        // Chip Select
    wire [3:0] SRAM_WEn;                  // Write Enable
    wire [addr_size-1:0] SRAM_A;          // Address
    wire [sram_word_size-1:0] SRAM_D;    // Data Input
    wire [sram_word_size-1:0] SRAM_Q;    // Data Input

    reg [1:0] START; // 2'b01 load image data, 2'b11 load weight data
    reg [22-1:0] DRAM_START_A;          // Address
    reg [31:0] DRAM_READ_SIZE;          // size to read from DRAM
    reg [31:0] SRAM_WRITE_SIZE;         // size to write to SRAM
    wire VALID;
    wire DMA_S;
    wire DMA_E;


    integer i;
    reg [311:0] prog_path; // "AI-on-Chip/project1/RTL_Dram/DRAM_INPUT" (39 * 8)
    // reg OE;
    // reg CS;
    // reg [3:0] WEB;
    // reg [13:0] AD;
    // reg [31:0] DI;
    // reg [31:0] DO;

    DRAM M1 (
        .CK(CK),  
        .Q(DRAM_Q),
        .RST(DRAM_RST),
        .CSn(DRAM_CSn),
        .WEn(DRAM_WEn),
        .RASn(DRAM_RASn),
        .CASn(DRAM_CASn),
        .A(DRAM_A),
        .D(DRAM_D)
    );

    SRAM M2 (
        .RST(SRAM_RST),
        .CK(CK),
        .CSn(SRAM_CSn),
        .WEn(SRAM_WEn),
        .Addr(SRAM_A),
        .DataIn(SRAM_D),
        .DataOut(SRAM_Q)
    );

    DMAC M3 (
        .CLK(CK),
        .RST(RST),
        .Start(START),
        .DRAM_START_A(DRAM_START_A),
        .DRAM_SIZE(DRAM_READ_SIZE),
        .SRAM_SIZE(SRAM_WRITE_SIZE),

        .DRAM_RST(DRAM_RST),
        .DRAM_CSn(DRAM_CSn),
        .DRAM_RASn(DRAM_RASn),
        .DRAM_CASn(DRAM_CASn),
        .DRAM_WEn(DRAM_WEn),
        .DRAM_A(DRAM_A),
        .DRAM_D(DRAM_D),
        .DRAM_Q(DRAM_Q),

        .SRAM_RST(SRAM_RST),
        .SRAM_CSn(SRAM_CSn),
        .SRAM_WEn(SRAM_WEn),
        .SRAM_Addr(SRAM_A),
        .SRAM_Data(SRAM_D),

        .Valid(VALID),
        .DMA_Start(DMA_S),
        .DMA_Done(DMA_E)
    );


    always #(`CYCLE / 2) CK = ~CK;
    
    initial begin
        prog_path = "AI-on-Chip/project1/RTL_Dram/DRAM_INPUT";

        $display("%s", {prog_path, "/dram0.hex"});
        CK = 0;
        RST = 1;

        // 開始並 load data 進 DRAM
        #(`CYCLE * 1) RST = 0;
        #(`CYCLE * 1);
        START = 2'b11;
        DRAM_START_A = 'h100000; // load weight data
        DRAM_READ_SIZE = 16 * 3 * 3 * 3; // 432 (0x1B0)
        SRAM_WRITE_SIZE = 16 * 3 * 3 * 3; // 432 (0x1B0)
        #(`CYCLE * 1);
        // $readmemh({prog_path, "/dram0.hex"}, M1.Memory_byte0);
        // $readmemh({prog_path, "/dram1.hex"}, M1.Memory_byte1);
        // $readmemh({prog_path, "/dram2.hex"}, M1.Memory_byte2);
        // $readmemh({prog_path, "/dram3.hex"}, M1.Memory_byte3);
        $readmemh({prog_path, "/input_0.hex"}, M1.Memory_byte0);
        $readmemh({prog_path, "/input_1.hex"}, M1.Memory_byte1);
        $readmemh({prog_path, "/input_2.hex"}, M1.Memory_byte2);
        $readmemh({prog_path, "/input_3.hex"}, M1.Memory_byte3);

        
        #(`CYCLE * 1000);

        RST = 1;
        #(`CYCLE * 2) RST = 0;
        #(`CYCLE * 1);
        START = 2'b01;
        DRAM_START_A = 'h000000; // load image data
        DRAM_READ_SIZE = 416 * 416 * 3; // 519168 (0x7EC00)
        SRAM_WRITE_SIZE = 52 * 52; // 2707 (0xA90)
        #(`CYCLE * 530000);

        $finish;
    end

    initial begin
        $dumpfile("tb_DMAC.vcd");
        $dumpvars(0, tb_DMAC);
    end

    initial begin
        // 監視信號變化
        // $monitor("At time %d, start = %h, dma_start = %h, dma_done = %h, valid = %h", $time, START, DMA_S, DMA_E, VALID);
        // $monitor("At time %d, dram addr = %h, dram data_out = %h", $time, DRAM_A, DRAM_Q);
        // $monitor("At time %d, sram addr = %h", $time, SRAM_A);
    end

    // initial begin
    //     #(`CYCLE * 50);
    //     for (i = 0; i < 60; i++)
    //     begin
    //         $display("%h: %h",`DRAM_START + i, `dram_mem_word(`DRAM_START + i));
    //     end
    //     $finish;
    // end

    initial begin
        // #(`CYCLE * 1000);
        // for (i = 0; i < 27 * 16; i++)
        // begin
        //     $display("%h: %h", `SRAM_START + i, `sram_mem_weight(`SRAM_START + i));
        // end

        #(`CYCLE * 530000);
        for (i = 0; i < 52 * 52; i++)
        begin
            $display("%h: [%h, %h]", `SRAM_START + i, `sram_mem_data1(`SRAM_START + i), `sram_mem_data2(`SRAM_START + i));
        end

        #(`CYCLE * 530000);
        for (i = 0; i < 60 * 60; i++)
        begin
            $display("%h: [%h, %h]", `SRAM_START + i, `sram_mem_data1(`SRAM_START + i), `sram_mem_data2(`SRAM_START + i));
        end

        $finish;
    end

    // 监测 DMA_Done 信号并结束仿真
    always @(posedge CK) begin
        if (DMA_E) begin
            $display("DMA_Done signal detected. Simulation finished.");
            RST = 1;
            // $finish;
        end
    end
    

endmodule
