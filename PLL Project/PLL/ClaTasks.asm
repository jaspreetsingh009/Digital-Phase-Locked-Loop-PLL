
;----------------------------------;
; 		     Includes 			   ;
;----------------------------------;

	.cdecls C, LIST, "CLA_SHARED.h"

;----------------------------------;
; 			 Defines			   ;
;----------------------------------;

SINEGEN_1P_EN	 .set		1
SINEGEN_3P_EN	 .set		1
CLA_DEBUG 		 .set  		1

;----------------------------------;
;		     Constants			   ;
;----------------------------------;

_PI 			.set 	 3.1415926
_PI_2 			.set 	 1.5707963
_CONST_A		.set     1.2732395
_CONST_B		.set	 0.4052847
_CONST_C		.set	 0.225

_THETA_120      .set     2.0943951
_THETA_240		.set     4.1887902

;----------------------------------;
;  FILTER VARIABLES : 2p2z Notch   ;
;----------------------------------;

;----------------------------------;
;  fo   <-  100 Hz  			   ;
;  Fs   <-  50kHz   			   ;
;  Gain <- -23dB    			   ;
;  Qf   <-  20      			   ;
;----------------------------------;

_A0_QP_0_1k		.set	 1.0
_A1_QP_0_1k		.set	-1.9974867
_A2_QP_0_1k		.set	 0.9976446

_B0_QP_0_1k		.set	 1.0
_B1_QP_0_1k		.set	-1.9999997
_B2_QP_0_1k		.set	 1.0001577

;----------------------------------;
;	    Function Definitions	   ;
;----------------------------------;

; CLA code must be within its own assembly section and must be even aligned.


       .sect        "Cla1Prog"
_Cla1Prog_Start
       .align       2



***********************************************************************
*          CLA TASK 1  ::  SineWave + Digital Filters				  *
***********************************************************************

_Cla1Task1:

	.if CLA_DEBUG == 1
		MDEBUGSTOP
	.endif


****************************************************************************************************************************************************

	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;	     SINEGEN 3-PHASE GENERATOR			;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; EXECUTION TIME: 1.48 us | 178 Instruction ;
	;											;
	;  SINE_3A <- SIN( THETA + 0   )			;
	;  SINE_3B <- SIN( THETA + 120 )			;
	;  SINE_3C <- SIN( THETA + 240 )			;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	.if SINEGEN_3P_EN == 1

	;-------------------------------------------;
	;       Theta Generation for Testing		;
	; EXECUTION TIME: 0.14 us | 28 Instructions ;
	;-------------------------------------------;

	; cycle = Fs / Fo  (Fo <- sine Freq.)
	;--------------------------------------------

	MUI32TOF32 MR2, @_SINEGEN_FREQ				; MR2 <- SINEGEN_FREQ
	MNOP

	MI32TOF32  MR1, @_SINE_3P_FREQ				; MR1 <- Fo

	MEINVF32   MR1, MR1							; MR1 <- 1/Fo
	MMPYF32	   MR1, MR2, MR1					; MR1 <- Fs/Fo

	; _pos = (_pos + 1)%_cycle
	;--------------------------------------------

	MI32TOF32   MR2, @_pos						; MR2 <- pos
	MADDF32		MR2, MR2, #1.0					; MR2 <- pos + #1.0

	MCMPF32		MR2, MR1						; Compare pos & cycle

	MNOP										; Do Nothing
	MNOP
	MNOP

	MBCNDD		_skipMOD, LT					; Jump if(pos < cycle)

	MNOP
	MNOP
	MNOP

	MMOVF32		MR2, #0.0						; MR2 <- 0.0
	MNOP

	MNOP

