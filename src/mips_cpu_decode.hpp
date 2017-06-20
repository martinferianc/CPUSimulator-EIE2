/*
DECODE
This set of functions perform the DECODE operation
They accept the instruction, decode the internals
and separate the bits into individual fields, agreed beforehand
3 functions - 3 types: R, I, J

Stores the result int the array of uint32_ts
padds if no value to be stored

instruction_data organization according to index instruction_data[index]
----------------------------------------------------------------------------------------------------------------------------
R TYPE: [OPCODE[0] | SOURCE1[1]   | SOURCE2[2]            | DESTINATION[3] | SHIFT[4] | FUNCTION[5] | SIZE[6]=6 |TYPE_4[7]=0]
I TYPE: [OPCODE[0] | SOURCE1[1]   | SOURCE2[2]/DESINATION | IMMEDIATE[3]   | EMPTY[4] | EMPTY[5]    | SIZE[6]=4 |TYPE_4[7]=1]
J TYPE: [OPCODE[0] | IMMEDIATE[1] | EMPTY[2]              | EMPTY[3]       | EMPTY[4] | EMPTY[5]    | SIZE[6]=2 |TYPE_4[7]=2]
----------------------------------------------------------------------------------------------------------------------------
*/
#include <iostream>
#include "mips.h"

using namespace std;

mips_error mips_decode(const uint32_t& instruction, uint32_t* instruction_data);
mips_error mips_decode_R(const uint32_t& instruction, uint32_t* instruction_data);
mips_error mips_decode_I(const uint32_t& instruction, uint32_t* instruction_data);
mips_error mips_decode_J(const uint32_t& instruction, uint32_t* instruction_data);
