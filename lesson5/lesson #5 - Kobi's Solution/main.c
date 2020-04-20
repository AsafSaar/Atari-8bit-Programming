////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

extern unsigned char *L_PHARAOSDLIST;
extern unsigned char *L_PHARAOSSCREEN;
extern unsigned char L_PHARAOSCHARSET[];
extern unsigned char L_PHARAOSCHARSET2[];
extern unsigned char L_PHARAOSCHARSET3[];

#define PHARAOS_CHARSET 0x1C
#define PHARAOS_CHARSET2 0x40
#define PHARAOS_CHARSET3 0x18


void dli_routine(void);
void dli_routine1(void);
void dli_routine2(void);
void wait_for_vblank(void);
void setup_dli(void);

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
} 

void dli_routine2(void)
{
	asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;
    ANTIC.chbase = PHARAOS_CHARSET3;

    OS.vdslst = &dli_routine;
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

    ANTIC.chbase = PHARAOS_CHARSET2;

    //Playfield colors: 00 | 2a ba 76 46
    GTIA_WRITE.colpf0 = 0x2a;
    GTIA_WRITE.colpf1 = 0xba;
    GTIA_WRITE.colpf2 = 0x76;
    GTIA_WRITE.colpf3 = 0x46;
    GTIA_WRITE.colbk = 0x00;

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

    ANTIC.chbase = PHARAOS_CHARSET;
    //Playfield colors: 00 | 2a ba 0a 68
    GTIA_WRITE.colpf0 = 0x2a;
    GTIA_WRITE.colpf1 = 0xba;
    GTIA_WRITE.colpf2 = 0x0a;
    GTIA_WRITE.colpf3 = 0x68;
    GTIA_WRITE.colbk = 0x00;


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
    unsigned char * ptr = (unsigned char*)L_PHARAOSCHARSET[0];
    OS.sdmctl = 0x00;
	OS.sdlst = (void*)&L_PHARAOSDLIST;

    //Playfield colors: 00 | 2a ba 76 46
	OS.color0 = 0x2a;
	OS.color1 = 0xba;
	OS.color2 = 0x76;
	OS.color3 = 0x46;
	OS.color4 = 0x00;

    OS.chbas = PHARAOS_CHARSET;

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