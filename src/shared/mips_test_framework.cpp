/* This file is an implementation of the functions
   defined in mips_test.h. It is designed to be
   linked against something which implements the
   functions from mips_cpu.h and mips_mem.h, plus
   some sort of main program to run the tests.
*/
#include "mips_test.h"

#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <string> 

static bool sg_started=false;

struct test_info_t
{
    int testId;
    std::string instruction;
    int status;
    std::string message;
};

static std::vector<test_info_t> sg_tests;

struct instr_info_t
{
    const char *instruction;
    const char *description;
};

static const instr_info_t sg_instructionsArray[]=
{
    {"<INTERNAL>", "Tests of things other than intructions."},
    {"ADD","Add (with overflow)"},
    {"ADDI","Add immediate (with overflow)"},
    {"ADDIU","Add immediate unsigned (no overflow)"},
    {"ADDU","Add unsigned (no overflow)"},
    {"AND","Bitwise and"},
    {"ANDI","Bitwise and immediate"},
    {"BEQ","Branch on equal"},
    {"BGEZ","Branch on greater than or equal to zero"},
    {"BGEZAL","Branch on greater than or equal to zero and link"},
    {"BGTZ","Branch on greater than zero"},
    {"BLEZ","Branch on less than or equal to zero"},
    {"BLTZ","Branch on less than zero"},
    {"BLTZAL","Branch on less than zero and link"},
    {"BNE","Branch on not equal"},
    {"DIV","Divide"},
    {"DIVU","Divide unsigned"},
    {"J","Jump"},
    {"JAL","Jump and link"},
    {"JALR","Jump and link register"},
    {"JR","Jump register"},
    {"LB","Load byte"},
    {"LBU","Load byte unsigned"},
    {"LH","Load half-word"},
    {"LHU","Load half-word unsigned"},
    {"LUI","Load upper immediate"},
    {"LW","Load word"},
    {"LWL","Load word left"},
    {"LWR","Load word right"},
    {"MFHI","Move from HI"},
    {"MFLO","Move from LO"},
    {"MTHI","Move to HI"},
    {"MTLO","Move to LO"},
    {"MULT","Multiply"},
    {"MULTU","Multiply unsigned"},
    {"OR","Bitwise or"},
    {"ORI","Bitwise or immediate"},
    {"SB","Store byte"},
    {"SH","Store half-word"},
    {"SLL","Shift left logical"},
    {"SLLV","Shift left logical variable"},
    {"SLT","Set on less than (signed)"},
    {"SLTI","Set on less than immediate (signed)"},
    {"SLTIU","Set on less than immediate unsigned"},
    {"SLTU","Set on less than unsigned"},
    {"SRA","Shift right arithmetic"},
    {"SRAV","Shift right arithmetic variable"},
    {"SRL","Shift right logical"},
    {"SRLV","Shift right logical variable"},
    {"SUB","Subtract"},
    {"SUBU","Subtract unsigned"},
    {"SW","Store word"},
    {"XOR","Bitwise exclusive or"},
    {"XORI","Bitwise exclusive or immediate"}
};
static const unsigned sg_instructionsCount = sizeof(sg_instructionsArray)/sizeof(sg_instructionsArray[0]);

static std::set<std::string> sg_knownInstructions;


extern "C" void mips_test_begin_suite()
{
    if(sg_started){
        fprintf(stderr, "Error:mips_test_begin_suite - Test suite has already been started\n");
        exit(1);
    }
    
    // Build up a list of known instruction names
    for(unsigned i=0; i<sg_instructionsCount; i++){
        sg_knownInstructions.insert(std::string(sg_instructionsArray[i].instruction));
    }
    
    sg_started=true;
}
  
extern "C" int mips_test_begin_test(const char *instruction)
{
    if(!sg_started){
        fprintf(stderr, "Error:mips_test_begin_test - Test suite has not been started with mips_test_begin_suite.\n");
        exit(1);
    }
    
    if(sg_tests.size()>0){
        if(sg_tests.back().status == -1){
            fprintf(stderr, "Error:mips_test_begin_test - Attempt to start new test of '%s', but previous test with id %u has not been completed.\n", instruction, sg_tests.back().testId);
            exit(1);
        }
    }
    
    int testId=sg_tests.size();
    
    test_info_t info;
    info.testId=testId;
    
    info.instruction=instruction; // We want the string in upper case (shouting!)
    std::transform(info.instruction.begin(), info.instruction.end(), info.instruction.begin(), ::toupper);
    
    if(sg_knownInstructions.find(info.instruction)==sg_knownInstructions.end()){
        fprintf(stderr, "Warning:mips_test_begin_test - Unknown instruction '%s', might want to check the spelling.\n", instruction);
    }
    
    info.status=-1;
    sg_tests.push_back(info);
    
    return testId;
}

