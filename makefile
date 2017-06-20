# Turn on all warnings
CPPFLAGS += -W -Wall

# Add debug information
CPPFLAGS += -g

# Pick up any headers in the ./include directories
CPPFLAGS += -I include

# C++11 by default
CXXFLAGS += -std=c++11

DEFAULT_OBJECTS = \
	src/shared/mips_test_framework.o \
	src/shared/mips_mem_ram.o

USER_CPU_SRCS = \
	$(wildcard src/mips_cpu.cpp) \
	$(wildcard src/mips_cpu_*.cpp) \
	$(wildcard src/mips_cpu/*.cpp) \
	$(wildcard src/mips_cpu_*/*.cpp)

USER_TEST_SRCS = \
	$(wildcard src/test_mips.cpp) \
	$(wildcard src/test_mips_*.cpp) \
	$(wildcard src/test_mips/*.cpp) \
	$(wildcard src/test_mips_*/*.cpp)

USER_CPU_OBJECTS = $(patsubst %.cpp,%.o,$(USER_CPU_SRCS))
USER_TEST_OBJECTS = $(patsubst %.cpp,%.o,$(USER_TEST_SRCS))

src/test_mips : $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)

simulator : $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)
			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o bin/test_mips $^ $(LFLAGS) $(LDLIBS)

test : $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o bin/test_mips $^ $(LFLAGS) $(LDLIBS)
		./test_mips.sh

fragments/run_fibonacci : fragments/run_fibonacci.cpp $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)

fragments/run_addu : fragments/run_addu.cpp $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS)

clean :
	-rm bin/test_mips
	-rm $(DEFAULT_OBJECTS) $(USER_CPU_OBJECTS) $(USER_TEST_OBJECTS)

all : src/test_mips
