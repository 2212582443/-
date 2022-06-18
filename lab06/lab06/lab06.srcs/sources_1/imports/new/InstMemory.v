`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/26 09:48:58
// Design Name: 
// Module Name: InstMem
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


module InstMemory(
        input[31:0] address,
        output[31:0] inst
    );

    reg [31:0] instFile[0:1023];
    integer i;
    initial begin
     for(i=0;i<1024;i=i+1)
          instFile[i]=0;
    end
    assign inst = instFile[address/4];
endmodule