_skipMOD

	MF32TOI32	MR0, MR2						; Float to int32
	MMOV32		@_pos, MR0						; pos <- MR0

	; MR1 <- cycle | MR2 <- pos

	;-------------------------------------------;
	; 		     Phase Calculation				;
	;-------------------------------------------;

	MEINVF32    MR1, MR1						; MR1 <- 1/cycle
	MNOP

	MMPYF32		MR3, MR2, MR1					; MR3 <- pos * 1/cycle

	MMOVF32		MR1, #_PI						; MR1 <- PI
	MNOP

	MADDF32		MR1, MR1, MR1					; MR1 <- PI + PI
	MMPYF32		MR3, MR1, MR3					; MR3 <- 2 * PI * pos * 1/cycle
	MMOV32		MR1, MR3						; MR1 <- MR3

	MMOV32		@_SINE_3P_THETA, MR3			; SINE_3P_THETA <- MR3;


	; x (MR1) <- theta (in radians)
	;-------------------------------------------;

	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;             SINE 3A  (0 DEG)              ;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; 		Approx. Sine Wave Generation 		;
	; EXECUTION TIME: 0.25 us | 50 Instructions ;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	MMOV32	  MR1, @_SINE_3P_THETA				; MR1 <- SINE_3P_THETA

	;-------------------------------------------;
	;      Check for THETA <- (-PI, PI)			;
	;-------------------------------------------;

	; MR1 <- theta (radians)
	;--------------------------------------------

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MCMPF32   MR1, MR2							; Compare MR1 & PI
	MNOP
	MNOP
	MNOP

	MBCNDD	 _LESSER_THAN_PI_1, LT				; Jump if MR1 < PI

	MNOP
	MNOP
	MNOP

	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI
	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI - PI
	MNOP

_LESSER_THAN_PI_1

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MNEGF32	  MR2, MR2							; MR2 <- (-) PI

	MCMPF32	  MR1, MR2							; Compare MR & (-) PI

	MNOP
	MNOP
	MNOP

	MBCNDD	 _GREATER_NEG_PI_1, GT				; Jump if MR1 > (-) PI

	MNOP
	MNOP
	MNOP

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI
	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI + PI

	MNOP

_GREATER_NEG_PI_1

	; MR1 <- theta in Radians (x)
	;-------------------------------------------

	MMOVF32		MR2, #_CONST_A
	MNOP
	MMOVF32		MR3, #_CONST_B
	MNOP

	MMPYF32		MR0, MR2, MR1					; MR0 <- A * x
	MMPYF32		MR2, MR3, MR1					; MR2 <- B * x
	MABSF32	    MR1, MR1						; MR1 <- abs(x)
	MMPYF32		MR2, MR2, MR1					; MR2 <- B * x * abs(x)

	MSUBF32		MR0, MR0, MR2					; MR0 <- A*x - B*x*abs(x)

	; MR0 <- Low Precision Sine Value (t)
	;--------------------------------------------

	MABSF32		MR1, MR0						; MR1 <-  abs(t)
	MMPYF32		MR1, MR1, MR0					; MR1 <-  abs(t) * t
	MSUBF32		MR1, MR1, MR0					; MR1 <- (abs(t) * t) - t

	MMOVF32		MR2, #_CONST_C					; MR2 <-  C
	MNOP

	MMPYF32		MR2, MR1, MR2					; MR2 <-  C * [(abs(t) * t) - t]
	MADDF32		MR1, MR2, MR0					; MR1 <- {C * [(abs(t) * t) - t]} + t

	MMOV32		@_SINE_3A, MR1					; PLL_SINE <- MR1


	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;             SINE 3B  (120 DEG)            ;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; 	    Approx. Sine Wave Generation 		;
	; EXECUTION TIME: 0.25 us | 50 Instructions ;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	; sin(y) = sin(120 + x)
	;-------------------------------------------

	MMOV32	  MR1, @_SINE_3P_THETA				; MR1 <- SINE_3P_THETA

	MMOVF32	  MR2, #_THETA_120					; MR2 <- THETA_120
	MNOP

	MADDF32	  MR1, MR1, MR2						; MR1 <- SINE_3P_THETA	 + THETA_120

	;-------------------------------------------;
	;      Check for THETA <- (-PI, PI)			;
	;-------------------------------------------;

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MCMPF32   MR1, MR2							; Compare MR1 & PI
	MNOP
	MNOP
	MNOP

	MBCNDD	 _LESSER_THAN_PI_2, LT				; Jump if MR1 < PI

	MNOP
	MNOP
	MNOP

	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI
	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI - PI
	MNOP

