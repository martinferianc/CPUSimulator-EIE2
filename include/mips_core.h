/*! \file mips_core.h
    Establishes central types and definitions used within the simulator and system.
*/
#ifndef mips_core_header
#define mips_core_header

/*! \file 
    The <stdint.h> header gives us types of a known width and signedness, like
    uint32_t and int16_t. These types can be very useful for
    matching the internal types of a processor, and for managing
    conversions from signed to unsigned types. The C/C++ standard has certain
    (very strict) rules for conversion between types, which are different from
    the rules of any particular CPU. In an extremely legalistic interpretation, the
    integer encoding is not necessarily twos complement, but we
    will mandate that all target architectures use a twos complement
    representation.
*/
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>

/* This allows the header to be used from both C and C++, so
programs can be written in either (or both) languages. */
#ifdef __cplusplus
extern "C"{
#endif

/*! This is a list of errors used within the cpu simulator to indicate
    various things which could go wrong. Some of those come from within
    the implementation of the simulator, while others will arise due to
    the execution of the program within the simulator.
*/
typedef enum _mips_error{
    mips_Success =0,
    
    //! Error from within the simulator.
    ///@{
    mips_ErrorNotImplemented=0x1000,
    mips_ErrorInvalidArgument=0x1001,
    mips_ErrorInvalidHandle=0x1002,
    mips_ErrorFileReadError=0x1003,
    mips_ErrorFileWriteError=0x1004,
    ///@}
    
    //! Error or exception from the simulated processor or program.
    ///@{
    mips_ExceptionBreak=0x2000,
    mips_ExceptionInvalidLength=0x2001,
    mips_ExceptionInvalidAddress=0x2002,
    mips_ExceptionInvalidAlignment=0x2003,
    mips_ExceptionAccessViolation=0x2004,
    mips_ExceptionInvalidInstruction=0x2005,
    mips_ExceptionArithmeticOverflow=0x2006,
    ///@}
    
    /*! This is an extension point for implementations. Codes
        at this number and above are available for the
        implementation, but are not generally understood.
        They shouldn't be exposed over public APIs. */
    mips_InternalError=0x3000
}mips_error;

#ifdef __cplusplus
};
#endif

#endif
