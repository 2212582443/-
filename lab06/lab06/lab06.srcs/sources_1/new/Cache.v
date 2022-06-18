`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/06/06 12:52:13
// Design Name: 
// Module Name: Cache
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


module Cache(
    input clk,
    input [31:0] address,
    input [31:0] writeData,
    input memWrite,
    input memRead,
    output [31:0] readData
    );
    reg[31:0] cacheFile[0:63];
    reg validBit[0:15];
    reg[25:0] tag[0:15];

    reg[31:0] ReadData;

    wire[127:0] dataFromMemFile;

    wire[3:0] cacheAddr = address[5:2];
    wire[31:0] MemFileAddress = {address[31:2],2'b00};
    
    dataMemory mem(
        .clk(clk),
        .address(MemFileAddress),
        .writeData(writeData),
        .memWrite(memWrite),
        .memRead(memRead),
        .readData(dataFromMemFile)
    );

    integer i;
    initial 
    begin
        for(i=0;i<16;i=i+1)
            validBit[i] = 0;
            tag[i] = 0;
        for(i=0;i<64;i=i+1)
            cacheFile[i] = 0;
    end

    always @(memRead or address or memWrite) 
    begin
        if(memRead)
        begin
            if(validBit[cacheAddr] && tag[cacheAddr] == address[31:6])
                ReadData = cacheFile[address[5:0]];
            else begin
                
                cacheFile[{cacheAddr,2'b00}] = dataFromMemFile[127:96];
                if(address[1:0]==2'b00) ReadData=dataFromMemFile[127:96];
                
                cacheFile[{cacheAddr,2'b01}] = dataFromMemFile[95:64];
                if(address[1:0]==2'b01) ReadData=dataFromMemFile[95:64];
                
                cacheFile[{cacheAddr,2'b10}] = dataFromMemFile[63:32];
                if(address[1:0]==2'b10) ReadData=dataFromMemFile[63:32];
                
                cacheFile[{cacheAddr,2'b11}] = dataFromMemFile[31:0];
                if(address[1:0]==2'b11) ReadData=dataFromMemFile[31:0];
                
                validBit[cacheAddr] = 1;
                tag[cacheAddr] = address[31:6];
            end
        end
    end

    always @(negedge clk)
    begin
        if(memWrite)
            validBit[cacheAddr] = 0;
    end

    assign readData = ReadData;
endmodule