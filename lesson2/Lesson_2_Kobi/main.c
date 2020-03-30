/************************************************
** Lesson #2 exercise                          **
** Kobi Tyrkel                                 ** 
** March 30, 2020                              **
************************************************/
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define SCREEN_MEM		0x5000
#define DLIST_MEM		0x4000	// aligned to 1K
#define CHARSET_MEM 	0x3000	// aligned to 1K

#define STICK_RIGHT		7
#define STICK_LEFT 		11

#define MOVING_NONE 	0
#define MOVING_RIGHT 	3
#define MOVING_LEFT 	4

unsigned char antic4_display_list[] = {
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
};

unsigned char sprite_data[] = {

	// ENEMY SPACESHIP
	//01234567
	//        
	//   33   
	//  2222  
	// 111111 
	// 111111 
	//  2222  
	//   11     
	//        

	0x00, 0x03, 0xAA, 0x15, 0x15, 0xAA, 0x03, 0x00,
	0x00, 0xC0, 0xA0, 0xC4, 0xC4, 0xA0, 0xC0, 0x00,

	// PLAYER SHIP
	//01234567
	//   11   
	//1  11  1
	//1  11  1
	//1  11  1
	//22222222
	//1  11  1

	0x01, 0x41, 0x41, 0x41, 0xAA, 0x41, 0x00, 0x00,
	0x40, 0x41, 0x41, 0x41, 0xAA, 0x41, 0x00, 0x00,
	
	// MISSILE
	//01234567
	//   11   
	//   22   
	//   33   
	//   22   
	//   11   
	//   22   
	//   33   
	//   22   

	0x01, 0x02, 0x03, 0x02, 0x01, 0x02, 0x03, 0x02,
	0x40, 0x80, 0xC0, 0x80, 0x40, 0x80, 0xC0, 0x80
};

unsigned char x_e_pos = 0;
unsigned char x_p_pos = 20;
unsigned char x_m_pos = 0;
unsigned char y_m_pos = 0;

unsigned char score = 0;
unsigned int time_left = 3600;

unsigned char stick0;
unsigned char strig0;

unsigned char is_moving_right = 0;
unsigned char is_moving_left = 0;
unsigned char moving_type = MOVING_NONE;
unsigned char enemy_move_delay = 0;
unsigned char is_hit = 0;
unsigned char is_game_over = 0;
unsigned char enemy_speed = 2;

void erase_sprite() {
	// ERASE ENEMY
	POKE(SCREEN_MEM + x_e_pos + 80, 0x00);
	POKE(SCREEN_MEM + x_e_pos + 81, 0x00);

	// ERASE PLAYER
	POKE(SCREEN_MEM + x_p_pos + 800, 0x00);
	POKE(SCREEN_MEM + x_p_pos + 801, 0x00);

	// ERASE MISSILE
	POKE(SCREEN_MEM+x_m_pos + 760 - 40 * y_m_pos, 0x00);
	POKE(SCREEN_MEM+x_m_pos + 761 - 40 * y_m_pos, 0x00);
}

void toggle_game_over() {
	if (is_game_over == 0) {
		is_game_over = 1;
		POKE(DLIST_MEM + 16, DL_CHR40x8x1);
	} else {
		is_game_over = 0;
		POKE(DLIST_MEM + 16, DL_CHR40x8x4);
		time_left = 3600;
		score = 0;
		POKE(SCREEN_MEM+440+14, 0); // 
		POKE(SCREEN_MEM+440+15, 0); // 
		POKE(SCREEN_MEM+440+16, 0); // 
		POKE(SCREEN_MEM+440+17, 0); // 
		POKE(SCREEN_MEM+440+18, 0); // 
		POKE(SCREEN_MEM+440+20, 0); // 
		POKE(SCREEN_MEM+440+21, 0); // 
		POKE(SCREEN_MEM+440+22, 0); // 
		POKE(SCREEN_MEM+440+23, 0); // 
		POKE(SCREEN_MEM+440+24, 0); // 
		enemy_speed = 2;
	}
}

void update_sprite() {
	// MISSILE PLOCATION UPDATE
	if (y_m_pos > 21) y_m_pos = 0;
	if (y_m_pos == 0) x_m_pos = x_p_pos;
	if (y_m_pos > 0) y_m_pos++;

	// PLAYER LOCATION UPDATE
	if (moving_type == MOVING_RIGHT) {
		if (x_p_pos < 37) x_p_pos++;
	} else if (moving_type == MOVING_LEFT) {
		if (x_p_pos > 0) x_p_pos--;
	}

	// TIMER UPDATE	
	if (time_left > 0) {
		time_left--;

		// ENEMY LOCATION UPDATE
		if (is_hit == 0) {
			enemy_move_delay++;
		}
		if (enemy_move_delay > enemy_speed) {
			x_e_pos++;
			enemy_move_delay = 0;
		}
		if (x_e_pos==37) x_e_pos = 0;
		

		// HANDLE HITS
		if (y_m_pos == 18) {
			if ((x_m_pos + 1 >= x_e_pos) && (x_m_pos - 1 <= x_e_pos)) {
				score++;
				is_hit=20;
				if (score == 10) {
					enemy_speed = 1;
				}
				if (score == 20) {
					enemy_speed = 0;
				}
			}
		}

		if (is_hit > 0) {
			is_hit--;
			if (is_hit == 0) {
				x_e_pos = 0;
				y_m_pos = 0;
			}
		}
	}

	// HANDLE GAME OVER
	if ((time_left == 0) && (is_game_over == 0)) {
		toggle_game_over();
	}
}

