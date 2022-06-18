`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/26 08:55:36
// Design Name: 
// Module Name: RegMux
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


module RegMux(
    input select,
    input [4:0] in0,
    input [4:0] in1,
    output [4:0] out
    );
    assign out = select?in1:in0;
endmodule