_LESSER_THAN_PI_2

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MNEGF32	  MR2, MR2							; MR2 <- (-) PI

	MCMPF32	  MR1, MR2							; Compare MR & (-) PI

	MNOP
	MNOP
	MNOP

	MBCNDD	 _GREATER_NEG_PI_2, GT				; Jump if MR1 > (-) PI

	MNOP
	MNOP
	MNOP

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI
	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI + PI

	MNOP

_GREATER_NEG_PI_2

	; MR1 <- theta in Radians (x)
	;-------------------------------------------

	MMOVF32		MR2, #_CONST_A					; MR2 <- CONST_A
	MNOP										; Do Nothing

	MMOVF32		MR3, #_CONST_B					; MR3 <- CONST_B
	MNOP										; Do Nothing

	MMPYF32		MR0, MR2, MR1					; MR0 <- A * x
	MMPYF32		MR2, MR3, MR1					; MR2 <- B * x
	MABSF32	    MR1, MR1						; MR1 <- abs(x)
	MMPYF32		MR2, MR2, MR1					; MR2 <- B * x * abs(x)

	MSUBF32		MR0, MR0, MR2					; MR0 <- A*x - B*x*abs(x)

	; MR0 <- Low Precision Cosine Value (t)
	;-------------------------------------------;

	MABSF32		MR1, MR0						; MR1 <-  abs(t)
	MMPYF32		MR1, MR1, MR0					; MR1 <-  abs(t) * t
	MSUBF32		MR1, MR1, MR0					; MR1 <- (abs(t) * t) - t

	MMOVF32		MR2, #_CONST_C					; MR2 <-  C
	MNOP

	MMPYF32		MR2, MR1, MR2					; MR2 <-  C * [(abs(t) * t) - t]
	MADDF32		MR1, MR2, MR0					; MR1 <- {C * [(abs(t) * t) - t]} + t

	MMOV32		@_SINE_3B, MR1				    ; SINE_3B <- MR1


	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;             SINE 3C  (240 DEG)            ;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; 	    Approx. Sine Wave Generation 		;
	; EXECUTION TIME: 0.25 us | 50 Instructions ;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	; sin(y) = sin(240 + x)
	;-------------------------------------------

	MMOV32	  MR1, @_SINE_3P_THETA				; MR1 <- SINE_3P_THETA

	MMOVF32	  MR2, #_THETA_120					; MR2 <- THETA_120
	MNOP

	MSUBF32	  MR1, MR1, MR2						; MR1 <- SINE_3P_THETA	- THETA_120

	;-------------------------------------------;
	;      Check for THETA <- (-PI, PI)			;
	;-------------------------------------------;

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MCMPF32   MR1, MR2							; Compare MR1 & PI
	MNOP
	MNOP
	MNOP

	MBCNDD	 _LESSER_THAN_PI_3, LT				; Jump if MR1 < PI

	MNOP
	MNOP
	MNOP

	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI
	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI - PI
	MNOP

_LESSER_THAN_PI_3

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MNEGF32	  MR2, MR2							; MR2 <- (-) PI

	MCMPF32	  MR1, MR2							; Compare MR & (-) PI

	MNOP
	MNOP
	MNOP

	MBCNDD	 _GREATER_NEG_PI_3, GT				; Jump if MR1 > (-) PI

	MNOP
	MNOP
	MNOP

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI
	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI + PI

	MNOP

