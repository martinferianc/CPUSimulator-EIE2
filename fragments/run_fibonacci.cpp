#include "mips.h"

#include "f_fibonacci.c"

int main(int argc, char *argv[])
{
    const char *srcName="f_fibonacci-mips.bin";
    
    if(argc>1){
        srcName=argv[1];
    }
    
    mips_mem_h m=mips_mem_create_ram(0x20000);
    mips_cpu_h c=mips_cpu_create(m);
    
    FILE *src=fopen(srcName,"rb");
    if(!src){
        fprintf(stderr, "Cannot load source file '%s', try specifying the relative path to f_fibonacci-mips.bin.", srcName);
        exit(1);
    }

    uint32_t v;
    uint32_t offset=0;
    while(1==fread(&v, 4, 1, src)){
        if(mips_mem_write(m, offset, 4, (uint8_t*)&v)){
            fprintf(stderr, "Memory error while loading binary.");
            exit(1);
        }
        offset+=4;
    }
    fprintf(stderr, "Loaded %d bytes of binary at address 0.", offset);
    
    fclose(src);
    
    // No error checking... oh my!
    
    uint32_t n=12;  // Value we will calculate fibonacci of
    
    uint32_t sentinelPC=0x10000000;
    
    mips_cpu_set_register(c, 31, sentinelPC);   // set return address to something invalid
    mips_cpu_set_register(c, 4, n);             // Set input argument
    mips_cpu_set_register(c, 29, 0x1000);       // Create a stack pointer
    
    uint32_t steps=0;
    while(!mips_cpu_step(c)){
        fprintf(stderr, "Step %d.\n", steps);
        ++steps;
        uint32_t pc;
        mips_cpu_get_pc(c, &pc);
        if(pc==sentinelPC)
            break;
    }
    
    uint32_t fib_n;
    mips_cpu_get_register(c, 2, &fib_n);    // Get the result back
    
    uint32_t fib_n_ref=f_fibonacci(n);
    
    fprintf(stderr, "fib(%d) = %d, expected = %d\n", n, fib_n, fib_n_ref);
    
    return 0;
}
