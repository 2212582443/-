`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/29 19:37:09
// Design Name: 
// Module Name: test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module test(

    );
    
    reg clk;
    reg reset;

    Top top(
        .clk(clk),
        .reset(reset)
    );
    

    initial begin
        $readmemh("mem_data.dat",top.memory.mem.memFile);
        $readmemb("mem_inst.dat",top.inst_mem.instFile);
        reset = 1;
        clk = 0;
    end

    always #10 clk = ~clk;

    initial begin
        #80 reset = 0;
        #1500;
        $finish;
    end
endmodule
