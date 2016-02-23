/*
 * GLOBAL_VARS.h
 *
 *  Created on: 07-Aug-2015
 *      Author: Jaspreet Singh
 */

#ifndef PLL_INCLUDE_GLOBAL_VARS_H_
#define PLL_INCLUDE_GLOBAL_VARS_H_

#define EXTERN            extern
#define INIT_VALUE(x) = x

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "USER_LIBS.h"


// USER LIBRARY Handles
//----------------------------------------------------

PLL_1P    pll1  =  PLL_1P_DEFAULTS;


// VARIABLES
//----------------------------------------------------

EXTERN  float32     PI_DUTY				INIT_VALUE(0);
EXTERN  float32		PLL_Kp				INIT_VALUE(0);
EXTERN  float32		PLL_Ki				INIT_VALUE(0);
EXTERN  float32 	PLL_setFreq			INIT_VALUE(50);

EXTERN  float32 	DAC_A_GAIN			INIT_VALUE(1000);
EXTERN  float32 	DAC_B_GAIN			INIT_VALUE(1000);
EXTERN  float32 	DAC_A_OFFSET		INIT_VALUE(1000);
EXTERN  float32 	DAC_B_OFFSET		INIT_VALUE(1000);

EXTERN  float32 	ADCINA2				INIT_VALUE(0);

EXTERN  float32 	DAC1				INIT_VALUE(0);
EXTERN  float32 	DAC2				INIT_VALUE(0);

EXTERN  int32		PLL_IN_SEL			INIT_VALUE(0);


// Data Logger Variables
//----------------------------------------------------

EXTERN  float32		UART_DATA_A			INIT_VALUE(0);
EXTERN  float32		UART_DATA_B			INIT_VALUE(0);

EXTERN  int32		UART1_ASSIGN		INIT_VALUE(0);
EXTERN  int32		UART2_ASSIGN		INIT_VALUE(0);

EXTERN  int32		UART_TX_FLAG		INIT_VALUE(0);
EXTERN  int32		RX_DATA 			INIT_VALUE(0);
EXTERN  int32		UART_EN				INIT_VALUE(0);
EXTERN  int32		UART_TX_STATE       INIT_VALUE(0);

EXTERN 	Uint32  	T0_TX_COUNT			INIT_VALUE(0);

int32   			RX_BUF_A[UART_BUF_LENGTH];
int32   			RX_BUF_B[UART_BUF_LENGTH];


// Defines
//----------------------------------------------------

#define ADC_A0			AdcaResultRegs.ADCRESULT0
#define ADC_A1			AdcaResultRegs.ADCRESULT1
#define ADC_A2			AdcaResultRegs.ADCRESULT2
#define ADC_A3			AdcaResultRegs.ADCRESULT3
#define ADC_A4			AdcaResultRegs.ADCRESULT4
#define ADC_A5			AdcaResultRegs.ADCRESULT5
#define ADC_A6			AdcaResultRegs.ADCRESULT6
#define ADC_A7			AdcaResultRegs.ADCRESULT7
#define ADC_A8			AdcaResultRegs.ADCRESULT8
#define ADC_A9			AdcaResultRegs.ADCRESULT9
#define ADC_A10			AdcaResultRegs.ADCRESULT10
#define ADC_A11			AdcaResultRegs.ADCRESULT11
#define ADC_A12			AdcaResultRegs.ADCRESULT12
#define ADC_A13			AdcaResultRegs.ADCRESULT13
#define ADC_A14			AdcaResultRegs.ADCRESULT14
#define ADC_A15			AdcaResultRegs.ADCRESULT15

#define ADC_B0			AdcbResultRegs.ADCRESULT0
#define ADC_B1			AdcbResultRegs.ADCRESULT1
#define ADC_B2			AdcbResultRegs.ADCRESULT2
#define ADC_B3			AdcbResultRegs.ADCRESULT3
#define ADC_B4			AdcbResultRegs.ADCRESULT4
#define ADC_B5			AdcbResultRegs.ADCRESULT5
#define ADC_B6			AdcbResultRegs.ADCRESULT6
#define ADC_B7			AdcbResultRegs.ADCRESULT7
#define ADC_B8			AdcbResultRegs.ADCRESULT8
#define ADC_B9			AdcbResultRegs.ADCRESULT9
#define ADC_B10			AdcbResultRegs.ADCRESULT10
#define ADC_B11			AdcbResultRegs.ADCRESULT11
#define ADC_B12			AdcbResultRegs.ADCRESULT12
#define ADC_B13			AdcbResultRegs.ADCRESULT13
#define ADC_B14			AdcbResultRegs.ADCRESULT14
#define ADC_B15			AdcbResultRegs.ADCRESULT15


#endif /* PLL_INCLUDE_GLOBAL_VARS_H_ */
