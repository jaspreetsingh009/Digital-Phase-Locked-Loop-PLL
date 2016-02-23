/*
 * CONTROL_LOOP.h
 *
 *  Created on: 07-Aug-2015
 *      Author: Jaspreet Singh
 */

#ifndef PLL_INCLUDE_CONTROL_LOOP_H_
#define PLL_INCLUDE_CONTROL_LOOP_H_


// UART DEFINES
//--------------------------------------------------------------//
#define UART_BUF_LENGTH         1200							//
#define DATA_LOG_EN				1								//
//--------------------------------------------------------------//

#define WAIT_FOR_BUF_READY      0
#define READY_TO_TRANSMIT		1


// Frequency Defines (in Hz) | Specify these

//--------------------------------------------------------------//
#define  CPU_TIMER_0_PERIOD		40   							// Specified in microSeconds {40uS <-> 25kHz}
//--------------------------------------------------------------//

#define  CPU_TIMER_0_FREQ		1000000/CPU_TIMER_0_PERIOD      // in Hz
#define  MILLI_SECOND_COUNT		1000/CPU_TIMER_0_PERIOD
#define  SECOND_COUNT			(long) 1000*MILLI_SECOND_COUNT

//--------------------------------------------------------------//
#define  Fs	 					25000  						    // Sampling Frequency (TIMER_0_PERIOD = 1000000/Fs)
//--------------------------------------------------------------//

#define  Ts					    (float) 1/Fs					// Sampling Period
#define  Ts_2					(float) 0.5/Fs					// Half Sampling Period



#endif /* PLL_INCLUDE_CONTROL_LOOP_H_ */
