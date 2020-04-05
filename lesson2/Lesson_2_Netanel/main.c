#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define SCREEN_MEM		0x5000
#define DLIST_MEM		0x4000		// aligned to 1K
#define CHARSET_MEM		0x3000		// aligned to 1K

#define STICK_RIGHT		7
#define STICK_LEFT		11
#define STICK_UP   		14
#define STICK_DOWN 		13
#define STICK_NOMOVE 	15
#define TRIG_FIRE		0

#define MOVING_NONE  	0
#define MOVING_UP	 	1
#define MOVING_DOWN  	2
#define MOVING_LEFT  	3
#define MOVING_RIGHT 	4

#define UFO_SPEED 		2

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


// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_BLK8,
	DL_LMS(DL_CHR40x8x1),
	0x00,
	0x50,
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
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
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

void wait_for_vblank(void)
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
		if (player_xpos == SCREEN_BOTTOM_RIGHT - 1)
		{
			moving_type = MOVING_NONE;
		} 
		else
		{
			moving_type = MOVING_RIGHT;
		}
	}
	else if (stick == STICK_NOMOVE)
	{
		moving_type = MOVING_NONE;
	}
	else if (stick ==  STICK_LEFT)
	{
		if (player_xpos == SCREEN_BOTTOM_RIGHT - SCREEN_WIDTH)
		{
			moving_type = MOVING_NONE;
		} 
		else
		{
			moving_type = MOVING_LEFT;
		}
	}

	if (trigger == TRIG_FIRE && missile_live == 0)
	{
		missile_live = 1;
		missile_xpos = player_xpos;
	}
}

void detect_collision(void)
{
	if (missile_xpos == ufo_xpos)
	{
		game_over = 1;
	}
}

void is_game_over(void)
{
	if (game_over == 1)
	{
		POKE(SCREEN_MEM+15, 55); // W
		POKE(SCREEN_MEM+16, 41); // I
		POKE(SCREEN_MEM+17, 46); // N
		POKE(SCREEN_MEM+18, 46); // N
		POKE(SCREEN_MEM+19, 37); // E
		POKE(SCREEN_MEM+20, 50); // R
		POKE(SCREEN_MEM+21, 0); // " "
		POKE(SCREEN_MEM+22, 41); // I
		POKE(SCREEN_MEM+23, 51); // S
		POKE(SCREEN_MEM+24, 0); // " "
		POKE(SCREEN_MEM+25, 57); // Y
		POKE(SCREEN_MEM+26, 47); // O
		POKE(SCREEN_MEM+27, 53); // U
		POKE(SCREEN_MEM + ufo_xpos,56); // X
		while(1);
	}
}

void main(void)
{
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	OS.sdlst = (void*)DLIST_MEM;

	OS.color0 = 0x0E;       // playerfield 0
	OS.color1 = 0X32;		// playerfield 1
	OS.color2 = 0XFF;		// playerfield 2
	OS.color3 = 0XFF;		// playerfield 3
	OS.color4 = 0x00;		// background

	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);

	memcpy((void*) (CHARSET_MEM + 8 * PLAYER_CHAR) , player_sprite, 8);
	memcpy((void*) (CHARSET_MEM + 8 * MISSILE_CHAR) , missile_sprite, 8);
	memcpy((void*) (CHARSET_MEM + 8 * UFO_CHAR) , ufo_sprite, 8);

	OS.chbas = CHARSET_MEM >> 8; // MSB / 256

	POKE(SCREEN_MEM + player_xpos,PLAYER_CHAR); // Initialize player sprite

	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();
		handle_input();
		erase_sprite();
		update_sprite();
		draw_sprite();
		detect_collision();
		is_game_over();
	};
}