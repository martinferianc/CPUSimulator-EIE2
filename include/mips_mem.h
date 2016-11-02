/*! \file mips_mem.h
    Defines the functions used to interact with simulated memory.
    
    Note that the notions of "memory/address space" and "RAM" are actually
    two related but distinct things (we will explore this more later).
    A memory space is some kind of addressable space that the CPU can
    read and write to, where addressable locations are identified by
    integers. For the moment we will only deal with one address space,
    but later on we'll see others. In this API, abstract memory spaces
    are accessed using the functions in \ref mips_mem_core, but they
    must be intially created using a device specific API from \ref mips_mem_devices.
    
    RAM is a particular kind of memory device, which maps reads and
    writes transactions at particular addresses to corresponding
    storage locations. ROM is another kind of memory device that you
    saw earlier. It is extremely common for multiple types of memory
    device to exist in one address space, but for now we will stick
    with the simple idea of having one RAM, which is created using mips_mem_create_ram.
*/
#ifndef mips_mem_header
#define mips_mem_header

#include "mips_core.h"

/* This allows the header to be used from both C and C++, so
programs can be written in either (or both) languages. */
#ifdef __cplusplus
extern "C"{
#endif
    
/*! \defgroup mips_mem Memory
    \addtogroup mips_mem
    @{
    
    \defgroup mips_mem_core Abstract Memory Interface
    \addtogroup mips_mem_core
    @{
*/

/*! Represents some sort of memory, but without saying
anything about how it is represented. See \ref mips_mem_h.

\struct mips_mem_provider    
*/
struct mips_mem_provider;

/*! An opaque handle to a memory space.

    We can pass this around without knowing who or what provides the
    memory. This is an example of an "opaque data type" http://en.wikipedia.org/wiki/Opaque_data_type
    and is commonly used in embedded systems and operating
    systems. An example you might have come across includes the
    FILE data-type used by fopen and fprintf in the C standard
    library.

    Because this is a pointer, we can safely give it the
    known value of 0 or NULL in order to get a known empty
    state. For example:
        
        mips_mem_h myMem=0; // Declare an empty handle
        
        if(some_condition)
            myMem=get_a_handle();
        
        if(myMem)
            do_something_with mem(myMem);
    
    So even without knowing what the data-structure is, we can still
    tell whether or not a handle is currently pointing at a
    data-structure.
*/
typedef struct mips_mem_provider *mips_mem_h;


/*! Perform a read transaction on the memory
    
    The implementation is expected to check that the transaction
    matches the alignment and block size requirements, and return an
    error if they are violated.
    
    See \ref mips_mem_write for more detailed discussion of its use.
*/
mips_error mips_mem_read(
    mips_mem_h mem,		//!< Handle to target memory
    uint32_t address,	//!< Byte address to start transaction at
    uint32_t length,	//!< Number of bytes to transfer
    uint8_t *dataOut	//!< Receives the target bytes
);

/*! Perform a write transaction on the memory
    
    The implementation is expected to check that the transaction
    matches the alignment requirements, and return an error if they are violated.
    
    The input pointer is a pointer to bytes, because the RAM is byte
    addressable, however the transaction size may be 1, 2, or 4 bytes.
    To write a larger piece of data, you can use pointer casting, or pass in an array.
    For example, if I want to write 0xFF001100 to address 12, I could do:
    
        uint32_t xv=0xFF001100;
        mips_error err=mips_mem_write(mem, 12, 4, (uint8_t*)&xv);
    
    alternatively, I could do:
    
        uint8_t xa[4];
        xa[0]=0x00;
        xa[1]=0x11;
        xa[2]=0x00;
        xa[3]=0xFF;
        mips_error err=mips_mem_write(mem, 12, 4, xa); 

    or more succinctly:
    
        uint8_t xa[4];
        xa[0]={0x00, 0x11, 0x00, 0xFF};
        mips_error err=mips_mem_write(mem, 12, 4, xa); 
    
    In the second two cases xa is an array of uint8_t, so can also be
    used as a pointer of type `uint8_t *`.
    
    This example assumes a little-endian architecture, such as x86. If
    the architecture running the  code was big-endian, the first example
    would need to be changed to:
    
        uint32_t xv=0x001100FF;
        mips_error err=mips_mem_write(mem, 12, 4, (uint8_t*)&xv);

    MIPS is a bi-endian architecture (like ARM), which can be configured
    to run in either little-endian or big-endian mode. The hardware translates
    on the fly (after all, endianess conversion is just re-ordering the bytes).
    
    For this work, we will have a little-endian (x86) processor running the
    simulator, but the thing being simulated (MIPS) is big-endian.
*/
mips_error mips_mem_write(
    mips_mem_h mem,	        //!< Handle to target memory
    uint32_t address,	    //!< Byte address to start transaction at
    uint32_t length,	    //!< Number of bytes to transfer
    const uint8_t *dataIn	//!< Receives the target bytes
);


/*! Release all resources associated with memory. The caller doesn't
    really know what is being released (it could be memory, it could
    be file handles), and shouldn't care. Calling mips_mem_free on an
    empty (zero) handle is legal. Calling mips_mem_free twice on the
    same non-empty handle is illegal, and the resulting behaviour is undefined
    (most likely a crash).
    
    A pattern that can be useful is:
    
        mips_mem_h h=0; // Initialise it to empty
        ...
        h=some_creator_function(...);
        ...
        use_memory_somehow(h);
        ...
        if(h){
            mips_mem_free(h);
            h=0; // Make sure it is never freed again
        }
*/
void mips_mem_free(mips_mem_h mem);

/*! @} */


/*! \defgroup mips_mem_devices Concrete Memory Devices
    \ingroup mips_mem_devices
    @{
*/

/*! Initialise a new RAM of the given size.

    The RAM will expect transactions to be aligned at the granularity
    of the transfer, so 4-byte transfers are aligned on 4-byte addresses,
    2-byte transfers on 2-byte address, and single byte transfers can
    have any address.
    
    This interface is over-simplifying things, and ignoring
    "byte-enables". These are what allows a CPU to indicate
    which of the bytes within a 32-bit bus are being actively
    written.
*/
mips_mem_h mips_mem_create_ram(
    uint32_t cbMem	//!< Total number of bytes of ram
);

/*!
    @}
    @}
*/


#ifdef __cplusplus
};
#endif

#endif
