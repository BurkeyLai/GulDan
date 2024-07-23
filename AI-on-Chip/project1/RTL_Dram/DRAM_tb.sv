`timescale 1ns/10ps
`define CYCLE 10
`include "DRAM.v"
`define MAX 10000
`define DRAM_START 'h0000

`define mem_word(addr){M1.Memory_byte3[addr],M1.Memory_byte2[addr],M1.Memory_byte1[addr],M1.Memory_byte0[addr]}

module test;

        parameter word_size = 32;       //Word Size
        parameter addr_size = 11;        //Address Size    
        logic CK;
        logic [word_size-1:0] Q;   //Data Output
        logic RST;
        logic CSn;                   //Chip Select
        logic [3:0] WEn;                  //Write Enable
        logic RASn;                  //Row Address Select
        logic CASn;                  //Column Address Select
        logic [addr_size-1:0] A;    //Address
        logic [word_size-1:0] D;    //Data Input

        string prog_path;
        integer i;

        logic OE;
        logic CS;
        logic [3:0] WEB;
        logic [13:0] AD;
        logic [31:0] DI;
        logic [31:0] DO;

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

    always #(`CYCLE/2) CK = ~CK;  
    
    
    initial begin
        //prog_path ="";
        $value$plusargs("prog_path=%s", prog_path);
        CK = 0;
        RST = 1;
        CSn = 0; WEn = 4'b1111;
        RASn = 1; CASn = 1;
        A = 0; D = 0;
        #(`CYCLE*2) RST = 0;
        $readmemh({prog_path, "/dram0.hex"}, M1.Memory_byte0);
        $readmemh({prog_path, "/dram1.hex"}, M1.Memory_byte1);
        $readmemh({prog_path, "/dram2.hex"}, M1.Memory_byte2);
        $readmemh({prog_path, "/dram3.hex"}, M1.Memory_byte3);
        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; WEn = 4'b0000; D = 10; // Column Address
        #(`CYCLE) CASn = 0;
        #(`CYCLE) A = 11;	D = 11;
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; // Column Address
        #(`CYCLE) CASn = 0; 
        #(`CYCLE) A = 11; // Column Address	
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        
        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; WEn = 4'b0000; D = 13; // Column Address
        #(`CYCLE) CASn = 0;
        #(`CYCLE) A = 11;	D = 14;
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;
        #(`CYCLE) A = 5; // Row Address
        #(`CYCLE) RASn = 0;
        #(`CYCLE) A = 10; // Column Address
        #(`CYCLE) CASn = 0; 
        #(`CYCLE) A = 11; // Column Address	
        #(`CYCLE) RASn = 1; CASn = 1; WEn = 4'b1111; D = 0;	
        
        #(`CYCLE*50) $finish;
    end

    initial begin
        $fsdbDumpfile("test.fsdb");
        $fsdbDumpvars("+struct","+mda", test);
    end

    // initial begin
    //     #(`CYCLE*50)
    //     for(i=0;i<60;i++)
    //     begin
    //         $display("%h",`mem_word(`DRAM_START +i));
    //     end
    //     $finish;
    // end
endmodule
