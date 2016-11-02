/*
DECODE
This set of functions perform the DECODE operation
They accept the instruction, decode the internals
and separate the bits into individual fields, agreed beforehand
3 functions - 3 types: R, I, J
*/

#include "mips_cpu_decode.hpp"

using namespace std;

//Initial function that determines the TYPE of instruction
mips_error mips_decode(const uint32_t& instruction, uint32_t* instruction_data){
	uint32_t opcode = (instruction >> 2 ) & 0x3F;
	if (opcode==0)
		return mips_decode_R(instruction, instruction_data);
	if ((opcode==2) || (opcode==3) || (opcode==26))
		return mips_decode_J(instruction, instruction_data);
	return mips_decode_I(instruction, instruction_data);
}
//DECODE R TYPE
mips_error mips_decode_R(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 2) & 0x3F; 																		 //OPcode
	instruction_data[1] = (uint32_t)(((instruction >> 13) & 0x7) | ((instruction << 3) & 0x18)) ;  //Source 1
	instruction_data[2] = (uint32_t)(instruction >> 8) & 0x1F; 																		 //Source 2
	instruction_data[3] = (uint32_t)(instruction >> 19) & 0x1F; 																	 //Destination
	instruction_data[4] = (uint32_t)(((instruction >> 14) & 0x1C) | ((instruction >> 30) & 0x3)) ; //Shift
	instruction_data[5] = (uint32_t)(instruction >> 24) & 0x3F; 																	 //Function
	instruction_data[6] = 6; 																																			 //Relevant data size: 6
	instruction_data[7] = 0; 																																			 //R-type
	return mips_Success;
}
//DECODE I TYPE
mips_error mips_decode_I(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 2) & 0x3F; 																		  //OPcode
	instruction_data[1] = (uint32_t)(((instruction << 3) & 0x18) | ((instruction >> 13) & 0x7)) ;   //Source 1
	instruction_data[2] = (uint32_t)(instruction >> 8) & 0x1F; 																			//Destination
	instruction_data[3] = (uint32_t)(((instruction >> 24) & 0xFF) | ((instruction >> 8) & 0xFF00)); //Immediate Constant
	instruction_data[4] = 0; 																																				//Leave Blank
	instruction_data[5] = 0; 																																				//Leave Blank
	instruction_data[6] = 4; 																																				//Relevant data size: 4
	instruction_data[7] = 1; 																																				//I-type
	return mips_Success;
}
//DECODE J TYPE
mips_error mips_decode_J(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 2)  & 0x3F; 																		//OPcode
	instruction_data[1] =
		(uint32_t)(((instruction << 22) & 0xC000000) |
		((instruction >> 24) & 0xFF) |
		((instruction >> 8) & 0xFF00) |
		((instruction << 8) & 0xFF0000)); 																		  											//Memory address
	instruction_data[2] = 0; 																																				//Leave Blank
	instruction_data[3] = 0; 																																				//Leave Blank
	instruction_data[4] = 0; 																																				//Leave Blank
	instruction_data[5] = 0; 																																				//Leave Blank
	instruction_data[6] = 2; 																																				//Relevant data size: 2
	instruction_data[7] = 2; 																																			  //J-type
	return mips_Success;
}
