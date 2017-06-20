/*
EXECUTE HELP
This is a set of functions that help to execute the instructions
*/
#include <iostream>
#include "mips_cpu_execute_help.hpp"

size_t highestOneBitPosition(uint32_t a) {
    size_t bits=0;
    while (a!=0) {
        ++bits;
        a>>=1;
    };
    return bits;
}

bool subtraction_overflow(uint32_t a, uint32_t b){
  if (highestOneBitPosition(a)!=highestOneBitPosition(b)) {
    int32_t temp = int32_t(a) - int32_t(b);
    if (highestOneBitPosition(temp)!=highestOneBitPosition(a))
      return true;
    else
      return false;
  } else
      return false;
}
bool addition_overflow(uint32_t a, uint32_t b) {
    if (highestOneBitPosition(a)==highestOneBitPosition(b)) {
      int32_t temp = int32_t(a) + int32_t(b);
      if (highestOneBitPosition(temp)!=highestOneBitPosition(a))
        return true;
      else
        return false;
    } else
        return false;
}

uint32_t endian32(const uint32_t& v){
  return (((v<<24)&0xFF000000) | ((v<<8)&0x00FF0000) | ((v>>8)&0x0000FF00) | ((v>>24)&0x000000FF));
}

uint16_t endian16(const uint16_t& v){
  return (((v<<8)&0xFF00) | ((v>>8)&0x00FF));
}
