#include "integrator.h"

void integrator_compute(INTEGRATOR *v){

	_iq temp     =  0.0;
	_iq SAMPLE_F =  Ts_2;

	temp    = _IQmpy(SAMPLE_F, (v->In + v->xn_1));  // Ts_2 * [x(n) + x(n-1)]
	temp   +=  v->yn_1;								// Ts_2 * [x(n) + x(n-1)] + y(n-1)

	if ( temp > v->ClampMax) {
		 temp = 0.0;
	}

	else if (temp < v->ClampMin){
		temp =  0.0;
	}

	v->yn_1  =  temp;
	v->xn_1  =  v->In;

	v->Out   =  v->yn_1;
}


