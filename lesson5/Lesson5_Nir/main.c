// Lession5 //
// 

#include <atari.h>
#include <string.h>
#include <peekpoke.h>


#define dlist_mem 0x3000
#define char1_mem 0x4000
#define char2_mem 0x5000
#define scren_mem 0x6000

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

unsigned char dlist[] =
{
0x70,0x70,0x70,
0x65,
0x00,0x70,

0x70,
0x65,0x00,0x60,

0x25,0x25,0x25,0x25,0x25,0x25,0x25,
0xA5,0x25,0x25,0x25, 
DL_JVB,0x0,0x30,
};

void dli_routine(void)
{
	asm("pha"); asm("tya"); asm("pha"); asm("tya"); asm("pha");

    ANTIC.wsync = 1;
    ANTIC.chbase = char2_mem >> 8;
    POKE(708,0x2A);

    asm("pla"); asm("tay"); asm("pla"); asm("tax"); asm("pla");
    asm("rti");
}

void setup_dli(void)
   {wait_for_vblank(); OS.vdslst = &dli_routine;}

void main(void)
{

	memcpy((void*)dlist_mem,dlist, sizeof(dlist));
    OS.sdlst = (void*)dlist_mem;
    OS.chbas = char1_mem >> 8;

	setup_dli(); wait_for_vblank();
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI; 

// Set Screen colors

    POKE(708,0x0A); POKE(709,0XBA); POKE(710,0X76); POKE(711,0X46); POKE(712,00);

// set dummy counter
    POKE (0x7023,0X010);POKE (0x7024,0X010);POKE (0x7025,0X010);POKE (0x7026,0X010);
 
 while(1){};
}