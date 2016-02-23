/*
 * SYS_FUNC.h
 *
 *  Created on: 14-Aug-2015
 *      Author: Jaspreet Singh
 */

#ifndef PLL_INCLUDE_SYS_FUNC_H_
#define PLL_INCLUDE_SYS_FUNC_H_

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "CONTROL_LOOP.h"

// Cla Trigger Functions
//----------------------------------------------------

void ClaForceTask1();  		 // CLA Main Function
void ClaForceTask2();  	     // CLA Variable Intialize


// DAC Functions
//----------------------------------------------------

// DAC A Output
void DAC_A_OUT(float value, int32 GAIN, int32 OFFSET);

// DAC B Output
void DAC_B_OUT(float value, int32 GAIN, int32 OFFSET);


// ADC Functions
//----------------------------------------------------

// ADC_A Data Acquistion Function
void sampleADC_A();

// ADC_B Data Acquistion Function
void sampleADC_B();


// SCI_A Functions (COMM. Functions)
//----------------------------------------------------

// Transmit a BYTE via UART
void scia_xmit(int a);

// Transmit a MSG via UART
void scia_msg(char * msg);

// Transmit INTEGER Data via UART
void scia_data(long data, unsigned char ndigits);


#endif /* PLL_INCLUDE_SYS_FUNC_H_ */
