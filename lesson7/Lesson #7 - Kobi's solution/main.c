/************************************************
** Image to Antic CC65 code		       **
** Generated with image2antic.py               **
** https://github.com/tyrkelko/image2antic     **
************************************************/
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define ROWS 24
#define COLS 448
#define SCREEN_RIGHT_BOUNDARY 408
#define CHARSET0_MEM	0x3000
#define CHARSET1_MEM	0x3400
#define CHARSET2_MEM	0x3800
#define CHARSET3_MEM	0x3C00
#define SCREEN_MEM	0x4C00
#define DLIST_MEM		0x4800 //0x6C00	// aligned to 1K
//extern unsigned char *L_DLIST_MEM;
extern unsigned char *L_SCREEN_MEM;
extern unsigned char *L_CHARSET0_MEM;
extern unsigned char *L_CHARSET1_MEM;


#define JOY_NO_MOVE   0
#define JOY_BIT_UP    1
#define JOY_BIT_DOWN  2
#define JOY_BIT_LEFT  4
#define JOY_BIT_RIGHT 8
#define JOY_BIT_TOP_RIGHT   9
#define JOY_BIT_TOP_LEFT    5

#define MOVING_NONE	0
#define MOVING_UP	1
#define MOVING_DOWN	2
#define MOVING_LEFT	3
#define MOVING_RIGHT	4

unsigned char charset_array[] = {
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8,
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8,
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8,
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8,
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8,
CHARSET0_MEM >> 8, CHARSET1_MEM >> 8, CHARSET2_MEM >> 8, CHARSET3_MEM >> 8
};

// CHARSET DLI CHANGES IN LINES: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23]
unsigned char antic4_display_list[] = {
	DL_DLI(DL_BLK8),
	DL_BLK8,
	DL_BLK8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),(SCREEN_MEM & 0x00FF),SCREEN_MEM >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 448) & 0x00FF),(SCREEN_MEM + 448) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 896) & 0x00FF),(SCREEN_MEM + 896) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 1344) & 0x00FF),(SCREEN_MEM + 1344) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 1792) & 0x00FF),(SCREEN_MEM + 1792) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 2240) & 0x00FF),(SCREEN_MEM + 2240) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 2688) & 0x00FF),(SCREEN_MEM + 2688) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 3136) & 0x00FF),(SCREEN_MEM + 3136) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 3584) & 0x00FF),(SCREEN_MEM + 3584) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 4032) & 0x00FF),(SCREEN_MEM + 4032) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 4480) & 0x00FF),(SCREEN_MEM + 4480) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 4928) & 0x00FF),(SCREEN_MEM + 4928) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 5376) & 0x00FF),(SCREEN_MEM + 5376) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 5824) & 0x00FF),(SCREEN_MEM + 5824) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 6272) & 0x00FF),(SCREEN_MEM + 6272) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 6720) & 0x00FF),(SCREEN_MEM + 6720) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 7168) & 0x00FF),(SCREEN_MEM + 7168) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 7616) & 0x00FF),(SCREEN_MEM + 7616) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 8064) & 0x00FF),(SCREEN_MEM + 8064) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 8512) & 0x00FF),(SCREEN_MEM + 8512) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 8960) & 0x00FF),(SCREEN_MEM + 8960) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 9408) & 0x00FF),(SCREEN_MEM + 9408) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 9856) & 0x00FF),(SCREEN_MEM + 9856) >> 8,
	DL_LMS(DL_DLI(DL_HSCROL(DL_CHR40x8x4))),((SCREEN_MEM + 10304) & 0x00FF),(SCREEN_MEM + 10304) >> 8,
	DL_JVB,0x00,DLIST_MEM>>8
};


unsigned char charset_index;
unsigned char charset_array_size;
unsigned char i;
unsigned int screen_pos;
unsigned char hscrol_bit;
unsigned int row_addr;
unsigned char vblank_counter;
unsigned char st;
unsigned char moving_type;
unsigned char auto_scroll;

void erase_sprite(void);
void update_sprite(void);
void draw_sprite(void);
void handle_input (void);
void wait_for_vblank(void);
void setup_dli(void);
void dli_routine(void);

void main(void)
{
	charset_index = 0;
	screen_pos = 0;
	hscrol_bit = 0;
	auto_scroll = 1;
	// SHUT DOWN ANTIC
	OS.sdmctl = 0;

	// CREATE SCREEN
	memcpy((void*) DLIST_MEM,antic4_display_list,sizeof(antic4_display_list));
	OS.sdlst=(void*)DLIST_MEM;

	// SET COLORS
	OS.color0 = 0;
	OS.color1 = 14;
	OS.color2 = 182;
	OS.color3 = 40;
	OS.color4 = 120;

	charset_array_size = sizeof(charset_array) + 1;
	setup_dli();
	wait_for_vblank();
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI; 

	OS.sdmctl = 0x3E;
	// GAME LOOP
	while (1) {
		wait_for_vblank();
		erase_sprite();
		handle_input();
		update_sprite();
		draw_sprite();
	};
}

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
    --vblank_counter;
} 
void dli_routine(void)
{
    asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");
    ANTIC.chbase = charset_array[charset_index];
    if (++charset_index >= charset_array_size)
    	charset_index = 0;
    ANTIC.wsync = 1;
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
void erase_sprite() {
	// ERASE SPRITE
}
void update_sprite() {
	// TODO: UPDATE SPRITE
    if ((moving_type == MOVING_RIGHT) || (auto_scroll == 1))
	{
		if ((vblank_counter & 0x03) == 3)
			screen_pos++;
		ANTIC.hscrol = vblank_counter & 0x03;
		vblank_counter = vblank_counter & 0x03;
	}
	if (moving_type == MOVING_LEFT)
	{
		if ((vblank_counter & 0x03) == 0)
			if (screen_pos > 0)
				screen_pos--;
	}
		if (screen_pos > SCREEN_RIGHT_BOUNDARY)
		screen_pos = 0;
}
void draw_sprite() {
	// TODO: DRAW SPRITE
    i = ROWS;
	row_addr = SCREEN_MEM + screen_pos + 24 * COLS;
	while (--i < 255)
	{
		row_addr = row_addr - COLS;
		POKE(DLIST_MEM + 4 + 3 * i, row_addr & 0x00FF);
		POKE(DLIST_MEM + 5 + 3 * i, row_addr >> 8);
	}
}
void handle_input (void)
{
 // TODO: HANDLE INPUT
    st = (PIA.porta & 0x0f);
	st = st ^ 0x0f;
	if (st == JOY_BIT_RIGHT)
	{
		moving_type = MOVING_RIGHT;
		auto_scroll = 0;
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
		auto_scroll = 0;
		return;
	}
	if (st == JOY_BIT_UP)
	{
		moving_type = MOVING_UP;
		auto_scroll = 1;
		return;
	}
	if (st == JOY_BIT_DOWN)
	{
	
		moving_type = MOVING_DOWN;
	
	}
}
