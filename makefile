# Your login. For example, mine is dt10. Yours
# won't be eie2ugs...
LOGIN ?= mf2915

# Turn on all warnings
CPPFLAGS += -W -Wall

# Add debug information
CPPFLAGS += -g

# Pick up any headers in the ./include directories
CPPFLAGS += -I include

# C++11 by default
CXXFLAGS += -std=c++11


# This is defining a variable containing the default object files
# for the memory and test sub-systems. Note that there is no
# guarantee that _I_ (dt10) will use these implementations, this
# is for your convenience.
DEFAULT_OBJECTS = \
	src/shared/mips_test_framework.o \
	src/shared/mips_mem_ram.o 

# This should collect all the files relating to your CPU
# implementation, according to the various patterns. It is
# useful to think of this collection as being a single library
# or sub-system (and in fact I will compile them into a single
# library). Anything within this sub-system can access the
# internal details of other parts, but should not rely on
# internal details of other sub-systems (e.g. the test).
# The $(wilcard ...) function is uses to expand to files that
# exist in the file-system.
USER_CPU_SRCS = \
	$(wildcard src/$(LOGIN)/mips_cpu.cpp) \
	$(wildcard src/$(LOGIN)/mips_cpu_*.cpp) \
	$(wildcard src/$(LOGIN)/mips_cpu/*.cpp) \
	$(wildcard src/$(LOGIN)/mips_cpu_*/*.cpp)

# This is another variable, listing all the files needed
# for the test test sub-system, which is independent of the
# CPU. 
USER_TEST_SRCS = \
	$(wildcard src/$(LOGIN)/test_mips.cpp) \
	$(wildcard src/$(LOGIN)/test_mips_*.cpp) \
	$(wildcard src/$(LOGIN)/test_mips/*.cpp) \
	$(wildcard src/$(LOGIN)/test_mips_*/*.cpp)

# This performs pattern substitution, converting patterns
# of the form *.cpp to *.o
# The make command already knows how to turn source files (.cpp)
# into object files (.o).
USER_CPU_OBJECTS = $(patsubst %.cpp,%.o,$(USER_CPU_SRCS))
USER_TEST_OBJECTS = $(patsubst %.cpp,%.o,$(USER_TEST_SRCS))

# A rule for building a test executable.
# This brings together:
# - All the object files from the default memory and test implementations
# - Your CPU implementation from $(USER_CPU_OBJECTS)
# - Your test implementation from $(USER_TEST_OBJECTS), including the main function
src/$(LOGIN)/test_mips : $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)
	
# This is a bonus gift. You may want to try building this
# using:
#    
#    make fragments/run_fibonacci
#
# then running it:
#
#    fragments/run_fibonacci
#
# The idea is that this is a different user of your CPU, distinct
# from your tester. It also explores the idea of running fragments
# of code, rather than just individual instructions.
#
# You may want to look at the corresponding binaries, code, and disassembly
# in the fragments directory.
fragments/run_fibonacci : fragments/run_fibonacci.cpp $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)
	
# Again, another bonus gift. If you are convinced that your
# program implements addu (and one other instruction) correctly,
# then try running this.
fragments/run_addu : fragments/run_addu.cpp $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)

# Gets rid of temporary files.
# The `-` prefix is to indicate that it doesn't matter if the
# command fails (because the file may not exist)
clean : 
	-rm src/$(LOGIN)/test_mips
	-rm $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)

# By convention `make all` does the default build, whatever that is.
all : src/$(LOGIN)/test_mips
