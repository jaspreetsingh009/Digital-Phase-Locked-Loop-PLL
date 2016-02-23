/*
 * SYS_INIT.c
 *
 *  Created on: 07-Aug-2015
 *      Author: Jaspreet Singh
 */

#include "SYS_INIT.h"

//---------------------------------------------//
//   SYS Peripheral Initialization Functions   //
//---------------------------------------------//

void sysInit()
{
	// If running from flash copy RAM only functions to RAM

#ifdef _FLASH
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

	// Initialize System Control:
	InitSysCtrl();

	// Set the GPIO to it's default state.
	InitGpio();

	// Initialize USER Defined GPIO Config.
	Initialize_GPIO_USR();


	// Disable CPU interrupts
	DINT;

	// This function is found in the F2837xS_PieCtrl.c file.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize DAC_A Module
	Initialize_DAC_A();

	// Initialize DAC_B Module
	Initialize_DAC_B();

	// Initialize ADC_A Module
	Initialize_ADC_A();

	// Initialize ADC_B Module
	Initialize_ADC_B();


	// CAL Initialization
	CLA_configClaMemory();
	CLA_initCpu1Cla1();

	// SCI A Initialize
	SCIA_Init();

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	InitPieVectTable();

	// Force Trigger CLATask2 to Initialize CLA Variables
	EALLOW;
	Cla1Regs.MIFRC.bit.INT2 = 1;
	EDIS;
}


void Initialize_GPIO_USR()
{
	// Initialize pins for the SCI-A
	//-----------------------------------
	EALLOW;
	GpioCtrlRegs.GPCMUX2.bit.GPIO84  = 1;
	GpioCtrlRegs.GPCMUX2.bit.GPIO85  = 1;
	GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
	GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
	EDIS;
}

void SCIA_Init()
{
	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function

	SciaRegs.SCICCR.all  = 0x0007;   // 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003;   // enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE

	SciaRegs.SCICTL2.bit.TXINTENA   = 1;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

	SciaRegs.SCIHBAUD.all = 0x0000;  // 115200 baud @LSPCLK = 22.5MHz (90 MHz SYSCLK).
	SciaRegs.SCILBAUD.all = 53;

	SciaRegs.SCICTL1.all = 0x0023;    // Relinquish SCI from Reset

	// Intialize FIFO
	SciaRegs.SCIFFTX.all = 0xE040;
	SciaRegs.SCIFFRX.all = 0x2044;
	SciaRegs.SCIFFCT.all = 0x0;
}

void Initialize_ADC_A()
{
	EALLOW;

	// Write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; 	//set ADCCLK divider to /4

	// ADC Configured in 12-bit mode
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	// Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	// power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	// delay for 1ms to allow ADC time to power up
	DELAY_US(1000);

	// Configure ADC SOC
	//------------------------------------------------------------------------------------

	//-- SOC0 --//
	AdcaRegs.ADCSOC0CTL.bit.CHSEL   =  0x02;   // SOC0 will convert pin ADCIN14
	AdcaRegs.ADCSOC0CTL.bit.ACQPS   =  25;     // Sample window is acqps + 1 SYSCLK cycles

	//-- SOC1 --//
	AdcaRegs.ADCSOC1CTL.bit.CHSEL   =  0x02;   // SOC1 will convert pin ADCIN14
	AdcaRegs.ADCSOC1CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC2 --//
	AdcaRegs.ADCSOC2CTL.bit.CHSEL   =  0x02;   // SOC2 will convert pin ADCIN14
	AdcaRegs.ADCSOC2CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC3 --//
	AdcaRegs.ADCSOC3CTL.bit.CHSEL   =  0x02;   // SOC3 will convert pin ADCIN14
	AdcaRegs.ADCSOC3CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC4 --//
	AdcaRegs.ADCSOC4CTL.bit.CHSEL   =  0x03;   // SOC4 will convert pin ADCIN14
	AdcaRegs.ADCSOC4CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC5 --//
	AdcaRegs.ADCSOC5CTL.bit.CHSEL   =  0x03;   // SOC5 will convert pin ADCIN14
	AdcaRegs.ADCSOC5CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC6 --//
	AdcaRegs.ADCSOC6CTL.bit.CHSEL   =  0x03;   // SOC6 will convert pin ADCIN14
	AdcaRegs.ADCSOC6CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC7 --//
	AdcaRegs.ADCSOC7CTL.bit.CHSEL   =  0x03;   // SOC7 will convert pin ADCIN14
	AdcaRegs.ADCSOC7CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC8 --//
	AdcaRegs.ADCSOC8CTL.bit.CHSEL   =  0x04;   // SOC8 will convert pin ADCIN14
	AdcaRegs.ADCSOC8CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC9 --//
	AdcaRegs.ADCSOC9CTL.bit.CHSEL   =  0x04;   // SOC9 will convert pin ADCIN14
	AdcaRegs.ADCSOC9CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC10 --//
	AdcaRegs.ADCSOC10CTL.bit.CHSEL  =  0x04;   // SOC10 will convert pin ADCIN14
	AdcaRegs.ADCSOC10CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC11 --//
	AdcaRegs.ADCSOC11CTL.bit.CHSEL  =  0x04;   // SOC11 will convert pin ADCIN14
	AdcaRegs.ADCSOC11CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC12 --//
	AdcaRegs.ADCSOC12CTL.bit.CHSEL  =  0x05;   // SOC12 will convert pin ADCIN14
	AdcaRegs.ADCSOC12CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC13 --//
	AdcaRegs.ADCSOC13CTL.bit.CHSEL  =  0x05;   // SOC13 will convert pin ADCIN14
	AdcaRegs.ADCSOC13CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC14 --//
	AdcaRegs.ADCSOC14CTL.bit.CHSEL  =  0x05;   // SOC14 will convert pin ADCIN14
	AdcaRegs.ADCSOC14CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC15 --//
	AdcaRegs.ADCSOC15CTL.bit.CHSEL  =  0x05;   // SOC15 will convert pin ADCIN14
	AdcaRegs.ADCSOC15CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//------------------------------------------------------------------------------------


	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL  =  1;   // end of SOC1 will set INT1 flag
	AdcaRegs.ADCINTSEL1N2.bit.INT1E    =  1;   // enable INT1 flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1  =  1;   // make sure INT1 flag is cleared

	EDIS;
}

