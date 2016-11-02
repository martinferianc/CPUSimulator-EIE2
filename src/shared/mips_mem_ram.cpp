/* This file is an implementation of the functions
 defined in mips_mem.h. It is designed to be
 linked against something which needs an implementation
 of a RAM device following that memory mapping
 interface.
 */
#include "mips_mem.h"

#include <stdio.h>
#include <stdlib.h>

struct mips_mem_provider
{
    uint32_t length;
    uint8_t *data;
};

extern "C" mips_mem_h mips_mem_create_ram(
                                          uint32_t cbMem	//!< Total number of bytes of ram
){
    if(cbMem>0x20000000){
        return 0; // No more than 512MB of RAM
    }
    
    uint8_t *data=(uint8_t*)malloc(cbMem);
    if(data==0)
        return 0;
    
    struct mips_mem_provider *mem=(struct mips_mem_provider*)malloc(sizeof(struct mips_mem_provider));
    if(mem==0){
        free(data);
        return 0;
    }
    
    mem->length=cbMem;
    mem->data=data;
    
    return mem;
}

static mips_error mips_mem_read_write(
                                      bool write,
                                      mips_mem_h mem,
                                      uint32_t address,
                                      uint32_t length,
                                      uint8_t *dataOut
                                      )
{
    if(mem==0){
        return mips_ErrorInvalidHandle;
    }
    
    if( (length!=1) && (length!=2) && (length!=4) ){
        return mips_ExceptionInvalidLength;
    }
    
    if(0 != (address % length) ){
        return mips_ExceptionInvalidAlignment;
    }
    if(((address+length) > mem->length) || (address > (UINT32_MAX - length))){	// A subtle bug here, maybe?
        return mips_ExceptionInvalidAddress;
    }
    
    if(write){
        for(unsigned i=0; i<length; i++){
            mem->data[address+i]=dataOut[i];
        }
    }else{
        for(unsigned i=0; i<length; i++){
            dataOut[i]=mem->data[address+i];
        }
    }
    return mips_Success;
}

mips_error mips_mem_read(
                         mips_mem_h mem,		//!< Handle to target memory
                         uint32_t address,	//!< Byte address to start transaction at
                         uint32_t length,	//!< Number of bytes to transfer
                         uint8_t *dataOut	//!< Receives the target bytes
)
{
    return mips_mem_read_write(
                               false,	// we want to read
                               mem,
                               address,
                               length,
                               dataOut
                               );
}

mips_error mips_mem_write(
                          mips_mem_h mem,	//! Handle to target memory
                          uint32_t address,		//! Byte address to start transaction at
                          uint32_t length,			//! Number of bytes to transfer
                          const uint8_t *dataIn	//! Receives the target bytes
)
{
    return mips_mem_read_write(
                               true,	// we want to write
                               mem,
                               address,
                               length,
                               (uint8_t*)dataIn
                               );
}

void mips_mem_free(mips_mem_h mem)
{
    if(mem){
        free(mem->data);
        mem->data=0;
        free(mem);
    }
}