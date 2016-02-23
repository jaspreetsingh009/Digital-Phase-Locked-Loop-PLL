/*
 * SYS_FUNC.c
 *
 *  Created on: 14-Aug-2015
 *      Author: Jaspreet
 */

#ifndef SOURCE_SYS_FUNC_C_
#define SOURCE_SYS_FUNC_C_

#include "SYS_FUNC.h"

// ADC SCALE CONVERT
//-------------------------------
float32 ADC_CONVERT(Uint16 value, float GAIN, float OFFSET)
{
	float32 temp = (value *3.3)/16384.0;
	temp = temp - OFFSET;
	temp = temp * GAIN;

	return temp;
}

// Output DAC A
//-------------------------------
void DAC_A_OUT(float value, int32 GAIN, int32 OFFSET)
{
	DacaRegs.DACVALS.bit.DACVALS =  (value*GAIN) + OFFSET;
}

// Output DAC B
//-------------------------------
void DAC_B_OUT(float value, int32 GAIN, int32 OFFSET)
{
	DacbRegs.DACVALS.bit.DACVALS =  (value*GAIN) + OFFSET;
}

// Transmit a BYTE via UART
//-------------------------------
void scia_xmit(int a)
{
	while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
	SciaRegs.SCITXBUF.all =a;
}

// Transmit a MSG via UART
//-------------------------------
void scia_msg(char * msg)
{
	int i;
	i = 0;
	while(msg[i] != '\0')
	{
		scia_xmit(msg[i]);
		i++;
	}
}

// Transmit INTEGER Data via UART
//---------------------------------
void scia_data(long data, unsigned char ndigits){
	unsigned char sign, s[6];
	unsigned int i;
	sign = ' ';
	if(data < 0) {
		sign='-';
		data = -data;
	}
	i = 0;
	do {
		s[i++] = data % 10 + '0';
		if(i == ndigits) {
			s[i++]='.';
		}
	} while( (data /= 10) > 0);
	s[i] = sign;
	do {
		while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
		SciaRegs.SCITXBUF.all = s[i];
	} while(i--);
}

// ADC_A Data Acquistion Function
//---------------------------------
void sampleADC_A()
{
	//Force start of conversion on SOC0 (All SOCs Selected)
	AdcaRegs.ADCSOCFRC1.all = 0xFFFF;

	//Wait for end of conversion.
	while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0){}  // Wait for ADCINT1
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        // Clear ADCINT1
}

// ADC_B Data Acquistion Function
//---------------------------------
void sampleADC_B()
{
	//Force start of conversion on SOC0 (All SOCs Selected)
	AdcbRegs.ADCSOCFRC1.all = 0xFFFF;

	//Wait for end of conversion.
	while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0){}  // Wait for ADCINT1
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        // Clear ADCINT1
}


#endif /* SOURCE_SYS_FUNC_C_ */
