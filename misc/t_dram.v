module tb_dram;
    parameter MEM_DEPTH = 2097152; // 2MB
    parameter MEM_WIDTH = 8;
    parameter ADDR_WIDTH = 24; // 2MB 定址 10 0000 0000 0000 0000 0000

    reg clk;
    reg [ADDR_WIDTH - 1:0] addr;
    wire [MEM_WIDTH - 1:0] data_out;
    integer i;


    DRAM #(
        .MEM_DEPTH(MEM_DEPTH),
        .MEM_WIDTH(MEM_WIDTH)
    ) dram (
        .clk(clk),
        .addr(addr),
        .data_out(data_out)
    );

    initial begin
        // 初始化時鐘
        clk = 0;
        forever #5 clk = ~clk; // 時鐘周期為10個時間單位
    end

    initial begin
        // 地址範例測試
        for (i = 0; i < MEM_DEPTH; i = i + 1) begin
            addr = i;
            #10; // 等待一個時鐘周期
        end
        $finish;
    end

    initial begin
        // 監視信號變化
        $monitor("At time %t, addr = %h, data_out = %h", $time, addr, data_out);
    end

    // 波形輸出
    initial begin
        $dumpfile("tb_dram.vcd");
        $dumpvars(0, tb_dram);
    end
endmodule