void Initialize_ADC_B()
{
	EALLOW;

	// Write configurations
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; 	//set ADCCLK divider to /4

	// ADC Configured in 12-bit mode
	AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	// Set pulse positions to late
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	// power up the ADC
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	// delay for 1ms to allow ADC time to power up
	DELAY_US(1000);

	// Configure ADC SOC
	//------------------------------------------------------------------------------------

	//-- SOC0 --//
	AdcbRegs.ADCSOC0CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC0CTL.bit.CHSEL   =  0x02;   // SOC0 will convert pin ADCIN14
	AdcbRegs.ADCSOC0CTL.bit.ACQPS   =  25;     // Sample window is acqps + 1 SYSCLK cycles

	//-- SOC1 --//
	AdcbRegs.ADCSOC1CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC1CTL.bit.CHSEL   =  0x02;   // SOC1 will convert pin ADCIN14
	AdcbRegs.ADCSOC1CTL.bit.ACQPS   =  25;	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC2 --//
	AdcbRegs.ADCSOC2CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC2CTL.bit.CHSEL   =  0x02;   // SOC2 will convert pin ADCIN14
	AdcbRegs.ADCSOC2CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC3 --//
	AdcbRegs.ADCSOC3CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC3CTL.bit.CHSEL   =  0x02;   // SOC3 will convert pin ADCIN14
	AdcbRegs.ADCSOC3CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC4 --//
	AdcbRegs.ADCSOC4CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC4CTL.bit.CHSEL   =  0x03;   // SOC4 will convert pin ADCIN14
	AdcbRegs.ADCSOC4CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC5 --//
	AdcbRegs.ADCSOC5CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC5CTL.bit.CHSEL   =  0x03;   // SOC5 will convert pin ADCIN14
	AdcbRegs.ADCSOC5CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC6 --//
	AdcbRegs.ADCSOC6CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC6CTL.bit.CHSEL   =  0x03;   // SOC6 will convert pin ADCIN14
	AdcbRegs.ADCSOC6CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC7 --//
	AdcbRegs.ADCSOC7CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC7CTL.bit.CHSEL   =  0x03;   // SOC7 will convert pin ADCIN14
	AdcbRegs.ADCSOC7CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC8 --//
	AdcbRegs.ADCSOC8CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC8CTL.bit.CHSEL   =  0x04;   // SOC8 will convert pin ADCIN14
	AdcbRegs.ADCSOC8CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC9 --//
	AdcbRegs.ADCSOC9CTL.bit.TRIGSEL =  0;      // Software FRC Trigger Only
	AdcbRegs.ADCSOC9CTL.bit.CHSEL   =  0x04;   // SOC9 will convert pin ADCIN14
	AdcbRegs.ADCSOC9CTL.bit.ACQPS   =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC10 --//
	AdcbRegs.ADCSOC10CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC10CTL.bit.CHSEL  =  0x04;   // SOC10 will convert pin ADCIN14
	AdcbRegs.ADCSOC10CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC11 --//
	AdcbRegs.ADCSOC11CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC11CTL.bit.CHSEL  =  0x04;   // SOC11 will convert pin ADCIN14
	AdcbRegs.ADCSOC11CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC12 --//
	AdcbRegs.ADCSOC12CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC12CTL.bit.CHSEL  =  0x05;   // SOC12 will convert pin ADCIN14
	AdcbRegs.ADCSOC12CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC13 --//
	AdcbRegs.ADCSOC13CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC13CTL.bit.CHSEL  =  0x05;   // SOC13 will convert pin ADCIN14
	AdcbRegs.ADCSOC13CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC14 --//
	AdcbRegs.ADCSOC14CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC14CTL.bit.CHSEL  =  0x05;   // SOC14 will convert pin ADCIN14
	AdcbRegs.ADCSOC14CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//-- SOC15 --//
	AdcbRegs.ADCSOC15CTL.bit.TRIGSEL =  0;     // Software FRC Trigger Only
	AdcbRegs.ADCSOC15CTL.bit.CHSEL  =  0x05;   // SOC15 will convert pin ADCIN14
	AdcbRegs.ADCSOC15CTL.bit.ACQPS  =  25; 	   // sample window is acqps + 1 SYSCLK cycles

	//------------------------------------------------------------------------------------

	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL  =  1;   // end of SOC1 will set INT1 flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1E    =  1;   // enable INT1 flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1  =  1;   // make sure INT1 flag is cleared

	EDIS;
}

