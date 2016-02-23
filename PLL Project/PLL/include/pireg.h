#ifndef __PIREG_H__
#define __PIREG_H__
#define GLOBAL_Q 	22

#include "IQmathLib.h"

typedef struct {
	_iq	Ref; 		// PI reference
	_iq	Fdb;    	// PI feedback
	_iq	Err;    	// (Do Not Change)
	_iq Kp;     	// Proportional Gain
	_iq	Ki_ts;  	// Integral Gain
	_iq	OutMax; 	// PI Output MAX Clamp
	_iq	OutMin; 	// PI Output MIN Clamp
	_iq	Out;    	// PI Output (Read)
	_iq	integral;   // (Do Not Change)

	void	(*compute)();
} PIREG;

typedef PIREG *PIREG_handle;

#define PIREG_DEFAULTS { 0, 0, 0, 0, 0, 0, 0, 0, 0, pireg_compute}
void pireg_compute(PIREG_handle);
#endif 