_GREATER_NEG_PI_3

	; MR1 <- theta in Radians (x)
	;-------------------------------------------

	MMOVF32		MR2, #_CONST_A					; MR2 <- CONST_A
	MNOP										; Do Nothing

	MMOVF32		MR3, #_CONST_B					; MR3 <- CONST_B
	MNOP										; Do Nothing

	MMPYF32		MR0, MR2, MR1					; MR0 <- A * x
	MMPYF32		MR2, MR3, MR1					; MR2 <- B * x
	MABSF32	    MR1, MR1						; MR1 <- abs(x)
	MMPYF32		MR2, MR2, MR1					; MR2 <- B * x * abs(x)

	MSUBF32		MR0, MR0, MR2					; MR0 <- A*x - B*x*abs(x)

	; MR0 <- Low Precision Cosine Value (t)
	;-------------------------------------------;

	MABSF32		MR1, MR0						; MR1 <-  abs(t)
	MMPYF32		MR1, MR1, MR0					; MR1 <-  abs(t) * t
	MSUBF32		MR1, MR1, MR0					; MR1 <- (abs(t) * t) - t

	MMOVF32		MR2, #_CONST_C					; MR2 <-  C
	MNOP

	MMPYF32		MR2, MR1, MR2					; MR2 <-  C * [(abs(t) * t) - t]
	MADDF32		MR1, MR2, MR0					; MR1 <- {C * [(abs(t) * t) - t]} + t

	MMOV32		@_SINE_3C, MR1				    ; SINE_3B <- MR1

	;-------------------------------------------;

	.endif

	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;	     SINEGEN 1-PHASE GENERATOR			;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; EXECUTION TIME: 1.48 us | 178 Instruction ;
	;											;
	;  SINE_3A <- SIN( THETA + 0   )			;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	.if SINEGEN_1P_EN == 1

	;-------------------------------------------;
	;       Theta Generation for Testing		;
	; EXECUTION TIME: 0.14 us | 28 Instructions ;
	;-------------------------------------------;

	; cycle = Fs / Fo  (Fo <- sine Freq.)
	;--------------------------------------------

	MUI32TOF32 MR2, @_SINEGEN_FREQ				; MR2 <- SINEGEN_FREQ
	MNOP

	MI32TOF32  MR1, @_SINE_1P_FREQ				; MR1 <- Fo

	MEINVF32   MR1, MR1							; MR1 <- 1/Fo
	MMPYF32	   MR1, MR2, MR1					; MR1 <- Fs/Fo

	; _pos = (_pos + 1)%_cycle
	;--------------------------------------------

	MI32TOF32   MR2, @_pos1						; MR2 <- pos
	MADDF32		MR2, MR2, #1.0					; MR2 <- pos + #1.0

	MCMPF32		MR2, MR1						; Compare pos & cycle

	MNOP										; Do Nothing
	MNOP
	MNOP

	MBCNDD		_skipMOD1, LT					; Jump if(pos < cycle)

	MNOP
	MNOP
	MNOP

	MMOVF32		MR2, #0.0						; MR2 <- 0.0
	MNOP

	MNOP

_skipMOD1

	MF32TOI32	MR0, MR2						; Float to int32
	MMOV32		@_pos1, MR0						; pos <- MR0

	; MR1 <- cycle | MR2 <- pos1

	;-------------------------------------------;
	; 		     Phase Calculation				;
	;-------------------------------------------;

	MEINVF32    MR1, MR1						; MR1 <- 1/cycle
	MNOP

	MMPYF32		MR3, MR2, MR1					; MR3 <- pos1 * 1/cycle

	MMOVF32		MR1, #_PI						; MR1 <- PI
	MNOP

	MADDF32		MR1, MR1, MR1					; MR1 <- PI + PI
	MMPYF32		MR3, MR1, MR3					; MR3 <- 2 * PI * pos1 * 1/cycle
	MMOV32		MR1, MR3						; MR1 <- MR3

	MMOV32		@_SINE_1P_THETA, MR3			; SINE_1P_THETA <- MR3;


	; x (MR1) <- theta (in radians)
	;-------------------------------------------;

	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;             SINE 3A  (0 DEG)              ;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	;											;
	; 		Approx. Sine Wave Generation 		;
	; EXECUTION TIME: 0.25 us | 50 Instructions ;
	;											;
	;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

	MMOV32	  MR1, @_SINE_1P_THETA					; MR1 <- SINE_1P_THETA

	;-------------------------------------------;
	;      Check for THETA <- (-PI, PI)			;
	;-------------------------------------------;

	; MR1 <- theta (radians)
	;--------------------------------------------

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MCMPF32   MR1, MR2							; Compare MR1 & PI
	MNOP
	MNOP
	MNOP

	MBCNDD	 _LESSER_THAN_PI_11, LT				; Jump if MR1 < PI

	MNOP
	MNOP
	MNOP

	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI
	MSUBF32	  MR1, MR1, MR2						; MR1 <- MR1 - PI - PI
	MNOP

