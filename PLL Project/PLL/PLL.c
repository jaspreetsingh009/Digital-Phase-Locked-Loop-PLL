/*
 * PLL Testing & SYNC Check
 * a. All Global Variables are defined in "./include/GLOBAL_VARS.h" file
 * b. All CLA variables are defined in "./include/CLA_SHARED.h" file
 * c. Set Configuration to CPU1_FLASH (Go to Properties/General, Click Manage Config, Set CPU1_FLASH to active)
 * d. PLL is configured in SYS_CONTROL() Function in this file
 *
 *
 *************************************************************
 *  			   TESTING CLA Routines						 *
 *************************************************************
 *
 *  1. 3-Phase Variable Freq SINE GEN:
 *  ----------------------------------
 *     OUTPUT VARS  : SINE_3A(0) | SINE_3B(120) | SINE_3C(240)
 *     SET FREQ VAR : SINE_3P_FREQ
 *
 *  2. 1-Phase Variable Freq SINE GEN:
 *------------------------------------
 *     OUTPUT VAR   : SINE_1A(0)
 *     SET FREQ VAR : SINE_1P_FREQ
 */


#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "CONTROL_LOOP.h"
#include "CLA_SHARED.h"
#include "GLOBAL_VARS.h"
#include "SYS_INIT.h"
#include "SYS_FUNC.h"


// CLA Variables - Data Section
//------------------------------------------------------

#pragma DATA_SECTION(SINE_1A, 	  	  "Cla1ToCpuMsgRAM")
float32 SINE_1A;

#pragma DATA_SECTION(SINE_3A, 	   	  "Cla1ToCpuMsgRAM")
float32 SINE_3A;

#pragma DATA_SECTION(SINE_3B, 	   	  "Cla1ToCpuMsgRAM")
float32 SINE_3B;

#pragma DATA_SECTION(SINE_3C, 	   	  "Cla1ToCpuMsgRAM")
float32 SINE_3C;

#pragma DATA_SECTION(SINE_1P_THETA,   "Cla1ToCpuMsgRAM")
float32 SINE_1P_THETA;

#pragma DATA_SECTION(SINE_3P_THETA,   "Cla1ToCpuMsgRAM")
float32 SINE_3P_THETA;

#pragma DATA_SECTION(pos, 		   	  "Cla1ToCpuMsgRAM")
int32 pos;

#pragma DATA_SECTION(pos1, 		   	  "Cla1ToCpuMsgRAM")
int32 pos1;

#pragma DATA_SECTION(SINE_1P_FREQ,    "CpuToCla1MsgRAM")
int32 SINE_1P_FREQ;

#pragma DATA_SECTION(SINE_3P_FREQ,    "CpuToCla1MsgRAM")
int32 SINE_3P_FREQ;

#pragma DATA_SECTION(SINEGEN_FREQ, 	  "CpuToCla1MsgRAM")
Uint32 SINEGEN_FREQ;


//  Function Prototypes
//------------------------------------------------------

__interrupt void cpu_timer0_isr(void);

void SYS_CONTROL(void);
void SYS_DATALOGGER(void);


void main(void)
{
	// Intialize Peripherals
	sysInit();

	// Intialize PSEUDO 3-PHASE SINEGEN Frequency
	SINEGEN_FREQ = CPU_TIMER_0_FREQ;;

	// Re-Map ISR Functions
	EALLOW;
	PieVectTable.TIMER0_INT = &cpu_timer0_isr;
	EDIS;

	// Initialize the Device Peripheral.
	InitCpuTimers();

	// Configure CPU-Timer 0 : 200MHz CPU Freq, CPU_TIMER_0_PERIOD  Period
	ConfigCpuTimer(&CpuTimer0, 200, CPU_TIMER_0_PERIOD);

	// To ensure precise timing, use write-only instructions to write to the entire register.
	CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

	// Enable CPU int1 which is connected to CPU-Timer 0
	IER |= M_INT1;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	while(1)
	{
		if (DATA_LOG_EN == 1)
		{
			if(UART_TX_STATE == READY_TO_TRANSMIT)
			{
				int i = 0;

				while(i<UART_BUF_LENGTH) {
					scia_data(RX_BUF_A[i], 0);
					scia_msg("A");
					scia_data(RX_BUF_B[i], 0);
					scia_msg("\n");
					i++;
				}

				UART_TX_STATE = WAIT_FOR_BUF_READY;
			}
		}
	}
}

