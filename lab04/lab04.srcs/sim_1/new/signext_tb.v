`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/23 12:16:55
// Design Name: 
// Module Name: signext_tb
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


module signext_tb(

    );
    reg [15 : 0] Inst;
    wire [31 : 0] Data;
    
    signext u0(.inst(Inst), .data(Data));
    
    initial begin
        // Initialize Input
        Inst = 0;
        
        // Current Time: 100 ns
        #100;
        Inst = 1;
        
        // Current Time: 200 ns
        #100;
        Inst = 16'b1111111111111111;
        
        // Current Time: 300 ns
        #100;
        Inst = 2;
        
        // Current Time: 400 ns
        #100;
        Inst = 16'b1111111111111110;
        
        // Current Time: 500 ns
        #100;
        Inst = 16'b1101001001010011;
        
        // Current Time: 600 ns
        #100;
        Inst = 16'b0100101011010111;
        
        // Current Time: 700 ns
        #100;
        Inst = 16'b1111111011111111;
        
        // Current Time: 800 ns
        #100;
        Inst = 16'b0000010000100000;
        
        // Current Time: 900 ns
        #100;
        Inst = 16'b0010010010010010;
    end
endmodule
