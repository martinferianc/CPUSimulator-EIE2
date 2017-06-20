#include <iostream>
#include "mips.h"
#include "mips_cpu.h"
#include <sstream>
#include <string>

using namespace std;

/*
EXECUTE HELP
This is a set of functions that help to execute the instructions
*/
size_t highestOneBitPosition(uint32_t a);
bool addition_overflow(uint32_t a, uint32_t b);
bool subtraction_overflow(uint32_t a, uint32_t b);

uint32_t endian32(const uint32_t& v);
uint16_t endian16(const uint16_t& v);
