////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>

unsigned char vblank_occurred = 0;

void wait_for_vblank(void)
{
    while (vblank_occurred == 0)
    {

    };

    vblank_occurred = 0;

   /* asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);*/

} 


#pragma optimize (push, off)
void NMI_handler()
{
    asm("bit %w", (unsigned)&ANTIC.nmires);
    asm("bpl %g", not_dli);
    asm("jmp (%w)", (unsigned)&OS.vdslst);
not_dli:
    asm("cld");
    asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");
    asm("sta %w", (unsigned)&ANTIC.nmires);
    asm("jmp (%w)", (unsigned)&OS.vvblki);
}
#pragma optimize (pop)

////////////////////////////////////////////////////////////////
// 3 main OS vectors to handle on your own when disabling the OS
// NMI
// rest 
// irq
// we will only handle NMI
////////////////////////////////////////////////////////////////
struct __vectors
{
    void (*nmi)(void);      // = $FFFA/$FFFB    6502 NMI VECTOR
    void (*reset)(void);    // = $FFFC/$FFFD    6502 RESET VECTOR
    void (*irqbrk)(void);   // = $FFFE/$FFFF    6502 IRQ/BRK VECTOR
};

// address of the 3 vectors
#define VECTOR (*(struct __vectors*)0xFFFA)

///////////////////////////////////////////////////////////////
// since OS is disabled we need to copy shadow to HW registers
// by ourselves
///////////////////////////////////////////////////////////////
void copy_shadow_to_hw_registers(void)
{
    ANTIC.dmactl = OS.sdmctl;

    ANTIC.dlistl = OS.sdlstl;
    ANTIC.dlisth = OS.sdlsth;

    ANTIC.chbase = OS.chbas;

    GTIA_WRITE.prior = OS.gprior;

    GTIA_WRITE.colpf0 = OS.color0;
    GTIA_WRITE.colpf1 = OS.color1;
    GTIA_WRITE.colpf2 = OS.color2;
    GTIA_WRITE.colpf3 = OS.color3;
    GTIA_WRITE.colbk  = OS.color4;
    
    GTIA_WRITE.colpm0 = OS.pcolr0;
    GTIA_WRITE.colpm1 = OS.pcolr1;
    GTIA_WRITE.colpm2 = OS.pcolr2;
    GTIA_WRITE.colpm3 = OS.pcolr3;      
}

///////////////////////////////////////////////////////////
// vbi routine for switching between screens
///////////////////////////////////////////////////////////
void vbi_routine(void)
{   
    copy_shadow_to_hw_registers();

//    OS.vdslst = &dli_routine;
    vblank_occurred = 1;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

///////////////////////////////////////////////////////////
// main routine to disable OS:
// - disable nmien as first step so no crash will occur
// - setting PORTB to 0xFE 
// - setting our own custom nmi handler
// - setting vbi
///////////////////////////////////////////////////////////
void disable_os(void)
{    
    asm("sei");
    asm("cld");  
    POKEY_WRITE.irqen = 0;
    ANTIC.nmien = 0;
    PIA.portb &= 0xFE;

    OS.vvblki = &vbi_routine;
    VECTOR.nmi = NMI_handler;
    ANTIC.nmien = NMIEN_VBI;
    asm("cli");
}

void main(void)
{	
    vblank_occurred = 0;
//	disable_os();

	//Endless loop to keep te app running
	while (1) 
	{
		wait_for_vblank();

	}
}