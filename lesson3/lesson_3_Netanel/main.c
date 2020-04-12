#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define SCREEN_MEM		0x5000
#define DLIST_MEM		0x4000		// aligned to 1K
#define CHARSET_MEM0		0x3000		// aligned to 1K
#define CHARSET_MEM1		0x3400		// aligned to 1K

#define STICK_RIGHT		7
#define STICK_LEFT		11
#define TRIG_FIRE		0

#define MOVING_NONE  	0
#define MOVING_UP	 	1
#define MOVING_DOWN  	2
#define MOVING_LEFT  	3
#define MOVING_RIGHT 	4

#define UFO_SPEED 		10

#define PLAYER_CHAR		60
#define MISSILE_CHAR	61
#define UFO_CHAR		62
#define UFO_ROW			3

#define SCREEN_WIDTH 	40
#define SCREEN_HIGHT	24
#define SCREEN_BOTTOM_RIGHT	SCREEN_WIDTH * SCREEN_HIGHT

unsigned char stick;
unsigned char trigger;
unsigned char moving_type = MOVING_NONE;
unsigned int player_xpos = SCREEN_BOTTOM_RIGHT - (SCREEN_WIDTH/2); // middle of bottom row
unsigned int missile_xpos;
unsigned char ufo_xpos = SCREEN_WIDTH * UFO_ROW;
unsigned char ufo_direction = MOVING_RIGHT;
unsigned char missile_live;
unsigned char update_ufo = 0;
unsigned char game_over = 0;
unsigned char vblank_occured = 0;
unsigned char * banner = "A WINNER IS YOU";


// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_BLK8,
	DL_DLI(DL_LMS(DL_CHR40x8x4)),
	0x00,
	0x50,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_DLI(DL_CHR40x8x4),
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
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
	DL_DLI(DL_CHR40x8x4),
	DL_JVB,
	0x00,
	0x40
};

unsigned char player_sprite[] = 
{
	0x28,0x14,0x96,0x41,0x41,0x41,0x82,0x00
};

unsigned char missile_sprite[] =
{
	0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x00
};

unsigned char ufo_sprite[] =
{
	0x14,0x55,0xAA,0x55,0x14,0x00,0x00,0x00
};

void dli_routine0(void);
void dli_routine1(void);
void dli_routine2(void);
void dli_routine3(void);

unsigned char atascii_to_internal(char * str) // converts atascii string to internal and returns string length 
{
	unsigned char i = 0;
	do {
		str[i] -= 32;
		i++;
	}while (str[i] != 0);
	return i;
}

void wait_for_vblank(void)
{
	while (vblank_occured == 0)
	{
	};

	vblank_occured = 0;
}

