/*
 * SYS_INIT.h
 *
 *  Created on: 07-Aug-2015
 *      Author: Jaspreet Singh
 */

#ifndef PLL_INCLUDE_SYS_INIT_H_
#define PLL_INCLUDE_SYS_INIT_H_

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "CLA_SHARED.h"
#include "CONTROL_LOOP.h"

// Defines
#define REFERENCE_VDAC     0
#define REFERENCE_VREF     1

// Main Call Function
void sysInit();

// Initialize GPIO
void Initialize_GPIO_USR();

// SCI_A Intialize Function
void SCIA_Init();

// ADC Initalize Functions
void Initialize_ADC_A();
void Initialize_ADC_B();

// DAC Initalize Functions
void Initialize_DAC_A();
void Initialize_DAC_B();

// CLA Intialize Functions
void CLA_initCpu1Cla1(void);
void CLA_configClaMemory(void);


#endif /* PLL_INCLUDE_SYS_INIT_H_ */
