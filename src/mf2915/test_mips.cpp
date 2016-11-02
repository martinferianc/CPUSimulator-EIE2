/*
TEST
This is a fully automated test bench to test mips cpu
The tests are loaded from "mips_cpu_instructions.txt" to a vector and then to memory
Please do not forget "mips_cpu_instructions.txt", do not change the order of instructions !
Tests BRANCHES, memory LOAD and STORE, INVALID instructions, VALID instructions

The tests in "mips_cpu_instructions.txt" are organized:

[INSTRUCTION | LABEL | VALUE R2 | VALUE R3 | EXPECTED RESULT]

*/

#include "mips.h"
#include "mips_test.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//Struct to hold and parse the test parameters included in the file
struct test{
  uint32_t instruction_code;
  string instruction_name;
  uint32_t instruction_result;
  uint32_t instruction_r1;
  uint32_t instruction_r2;
  string str(){
    stringstream ss;
    ss<<std::hex<<instruction_code<<" "
    <<instruction_name<<" "
    <<" R2: "<<instruction_r1
    <<" R3: "<<instruction_r2
    <<" Ex. Result: "<<instruction_result;
    return ss.str();
  }
};
//Loads the instruction vector from mips_cpu_instructions.txt, change endianess and load into memory
mips_error load_instructions(vector<test>& instructions){
  ifstream file;
  file.open("mips_cpu_instructions.txt");
  test temp;
  string t1, t2, t3, t4, t5;
  while(file >> t1 >> t2 >> t3 >> t4 >> t5){
    stringstream ss;
    ss << std::hex << t1;
    ss >> temp.instruction_code;
    //Change the endianness
    temp.instruction_code =
      uint32_t((temp.instruction_code & 0x000000FF) << 24) |
      uint32_t((temp.instruction_code & 0x0000FF00) << 8) |
      uint32_t((temp.instruction_code & 0x00FF0000) >> 8) |
      uint32_t((temp.instruction_code & 0xFF000000) >> 24);
    temp.instruction_name = t2;
    temp.instruction_r1 = atoi(t3.c_str());
    temp.instruction_r2 = atoi(t4.c_str());
    temp.instruction_result = atoi(t5.c_str());
    instructions.push_back(temp);
  }
  file.close();
  return mips_Success;
}

//Loads the memory with predefined instructions
mips_error load_memory(mips_mem_h& mem, const vector<test> instructions){
  mips_error err = mips_Success;
  for (int i = 0; i < (int)instructions.size(); ++i){
    err = mips_mem_write(mem, 0+(4*i),4, (uint8_t*)&instructions[i].instruction_code);
    if (err!=mips_Success)
    return err;
  }
  return err;
}
//Print the array of registers
void print_registers(const uint32_t *array){
  for (size_t i = 0; i < 32; i++) {
    if (i % 4 == 0)
    cout<<endl;
    cout<<"R["<<i<<"]"<<":= "<<array[i]<<"\t";
  }
  cout<<endl;
}
//Load the registers from the CPU
void load_registers(uint32_t* array, mips_cpu_h cpu){
  for (size_t i = 0; i < 32; i++) {
    uint32_t* reg = new uint32_t;
    mips_cpu_get_register(cpu, i, reg);
    array[i] = uint32_t(*reg);
    delete reg;
  }
}

