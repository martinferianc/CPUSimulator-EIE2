/*! \file mips_test.h
    Defines the functions used to test a processor.
*/
#ifndef mips_test_header
#define mips_test_header

#include "mips_cpu.h"

/* This allows the header to be used from both C and C++, so
programs can be written in either (or both) languages. */
#ifdef __cplusplus
extern "C"{
#endif
    
/*! \defgroup mips_test Testing
    
    This collection of functions is used for defining test suites.
    Just like the CPU and memory APIs, it is defined as a set of
    functions, leaving freedom to have different implementations and
    clients. The broad idea is that of Unit Testing http://en.wikipedia.org/wiki/Unit_testing
    so there are lots of small tests which try to check the functionality
    of many independent parts, in this case instructions. Usually this
    would be followed by Integration Testing http://en.wikipedia.org/wiki/Integration_testing
    to check that the parts work correctly together, but that is mostly out
    of scope here.

    Integration testing is a huge problem in real-world CPU design and
    implementation, as the interaction between many individual components
    within a CPU can lead to very subtle bugs, even when the components appear
    to be fine. You will be able to test some instructions individually, but
    some instructions are impossible to test in isolation (the API is carefully
    designed in that regard), and will require a sequence of instructions.
    I will also give you larger programs to try running later on, so you'll be
    able to see whether your testing of individual instructions results in
    a processor that can run real programs with thousands of instructions.
    
    This particular unit testing API is extremely simple, and has few
    of the standard features one would expect from a standard API,
    such as JUnit or CppUnit. Typically they support things like nesting
    of test suites, can indicate dependencies ("don't run this complex test if
    another simpler test already failed"), and support multiple output formats
    (GUI dashboards are quite common). Another thing that is missing is
    Continuous Integration, whereby the tests are automatically run as
    development continues, usually in concert with source control. For
    this test framework, it is assumed that every time you run the test suite
    all tests will be run, so you'll be able to see if adding new functionality
    has broken things that used to work - when new features break
    old ones it is a regression (things have got worse), so this is a form
    of Regression Testing http://en.wikipedia.org/wiki/Regression_testing.
    
    The main things the test framework requires you to do are:
    
    - Indicate when you are starting the test suite with mips_test_begin_suite
    
    - For each individual test you run, use mips_test_begin_test to record the
      what the test is testing, then mips_test_end_test to record if it passed
      or failed.
    
    - Use mips_test_end_suite to say that all tests have ended, so that the
      test framework can do things like calculate statistics.
    
    So an individual test would look something like:
    
        int testId=mips_test_begin_test("ADD");
        ...
        mips_cpu_set_register(cpu, 1, 5);
        mips_cpu_set_register(cpu, 2, 5);
        ...
        // Make the simulator do reg[1]=reg[1]+reg[2]
        ...
        uint32_t got;
        mips_error err=mips_cpu_get_register(cpu, 1, &got);
        mips_test_end_test(testId, (err==mips_Success) && (got==10), "Testing 5+5 == 10");
    
    While the overall test suite would look like:
    
        mips_test_begin_suite();
     
        testId=mips_test_begin_test(...);
        ...
        mips_test_end_test(testId, ...);
        
        testId=mips_test_begin_test(...);
        ...
        mips_test_end_test(testId, ...);
        
        mips_test_end_suite();
        
    Exactly how you structure it is up to you, the main
    requirement is that a given test should actually test
    the instruction it says it is - if you say you are testing
    MULU, but that instruction never executes within the
    scope of the test, then the test doesn't count. You
    should aim to test all instructions that you implement.
    
    As you add tests, you will notice a lot of repetition,
    between different tests of one instruction, and the testing
    of different instructions. As a programmer, whenever you
    see repetition you should thing about automation. This
    is a programmatic framework, so how much replicated
    functionality can be wrapped up inside a function?
    
    \addtogroup mips_test
    @{
*/

/*! Call once at the beginning of all tests to setup
    testing information.
*/
void mips_test_begin_suite();
  
/*! Used before starting an individual test
    \param instruction String identifying which instruction the
    test is targetting, for example "add", "lw", etc.
    
    \retval A unique identifier identifying the test
    
    You may have some tests which are not associated with any
    instruction, in which case use the string "<internal>". These
    can be useful to establish certain invariants, like "if I set
    register 3 to a value, then if I read register 3 it should still
    be the same value".
*/
int mips_test_begin_test(const char *instruction);

/*! Used to indicate whether an individual test passed or failed.

    \param testId The unique identifier returned from mips_test_begin_test.
    
    \param passed Flag to indicate if the test succeeded (passed!=0) or failed (passed==0).
    
    \param msg An optional message to explain what you the test was looking for in
        case it failed. Can be NULL if there is nothing useful to print, or you don't want to
        write a message.
*/
void mips_test_end_test(int testId, int passed, const char *msg);

/*! Call once at the end of all tests to indicate that all tests
    have now ended.
*/
void mips_test_end_suite();

/*! @} */    
    

#ifdef __cplusplus
};
#endif

#endif
