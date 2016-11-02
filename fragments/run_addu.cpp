#include "mips.h"

#include "f_addu.c"

int main(int argc, char *argv[])
{
    const char *srcName="f_addu-mips.bin";
    
    if(argc>1){
        srcName=argv[1];
    }
    
    mips_mem_h m=mips_mem_create_ram(0x20000);
    mips_cpu_h c=mips_cpu_create(m);
    
    FILE *src=fopen(srcName,"rb");
    if(!src){
        fprintf(stderr, "Cannot load source file '%s', try specifying the relative path to f_addu-mips.bin.", srcName);
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
    fprintf(stderr, "Loaded %d bytes of binary at address 0.\n", offset);
    
    fclose(src);
    
    // No error checking... oh my!
    
    ///////////////////////////////////////////////////////
    // we can start the program at the addu instruction
    
    uint32_t a=63, b=45;
    
    mips_cpu_set_pc(c, 4);  // Place the program counter on the addu instruction
    
    mips_cpu_set_register(c, 4, a);             // Set first input
    mips_cpu_set_register(c, 5, b);             // Set second input
    
    mips_cpu_step(c);   // Execute the addu at address 4
    
    uint32_t sum;
    mips_cpu_get_register(c, 2, &sum);    // Get the result back
    
    uint32_t sum_ref=a+b;
    fprintf(stderr, "%d + %d = %d, expected = %d\n", a, b, sum, sum_ref);
    if(sum_ref!=sum){
        fprintf(stderr, "Sum is wrong.\n");
        exit(1);
    }
    
    
    ///////////////////////////////////////////////////////////////
    // Or we can execute the whole thing as a proper MIPS function call
    
    a=10000;
    b=34239809;
    
    uint32_t sentinelPC=0x10000000;
    
    mips_cpu_reset(c);  // get back to a zero state, with pc=0
    
    mips_cpu_set_register(c, 31, sentinelPC);   // set function return address to something invalid just in case
    mips_cpu_set_register(c, 4, a);             // Set first function input (MIPS calling convention)
    mips_cpu_set_register(c, 5, b);             // Set second input argument (MIPS calling convention)
    
    mips_cpu_step(c);   // execute the jr at address 0
    mips_cpu_step(c);   // execute the addu in the delay slot at address 4
    
    mips_cpu_get_register(c, 2, &sum);    // Get function result back (MIPS calling convention)
    
    sum_ref=a+b;
    fprintf(stderr, "%d + %d = %d, expected = %d\n", a, b, sum, sum_ref);
    if(sum_ref!=sum){
        fprintf(stderr, "Sum is wrong.\n");
        exit(1);
    }
    
    return 0;
}