void draw_sprite() {
	// DRAW SCORE
	POKE(SCREEN_MEM+2, 51); // S
	POKE(SCREEN_MEM+3, 35); // C
	POKE(SCREEN_MEM+4, 47); // O
	POKE(SCREEN_MEM+5, 50); // R
	POKE(SCREEN_MEM+6, 37); // E
	POKE(SCREEN_MEM+7, 26); // :
	POKE(SCREEN_MEM+9, 16 + score / 10); // 0
	POKE(SCREEN_MEM+10, 16 + score % 10); // 0
	POKE(SCREEN_MEM+11, 16); // 0

	// DRAW TIME LEFT
	POKE(SCREEN_MEM+25, 52); // T
	POKE(SCREEN_MEM+26, 41); // I
	POKE(SCREEN_MEM+27, 45); // M
	POKE(SCREEN_MEM+28, 37); // E
	POKE(SCREEN_MEM+30, 44); // L
	POKE(SCREEN_MEM+31, 37); // E
	POKE(SCREEN_MEM+32, 38); // F
	POKE(SCREEN_MEM+33, 52); // T
	POKE(SCREEN_MEM+34, 26); // :
	POKE(SCREEN_MEM+36, 16 + time_left / 600); // s
	POKE(SCREEN_MEM+37, 16 + (time_left / 60) % 10); // s

	// DRAW ENEMY
	POKE(SCREEN_MEM+x_e_pos+80, 64);
	POKE(SCREEN_MEM+x_e_pos+81, 65);

	// DRAW PLAYER
	POKE(SCREEN_MEM+x_p_pos+800, 66);
	POKE(SCREEN_MEM+x_p_pos+801, 67);

	// ERASE MISSILE
	POKE(SCREEN_MEM+x_m_pos+760-40*y_m_pos, 68);
	POKE(SCREEN_MEM+x_m_pos+761-40*y_m_pos, 69);


	if ((is_game_over >= 1) && (is_game_over <120)) {
		POKE(SCREEN_MEM+440+14, 0);  //  		
		POKE(SCREEN_MEM+440+15, 39); // G
		POKE(SCREEN_MEM+440+16, 33); // A
		POKE(SCREEN_MEM+440+17, 45); // M
		POKE(SCREEN_MEM+440+18, 37); // E
		POKE(SCREEN_MEM+440+20, 47); // O
		POKE(SCREEN_MEM+440+21, 54); // V
		POKE(SCREEN_MEM+440+22, 37); // E
		POKE(SCREEN_MEM+440+23, 50); // R
		POKE(SCREEN_MEM+440+24, 0);  //
		is_game_over++;
	}
	
	if ((is_game_over >= 120) && (is_game_over <240)) {
		POKE(SCREEN_MEM+440+14, 48); // P
		POKE(SCREEN_MEM+440+15, 50); // R
		POKE(SCREEN_MEM+440+16, 37); // E
		POKE(SCREEN_MEM+440+17, 51); // S
		POKE(SCREEN_MEM+440+18, 51); // S
		POKE(SCREEN_MEM+440+20, 51); // S
		POKE(SCREEN_MEM+440+21, 52); // T
		POKE(SCREEN_MEM+440+22, 33); // A
		POKE(SCREEN_MEM+440+23, 50); // R
		POKE(SCREEN_MEM+440+24, 52); // T
		is_game_over++;
		if (is_game_over == 240) is_game_over = 1;
	}
}

void handle_input (void)
{
	stick0 = OS.stick0;
	strig0 = OS.strig0;
	if (stick0==STICK_RIGHT) {
		moving_type = MOVING_RIGHT;
	} else if (stick0==STICK_LEFT) {
		moving_type = MOVING_LEFT;
	} else {
		moving_type = MOVING_NONE;
	}
	if ((time_left > 0) && (y_m_pos == 0) && (strig0==0)) {
		y_m_pos++;
	}

	if (is_game_over > 0) {
		if (CONSOL_START(GTIA_READ.consol)) {
			toggle_game_over();
		}
	}
}

void wait_for_vblank(void);

void main(void)
{
	// CREATE SCREEN
	memcpy((void*) DLIST_MEM,antic4_display_list,sizeof(antic4_display_list));
	OS.sdlst=(void*)DLIST_MEM;

	// CREATE CHARSET
	memcpy((void*)CHARSET_MEM, 0xE000, 0x400);
	memcpy((void*)(CHARSET_MEM+8*64), sprite_data, 8*6);
	OS.chbas = CHARSET_MEM >> 8;

	// UPDATE COLORS
	OS.color1 = 0xA3;
	OS.color2 = 0x26;
	OS.color3 = 0xE1;
	OS.color4 = 0x00;

	// GAME LOOP
	while (1) {
		wait_for_vblank();
		erase_sprite();
		handle_input();
		update_sprite();
		draw_sprite();
	};
}

void wait_for_vblank(void) {
	asm("lda $14");
wvb:
	asm("cmp $14");
	asm("beq %g", wvb);
}