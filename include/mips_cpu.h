/*! \file mips_cpu.h

*/
#ifndef mips_cpu_header
#define mips_cpu_header

#include "mips_mem.h"

#ifdef __cplusplus
extern "C"{
#endif
	
/*! \defgroup mips_cpu CPU API
	\addtogroup mips_cpu
	@{
*/


/*! Represents the state of a cpu.
	
	This an opaque data type, similar to \ref mips_mem_provider.
	
	\struct mips_cpu_impl
*/
struct mips_cpu_impl;

/*! An opaque handle to a mips.

	This represents a handle to a data-type that clients can use, without
	knowing how the CPU is implemented. See \ref mips_mem_h for more
	commentary.
*/
typedef struct mips_cpu_impl *mips_cpu_h;


/*! Creates and initialises a new CPU instance.
	
	The CPU should be bound to the given
	\ref mips_mem_core "memory space", and have all registers set
	to zero. The memory is not owned by the CPU, so it should not
	be \ref mips_mem_free "freed" when the CPU is \ref mips_cpu_free "freed".
	
	\param mem The memory space the processor is connected to; think of it
	as the address bus to which the CPU has been wired.
*/
mips_cpu_h mips_cpu_create(mips_mem_h mem);

/*! Reset the CPU as if it had just been created, with all registers zerod.
	However, it should not modify RAM. Imagine this as asserting the reset
	input of the CPU core.
*/
mips_error mips_cpu_reset(mips_cpu_h state);

/*! Returns the current value of one of the 32 general purpose MIPS registers */
mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
);

/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
);

/*! Sets the program counter for the next instruction to the specified byte address.

	While this sets the value of the PC, it should not cause any kind of
	execution to happen. Once you look at branches in detail, you will
	see that there is some slight ambiguity about this function. Choose the
	only option that makes sense.
*/
mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
);

/*! Gets the pc for the next instruction.
	
	Returns the program counter for the next instruction to be executed.
*/
mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
);

/*! Advances the processor by one instruction.

	If an exception or error occurs, the CPU and memory state
	should be left unchanged. This is so that the user can
	inspect what happened and find out what went wrong. So
	this should be true:
	
		uint32_t pcOrig, pcGot;
		mips_cpu_get_pc(cpu, &pcOrig);
		mips_error err=mips_cpu_step(cpu);
		if(err!=mips_Success){
			mips_cpu_get_pc(cpu, &pcGot);
			assert(pcOrig==pcGot);
			assert(mips_cpu_step(cpu)==err);
	    }
	
	Maintaining this property in all cases is actually pretty
	difficult, so _try_ to maintain it, but don't worry too
	much if under some exceptions it doesn't quite work.
*/
mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
);

/*! Controls printing of diagnostic and debug messages.

	You are encouraged to include diagnostic and debugging
	information in your CPU, but you should include a way
	to control how much is printed. The default should be
	to print nothing, but it is a good idea to have a way
	of turning it on and off _without_ recompiling. This function
	provides a way for the user to indicate both how much
	output they are interested in, and where that output
	should go (should it go to stdout, or stderr, or a file?).
	
	\param state Valid (non-empty) CPU handle.
	
	\param level What level of output should be printed. There
	is no specific format for the output format, the only
	requirement is that for level zero there is no output.
	
	\param dest Where the output should go. This should be
	remembered by the CPU simulator, then later passed
	to fprintf to provide output.
	
	\pre It is required that if level>0 then dest!=0, so the
	caller will always provide a valid destination if they
	have indicated they will require output.
	
	It is suggested that for level=1 you print out one
	line of information per instruction with basic information
	like the program counter and the instruction type, and for higher
	levels you may want to print the CPU state before each
	instruction. Both of these can usually be inserted in
	just one place in the processor, and can greatly aid
	debugging.

	However, this is completely implementation defined behaviour,
	so your simulator does not have to print anything for
	any debug level if you don't want to. 
	
	The intent is that this function merely modifies the type
	of reporting that is performed during mips_cpu_step:
	
		// Enable basic debugging to stderr
		mips_cpu_set_debug_level(cpu, 1, stderr);
		
		// The implementation may now choose to print things
		// to stderr (what exactly is up to the implementation)
		mips_cpu_step(cpu);	// e.g. prints "pc = 0x12, encoding=R"
		
		// Tell the CPU to print nothing
		mips_cpu_set_debug_level(cpu, 0, NULL);
		
		// Now the implementation must not print any debug information
		mips_cpu_step(cpu);
		
		// Send detailed debug output to a text file
		FILE *dump=fopen("dump.txt", "wt");
		mips_cpu_set_debug_level(cpu, 2, dump);
		
		// Run lots of instructions until the CPU reports an error.
		// The implementation can write lots of useful information to
		// the file
		mips_error err=mips_Success;
		while(!err) {
			mips_cpu_step(cpu);
		};
		
		// Detach the text file, and close it
		mips_cpu_set_debug_level(cpu, 0, NULL);
		fclose(dump);
		
		// You can now read through the text file "dump.txt" to see what happened

	However, you could decide that you want to print something out
	at the point that mips_cpu_set_debug_level is called with level>0,
	such as the current PC and registers. Up to you.
*/
mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest);

/*! Free all resources associated with state.

	\param state Either a handle to a valid simulation state, or an empty (NULL) handle.

	It is legal to pass an empty (NULL) handle to mips_cpu_free. It is illegal
	to pass the same non-empty handle to mips_cpu_free twice, and will
	result in undefined behaviour (i.e. anything could happen):
	
		mips_cpu_h cpu=mips_cpu_create(...);
		...
		mips_cpu_free(h);	// This is fine
		...
		mips_cpu_free(h);	// BANG! or nothing. Or format the hard disk.
	
	A better pattern is to always zero the variable after calling free,
	in case elsewhere you are not sure if resources have been released yet:
	
		mips_cpu_h cpu=mips_cpu_create(...);
		...
		mips_cpu_free(h);	// This is fine
		h=0;	// Make sure it is now empty
		...
		mips_cpu_free(h);	// Fine, nothing happens
		h=0;    // Does nothing here, might could stop other errors
*/
void mips_cpu_free(mips_cpu_h state);

/*!
	@}
*/

#ifdef __cplusplus
};
#endif

#endif
