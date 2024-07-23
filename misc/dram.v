module DRAM #(
    parameter MEM_DEPTH = 2097152, 
    parameter MEM_WIDTH = 8,     // 假設每個單元寬度為8位
    parameter ADDR_WIDTH = 24
)(
    input wire clk,
    input wire [ADDR_WIDTH-1:0] addr,
    output reg [MEM_WIDTH-1:0] data_out
);

    reg [MEM_WIDTH-1:0] memory [0:MEM_DEPTH-1];

    initial begin
        $readmemh("AI-on-Chip/project1/RTL_Dram/DRAM_INPUT/input_0.hex", memory);
    end

    always @(posedge clk) begin
        data_out <= memory[addr];
    end
endmodule
