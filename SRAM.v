module SRAM #(
    parameter MEM_DEPTH  = 4096, 
    parameter MEM_WIDTH  = 52,
    parameter MEM_HEIGHT = 52,
    parameter MEM_LENGTH = 32, // 假設每個單元寬度為32位
    parameter ADDR_WIDTH = 12,
    parameter HI_Z = {MEM_LENGTH{1'bz}}
)(
    input wire RST,
    input wire CK,
    input wire CSn,
    input wire [3:0] WEn,
    input wire [ADDR_WIDTH-1:0] Addr,
    input wire [MEM_LENGTH-1:0] DataIn,
    output reg [MEM_LENGTH-1:0] DataOut
);

    reg [MEM_LENGTH-1:0] Memory0 [0:MEM_DEPTH-1];
    reg [MEM_LENGTH-1:0] Memory1 [0:MEM_DEPTH-1];
    reg [MEM_LENGTH-1:0] Memory2 [0:MEM_DEPTH-1]; // weight data
    integer i;

    // Write
    always @(posedge RST or posedge CK) begin
        if (RST) begin
            for (i = 0; i < MEM_DEPTH; i = i + 1) begin
				Memory0[i] <= 0;
                Memory1[i] <= 0;
                Memory2[i] <= 0;
            end
        end
        else begin
            if (~CSn && ~WEn[0]) begin
                Memory0[Addr] <= DataIn;
            end
            if (~CSn && ~WEn[1]) begin
                Memory1[Addr] <= DataIn;
            end
            if (~CSn && ~WEn[2]) begin
                Memory2[Addr] <= DataIn;
            end
        end
    end

    // Read
    always @(posedge RST or posedge CK) begin
        if (RST) begin
            DataOut <= HI_Z;
        end
        else if (~CSn) begin
            DataOut <= Memory0[Addr];
        end
    end
endmodule
