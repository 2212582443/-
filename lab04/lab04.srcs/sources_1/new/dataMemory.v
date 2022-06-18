`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/23 11:43:57
// Design Name: 
// Module Name: dataMemory
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


module dataMemory(
    input Clk,
    input [31:0] address,
    input [31:0] writeData,
    input memWrite,
    input memRead,
    output [31:0] readData
    );
    reg [31:0] ReadData;
    reg [31:0] memFile[0:63];
    integer i;
    
    initial begin
    for(i=0;i<64;i=i+1)
        memFile[i]=0;
    ReadData=0;
    end
    
    always @ (memRead or address)
    begin
        // check if the address is valid
        if (memRead)
        begin
            if(address <= 63)
                ReadData = memFile[address];
            else
                ReadData = 0;
        end
    end
    
    always @ (negedge Clk)
    begin
        if (memWrite && address <= 63)
            memFile[address] = writeData;
    end
    assign readData=ReadData;
    
endmodule
