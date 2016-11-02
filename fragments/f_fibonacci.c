#include <stdint.h>

uint32_t f_fibonacci(uint32_t n)
{
	if(n<=1)
		return n;
	return f_fibonacci(n-1)+f_fibonacci(n-2);
}
