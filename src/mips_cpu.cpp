#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "mips.h"
#include "mips_cpu_decode.hpp"
#include "mips_cpu_execute.hpp"

#define NDEBUG

using namespace std;

//Debug functions
void debug_level_1(const mips_cpu_h& state, const string& instruction);
void debug_level_2(const mips_cpu_h& state, const string& instruction);
void debug_level_3(const mips_cpu_h& state, const string& instruction, FILE* file);

//CPU IMPLEMENT - registers, program counter, program counter new, debug level, debug destination, memory, hi, lo
struct mips_cpu_impl{
	uint32_t pc;
	uint32_t hi;
	uint32_t lo;
	uint32_t pcN;
	uint32_t regs[32];
	unsigned level;
	FILE* dest;
	mips_mem_h mem;
};


//CPU CREATE - creates the CPU
mips_cpu_h mips_cpu_create(mips_mem_h mem){
	if (mem==0)
		return NULL;

	mips_cpu_h state = new mips_cpu_impl;
	state->hi = 0;
	state->lo = 0;
	state->pc = 0;
	state->pcN = 4;
	state->mem=mem;
	for (unsigned i = 0; i < 32; ++i)
		state->regs[i]=0;

	return state;
}
//CPU RESET - resets the CPU, rases program counter and registers
mips_error mips_cpu_reset(mips_cpu_h state){
	if (state==0)
		return mips_ErrorInvalidHandle;

	state->pc = 0;
	state->pcN = 4;
	state->hi = 0;
	state->lo = 0;
	for (unsigned i = 0; i < 32; ++i)
		state->regs[i]=0;

	return mips_Success;
}
//CPU GET - fetches a value from a register
mips_error mips_cpu_get_register(mips_cpu_h state, unsigned index, uint32_t* value){
	if(state==0)
		return mips_ErrorInvalidHandle;

	if(index>31)
		return mips_ErrorInvalidArgument;

	*value = state->regs[index];
	return mips_Success;
}

//CPU SET - sets a register to a value
mips_error mips_cpu_set_register(mips_cpu_h state, unsigned index, uint32_t value){
	if(state==0)
		return mips_ErrorInvalidHandle;

	if(index>31)
		return mips_ErrorInvalidArgument;

	if(index!=0)
		state->regs[index]=value;

	return mips_Success;
}
//CPU SET PC - sets the program counter
mips_error mips_cpu_set_pc(mips_cpu_h state, uint32_t pc){
	if(state==0)
		return mips_ErrorInvalidHandle;

	state->pc = pc;
	state->pcN = pc + 4;
	return mips_Success;
}
//CPU GET PC - sets the program counter
mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc){
	if(state==0)
		return mips_ErrorInvalidHandle;

	*pc = state->pc;
	return mips_Success;
}

//CPU STEP - steps the program counter
mips_error mips_cpu_step(mips_cpu_h state){
	if(state==0)
		return mips_ErrorInvalidHandle;

	uint32_t* instruction_data = new uint32_t[8];
	string instruction;
	mips_error err;
	uint32_t mem_value;
	uint8_t dataOut[4];

	//FETCH
	if (state->pc % 4 == 0){
		err = mips_mem_read(state->mem, state->pc, 4, dataOut);
		mem_value = dataOut[0] | (uint32_t(dataOut[1]) << 8) | (uint32_t(dataOut[2]) << 16) | (uint32_t(dataOut[3]) << 24);
	} else {
		err = mips_ExceptionInvalidAlignment;
	}

	//DECODE
	if (err == mips_Success)
		err = mips_decode(mem_value, instruction_data);

	//EXECUTE
	if (err == mips_Success)
		err = mips_execute(state, state->mem, state->hi, state->lo, instruction_data, instruction, state->pcN, state->pc);

	//IF SUCCESS increase PC to new value and not JUMP or BRANCH
	if (err == mips_Success){
		state->pc = state->pcN;
		state->pcN = state->pcN + 4;
	}
	//IF JUMP/BRANCH - PC, PCN adjusted in execution
	if (err == mips_ExceptionBreak)
		err = mips_Success;

	//DEBUG
	switch(state->level){
		case 1:
			debug_level_1(state, instruction);
		break;

		case 2:
			debug_level_2(state, instruction);
		break;

		case 3:
			debug_level_3(state, instruction, state->dest);
		break;

		default:
		break;
	}

	delete [] instruction_data;
	return err;
}
//CPU - SET DEBUG LEVEL
mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest){
	if ((level>=4) || (state ==0))
		return mips_ErrorInvalidArgument;
	state->level = level;
	state->dest = dest;
	return mips_Success;
}
void debug_level_1(const mips_cpu_h& state, const string& instruction){
	cout<<endl<<"Executed instruction: "<<instruction<<endl;
	cout<<"Next PC:="<<state->pc<<endl;
}
void debug_level_2(const mips_cpu_h& state, const string& instruction){
	cout<<endl<<"Executed instruction: "<<instruction<<endl;
	cout<<"Next PC:="<<state->pc<<endl;
	cout<<"HI: "<<state->hi<<" LO: "<<state->lo<<endl;
	for (int i = 0; i < 32; ++i){
		if (i % 4 == 0)
			cout<<endl;
		cout<<"R["<<i<<"]"<<":= "<<state->regs[i]<<"\t";
	}
	cout<<endl;
}
void debug_level_3(const mips_cpu_h& state, const string& instruction, FILE* file){
	if (file == NULL) perror ("Error opening file");
	fprintf(state->dest,"Executed instruction: %s", instruction.c_str());
	fprintf(state->dest,"Next PC:= %i \n", state->pc);
	for (int i = 0; i < 32; ++i){
		if (i % 4 == 0)
			fprintf(state->dest,"\n");
		fprintf(state->dest,"R[%i]:= %i \t",i, state->regs[i]);
	}
	cout<<endl;
}
//CPU FREE - releases the CPU
void mips_cpu_free(mips_cpu_h state){
	if(state==0)
		return;
	free(state);
}
