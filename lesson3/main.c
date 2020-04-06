////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <string.h>
#include <peekpoke.h>


#define SCREEN_MEM	0x5000
#define DLIST_MEM	0x4000		// aligned to 1K
#define CHARSET_MEM	 0x3000		// aligned to 1K
#define CHARSET_MEM2 0x3400     // aligned to 1K

#define JOY_NO_MOVE   0
#define JOY_BIT_UP    1
#define JOY_BIT_DOWN  2
#define JOY_BIT_LEFT  4
#define JOY_BIT_RIGHT 8
#define JOY_BIT_TOP_RIGHT   9
#define JOY_BIT_TOP_LEFT    5

#define MOVING_NONE  0
#define MOVING_UP	 1
#define MOVING_DOWN  2
#define MOVING_LEFT  3
#define MOVING_RIGHT 4


unsigned char st;
unsigned char vblank_occured;
unsigned char moving_type;

// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_DLI(DL_BLK8),
	DL_LMS(DL_CHR40x8x4),
	0x00,
	0x50,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_DLI(DL_CHR40x8x4),
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_JVB,
	0x00,
	0x40
};

//00 - background
//01 - color1
//10 - color 2
//11 - color 3

// 00 01 01 00
// 01 01 01 01
// 00 10 10 00
// 00 10 10 00
// 00 10 10 00
// 01 01 01 01
// 01 01 01 01
// 01 01 01 01



unsigned char sprite_dat[] = 
{
	0x14,0x55,0x28,0x28,0x28,0x55,0x55,0x55
};

void wait_for_vblank_clock(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
} 


void wait_for_vblank(void)
{
	while (vblank_occured == 0)
	{
	};

	vblank_occured = 0;
}

unsigned char xpos;

void erase_sprite(void)
{
	POKE(SCREEN_MEM + xpos,0);
}

void update_sprite(void)
{
	if (moving_type == MOVING_UP)
	{
		xpos -= 40;
	}
	else if (moving_type == MOVING_DOWN)
	{
		xpos += 40;
	}
	else if (moving_type == MOVING_RIGHT)
	{
		xpos++;
	}
	else if (moving_type == MOVING_LEFT)
	{
		xpos--;
	}

}

void draw_sprite(void)
{
	POKE(SCREEN_MEM + xpos,33);
}

void handle_input(void)
{
	st = (PIA.porta & 0x0f);
    st = st ^ 0x0f;

	if (st == JOY_BIT_RIGHT)
	{
		moving_type = MOVING_RIGHT;
		return;
	}
	
	if (st == JOY_NO_MOVE)
	{
		moving_type = MOVING_NONE;
		return;
	}

	if (st == JOY_BIT_LEFT)
	{
		moving_type = MOVING_LEFT;
		return;
	}

	if (st == JOY_BIT_UP)
	{
		moving_type = MOVING_UP;
		return;
	}
	
	if (st == JOY_BIT_DOWN)
	{
		moving_type = MOVING_DOWN;
	}

}
void dli_routine(void);

void dli_routine1(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;

	GTIA_WRITE.colbk = 0xFF;

	OS.vdslst = &dli_routine;

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
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x00;
    ANTIC.chbase = 0xE0; //MSB of 0xE000
    GTIA_WRITE.colpf0 = 0x08;

    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    
    ANTIC.chbase = CHARSET_MEM >> 8; // MSB / 256
    
    GTIA_WRITE.colpf0 = 0x55;

    OS.vdslst = &dli_routine1;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void vbi_routine(void)
{
	OS.vdslst = &dli_routine;

	vblank_occured = 1;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void init_vbi2(void)
{
	asm("lda #7");
	asm("ldx #>%v", vbi_routine);
	asm("ldy #<%v", vbi_routine);
	asm("jsr $E45C");
}


void init_vbi_dli(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;
}

void main(void)
{
	moving_type = MOVING_NONE;
	vblank_occured = 0;
	xpos = 0;
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	
	OS.sdlst = (void*)DLIST_MEM;


	OS.color0 = 0x55;		// playerfield 0
	OS.color1 = 0xA3;       // playerfield 1
	OS.color2 = 0X26;		// playerfield 2
	OS.color3 = 0xE1;		// playerfield 3
	OS.color4 = 0x00;		// background

	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);

	// [CHARSETADDRESS] + 8 * charcode
	memcpy((void*) (CHARSET_MEM + 8 * 33) , sprite_dat, 8);

	OS.chbas = CHARSET_MEM >> 8; // MSB / 256

	POKE(SCREEN_MEM + xpos,33);
	
	init_vbi_dli();

	// start the DLI by adding it to the VBI with the NMIEN register
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;

	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();

		handle_input();

		erase_sprite();
		update_sprite();
		draw_sprite();
	};


	// stop DLI by settign NMIEN to point only on VBI
	ANTIC.nmien = NMIEN_VBI; 
}