int main(){
  mips_test_begin_suite();
  vector<test> instructions;
  mips_error err;

  //Test #1 if the cpu was created correctly
  mips_mem_h mem = mips_mem_create_ram(4096);
  mips_cpu_h cpu = mips_cpu_create(mem);
  int testId = mips_test_begin_test("<INTERNAL>");
  if (cpu!=0){
    mips_test_end_test(testId, true, "CPU Created");
  } else{
    mips_test_end_test(testId, false, "CPU Creation error");
    mips_test_end_suite();
  }
  //ENDTEST

  //Test #2 writing to register 0
  testId = mips_test_begin_test("<INTERNAL>");
  err = mips_cpu_set_register(cpu, 0, 1);
  uint32_t *r0 = new uint32_t;
  mips_cpu_get_register(cpu, 0, r0);
  if ((err!=mips_Success) || (*r0==0))
    mips_test_end_test(testId, true, "R[0] unchanged");
  else
    mips_test_end_test(testId, false, "R[0] changed");
  delete r0;
  //ENDTEST

  //Test #3 checking if registers are 0
  testId = mips_test_begin_test("<INTERNAL>");
  uint32_t *v = new uint32_t;
  for (int i = 0; i < 32; ++i){
    mips_cpu_get_register(cpu,i,v);
    if (v!=0)
    break;
  }
  if (v!=0)
    mips_test_end_test(testId, true, "Registers are zero");
  else
    mips_test_end_test(testId, false, "Registers are non-zero");
  *v=0;
  //ENDTEST


  //Test #4 Accessing invalid index
  testId = mips_test_begin_test("<INTERNAL>");
  err = mips_cpu_get_register(cpu, 32, v);
  if (err!=mips_Success)
    mips_test_end_test(testId, true, "Register could not be loaded");
  else
    mips_test_end_test(testId, false, "Register index error");
  //ENDTEST

  //Test #5 Test the CPU RESET
  testId = mips_test_begin_test("<INTERNAL>");
  mips_cpu_reset(cpu);
  for (int i = 0; i < 32; ++i){
    mips_cpu_get_register(cpu,i,v);
    if (v!=0)
    break;
  }
  if (v!=0)
    mips_test_end_test(testId, true, "Registers are zero");
  else
    mips_test_end_test(testId, false, "Registers are non-zero");
  delete v;

  //////////TEST SUITE//////////////////
  //ALWAYS R1(result), R2, R3(operands)

  //mips_cpu_set_debug_level(cpu, 2, NULL);

  err = load_instructions(instructions);
  err = load_memory(mem, instructions);
  if (err!=mips_Success){
    cout<<"!   (Error) Instructions could not be loaded from the file please check mips_cpu_instructions.txt"<<endl;
    mips_test_end_suite();
    mips_cpu_free(cpu);
    mips_mem_free(mem);
    return 0;
  }

  int skip_count = 0; //To skip the SKIP parts
  for (uint32_t i = 0; i < instructions.size(); ++i){
    uint32_t *pc = new uint32_t;
    mips_cpu_get_pc(cpu,pc);
    if (instructions[i].instruction_code==0){ //In case the CPU tries to execute the skip
      if (*pc==i*4)
        mips_cpu_set_pc(cpu,*pc+4);
      skip_count++;
      continue;
    }
    uint32_t regs_before [32];
    uint32_t regs_after [32];

    bool success = false;
    int testId = mips_test_begin_test(instructions[i].instruction_name.c_str());

    mips_cpu_set_register(cpu, 2, instructions[i].instruction_r1);
    mips_cpu_set_register(cpu, 3, instructions[i].instruction_r2);
    load_registers(regs_before, cpu);

    err = mips_cpu_step(cpu);
    load_registers(regs_after, cpu);

    mips_cpu_get_pc(cpu,pc);
    switch(instructions[i].instruction_name[0]){
      case 'B':
        //Branch should be taken, expected result == 1, pc changed in the next instruction
        if ((*pc==(i+1)*4) && (instructions[i].instruction_result==1) && (err==mips_Success)){
          if (instructions[i].instruction_name.length() > 4) {
            if(regs_after[31] != (i+2)*4)
            break;
          }
          err = mips_cpu_step(cpu); //Step to the instruction from where branch is performed
          if (err!=mips_Success)
            break;

          mips_cpu_get_pc(cpu,pc);
          if ((*pc==(i+3)*4) && (err==mips_Success)) //Branches always by 2
            success = true;

          break;
        }
        //Branch should not be taken, expected result == 0, pc = pc + 4
        if ((*pc==(i+1)*4) && (instructions[i].instruction_result==0) && (err==mips_Success)){

          err = mips_cpu_step(cpu); //Step to the instruction from where branch is performed
          if (err!=mips_Success)
            break;

          mips_cpu_get_pc(cpu,pc);

          if ((*pc==(i+2)*4) && (err==mips_Success))
            success = true;

          break;
          }

      case '<':
        if (err!=mips_Success)
          success = true;
        break;

      case 'J':
        if ((*pc==(i+1)*4) && (err==mips_Success)){
          if (instructions[i].instruction_name.length() >= 3) {
            if(regs_after[31] != (i+2)*4)
            break;
          }
          err = mips_cpu_step(cpu);
          if (err!=mips_Success)
            break;

          err = mips_cpu_get_pc(cpu,pc);
          if ((*pc==(i+3)*4) && (err==mips_Success))
            success = true;

          break;
        }
      break;
      default:

      //Test all other instructions
      if (regs_after[1] == instructions[i].instruction_result)
        success = true;

    }
    if (success){
      mips_test_end_test(testId, true, instructions[i].instruction_name.c_str());
      if (err==mips_ExceptionArithmeticOverflow)
        mips_cpu_set_pc(cpu, *pc + 4);

      switch (instructions[i].instruction_name[0]) {
        case '<':
          //Manually adjust the PC because the instruction resulted in an error
          mips_cpu_set_pc(cpu, *pc + 4);
          break;

        case 'B':
          if (instructions[i].instruction_result==1) {
            i=i+2;
            skip_count=skip_count+2;
          } else {
            i=i+1;
            skip_count=skip_count+1;
          }
          break;

        case 'J':
          i=i+2;
          skip_count=skip_count+2;
          break;
      }
    } else {
      cout<<"-----------------------------------------------------------------------"<<endl;
      cout<<"| Test: "<<i+1-skip_count<<"| To memory: "<<instructions[i].str()<<"|"<<endl;
      cout<<"-----------------------REGISTERS BEFORE-------------------------------"<<endl;
      print_registers(regs_before);
      cout<<endl<<"----------------------REGISTERS AFTER---------------------------"<<endl;
      print_registers(regs_after);
      mips_cpu_get_pc(cpu,pc);
      mips_test_end_test(testId, false, instructions[i].instruction_name.c_str());
      switch (instructions[i].instruction_name[0]) {
        case '<':
          cout<<endl<<"!   (Error) Instruction should have not been executed: Instruction is invalid!"<<endl;
          if (err != mips_Success)
            mips_cpu_set_pc(cpu, *pc + 4);
        break;

        case 'B':
          if (instructions[i].instruction_result==1){
            if (instructions[i].instruction_name.length() > 4) {
              if(regs_after[31] != (i+2)*4)
                cout<<endl<<"!   (Error) PC + 8 not in R[31]:"<<regs_after[31]<<" PC Awaited: "<<(i+3)*4<<endl;
            }
            cout<<endl<<"!   (Error) PC: "<<*pc<<" PC Awaited: "<<(i+3)*4<<endl;
          }else{
            cout<<endl<<"!   (Error) PC: "<<*pc<<" PC Awaited: "<<(i+1)*4<<endl;
          }
          mips_cpu_set_pc(cpu, (i+3)*4);
          i=i+2;
          skip_count=skip_count+2;
        break;

        case 'J':
          cout<<endl<<"!   (Error) PC: "<<*pc<<" PC Awaited: "<<(i+2)*4<<endl;
          mips_cpu_set_pc(cpu, (i+3)*4);
          i=i+2;
          skip_count=skip_count+2;
        break;

        default:
          cout<<endl<<"!   (Error) Got:"<<regs_after[1]<<" Actual Result: "<<instructions[i].instruction_result<<endl;
          if (err != mips_Success)
            mips_cpu_set_pc(cpu, *pc + 4);
        break;
      }
      cout<<"!    Going to the next memory location PC + 4:"<<endl;
      cout<<"-----------------------------------------------------"<<endl;

      delete pc;
    }
  }
  //////////END TEST SUITE//////////////////
  mips_test_end_suite();
  mips_cpu_free(cpu);
  mips_mem_free(mem);
  return 0;
}
