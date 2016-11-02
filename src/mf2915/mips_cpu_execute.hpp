#include <iostream>
#include "mips.h"
#include "mips_cpu_execute_help.hpp"
#include "mips_mem.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

/*
EXECUTE
This is a set of functions that executes decoded instructions
Instructions are split according to type and then according to function

instruction_data organization according to index instruction_data[index]
----------------------------------------------------------------------------------------------------------------------------
R TYPE: [OPCODE[0] | SOURCE1[1]   | SOURCE2[2]            | DESTINATION[3] | SHIFT[4] | FUNCTION[5] | SIZE[6]=6 |TYPE_4[7]=0]
I TYPE: [OPCODE[0] | SOURCE1[1]   | SOURCE2[2]/DESINATION | IMMEDIATE[3]   | EMPTY[4] | EMPTY[5]    | SIZE[6]=4 |TYPE_4[7]=1]
J TYPE: [OPCODE[0] | IMMEDIATE[1] | EMPTY[2]              | EMPTY[3]       | EMPTY[4] | EMPTY[5]    | SIZE[6]=2 |TYPE_4[7]=2]
----------------------------------------------------------------------------------------------------------------------------

*/

mips_error mips_execute(mips_cpu_h state, mips_mem_h mem, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc);
mips_error mips_execute_R(mips_cpu_h state, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc);
mips_error mips_execute_I(mips_cpu_h state, mips_mem_h mem, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc);
mips_error mips_execute_J(mips_cpu_h state, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc);

mips_error mips_execute_R_X(mips_cpu_h state, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc);
mips_error mips_execute_I_X(mips_cpu_h state, mips_mem_h mem, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc);
mips_error mips_execute_J_X(mips_cpu_h state, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc);
