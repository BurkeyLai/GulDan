`timescale 1ns/10ps
`define CYCLE 10
// `include "DRAM.v"
`define MAX 10000
`define DRAM_START 'h0000

`define mem_word(addr) {M1.Memory_byte3[addr], M1.Memory_byte2[addr], M1.Memory_byte1[addr], M1.Memory_byte0[addr]}

module tb_DRAM;

    parameter word_size = 32;       // Word Size
    parameter addr_size = 12;       // Address Size    
    reg CK;
    wire [word_size-1:0] Q;          // Data Output
    reg RST;
    reg CSn;                        // Chip Select
    reg [3:0] WEn;                  // Write Enable
    reg RASn;                       // Row Address Select
    reg CASn;                       // Column Address Select
    reg [addr_size-1:0] A;          // Address
    reg [word_size-1:0] D;          // Data Input

    integer i;
    reg [255:0] prog_path; // 使用较大的reg来存储路径字符串

    // reg OE;
    // reg CS;
    // reg [3:0] WEB;
    // reg [13:0] AD;
    // reg [31:0] DI;
    // reg [31:0] DO;

    DRAM M1 (
        .CK(CK),  
        .Q(Q),
        .RST(RST),
        .CSn(CSn),
        .WEn(WEn),
        .RASn(RASn),
        .CASn(CASn),
        .A(A),
        .D(D)
    );

    always #(`CYCLE / 2) CK = ~CK;  
    
    initial begin
        prog_path = "DRAM_INPUT";
        // if ($value$plusargs("prog_path=%s", prog_path)) begin
        $display("%s", {prog_path, "/dram0.hex"});
        CK = 0;
        RST = 1;
        CSn = 0; WEn = 4'b1111;
        RASn = 1; CASn = 1;
        A = 0; D = 0;
        #(`CYCLE * 2) RST = 0;
        // $readmemh({prog_path, "/dram0.hex"}, M1.Memory_byte0);
        // $readmemh({prog_path, "/dram1.hex"}, M1.Memory_byte1);
        // $readmemh({prog_path, "/dram2.hex"}, M1.Memory_byte2);
        // $readmemh({prog_path, "/dram3.hex"}, M1.Memory_byte3);
        $readmemh({prog_path, "/input_0.hex"}, M1.Memory_byte0);
        $readmemh({prog_path, "/input_1.hex"}, M1.Memory_byte1);
        $readmemh({prog_path, "/input_2.hex"}, M1.Memory_byte2);
        $readmemh({prog_path, "/input_3.hex"}, M1.Memory_byte3);

        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; WEn = 4'b0000; D = 10; // Column Address
        #(`CYCLE) CASn = 0;
        #(`CYCLE) A = 11; D = 11;
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; // Column Address
        #(`CYCLE) CASn = 0; 
        #(`CYCLE) A = 11; // Column Address    
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        
        // #(`CYCLE) A = 5; // Row Address
        // #(`CYCLE) RASn = 0;
        // #(`CYCLE) A = 10; WEn = 4'b0000; D = 13; // Column Address
        // #(`CYCLE) CASn = 0;
        // #(`CYCLE) A = 11; D = 14;
        // #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        // #(`CYCLE) A = 5; // Row Address
        // #(`CYCLE) RASn = 0;
        // #(`CYCLE) A = 10; // Column Address
        // #(`CYCLE) CASn = 0; 
        // #(`CYCLE) A = 11; // Column Address    
        // #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;    
        
        // #(`CYCLE * 50) $finish;
        // #(`CYCLE);
        // $finish;
        // end
    end

    // initial begin
    //     $dumpfile("tb_DRAM.vcd");
    //     $dumpvars(0, tb_DRAM);
    // end

    initial begin
        #(`CYCLE * 20);
        // for (i = 0; i < 60; i++)
        // for (i = 10240; i < 10260; i++)
        for (i = 5120; i < 5140; i++)
        // for (i = 0; i < 416 * 416 * 3; i++)
        // for (i = 0; i < 10000; i++)
        begin
            $display("%h: %h",`DRAM_START + i, `mem_word(`DRAM_START + i));
        end
        $finish;
    end
    

endmodule