__interrupt void cpu_timer0_isr(void)
{

	// UART RX | VARS UPDATE ROUTINE
	//--------------------------------------------------

	if(SciaRegs.SCIFFRX.bit.RXFFST == 1)
	{
		RX_DATA = SciaRegs.SCIRXBUF.all;

		if(DATA_LOG_EN == 1)
		{
			if(RX_DATA == 48) {
				UART_EN = 1;
			}

			if(RX_DATA == 49) {
				UART_EN     =  0;
				T0_TX_COUNT =  0;
			}
		}
	}

	// UART Data Acquistion

	if(DATA_LOG_EN == 1) { SYS_DATALOGGER(); }

	// PLL Control Loop
	SYS_CONTROL();

	// Force Start CLA Task1
	ClaForceTask1();

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

void SYS_DATALOGGER()
{
	if(UART_TX_STATE == WAIT_FOR_BUF_READY && UART_EN == 1)
	{
		T0_TX_COUNT = T0_TX_COUNT + 1;

		RX_BUF_A[T0_TX_COUNT]  =  (int32) UART_DATA_A;
		RX_BUF_B[T0_TX_COUNT]  =  (int32) UART_DATA_B;

		if(T0_TX_COUNT == UART_BUF_LENGTH) {
			T0_TX_COUNT        =  0;
			UART_TX_STATE      =  READY_TO_TRANSMIT;
		}
	}
}


void SYS_CONTROL()
{
	// Sample ADC Data
	//-----------------------------------------------------

	sampleADC_A();


	// Set CLA Sine Generators Frequencies
	//-----------------------------------------------------

	SINE_3P_FREQ = 50;    	// 	Set CLA SINE 3P GENERATOR FREQ to 50 Hz;
	SINE_1P_FREQ = 1000;  	// 	Set CLA SINE 1P GENERATOR FREQ to 1000 Hz;


	// Unity Scale ADCIN Conversion
	//-----------------------------------------------------

	ADCINA2 = ( ADC_A0  + ADC_A1  + ADC_A2  + ADC_A3  )/16384.0;


	// Control Loop Starts Here
	//------------------------------------------------------

	switch(PLL_IN_SEL) {
		case 0  :	pll1.In  =  SINE_3A;    				      // 50 Hz Sine Wave
					break;
		case 1  :   pll1.In  =  (0.9*SINE_3A) + (0.1*SINE_1A);    // 50 Hz Sine Wave + 1000 Hz Sine Wave
					break;
		default :   break;
	}


	pll1.setFreq      =   PLL_setFreq;
	pll1.Kp 	      =   PLL_Kp;
	pll1.Ki_ts 	      =   PLL_Ki;
	pll1.Notch_EN     =   false;

	pll1.compute(&pll1);

	// UART Output Assignment (for Data Logger)

	switch (UART1_ASSIGN) {
		case 0  :	UART_DATA_A = (pll1.In * 100) + 100;
					break;
		case 1  :   UART_DATA_A = (((pll1.out_theta) / 6.2831852) * 200);
					break;
		case 2  :   UART_DATA_A = (_IQsin(pll1.out_theta) * 100) + 100;
					break;
		case 3  :   UART_DATA_A = (_IQsin(pll1.out_theta*3) * 100) + 100;
					break;
		case 4  :   UART_DATA_A = (_IQsin(pll1.out_theta*5) * 100) + 100;
					break;
		default :   break;
	}

	switch (UART2_ASSIGN) {
		case 0 :	UART_DATA_B = (pll1.out_sine * 100) + 100;
					break;
		case 1 :    UART_DATA_B = ((pll1.out_theta) / 6.2831852) * 200;
					break;
		case 2  :   UART_DATA_B = (_IQsin(pll1.out_theta) * 100) + 100;
					break;
		case 3  :   UART_DATA_B = (_IQsin(pll1.out_theta*3) * 100) + 100;
					break;
		case 4  :   UART_DATA_B = (_IQsin(pll1.out_theta*5) * 100) + 100;
					break;
		default :   break;
	}


	DAC1 = SINE_3A;
	DAC2 = pll1.out_sine;


	// Update DAC A Output (DACA <- ADCINA0 Pin)
	DAC_A_OUT(DAC1, DAC_A_GAIN, DAC_A_OFFSET);

	// Update DAC B Output (DACB <- ADCINA1 Pin)
	DAC_B_OUT(DAC2, DAC_B_GAIN, DAC_B_OFFSET);
}

