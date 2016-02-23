/*
 * PLL_1P.c
 *
 *  Created on: 16-Aug-2015
 *      Author: Jaspreet
 */

#include "PLL_1P.h"

void pll_1p_compute(PLL_1P *v)
{

	_iq OMEGA_0 = _IQmpy(6.2831852, v->setFreq);

	//------------------------------------//
	//    OPT Input Notch Filter (2P2Z)   //
	//------------------------------------//

	if(v->Notch_EN == true) {
		v->Notch1.In   =   _IQmpy(v->In, v->out_cosine);
		v->Notch1.compute(&v->Notch1);
		v->pi1.Ref 	   =   v->Notch1.Out;
	}

	else {
		v->pi1.Ref     =   _IQmpy(v->In, v->out_cosine);
	}


	//------------------------------------//
	//  	  PLL Loop Filter (PI)        //
	//------------------------------------//

	v->pi1.Fdb 	   =   0;
	v->pi1.Kp  	   =   v->Kp;
	v->pi1.Ki_ts   =   v->Ki_ts;
	v->pi1.OutMax  =   v->OutMax;
	v->pi1.OutMin  =   v->OutMin;

	v->pi1.compute(&v->pi1);


	OMEGA_0   = OMEGA_0 + v->pi1.Out;


	//------------------------------------//
	// 		   Integration (1/s)		  //
	//------------------------------------//

	v->intg1.In        =  OMEGA_0;
	v->intg1.ClampMax  =  6.2831852;
	v->intg1.ClampMin  =  0;

	v->intg1.compute(&v->intg1);


	// PLL Outputs
	//--------------------------------------

	v->out_theta  =  v->intg1.Out;
	v->out_sine   =  _IQsin(v->out_theta);
	v->out_cosine =  _IQcos(v->out_theta);
}



