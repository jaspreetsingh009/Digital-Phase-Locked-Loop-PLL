#include "pireg.h"

void pireg_compute(PIREG *v){
    v->Err =  v->Ref - v->Fdb;

    v->integral =  v->integral + _IQmpy(v->Ki_ts,v->Err);

    if ( v->integral > v->OutMax){
    	 v->integral = v->OutMax;
    } else if (v->integral < v->OutMin){
    	 v->integral =  v->OutMin;
    }

    v->Out = _IQmpy(v->Kp,v->Err) + v->integral;

    if ( v->Out > v->OutMax){
		 v->Out = v->OutMax;
	} else if ( v->Out < v->OutMin){
		 v->Out =  v->OutMin;
	}
}
