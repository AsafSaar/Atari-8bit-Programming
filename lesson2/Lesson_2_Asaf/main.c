/************************************************
** Lesson #2 exercise                          **
** Asaf Saar                                   ** 
** March 31, 2020                              **
************************************************/
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

#define MOVING_NONE  	0
#define MOVING_UP	 	1
#define MOVING_DOWN  	2
#define MOVING_LEFT  	3
#define MOVING_RIGHT 	4

unsigned char joystick0;
unsigned char joystick0_fire;
unsigned char moving_type;
unsigned int enemy_pos;
unsigned int missile_pos;
unsigned int spaceship_pos;
unsigned char score;

unsigned char antic4_display_list[] = // 32 bytes
{
	DL_BLK8, DL_BLK8, DL_BLK8,
	DL_LMS(DL_CHR40x8x1), 0x00, 0x50,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, DL_CHR40x8x4,
	DL_CHR40x8x4, DL_CHR40x8x4, 	
	DL_JVB, 0x00, 0x40
};

unsigned char my_sprite[] = 
{
	0x14, 0x14, 0x14, 0x14, 0x82, 0x82, 0x82, 0x82,
};

void wait_for_vblank(void)
{
	// wait for 1 frame to pass
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void print_you_win(void) 
{
	// Game over

	POKE(SCREEN_MEM+0, 37);  // E
	POKE(SCREEN_MEM+1, 56);  // X
	POKE(SCREEN_MEM+2, 35);  // C
	POKE(SCREEN_MEM+3, 37);  // E
	POKE(SCREEN_MEM+4, 44);  // L
	POKE(SCREEN_MEM+5, 44);  // L
	POKE(SCREEN_MEM+6, 37);  // E
	POKE(SCREEN_MEM+7, 46);  // N
	POKE(SCREEN_MEM+8, 52);  // T
	POKE(SCREEN_MEM+9, 1);   // !
	POKE(SCREEN_MEM+10, 0);  // 
	POKE(SCREEN_MEM+11, 57); // Y
	POKE(SCREEN_MEM+12, 47); // O
	POKE(SCREEN_MEM+13, 53); // U
	POKE(SCREEN_MEM+14, 0);  // 
	POKE(SCREEN_MEM+15, 55); // W
	POKE(SCREEN_MEM+16, 41); // I
	POKE(SCREEN_MEM+17, 46); // N
	POKE(SCREEN_MEM+18, 0);  // 
	POKE(SCREEN_MEM+19, 0);  // 
	POKE(SCREEN_MEM+20, 0);  // 
	POKE(SCREEN_MEM+21, 0);  // 
	POKE(SCREEN_MEM+22, 0);  // 
	POKE(SCREEN_MEM+23, 0);  // 
	POKE(SCREEN_MEM+24, 0);  // 
	POKE(SCREEN_MEM+25, 0);  // 
	POKE(SCREEN_MEM+26, 0);  //
	POKE(SCREEN_MEM+27, 0);  //
	POKE(SCREEN_MEM+28, 0);  //
	POKE(SCREEN_MEM+29, 0);  //
	POKE(SCREEN_MEM+30, 0);  //
}

void print_you_lose(void) 
{
	// Game over

	POKE(SCREEN_MEM+0, 34);  // B
	POKE(SCREEN_MEM+1, 47);  // O
	POKE(SCREEN_MEM+2, 47);  // O
	POKE(SCREEN_MEM+3, 0);   // 
	POKE(SCREEN_MEM+4, 57);  // Y
	POKE(SCREEN_MEM+5, 47);  // O
	POKE(SCREEN_MEM+6, 53);  // U
	POKE(SCREEN_MEM+7, 0);   // 
	POKE(SCREEN_MEM+8, 40);  // H
	POKE(SCREEN_MEM+9, 33);  // A
	POKE(SCREEN_MEM+10, 54); // V
	POKE(SCREEN_MEM+11, 37); // E
	POKE(SCREEN_MEM+12, 0);  // 
	POKE(SCREEN_MEM+13, 44); // L
	POKE(SCREEN_MEM+14, 47); // O
	POKE(SCREEN_MEM+15, 51); // S
	POKE(SCREEN_MEM+16, 52); // T
	POKE(SCREEN_MEM+17, 0);  // 
	POKE(SCREEN_MEM+18, 0);  // 
	POKE(SCREEN_MEM+19, 0);  // 
	POKE(SCREEN_MEM+20, 0);  // 
	POKE(SCREEN_MEM+21, 0);  // 
	POKE(SCREEN_MEM+22, 0);  // 
	POKE(SCREEN_MEM+23, 0);  // 
	POKE(SCREEN_MEM+24, 0);  // 
	POKE(SCREEN_MEM+25, 0);  // 
	POKE(SCREEN_MEM+26, 0);  //
	POKE(SCREEN_MEM+27, 0);  //
	POKE(SCREEN_MEM+28, 0);  //
	POKE(SCREEN_MEM+29, 0);  //
	POKE(SCREEN_MEM+30, 0);  //
}

void draw_sprite(void)
{
	// Draw Top Border
	POKE(SCREEN_MEM+0, 33);  // A
	POKE(SCREEN_MEM+1, 51);  // S
	POKE(SCREEN_MEM+2, 33);  // A
	POKE(SCREEN_MEM+3, 38);  // F
	POKE(SCREEN_MEM+4, 7);   // '
	POKE(SCREEN_MEM+5, 51);  // S

	POKE(SCREEN_MEM+7, 36);  // D
	POKE(SCREEN_MEM+8, 53);  // U
	POKE(SCREEN_MEM+9, 45);  // M
	POKE(SCREEN_MEM+10, 45); // M
	POKE(SCREEN_MEM+11, 57); // Y

	POKE(SCREEN_MEM+13, 51); // S
	POKE(SCREEN_MEM+14, 40); // H
	POKE(SCREEN_MEM+15, 47); // O
	POKE(SCREEN_MEM+16, 47); // O
	POKE(SCREEN_MEM+17, 52); // T
	POKE(SCREEN_MEM+18, 37); // E
	POKE(SCREEN_MEM+19, 50); // R

	POKE(SCREEN_MEM+22, 19); // 3
	POKE(SCREEN_MEM+23, 0);  // 
	POKE(SCREEN_MEM+24, 52); // T
	POKE(SCREEN_MEM+25, 47); // O
	POKE(SCREEN_MEM+26, 0);  // 
	POKE(SCREEN_MEM+27, 55); // W
	POKE(SCREEN_MEM+28, 41); // I
	POKE(SCREEN_MEM+29, 46); // N

	POKE(SCREEN_MEM+33, 51); // S
	POKE(SCREEN_MEM+34, 35); // C
	POKE(SCREEN_MEM+35, 47); // O
	POKE(SCREEN_MEM+36, 50); // R
	POKE(SCREEN_MEM+37, 37); // E
	POKE(SCREEN_MEM+38, 26); // :
	POKE(SCREEN_MEM+39, score); // Score placeholder


	//Draw spaceship
	POKE(SCREEN_MEM + spaceship_pos, 64);

	//Draw enemy ship
	POKE(SCREEN_MEM + enemy_pos, 0);
	enemy_pos++;
	if (enemy_pos == 120) 
	{
		enemy_pos = 121;
	}

	if (enemy_pos == spaceship_pos)
	{
		print_you_lose();
		while (1){}
	}

	POKE(SCREEN_MEM + enemy_pos, 10);
	wait_for_vblank();

	//Draw missile
	POKE(SCREEN_MEM + missile_pos, 7);

}

void erase_sprite(void)
{
	POKE(SCREEN_MEM + spaceship_pos, 0);
	POKE(SCREEN_MEM + missile_pos, 0);
	missile_pos-=40;
}

void update_sprite(void)
{
	if (moving_type == MOVING_UP)
	{
		//spaceship_pos -= 40;
	}
	else if (moving_type == MOVING_DOWN)
	{
		//spaceship_pos += 40;
	}
	else if (moving_type == MOVING_RIGHT)
	{
		if (spaceship_pos <959)
		{
			spaceship_pos++;
		}
	}
	else if (moving_type == MOVING_LEFT)
	{
		if (spaceship_pos > 920)
		{
			spaceship_pos--;
		}
	}

}

void handle_input(void)
{
	joystick0 = OS.stick0;
	joystick0_fire = OS.strig0;

	if (joystick0 == STICK_RIGHT)
	{
		moving_type = MOVING_RIGHT;
	}
	else if (joystick0 == STICK_NOMOVE)
	{
		moving_type = MOVING_NONE;
	}
	else if (joystick0 ==  STICK_LEFT)
	{
		moving_type = MOVING_LEFT;
	}
	else if (joystick0 == STICK_UP)
	{
		moving_type = MOVING_UP;
	}
	else if (joystick0 == STICK_DOWN)
	{
		moving_type = MOVING_DOWN;
	}

	// Handle fire
	if (joystick0_fire == 0)
	{
		missile_pos = spaceship_pos - 40;
	}

	// Check for hit, nearby hit counts too ;)
	if ((missile_pos == enemy_pos) || (missile_pos -1 == enemy_pos) || (missile_pos +1 == enemy_pos))
	{	
		// We have a hit!
		score++;
		POKE(SCREEN_MEM+39, score); 

		if (score == 19)
		{
			print_you_win();
			while (1){}
		}
	}
}


void main(void)
{
	moving_type = MOVING_NONE;
	score = 16;

	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));

	// Sprites color
	OS.sdlst = (void*)DLIST_MEM;
	OS.color1 = 0xFF;       // playerfield 1
	OS.color2 = 0x00;		// playerfield 2
	OS.color3 = 0x00;		// playerfield 3

	// Create and Copy sprite data to memory, replacing char table
	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);
	memcpy((void*) (CHARSET_MEM + 8 * 64), my_sprite, 8);
	OS.chbas = CHARSET_MEM >> 8;

	// Position of the sprite
	enemy_pos = 280;
	spaceship_pos = 940;


	while(1)
	{
		erase_sprite();
		handle_input();
		update_sprite();
		draw_sprite();
		wait_for_vblank();
	};
}