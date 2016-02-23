/*
 * PLL_1P.h
 *
 *  Created on: 16-Aug-2015
 *      Author: Jaspreet
 */

#ifndef INCLUDE_PLL_1P_H_
#define INCLUDE_PLL_1P_H_
#define GLOBAL_Q 	22

#include "IQmathLib.h"
#include "USER_LIBS.h"
#include "CONTROL_LOOP.h"


typedef struct {
	_iq	In;			    // PLL Input
	_iq Kp;             // Proportional Gain for PLL PI
	_iq Ki_ts;          // Integral Gain for PLL PI
	_iq	OutMax; 	    // PI Output MAX Clamp
	_iq	OutMin; 	    // PI Output MIN Clamp
	_iq setFreq;	    // PLL Lock Frequency (Hz)

	_iq out_sine;		// PLL SINE   Output (Read)
	_iq out_cosine;	    // PLL COSINE Output (Read)
	_iq out_theta;	    // PLL THETA  Output (Read)

	bool Notch_EN;      // Set to "True" to enable Input 100Hz Notch

	PIREG 		pi1;    // (Do Not Change)
	FILT_2P2Z 	Notch1; // (Do Not Change)
	INTEGRATOR  intg1;  // (Do Not Change)

	void	(*compute)();

} PLL_1P;

typedef PLL_1P *PLL_1P_handle;

#define PLL_1P_DEFAULTS  {0.0, 50.0, 0.4, 200.0, -200.0, 50.0, 0.0, 0.0, 0.0, false, PIREG_DEFAULTS, NOTCH_2P2Z_F0_0_1k_FS_25k, INTEGRATOR_DEFAULTS, pll_1p_compute};

void pll_1p_compute(PLL_1P_handle);

#endif /* INCLUDE_PLL_1P_H_ */
