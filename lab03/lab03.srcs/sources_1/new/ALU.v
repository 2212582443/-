`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/22 16:33:43
// Design Name: 
// Module Name: ALU
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


module ALU(
    input [31:0] input1,
    input [13:0] input2,
    input [3:0] aluCtr,
    output zero,
    output [31:0] aluRes
    );
    
    reg Zero;
    reg [31:0] ALURes;
    
    always @ (input1 or input2 or aluCtr)
    begin
        case(aluCtr)
        4'b0000:
        ALURes=input1 & input2;
        4'b0001:
        ALURes=input1 | input2;
        4'b0010:
        ALURes=input1+input2;
        4'b0110:
        ALURes=input1-input2;
        4'b0111:
        ALURes=($signed(input1)<$signed(input2));
        4'b1100:
        ALURes=~(input1 | input2);
        default:
        ALURes=0;
        endcase
        if(ALURes==0)
           Zero=1;
        else
           Zero=0;
   end
   assign aluRes=ALURes;
   assign zero=Zero;
endmodule