_LESSER_THAN_PI_11

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MNEGF32	  MR2, MR2							; MR2 <- (-) PI

	MCMPF32	  MR1, MR2							; Compare MR & (-) PI

	MNOP
	MNOP
	MNOP

	MBCNDD	 _GREATER_NEG_PI_11, GT				; Jump if MR1 > (-) PI

	MNOP
	MNOP
	MNOP

	MMOVF32	  MR2, #_PI							; MR2 <- PI
	MNOP

	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI
	MADDF32	  MR1, MR1, MR2						; MR1 <- MR1 + PI + PI

	MNOP

_GREATER_NEG_PI_11

	; MR1 <- theta in Radians (x)
	;-------------------------------------------

	MMOVF32		MR2, #_CONST_A
	MNOP
	MMOVF32		MR3, #_CONST_B
	MNOP

	MMPYF32		MR0, MR2, MR1					; MR0 <- A * x
	MMPYF32		MR2, MR3, MR1					; MR2 <- B * x
	MABSF32	    MR1, MR1						; MR1 <- abs(x)
	MMPYF32		MR2, MR2, MR1					; MR2 <- B * x * abs(x)

	MSUBF32		MR0, MR0, MR2					; MR0 <- A*x - B*x*abs(x)

	; MR0 <- Low Precision Sine Value (t)
	;--------------------------------------------

	MABSF32		MR1, MR0						; MR1 <-  abs(t)
	MMPYF32		MR1, MR1, MR0					; MR1 <-  abs(t) * t
	MSUBF32		MR1, MR1, MR0					; MR1 <- (abs(t) * t) - t

	MMOVF32		MR2, #_CONST_C					; MR2 <-  C
	MNOP

	MMPYF32		MR2, MR1, MR2					; MR2 <-  C * [(abs(t) * t) - t]
	MADDF32		MR1, MR2, MR0					; MR1 <- {C * [(abs(t) * t) - t]} + t

	MMOV32		@_SINE_1A, MR1					; PLL_SINE <- MR1

	.endif


****************************************************************************************************************************************************

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T1End:


***********************************************************************
*      			CLA TASK 2  ::  Initalization						  *
***********************************************************************

_Cla1Task2:

	.if CLA_DEBUG == 1
		MDEBUGSTOP
	.endif

	MMOVIZ      MR0, #0.0						; MR0 <- 0.0
	MF32TOI32	MR1, MR0						; MR1 <- (int32) MR0

	;-------------------------------------------;
	; 		  Floating Point Variables          ;
	;-------------------------------------------;

	MMOV32		@_SINE_3A, MR0					; SINE_3A <- 0.0
	MMOV32		@_SINE_3B, MR0					; SINE_3B <- 0.0
	MMOV32		@_SINE_3C, MR0					; SINE_3C <- 0.0

	MMOV32		@_SINE_1A, MR0					; SINE_3C <- 0.0

	MMOV32		@_SINE_1P_THETA, MR0			; SINE_1P_THETA <- 0.0
	MMOV32		@_SINE_3P_THETA, MR0			; SINE_1P_THETA <- 0.0

	;-------------------------------------------;
	; 		    Fixed Point Variables           ;
	;-------------------------------------------;

	MMOV32		@_pos, MR1						; pos <- 0
	MMOV32		@_pos1, MR1						; pos1 <- 0

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T2End:

_Cla1Task3:

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T3End:
    

_Cla1Task4:

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T4End:
    
    
_Cla1Task5:  

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T5End:
    
_Cla1Task6:

    MSTOP
    MNOP
    MNOP
    MNOP

_Cla1T6End:    

_Cla1Task7:

	MSTOP
	MNOP
	MNOP
	MNOP

_Cla1T7End:
          
_Cla1Task8:

	MSTOP
	MNOP
	MNOP
	MNOP

_Cla1T8End:

_Cla1Prog_End:

	.end
