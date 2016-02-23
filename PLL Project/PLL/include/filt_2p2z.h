#ifndef __FILT_2P2Z_H__
#define __FILT_2P2Z_H__

#define GLOBAL_Q 	22

#include "IQmathLib.h"
#include "F28x_Project.h"

typedef struct {
	_iq	In;
	_iq	A0;
	_iq A1;
	_iq	A2;
	_iq	B0;
	_iq	B1;
	_iq	B2;
	_iq yn_1;
	_iq yn_2;
	_iq xn_1;
	_iq xn_2;

	float32	Out;

	void	(*compute)();
} FILT_2P2Z;

typedef FILT_2P2Z *FILT_2P2Z_handle;

#define NOTCH_2P2Z_F0_0_1k_FS_25k  { 0.0, 1.0, -1.9972770, 0.9979080, 0.9989540, -1.9972770, 0.9989540, 0.0, 0.0, 0.0, 0.0, 0.0, filt_2p2z_compute }
#define FILT_2P2Z_DEFAULTS         { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, filt_2p2z_compute}

void filt_2p2z_compute(FILT_2P2Z_handle);

#endif 
