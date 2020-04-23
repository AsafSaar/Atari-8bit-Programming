#include <atari.h>
#include <string.h>
#include <peekpoke.h>

extern unsigned char *L_MYDLIST;
extern unsigned char *L_MEM1;

#define CHAR1 0x40
#define CHAR2 0x44
#define CHAR3 0x48

unsigned char vblank_occured = 0;

void dli_routine0(void);
void dli_routine1(void);
void dli_routine2(void);

void wait_for_vblank(void)
{
    asm("lda $14");
	wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void dli_routine0(void) // draw ground
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;
	ANTIC.chbase = CHAR1;
	GTIA_WRITE.colpf0 = 0x2A;
	GTIA_WRITE.colpf1 = 0xBA;
	GTIA_WRITE.colpf2 = 0x0A;
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

void dli_routine1(void) // draw ground
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;
	ANTIC.chbase = CHAR2;
	GTIA_WRITE.colpf0 = 0x2A;
	GTIA_WRITE.colpf1 = 0xBA;
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

void dli_routine2(void) // draw ground
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;
	ANTIC.chbase = CHAR3;
    OS.vdslst = &dli_routine0;
	
	
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}

void init_nmis(void)
{
	wait_for_vblank();
	OS.vdslst = &dli_routine0;
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;

}

void main(void)
{
	// shut down ANTIC DMA
	OS.sdmctl = 0;

	OS.sdlst = &L_MYDLIST;
	
	init_nmis();

    OS.sdmctl = 0x3E; // single line

	while(1)
	{
		wait_for_vblank();
	};
}