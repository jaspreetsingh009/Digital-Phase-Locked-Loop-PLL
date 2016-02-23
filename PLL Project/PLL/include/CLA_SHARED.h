#ifndef _CLA_SHARED_H_
#define _CLA_SHARED_H_

#include "F28x_Project.h"
#include "F2837xS_Cla_defines.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// DEFINES
//-----------------------------------------------------------


// typedefs
//-----------------------------------------------------------


// Global Variables
//-----------------------------------------------------------

// Task 1 (ASM) Variables

extern float    SINE_3P_THETA;
extern float    SINE_1P_THETA;
extern float    SINE_3A;
extern float    SINE_1A;
extern float    SINE_3B;
extern float    SINE_3C;

extern Uint32  	SINEGEN_FREQ;
extern int32    SINE_1P_FREQ;
extern int32    SINE_3P_FREQ;
extern int32    pos;
extern int32    pos1;

// Task 2 (ASM) Variables

// Task 3 (ASM) Variables

// Task 4 (ASM) Variables

// Task 5 (ASM) Variables

// Task 6 (ASM) Variables

// Task 7 (ASM) Variables

// Task 8 (ASM) Variables

// Common (ASM) Variables


// Function Prototpes
//-----------------------------------------------------------

// The following are symbols defined in the CLA assembly code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.

//CLA ASM Tasks

__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

#ifdef __cplusplus
}

#endif // extern "C"

#endif //end of _CLA_SHARED_H_ definition
