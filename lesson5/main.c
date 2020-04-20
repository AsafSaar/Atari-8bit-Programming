////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

extern unsigned char *L_CROWNLANDDLIST;
extern unsigned char *L_CROWNLANDSCREEN;
extern unsigned char L_CROWNLANDCHARSET[];
extern unsigned char L_CROWNLANDCHARSET2;

#define CROWNLAND_CHARSET 0x60
#define CROWNLAND_CHARSET2 0x70

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
} 

void dli_routine(void);
void dli_routine5(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET;

    OS.vdslst = &dli_routine;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}
void dli_routine4(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET2;

    OS.vdslst = &dli_routine5;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}
void dli_routine3(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET;

    OS.vdslst = &dli_routine4;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine2(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET2;

    OS.vdslst = &dli_routine3;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine1(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET;

    OS.vdslst = &dli_routine2;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;

    ANTIC.chbase = CROWNLAND_CHARSET2;

    OS.vdslst = &dli_routine1;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void setup_dli(void)
{
	wait_for_vblank();

    OS.vdslst = &dli_routine;
}

void main(void)
{
    unsigned char * ptr = (unsigned char*)L_CROWNLANDCHARSET[0];
    OS.sdmctl = 0x00;
//	memset((void*)0xBC20,0,2*0xF);
	OS.sdlst = (void*)&L_CROWNLANDDLIST;
//	OS.chbas = 0x60;

// Playfield colors: 00 | 24 98 fc 98
	OS.color0 = 0x24;
	OS.color1 = 0x98;
	OS.color2 = 0xFC;
	OS.color3 = 0x98;
	OS.color4 = 0x00;

    OS.chbas = CROWNLAND_CHARSET;

 	setup_dli();

	wait_for_vblank();
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI; 

    OS.sdmctl = 0x3E;
	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();  
	};
}