extern "C" void mips_test_end_test(int testId, int passed, const char *msg)
{
    if(!sg_started){
        fprintf(stderr, "Error:mips_test_finish_test - Test suite has not been started with mips_test_begin_suite.");
        exit(1);
    }
    
    if(sg_tests.size()==0){
        fprintf(stderr, "Error:mips_test_finish_test - No tests have been started.\n");
        exit(1);
    }
    if(sg_tests.back().testId!=testId){
        fprintf(stderr, "Error:mips_test_finish_test - Attempt to finish test %u, but last test started was %u.\n", testId, sg_tests.back().testId);
        exit(1);
    }
    if(sg_tests.back().status!=-1){
        fprintf(stderr, "Error:mips_test_finish_test - Attempt to finish test %u, but it already finished with status %u.\n", testId, sg_tests.back().status);
        exit(1);  
    }
    
    sg_tests.back().status=passed ? 1 : 0;
    if(msg){
        sg_tests.back().message=msg;
    }
}


extern "C" void mips_test_end_suite()
{
    if(!sg_started){
        fprintf(stderr, "Error:mips_test_finish_suite - Test suite has not been started with mips_test_begin_suite.\n");
        exit(1);
    }
    if(sg_tests.size()==0){
        fprintf(stderr, "Error:mips_test_finish_suite - No tests have been executed.\n");
        exit(1);
    }
    if(sg_tests.back().status==-1){
        fprintf(stderr, "Error:mips_test_finish_suite - The final test has not been completed yet.\n");
        exit(1);
    }
    
    // Now we will go through an collect some statistics about what happened
    
    // Build a map from instruction name to a pair of (tests,passed)
    typedef std::map<std::string, std::pair<int,int> > stats_t;
    stats_t statistics;
    
    for(unsigned i=0; i<sg_tests.size(); i++){
        test_info_t info=sg_tests[i];
        
        statistics[info.instruction].first++;   // count all tests
        if(info.status==1){
            statistics[info.instruction].second++;  // count the ones that passed
        }
    }
    
    fprintf(stderr, "\n");
    fprintf(stderr, "| Instruction |  tests | passed | success |\n");
    fprintf(stderr, "+-------------+--------+--------+---------+\n");
    
    // Work out what happened for each instruction
    int totalTested=0;
    int totalNotWorking=0;
    int totalPartiallyWorking=0;
    int totalFullyWorking=0;
    
    stats_t::const_iterator it=statistics.begin();
    while(it!=statistics.end()){
        std::string name=it->first;
        int total=it->second.first;
        int passed=it->second.second;
        
        totalTested++;
        if(passed==0){
            totalNotWorking++;
        }else if(passed<total){
            totalPartiallyWorking++;
        }else{
            totalFullyWorking++;
        }
            
        fprintf(stderr, "|%12s |   %4u |   %4u |  %5.1f%% |\n", name.c_str(), total, passed, 100.0*passed/(double)total);
        
        if(sg_knownInstructions.find(name)==sg_knownInstructions.end()){
            fprintf(stderr, "+ Warning: previous instruction not known +\n");
        }
        
        ++it;
    }
   
    fprintf(stderr, "+-------------+--------+--------+---------+\n"); 
    fprintf(stderr, "\n");
    fprintf(stderr, "Total instructions tested: %3u\n", totalTested);
    fprintf(stderr, "Fully working :            %3u (%5.1f%%)\n", totalFullyWorking, 100.0*totalFullyWorking/(double)totalTested);
    fprintf(stderr, "Partially working :        %3u (%5.1f%%)\n", totalPartiallyWorking, 100.0*totalPartiallyWorking/(double)totalTested);
    fprintf(stderr, "Not working at all :       %3u (%5.1f%%)\n", totalNotWorking, 100.0*totalNotWorking/(double)totalTested);
}
