
#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#define GLOBAL_Q 	22


#include "IQmathLib.h"
#include "CONTROL_LOOP.h"


typedef struct {
	_iq	In; 		// Input Value
	_iq	Out;		// Output Value
	_iq	ClampMin;	// Integrator Min Clamp Value
	_iq	ClampMax;   // Integrator Max Clamp Value
	_iq yn_1;		// Output buffer
	_iq xn_1;       // Input buffer

	void	(*compute)();

} INTEGRATOR;

typedef INTEGRATOR *INTEGRATOR_handle;

#define INTEGRATOR_DEFAULTS {0.0, 0.0, -100, 100, 0.0, 0.0, integrator_compute}

void integrator_compute(INTEGRATOR_handle);


#endif 
