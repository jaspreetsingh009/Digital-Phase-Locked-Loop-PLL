#include "filt_2p2z.h"

void filt_2p2z_compute(FILT_2P2Z *v){

	_iq temp = 0.0;

    temp   =  _IQmpy(v->B0, v->In);
    temp  +=  _IQmpy(v->B1, v->xn_1);
    temp  +=  _IQmpy(v->B2, v->xn_2);
    temp  -=  _IQmpy(v->A1, v->yn_1);
    temp  -=  _IQmpy(v->A2, v->yn_2);

    v->xn_2 = v->xn_1;
    v->xn_1 = v->In;

    v->yn_2 = v->yn_1;
    v->yn_1 = temp;
    v->Out  = temp;
}