void wait_for_vblank_clock(void)
{
    asm("lda $14");
	wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void erase_sprite(void)
{
	POKE(SCREEN_MEM + player_xpos,0);
	POKE(SCREEN_MEM + missile_xpos,0);
	POKE(SCREEN_MEM + ufo_xpos,0);
}

void update_ufo_sprite(void)
{
	if (update_ufo == UFO_SPEED)
	{
		if (ufo_direction == MOVING_RIGHT)
		{
			ufo_xpos++;
			if (ufo_xpos >= SCREEN_WIDTH * (UFO_ROW + 1) -1)
			{
				ufo_direction = MOVING_LEFT;
			}
		}
		else if (ufo_direction == MOVING_LEFT)
		{
			ufo_xpos--;
			if (ufo_xpos <= SCREEN_WIDTH * UFO_ROW)
			{
				ufo_direction = MOVING_RIGHT;
			}
		}
		update_ufo = 0;
	}
	else
	{
		update_ufo++;
	}
}

void update_missile_sprite(void)
{
	if (missile_live == 1)
	{
		missile_xpos -= SCREEN_WIDTH;
		if (missile_xpos < SCREEN_WIDTH)
		{
			missile_live = 0;
		}
	}
}

void update_player_sprite(void)
{
	if (moving_type == MOVING_RIGHT)
	{
		player_xpos++;
	}
	else if (moving_type == MOVING_LEFT)
	{
		player_xpos--;
	}
}

void update_sprite(void)
{

	update_player_sprite();
	update_missile_sprite();
	update_ufo_sprite();
}


void draw_sprite(void)
{
	POKE(SCREEN_MEM + player_xpos,PLAYER_CHAR);
	POKE(SCREEN_MEM + ufo_xpos,UFO_CHAR);
	if (missile_live == 1)
	{
		POKE(SCREEN_MEM + missile_xpos,MISSILE_CHAR);
	}
}

void handle_input(void)
{
	stick = OS.stick0;
	trigger = OS.strig0;

	if (stick == STICK_RIGHT)
	{
		if (player_xpos == SCREEN_BOTTOM_RIGHT - 1) // stop at right edge of screen
		{
			moving_type = MOVING_NONE;
		} 
		else
		{
			moving_type = MOVING_RIGHT;
		}
	}
	else if (stick ==  STICK_LEFT)
	{
		if (player_xpos == SCREEN_BOTTOM_RIGHT - SCREEN_WIDTH) // stop at left edge of screen
		{
			moving_type = MOVING_NONE;
		} 
		else
		{
			moving_type = MOVING_LEFT;
		}
	}
	else
	{
		moving_type = MOVING_NONE;
	}
	
	if (trigger == TRIG_FIRE && missile_live == 0)
	{
		missile_live = 1;
		missile_xpos = player_xpos;
	}
}

void detect_collision(void)
{
	if (missile_xpos > ufo_xpos-2 && missile_xpos < ufo_xpos+2) // easy mode ;)
	{
		game_over = 1;
	}
}

void handle_game_over(void)
{
	if (game_over == 1)
	{
		POKE(DLIST_MEM+16, DL_CHR40x8x1); // change middle row graphic mode
		memcpy((void*)(SCREEN_MEM+SCREEN_WIDTH*11+SCREEN_WIDTH/3), (void*)banner, atascii_to_internal(banner)); // print banner
		POKE(SCREEN_MEM + ufo_xpos,56); // X
		while(1);
	}
}


void vbi_routine(void)
{
	vblank_occured = 1;
	asm("jmp $E45F"); // continue normal vblank flow (SYSVBV)
}

void dli_routine0(void) // handle sky and ufo charset
{
	OS.vdslst = &dli_routine1;
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;

	ANTIC.chbase = CHARSET_MEM0 >> 8;
	GTIA_WRITE.colbk = 0x8E;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x8C;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x8A;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x88;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x86;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x84;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x82;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x80;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}

void dli_routine1(void) // handle player charset
{
	OS.vdslst = &dli_routine2;
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");
	ANTIC.wsync = 1;

	ANTIC.chbase = CHARSET_MEM1 >> 8;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}

void dli_routine2(void) // handle game over banner colors
{
	OS.vdslst = &dli_routine3;
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");
	

	if (game_over == 1){
		GTIA_WRITE.colpf1 = 0xFF;
		GTIA_WRITE.colpf2 = 0x00;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		ANTIC.wsync = 1;
		GTIA_WRITE.colpf1 = 0x32;
		GTIA_WRITE.colpf2 = 0xFF;
	}

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}

void dli_routine3(void) // paint ground
{
	OS.vdslst = &dli_routine0;
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	GTIA_WRITE.colbk = 0xF0;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0xF2;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0xF4;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0xF5;
	
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}
void init_nmis(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;
	OS.vdslst = &dli_routine0;
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;

}

void main(void)
{
	// set custom display liat
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	OS.sdlst = (void*)DLIST_MEM;
	
	// init colors
	OS.color0 = 0x0E;       // playerfield 0
	OS.color1 = 0X32;		// playerfield 1
	OS.color2 = 0XFF;		// playerfield 2
	OS.color3 = 0X00;		// playerfield 3
	OS.color4 = 0x00;		// background

	// copy oroginal charcter set to new location
	memcpy((void*)CHARSET_MEM0, (void*)0xE000, 0x400);
	memcpy((void*)CHARSET_MEM1, (void*)0xE000, 0x400);
	// override original characters with custom ones
	memcpy((void*) (CHARSET_MEM0 + 8 * UFO_CHAR) , ufo_sprite, 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * MISSILE_CHAR) , missile_sprite, 8);
	memcpy((void*) (CHARSET_MEM1 + 8 * PLAYER_CHAR) , player_sprite, 8);
	memcpy((void*) (CHARSET_MEM1 + 8 * MISSILE_CHAR) , missile_sprite, 8);

	init_nmis();
	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();
		handle_input();
		erase_sprite();
		update_sprite();
		draw_sprite();
		detect_collision();
		handle_game_over();
	};
}