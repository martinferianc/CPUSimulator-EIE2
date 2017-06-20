#include "mips_cpu_execute.hpp"
/*
EXECUTE
This is a set of functions that executes decoded instructions
Instructions are split according to type and then according to function
They are also tested for invalid instruction formats

1. Test right formatting -> 2. Execute them -> 3. Return cascaded error/success
*/

mips_error mips_execute(mips_cpu_h state, mips_mem_h mem, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc){
	uint32_t type = instruction_data[7];
	switch (type) {
		case 0: return mips_execute_R(state, hi, lo, instruction_data, instruction, pcN, pc);
		case 1: return mips_execute_I(state, mem, instruction_data, instruction, pcN, pc);
		case 2: return mips_execute_J(state, instruction_data, instruction, pcN, pc);
	}
	return mips_InternalError;
}

mips_error mips_execute_R(mips_cpu_h state, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc){
	uint32_t source1 = instruction_data[1];
	uint32_t source2 = instruction_data[2];
	uint32_t destination = instruction_data[3];
	uint32_t shift = instruction_data[4];
	uint32_t function = instruction_data[5];

	switch(function){
		case 0b100001:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data,"ADDU", instruction, pcN, pc);
		break;
		case 0b100101:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data,"OR", instruction, pcN, pc);
		break;
		case 0b100100:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "AND", instruction, pcN, pc);
		break;
		case 0b100110:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "XOR", instruction, pcN, pc);
		break;
		case 0b100011:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SUBU", instruction, pcN, pc);
		break;
		case 0b100010:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SUB", instruction, pcN, pc);
		break;
		case 0b100000:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "ADD", instruction, pcN, pc);
		break;
		case 0b000010:
		if ((source1==0) && (shift<=31))
			return mips_execute_R_X(state, hi, lo, instruction_data, "SRL", instruction, pcN, pc);
		break;
		case 0b000011:
		if ((source1==0) && (shift<=31))
			return mips_execute_R_X(state, hi, lo, instruction_data, "SRA", instruction, pcN, pc);
		break;
		case 0b000111:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SRAV", instruction,pcN, pc);
		break;
		case 0b000110:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SRLV", instruction, pcN, pc);
		break;
		case 0b000000:
		if (source1==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SLL", instruction, pcN, pc);
		break;
		case 0b101011:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SLTU", instruction, pcN, pc);
		break;
		case 0b101010:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SLT", instruction, pcN, pc);
		break;
		case 0b000100:
		if (shift==0)
			return mips_execute_R_X(state, hi, lo, instruction_data, "SLLV", instruction, pcN, pc);
		break;
		case 0b010010:
		if ((source1==0) && (source2==0) && (shift==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MFLO", instruction, pcN, pc);
		break;
		case 0b010000:
		if ((source1==0) && (source2==0) && (shift==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MFHI", instruction, pcN, pc);
		break;
		case 0b010011:
		if ((source2==0) && (destination==0) && (shift==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MTLO", instruction, pcN, pc);
		break;
		case 0b010001:
		if ((source2==0) && (destination==0) && (shift==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MTHI", instruction, pcN, pc);
		break;
		case 0b011010:
		if ((shift==0) && (destination==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "DIV", instruction, pcN, pc);
		break;
		case 0b011011:
		if ((shift==0) && (destination==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "DIVU", instruction, pcN, pc);
		break;
		case 0b011001:
		if ((shift==0) && (destination==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MULTU", instruction, pcN, pc);
		break;
		case 0b011000:
		if ((shift==0) && (destination==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "MULT", instruction, pcN, pc);
		break;
		case 0b001001:
		if ((source2==0) && (shift==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "JALR", instruction, pcN, pc);
		break;
		case 0b001000:
		if ((source2==0) && (shift==0) && (destination==0))
			return mips_execute_R_X(state, hi, lo, instruction_data, "JR", instruction, pcN, pc);
		break;
		default:
		break;
	}
	instruction = "Invalid instruction format";
	return mips_ExceptionInvalidInstruction;
}
mips_error mips_execute_I(mips_cpu_h state, mips_mem_h mem, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc ){
	uint32_t source1 = instruction_data[1];
	uint32_t destination = instruction_data[2];

	switch(instruction_data[0]){
		case 0b001001:
			return mips_execute_I_X(state, mem, instruction_data,"ADDIU", instruction, pcN, pc);
		case 0b001000:
			return mips_execute_I_X(state, mem, instruction_data,"ADDI", instruction, pcN, pc);
		case 0b001100:
			return mips_execute_I_X(state, mem, instruction_data,"ANDI", instruction, pcN, pc);
		case 0b001101:
			return mips_execute_I_X(state, mem, instruction_data,"ORI", instruction, pcN, pc);
		case 0b001110:
			return mips_execute_I_X(state, mem, instruction_data,"XORI", instruction, pcN, pc);
		case 0b001011:
			return mips_execute_I_X(state, mem, instruction_data,"SLTIU", instruction, pcN, pc);
		case 0b001010:
			return mips_execute_I_X(state, mem, instruction_data,"SLTI", instruction, pcN, pc);
		case 0b000100:
			return mips_execute_I_X(state, mem, instruction_data,"BEQ", instruction, pcN, pc);
		case 0b000101:
			return mips_execute_I_X(state, mem, instruction_data,"BNE", instruction, pcN, pc);
		case 0b000001:
			if (destination==0b00001)
				return mips_execute_I_X(state, mem, instruction_data,"BGEZ", instruction, pcN, pc);
			if (destination==0b00000)
				return mips_execute_I_X(state, mem, instruction_data,"BLTZ", instruction, pcN, pc);
			if (destination==0b10000)
				return mips_execute_I_X(state, mem, instruction_data,"BLTZAL", instruction, pcN, pc);
			if (destination==0b10001)
				return mips_execute_I_X(state, mem, instruction_data,"BGEZAL", instruction, pcN, pc);
		break;
		case 0b000111:
		if (destination==0)
			return mips_execute_I_X(state, mem, instruction_data,"BGTZ", instruction, pcN, pc);
		break;
		case 0b000110:
		if (destination==0)
			return mips_execute_I_X(state, mem, instruction_data,"BLEZ", instruction, pcN, pc);
		break;
		case 0b100011:
			return mips_execute_I_X(state, mem, instruction_data,"LW", instruction, pcN, pc);
		case 0b101011:
			return mips_execute_I_X(state, mem, instruction_data,"SW", instruction, pcN, pc);
		case 0b100100:
			return mips_execute_I_X(state, mem, instruction_data,"LBU", instruction, pcN, pc);
		case 0b101000:
			return mips_execute_I_X(state, mem, instruction_data,"SB", instruction, pcN, pc);
		case 0b100000:
			return mips_execute_I_X(state, mem, instruction_data,"LB", instruction, pcN, pc);
		case 0b101001:
			return mips_execute_I_X(state, mem, instruction_data,"SH", instruction, pcN, pc);
		case 0b100001:
			return mips_execute_I_X(state, mem, instruction_data,"LH", instruction, pcN, pc);
		case 0b100101:
			return mips_execute_I_X(state, mem, instruction_data,"LHU", instruction, pcN, pc);
		case 0b001111:
		if (source1==0)
			return mips_execute_I_X(state, mem, instruction_data,"LUI", instruction, pcN, pc);
		break;
		case 0b100010:
			return mips_execute_I_X(state, mem, instruction_data,"LWL", instruction, pcN, pc);
		case 0b100110:
			return mips_execute_I_X(state, mem, instruction_data,"LWR", instruction, pcN, pc);
		default:
		break;
	}
	instruction = "Invalid instruction format";
	return mips_ExceptionInvalidInstruction;
}
mips_error mips_execute_J(mips_cpu_h state, const uint32_t* instruction_data, string& instruction, uint32_t& pcN, uint32_t& pc){

	switch(instruction_data[0]){
		case 0b000010:
			return mips_execute_J_X(state, instruction_data, "JUMP", instruction, pcN, pc);
		case 0b000011:
			return mips_execute_J_X(state, instruction_data, "JAL", instruction, pcN, pc);
		default:
		break;
	}

	instruction = "Invalid instruction format";
	return mips_ExceptionInvalidInstruction;
}
mips_error mips_execute_R_X(mips_cpu_h state, uint32_t& hi, uint32_t& lo, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc){

	stringstream ss;

	mips_error err = mips_ExceptionInvalidInstruction;;
	uint32_t* source1 = new uint32_t;
	uint32_t* source2 = new uint32_t;
	uint32_t destination = instruction_data[3];
	uint32_t shift = instruction_data[4];

	err = mips_cpu_get_register(state, (unsigned)instruction_data[1], source1);
	if (err==mips_Success)
		err = mips_cpu_get_register(state, (unsigned)instruction_data[2], source2);
	if (err!=mips_Success)
		return mips_ExceptionAccessViolation;

	if (op.compare("ADDU")==0)
		err = mips_cpu_set_register(state, destination, *source1 + *source2);

	if (op.compare("SRL")==0)
		err = mips_cpu_set_register(state, destination, uint32_t(*source2) >> shift);

	if (op.compare("SRA")==0)
		err = mips_cpu_set_register(state, destination, int32_t(*source2) >> shift);

	if (op.compare("SRAV")==0)
		err = mips_cpu_set_register(state, destination, int32_t(*source2) >> (*source1 & 0x1F));

	if (op.compare("SRLV")==0)
		err = mips_cpu_set_register(state, destination, uint32_t(*source2) >> (*source1 & 0x1F));

	if (op.compare("SLL")==0)
		err = mips_cpu_set_register(state, destination, *source2 << shift);

	if (op.compare("SLLV")==0)
		err = mips_cpu_set_register(state, destination, *source2 << (*source1 & 0x1F));

	if (op.compare("SUBU")==0)
		err = mips_cpu_set_register(state, destination, *source1 - *source2);

	if (op.compare("SUB")==0){
		if (!subtraction_overflow(*source1,*source2))
			err = mips_cpu_set_register(state, destination, int32_t(*source1) - int32_t(*source2));
		else
			err = mips_ExceptionArithmeticOverflow;
	}

	if (op.compare("AND")==0)
		err = mips_cpu_set_register(state, destination, *source1 & *source2);

	if (op.compare("OR")==0)
		err = mips_cpu_set_register(state, destination, *source1 | *source2);

	if (op.compare("XOR")==0)
		err = mips_cpu_set_register(state, destination, uint32_t(*source1 ^ *source2));

	if(op.compare("SLT")==0)
		err = mips_cpu_set_register(state, destination, int((int32_t(*source1) < int32_t(*source2))));

	if (op.compare("SLTU")==0)
		err = mips_cpu_set_register(state, destination, int(*source1 < *source2));

	if (op.compare("ADD")==0){
		if (!addition_overflow(*source1 ,*source2))
			err = mips_cpu_set_register(state, destination, *source1 + *source2);
		else
			err = mips_ExceptionArithmeticOverflow;
	}
	if (op.compare("MFLO")==0)
		err = mips_cpu_set_register(state, destination, lo);

	if (op.compare("MTLO")==0){
		lo = *source1;
		err = mips_Success;
	}
	if (op.compare("MFHI")==0)
		err = mips_cpu_set_register(state, destination, hi);

	if (op.compare("MTHI")==0){
		hi = *source1;
		err = mips_Success;
	}
	if (op.compare("MULT")==0){
		int64_t result =  int64_t(*source1) * int64_t(*source2);
		lo = uint32_t(result & 0xFFFFFFFF);
		hi = int32_t((result & 0xFFFFFFFF00000000) >> 32);
		err = mips_Success;
	}
	if (op.compare("MULTU")==0){
		uint64_t result =  uint64_t(*source1) * uint64_t(*source2);
		lo = uint32_t(result & 0xFFFFFFFF);
		hi = (uint32_t(result >> 32 ) & 0xFFFFFFFF);
		err = mips_Success;
	}

	if (op.compare("DIV")==0){
		if (*source2!=0) {
			lo = (int32_t(*source1) / int32_t(*source2));
			hi = (int32_t(*source1) % int32_t(*source2));
			err = mips_Success;
		} else {
			err = mips_ExceptionInvalidInstruction;
		}
	}

	if (op.compare("DIVU")==0){
		if (*source2!=0){
			lo = (uint32_t(*source1) / uint32_t(*source2));
			hi = (uint32_t(*source1) % uint32_t(*source2));
			err = mips_Success;
		} else {
			err = mips_ExceptionInvalidInstruction;
		}
	}

	if (op.compare("JR")==0){
		pc = pcN;
		pcN = *source1;
		err = mips_ExceptionBreak;
	}

	if (op.compare("JALR")==0){
		mips_cpu_set_register(state, destination, pc + 8 );
		pc = pcN;
		pcN = *source1;
		err = mips_ExceptionBreak;
	}


	//COUT OUTPUT
	if ((err==mips_Success) || (err == mips_ExceptionBreak)) {
		if ((op.compare("MTHI")==0) || (op.compare("MTLO")==0) || (op.compare("JR")==0))
		ss <<op<<" R["<<instruction_data[1]<<"]";
		if (op.compare("JALR")==0)
		ss <<op<<" R["<<instruction_data[3]<<"] , R["<<instruction_data[1]<<"]";
		if ((op.compare("MFHI")==0) || (op.compare("MFLO")==0))
		ss <<op<<" R["<<instruction_data[3]<<"]";
		if ((op.compare("SLL")==0)  || (op.compare("SRL")==0) || (op.compare("SRA")==0))
		ss <<op<<" R["<<instruction_data[3]<<"] , R["<<instruction_data[2]<<"], #"<<instruction_data[4];
		if ((op.compare("SLLV")==0) || (op.compare("SRAV")==0) || (op.compare("SRLV")==0))
		ss <<op<<" R["<<instruction_data[3]<<"] , R["<<instruction_data[2]<<"] , R["<<instruction_data[1]<<"]";
		if ((op.compare("SLT")==0) || (op.compare("SLTU")==0))
		ss <<op<<" R["<<instruction_data[3]<<"] , R["<<instruction_data[1]<<"] , R["<<instruction_data[2]<<"]";
		if ((op.compare("MULT")==0) || (op.compare("MULTU")==0) || (op.compare("DIV")==0) || (op.compare("DIVU")==0))
		ss <<op<<" R["<<instruction_data[1]<<"] , R["<<instruction_data[2]<<"]";
		if (ss.str().empty())
		ss <<op<<" R["<<instruction_data[3]<<"] , R["<<instruction_data[1]<<"] , R["<<instruction_data[2]<<"]";
		instruction = ss.str();
	} else
		instruction = "Invalid instruction";


	delete source1; delete source2;
	return err;
}
mips_error mips_execute_I_X(mips_cpu_h state, mips_mem_h mem, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc){
	stringstream ss;
	mips_error err = mips_ExceptionInvalidInstruction;

	uint32_t* source2 = new uint32_t;
	uint32_t* source1 = new uint32_t;
	uint32_t destination = instruction_data[2];
	int32_t imm_signed =  int32_t(int16_t(instruction_data[3]));
	uint32_t imm_unsigned = uint32_t(instruction_data[3]);


	err = mips_cpu_get_register(state, (unsigned)instruction_data[1], source1);
	if (err==mips_Success)
	err = mips_cpu_get_register(state, (unsigned)instruction_data[2], source2);
	if (err!=mips_Success)
	return mips_ExceptionAccessViolation;

	if (op.compare("ADDIU")==0)
		err = mips_cpu_set_register(state, destination, *source1 + imm_signed);

	if (op.compare("ADDI")==0){
		if (!addition_overflow(imm_signed,*source1))
			err = mips_cpu_set_register(state, destination, *source1 + imm_signed);
		else
			err = mips_ExceptionArithmeticOverflow;
	}
	if (op.compare("ORI")==0)
	err = mips_cpu_set_register(state, destination, imm_unsigned | *source1);

	if (op.compare("XORI")==0)
	err = mips_cpu_set_register(state, destination, uint32_t(*source1 ^ imm_unsigned));

	if (op.compare("ANDI")==0)
		err = mips_cpu_set_register(state, destination, imm_unsigned & *source1);

	if (op.compare("SLTIU")==0)
	err = mips_cpu_set_register(state, destination, int(uint32_t(*source1) < uint32_t(imm_signed)));

	if (op.compare("SLTI")==0)
	err = mips_cpu_set_register(state, destination, int((int32_t(*source1) < imm_signed)));

	if (op.compare("BEQ")==0){
		if (*source1 == *source2){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}
	if (op.compare("BNE")==0){
		if (*source1 != *source2){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}
	if (op.compare("BGEZ")==0){
		if (int32_t(*source1) >= 0){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}

	if (op.compare("BLEZ")==0){
		if (int32_t(*source1) <= 0){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}

	if (op.compare("BGTZ")==0){
		if (int32_t(*source1) > 0){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}

	if (op.compare("BLTZ")==0){
		if (int32_t(*source1) < 0){
			pc = pcN;
			pcN = int32_t(pcN) + (imm_signed << 2);
			err = mips_ExceptionBreak;
		} else {
			err = mips_Success;
		}
	}

	if (op.compare("BLTZAL")==0){
		if (instruction_data[1]==31) {
			err = mips_ErrorInvalidArgument;
		} else {
			if (int32_t(*source1) < 0){
				mips_cpu_set_register(state, 31, pc + 8);
				pc = pcN;
				pcN = int32_t(pcN) + (imm_signed << 2);
				err = mips_ExceptionBreak;
			} else {
				err = mips_Success;
			}
		}
	}
	if (op.compare("BGEZAL")==0){
		if (instruction_data[1]==31) {
			err = mips_ErrorInvalidArgument;
		} else {
			if (int32_t(*source1) >= 0){
				mips_cpu_set_register(state, 31, pc + 8);
				pc = pcN;
				pcN = int32_t(pcN) + (imm_signed << 2);
				err = mips_ExceptionBreak;
			} else {
				err = mips_Success;
			}
		}
	}

	if (op.compare("LW")==0){
		if ((imm_signed + *source1)%4!=0) {
			err = mips_ExceptionInvalidAlignment;
		} else {
			uint8_t dataOut[4];
			err = mips_mem_read(mem, imm_signed + *source1, 4, dataOut);
			uint32_t mem_value =  dataOut[3] | (uint32_t(dataOut[2]) << 8) | (uint32_t(dataOut[1]) << 16) | (uint32_t(dataOut[0]) << 24);
			err = mips_cpu_set_register(state, destination, mem_value);
		}
	}

	if (op.compare("LBU")==0){
		uint8_t dataOut;
		err = mips_mem_read(mem, imm_signed + *source1, 1, (uint8_t*)&dataOut);
		uint32_t mem_value = uint32_t(dataOut);
		err = mips_cpu_set_register(state, destination, mem_value);
	}

	if (op.compare("LB")==0){
		uint8_t dataOut;
		err= mips_mem_read(mem, imm_signed + *source1, 1, (uint8_t*)&dataOut);
		int32_t mem_value = int8_t(dataOut);
		err = mips_cpu_set_register(state, destination, mem_value);
	}

	if (op.compare("LHU")==0){
		if ((imm_signed + *source1)%2!=0) {
			err = mips_ExceptionInvalidAlignment;
		} else {
			uint16_t dataOut;
			err = mips_mem_read(mem, imm_signed + *source1, 2, (uint8_t*)&dataOut);
			uint32_t mem_value = uint32_t(endian16(dataOut));
			err = mips_cpu_set_register(state, destination, mem_value);
		}
	}

	if (op.compare("LH")==0){
		if ((imm_signed + *source1)%2!=0) {
			err = mips_ExceptionInvalidAlignment;
		} else {
			uint16_t dataOut;
			err = mips_mem_read(mem, imm_signed + *source1, 2, (uint8_t*)&dataOut);
			int32_t mem_value = int32_t(int16_t(endian16(dataOut)));
			err = mips_cpu_set_register(state, destination, mem_value);
		}
	}

	if (op.compare("SW")==0){
		if ((imm_signed + *source1)%4!=0) {
			err = mips_ExceptionInvalidAlignment;
		} else {
			uint32_t value = uint32_t(endian32(*source2));
			err = mips_mem_write(mem, imm_signed + *source1 ,4, (uint8_t*)&value);
		}
	}


	if (op.compare("SB")==0){
		uint8_t mem_value = *source2 & 0xFF;
		err = mips_mem_write(mem, imm_signed + *source1 ,1, (uint8_t*)&mem_value);
	}

	if (op.compare("SH")==0){
		if ((imm_signed + *source1)%2!=0) {
			err = mips_ExceptionInvalidAlignment;
		} else {
			uint16_t mem_value = endian16(uint16_t(*source2 & 0x0000FFFF));
			uint16_t value = uint16_t(mem_value);
			err = mips_mem_write(mem, imm_signed + *source1 ,2, (uint8_t*)&value);
		}
	}

	if (op.compare("LUI")==0)
		err = mips_cpu_set_register(state, destination, imm_signed << 16);

	if (op.compare("LWL")==0){
		uint32_t address = imm_signed + *source1;
		uint8_t length = 4 - int(address % 4);
		uint8_t dataOut[4];
		mips_error err= mips_mem_read(mem, address - int(address % 4), 4, dataOut);
		uint32_t mem_value = uint32_t(dataOut[3]) | (uint32_t(dataOut[2]) << 8) | (uint32_t(dataOut[1]) << 16) | (uint32_t(dataOut[0]) << 24);
		mem_value = uint32_t(mem_value) << (4-length)*8;
		*source2 = *source2 & (uint32_t(0xFFFFFFFF) >> length*8);
		*source2 = (uint32_t(mem_value)) | uint32_t(*source2);
		err = mips_cpu_set_register(state, destination, *source2);
	}

	if (op.compare("LWR")==0){
		uint32_t address = imm_signed + *source1;
		uint8_t length = int(address % 4) + 1;
		uint8_t dataOut[4];
		mips_error err= mips_mem_read(mem, address - int(address % 4), 4, dataOut);
		uint32_t mem_value = uint32_t(dataOut[3]) | (uint32_t(dataOut[2]) << 8) | (uint32_t(dataOut[1]) << 16) | (uint32_t(dataOut[0]) << 24);
		mem_value = uint32_t(mem_value) >> (4-length)*8;
		*source2 = *source2 & (uint32_t(0xFFFFFFFF) << length*8);
		*source2 = (uint32_t(mem_value)) | uint32_t(*source2);
		err = mips_cpu_set_register(state, destination, *source2);
	}

	//COUT OUTPUT
	if ((err==mips_Success) || (err==mips_ExceptionBreak)) {
		if ((op.compare("SB")==0) || (op.compare("SW")==0) || (op.compare("LBU")==0) || (op.compare("LW")==0)
		|| (op.compare("SH")==0) || (op.compare("LH")==0) || (op.compare("LHU")==0) || (op.compare("LB")==0))
		ss <<op<<" R["<<instruction_data[2]<<"],"<<" #"<<instruction_data[3]<<", R["<<instruction_data[1]<<"] ";

		if ((op.compare("BGEZ")==0) || (op.compare("BGEZAL")==0) || (op.compare("BGTZ")==0) || (op.compare("BLEZ")==0) ||
		(op.compare("BLTZAL")==0))
		ss <<op<<" R["<<instruction_data[1]<<"],"<<" #"<<instruction_data[3];

		if((op.compare("LUI")==0))
		ss <<op<<" R["<<instruction_data[2]<<"],"<<" #"<<instruction_data[3];

		if ((op.compare("BEQ")==0) || (op.compare("BNE")==0))
		ss <<op<<" R["<<instruction_data[1]<<"] , R["<<instruction_data[2]<<"], "<<"#"<<instruction_data[3];

		if (ss.str().empty())
		ss <<op<<" R["<<instruction_data[2]<<"] , R["<<instruction_data[1]<<"], "<<"#"<<instruction_data[3];
		instruction = ss.str();
	} else {
		instruction = "Invalid instruction format";
	}

	delete source1;
	return err;
}
mips_error mips_execute_J_X(mips_cpu_h state, const uint32_t* instruction_data, const string& op, string& instruction, uint32_t& pcN, uint32_t& pc){

	stringstream ss;
	mips_error err = mips_ExceptionInvalidInstruction;
	uint32_t imm_unsigned = uint32_t(instruction_data[1]);

	if (op.compare("JUMP")==0){
		pc = pcN;
		pcN = (pc & 0xF0000000) | (imm_unsigned << 2);
		err = mips_ExceptionBreak;
	}

	if (op.compare("JAL")==0){
		mips_cpu_set_register(state, 31, pc + 8);
		pc = pcN;
		pcN = (pc & 0xF0000000) | (imm_unsigned << 2);
		err = mips_ExceptionBreak;
	}

	ss <<op<<" "<<"#"<<instruction_data[1];

	instruction = ss.str();
	return err;
}
