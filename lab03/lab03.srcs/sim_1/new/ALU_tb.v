`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/22 16:56:14
// Design Name: 
// Module Name: ALU_tb
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


module ALU_tb(

    );
    wire Zero;
    wire [31 : 0] ALURes;
    reg [31 : 0] Input1;
    reg [31 : 0] Input2;
    reg [3 : 0] ALUCtr;
    
    
    ALU u0(.input1(Input1), .input2(Input2),
           .aluCtr(ALUCtr), .zero(Zero),
           .aluRes(ALURes));
    
    initial begin
        // Initialize Inputs
        Input1 = 0;
        Input2 = 0;
        ALUCtr = 4'b0000;
        
        // Wait 100 ns for global reset to finish
        #100;
        
        // testing and
        Input1 = 15;
        Input2 = 10;
        ALUCtr = 4'b0000;
        #100;
        
        // testing or
        Input1 = 15;
        Input2 = 10;
        ALUCtr = 4'b0001;
        #100;
        
        // testing add
        Input1 = 15;
        Input2 = 10;
        ALUCtr = 4'b0010;
        #100;
        
        // testing sub 1
        Input1 = 15;
        Input2 = 10;
        ALUCtr = 4'b0110;
        #100;
        
        // testing sub 2
        Input1 = 10;
        Input2 = 15;
        ALUCtr = 4'b0110;
        #100;
        
        // testing set on less than 1
        Input1 = 15;
        Input2 = 10;
        ALUCtr = 4'b0111;
        #100;
        
        // testing set on less than 2
        Input1 = 10;
        Input2 = 15;
        ALUCtr = 4'b0111;
        #100;
        
        // testing nor 1
        Input1 = 1;
        Input2 = 1;
        ALUCtr = 4'b1100;
        #100;
        
        // testing nor 2
        Input1 = 16;
        Input2 = 1;
        ALUCtr = 4'b1100;
        #100;
    end
endmodule
