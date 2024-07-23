`timescale 1ns/10ps
`define CYCLE 10
`include "DRAM.v"
`include "top.sv"
`define MAX 10000
`define INPUT_START 'h0000
`define WEIGHT_START 'h100000
`define OUTPUT_START 'h180000
`define SIM_END 'hFFFFF
`define SIM_END_CODE -32'd1			

`define mem_word(addr){M1.Memory_byte3[addr],M1.Memory_byte2[addr],M1.Memory_byte1[addr],M1.Memory_byte0[addr]}

module top_tb;

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
        integer gf, i, num, j,k, err;

        logic OE;
        logic CS;
        logic [3:0] WEB;
        logic [13:0] AD;
        logic [31:0] DI;
        logic [31:0] DO;
        logic [31:0] GOLDEN[4096];

    top top1(
        /*
        your code
        */
    );

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
        $value$plusargs("prog_path=%s", prog_path);

		$readmemh({prog_path, "/input_0.hex"}, M1.Memory_byte0);
        $readmemh({prog_path, "/input_1.hex"}, M1.Memory_byte1);
        $readmemh({prog_path, "/input_2.hex"}, M1.Memory_byte2);
        $readmemh({prog_path, "/input_3.hex"}, M1.Memory_byte3);

        // $readmemh({prog_path, "/input_feature0.hex"}, top_1.DRAM_1.Memory_byte0);
        // $readmemh({prog_path, "/input_feature1.hex"}, top_1.DRAM_1.Memory_byte1);
        // $readmemh({prog_path, "/input_feature2.hex"}, top_1.DRAM_1.Memory_byte2);
        // $readmemh({prog_path, "/input_feature3.hex"}, top_1.DRAM_1.Memory_byte3);

        gf = $fopen({prog_path, "/output.txt"}, "r");
        while (num < 4096)
        begin
            $fscanf(gf, "%d\n", GOLDEN[num]);
            num++;
        end
        $fclose(gf);
        while (1)
        begin
			#(`CYCLE)
      		if (`mem_word(`SIM_END) == `SIM_END_CODE)
      		begin
        		break; 
      		end
        end
		$display("\nDone\n");
		// for(i=0;i<60;i++)   //print first 60 value
        // begin
        //     $display("%h",`mem_word(`INPUT_START + i));
        // end
		for (i = 0; i < num; i++)
		begin
			if (`mem_word(`OUTPUT_START + i) !== GOLDEN[i])
			begin
				$display("DRAM[%8d] = %h, expect = %h", `OUTPUT_START + i, `mem_word(`OUTPUT_START + i), GOLDEN[i]);
				err = err + 1;
			end
			else
			begin
				$display("DRAM[%8d] = %h, pass", `OUTPUT_START + i, `mem_word(`OUTPUT_START + i));
			end
		end
		if (err === 0)
	    begin
	        $display("\n");
	        $display("\n");
	        $display("        ****************************               ");
	        $display("        **                        **       |\__||  ");
	        $display("        **  Congratulations !!    **      / O.O  | ");
	        $display("        **                        **    /_____   | ");
	        $display("        **  Simulation PASS!!     **   /^ ^ ^ \\  |");
	        $display("        **                        **  |^ ^ ^ ^ |w| ");
	        $display("        ****************************   \\m___m__|_|");
	        $display("\n");
	    end
	    else
	    begin
	    	$display("\n");
	        $display("\n");
	        $display("        ****************************               ");
	        $display("        **                        **       |\__||  ");
	        $display("        **  OOPS!!                **      / X,X  | ");
	        $display("        **                        **    /_____   | ");
	        $display("        **  Simulation Failed!!   **   /^ ^ ^ \\  |");
	        $display("        **                        **  |^ ^ ^ ^ |w| ");
	        $display("        ****************************   \\m___m__|_|");
	        $display("         Totally has %d errors                     ", err); 
	        $display("\n");
	    end
		$finish;
    end

    initial begin
        $fsdbDumpfile("test.fsdb");
        $fsdbDumpvars("+struct","+mda", test);
    end

endmodule