void Initialize_DAC_A()
{
	EALLOW;

	DacaRegs.DACCTL.bit.DACREFSEL  = REFERENCE_VREF; 	// Use VDAC as the reference for DAC
	DacaRegs.DACOUTEN.bit.DACOUTEN = 1;					// Enable DAC output

	EDIS;
}

void Initialize_DAC_B()
{
	EALLOW;

	DacbRegs.DACCTL.bit.DACREFSEL  = REFERENCE_VREF; 	// Use VDAC as the reference for DAC
	DacbRegs.DACOUTEN.bit.DACOUTEN = 1;					// Enable DAC output

	EDIS;
}

void ClaForceTask1() {
	// Force Trigger CLA1 Task 1
	EALLOW;
	Cla1Regs.MIFRC.bit.INT1 = 1;
	EDIS;
}

void ClaForceTask2() {
	// Force Trigger CLA1 Task 2
	EALLOW;
	Cla1Regs.MIFRC.bit.INT2 = 1;
	EDIS;
}

void CLA_configClaMemory(void)
{
	extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;

	EALLOW;

#ifdef _FLASH
	// Copy over code from FLASH to RAM
	memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
			(uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

	// Initialize and wait for CLA1ToCPUMsgRAM

	MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
	while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

	// Initialize and wait for CPUToCLA1MsgRAM

	MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
	while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

	// Select LS5RAM to be the programming space for the CLA
	// First configure the CLA to be the master for LS5 and then
	// set the space to be a program block

	MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
	MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

	EDIS;
}

void CLA_initCpu1Cla1(void)
{
	// Compute all CLA task vectors
	// On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
	// opposed to offsets used on older Type-0 CLAs

	EALLOW;

	Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
	Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
	Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
	Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
	Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
	Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
	Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
	Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

	// Enable the IACK instruction to start a task on CLA in software
	// for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
	// subset of tasks) by writing to their respective bits in the
	// MIER register

	// CLA Interrupt Sources

	DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = 0;   // 0 - Software Trigger
	DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2 = 0;	  // 0 - Software Trigger
	DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK3 = 0;
	DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK4 = 0;
	DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK5 = 0;
	DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK6 = 0;
	DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK7 = 0;
	DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK8 = 0;

	// CLA task interruts

	Cla1Regs.MIER.bit.INT1  =  1;		// Enable  CLA interrupt 1
	Cla1Regs.MIER.bit.INT2  =  1; 		// Disable CLA interrupt 2
	Cla1Regs.MIER.bit.INT3  =  0;		// Disable CLA interrupt 3
	Cla1Regs.MIER.bit.INT4  =  0; 		// Disable CLA interrupt 4
	Cla1Regs.MIER.bit.INT5  =  0;		// Disable CLA interrupt 5
	Cla1Regs.MIER.bit.INT6  =  0;		// Disable CLA interrupt 6
	Cla1Regs.MIER.bit.INT7  =  0;		// Disable CLA interrupt 7
	Cla1Regs.MIER.bit.INT8  =  0;		// Disable CLA interrupt 8

	EDIS;

	// Enable using software to start a task (IACK)
	// Cla1Regs.MCTL.bit.IACKE =  1;

	PieCtrlRegs.PIEIER11.bit.INTx1 = 0;	// Enable CLA Task1 in PIE group #1
	PieCtrlRegs.PIEIER11.bit.INTx2 = 0;	// Enable CLA Task1 in PIE group #1
	IER |= 0x0400;						// Enable INT11 in IER to enable PIE group 11
}
