// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File 
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are. 
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

   BEGIN           	: origin = 0x080000, length = 0x000002  // FREE
   RAMM0           	: origin = 0x000122, length = 0x0002DE	// FREE
   RAMD0           	: origin = 0x00B000, length = 0x000800  // FREE
   RAMLS4      		: origin = 0x00A000, length = 0x000800
   RAMLS5           : origin = 0x00A800, length = 0x000800
   RAMGS14          : origin = 0x01A000, length = 0x001000	// FREE
   RAMGS15          : origin = 0x01B000, length = 0x001000  // FREE
   RESET           	: origin = 0x3FFFC0, length = 0x000002

   /* Flash sectors */
   FLASHA           : origin = 0x080002, length = 0x001FFE	/* on-chip Flash */
   FLASHB           : origin = 0x082000, length = 0x002000	/* on-chip Flash */
   FLASHC           : origin = 0x084000, length = 0x002000	/* on-chip Flash */
   FLASHD           : origin = 0x086000, length = 0x002000	/* on-chip Flash */
   FLASHE           : origin = 0x088000, length = 0x008000	/* on-chip Flash */
   FLASHF           : origin = 0x090000, length = 0x008000	/* on-chip Flash */  // FREE
   FLASHG           : origin = 0x098000, length = 0x008000	/* on-chip Flash */  // FREE
   FLASHH           : origin = 0x0A0000, length = 0x008000	/* on-chip Flash */  // FREE
   FLASHI           : origin = 0x0A8000, length = 0x008000	/* on-chip Flash */  // FREE
   FLASHJ           : origin = 0x0B0000, length = 0x008000	/* on-chip Flash */  // FREE
   FLASHK           : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */  // FREE
   FLASHL           : origin = 0x0BA000, length = 0x002000	/* on-chip Flash */  // FREE
   FLASHM           : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */  // FREE
   FLASHN           : origin = 0x0BE000, length = 0x002000	/* on-chip Flash */  // FREE

PAGE 1 :

   BOOT_RSVD        : origin = 0x000002, length = 0x000120     /* Part of M0, BOOT rom will use this for stack */
   RAMM1            : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */

   RAMLS0          	: origin = 0x008000, length = 0x000800
   RAMLS1          	: origin = 0x008800, length = 0x000800
   RAMLS2      		: origin = 0x009000, length = 0x000800
   RAMLS3      		: origin = 0x009800, length = 0x000800     // FREE
   
   
   RAMGS0           : origin = 0x00C000, length = 0x001000
   RAMGS1           : origin = 0x00D000, length = 0x001000
   RAMGS2           : origin = 0x00E000, length = 0x001000
   RAMGS3           : origin = 0x00F000, length = 0x001000
   RAMGS4           : origin = 0x010000, length = 0x001000
   RAMGS5           : origin = 0x011000, length = 0x001000	   // FREE
   RAMGS6           : origin = 0x012000, length = 0x001000	   // FREE
   RAMGS7           : origin = 0x013000, length = 0x001000	   // FREE
   RAMGS8           : origin = 0x014000, length = 0x001000	   // FREE
   RAMGS9           : origin = 0x015000, length = 0x001000	   // FREE

   // .ebss RAM

   RAMEBSS          : origin = 0x016000, length = 0x004000

                    
   CLA1_MSGRAMLOW   : origin = 0x001480, length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500, length = 0x000080
   
}


SECTIONS
{
   /* Allocate program areas: */
   .cinit           : > FLASHB      PAGE = 0, ALIGN(4)

   //.cinit           : > FLASHB | FLASHH

   .pinit           : > FLASHB,     PAGE = 0, ALIGN(4)
   .text            : > FLASHB      PAGE = 0, ALIGN(4)
   codestart        : > BEGIN       PAGE = 0, ALIGN(4)
   ramfuncs         : LOAD = FLASHD,
                      RUN = RAMLS4,
                      LOAD_START(_RamfuncsLoadStart),
                      LOAD_SIZE(_RamfuncsLoadSize),
                      LOAD_END(_RamfuncsLoadEnd),
                      RUN_START(_RamfuncsRunStart),
                      RUN_SIZE(_RamfuncsRunSize),
                      RUN_END(_RamfuncsRunEnd),
                      PAGE = 0, ALIGN(4)
					 
   /* Allocate uninitalized data sections: */
   .stack           : > RAMM1        PAGE = 1
   .ebss            : > RAMEBSS      PAGE = 1
   .esysmem         : > RAMLS2       PAGE = 1


   /* Initalized sections go in Flash */
   .econst          : > FLASHB | FLASHE      PAGE = 0, ALIGN(4)
   .switch          : > FLASHB      PAGE = 0, ALIGN(4)
   
   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   Filter_RegsFile  : > RAMGS0,	   PAGE = 1
   
    /* CLA specific sections */
   Cla1Prog         : LOAD = FLASHD,
                      RUN = RAMLS5,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(4)

   CLADataLS0		: > RAMLS0, PAGE=1

   //CLADataLS0		: >> RAMLS0 | RAMLS3, PAGE=1      Check the double >>

   CLADataLS1		: > RAMLS1, PAGE=1

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1
  
   /* The following section definition are for SDFM examples */		
   Filter1_RegsFile : > RAMGS1,	PAGE = 1, fill=0x1111
   Filter2_RegsFile : > RAMGS2,	PAGE = 1, fill=0x2222
   Filter3_RegsFile : > RAMGS3,	PAGE = 1, fill=0x3333
   Filter4_RegsFile : > RAMGS4,	PAGE = 1, fill=0x4444
   
   /* Allocate IQ math areas: */
   IQmath			: > FLASHB, PAGE = 0, ALIGN(4)            /* Math Code */
   IQmathTables		: > FLASHC, PAGE = 0, ALIGN(4)
   
#ifdef CLA_C
   /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
   CLAscratch       :
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  RAMLS1,  PAGE = 1

   .scratchpad      : > RAMLS1,       PAGE = 1
   .bss_cla		    : > RAMLS1,       PAGE = 1
   .const_cla	    :  LOAD = FLASHB,
                       RUN = RAMLS1,
                       RUN_START(_Cla1ConstRunStart),
                       LOAD_START(_Cla1ConstLoadStart),
                       LOAD_SIZE(_Cla1ConstLoadSize),
                       PAGE = 1
#endif //CLA_C
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
