/************************************************
** Lesson #3 exercise                          **
** Display Line Interupt for changing color    **
** Pallete between lines					   **
** Kobi Tyrkel                                 ** 
** April 7, 2020                               **
************************************************/
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define SCREEN_MEM					0x5000
#define DLIST_MEM					0x4000	// aligned to 1K
#define CHARSET_MEM 				0x3400	// aligned to 1K
#define CHARSET_MEM2 				0x3800  // aligned to 1K

#define STICK_RIGHT					7
#define STICK_LEFT 					11

#define MOVING_NONE 				0
#define MOVING_RIGHT 				3
#define MOVING_LEFT 				4

#define SCORE_STR_INDEX 			0
#define TIME_LEFT_STR_INDEX			1
#define CREDITS_STR_INDEX			2
#define GAME_NAME_STR_INDEX			3
#define THEY_CAME_STR_INDEX			4
#define GAME_OVER_STR_INDEX			5
#define PRESS_START_STR_INDEX		6
#define BLANK_STR_INDEX				7
#define ENEMY_SHIP_STR_INDEX		8
#define PLAYER_SHIP_STR_INDEX		9
#define MISSILE_STR_INDEX			10
#define ENEMY_ATK1_CHAR_STR_INDEX	11
#define ENEMY_ATK2_CHAR_STR_INDEX	12
#define ENEMY_ATK3_CHAR_STR_INDEX	13
#define ENEMY_ATK4_CHAR_STR_INDEX	14
#define ENEMY_ATK5_CHAR_STR_INDEX	15
#define ENEMY_ATK6_CHAR_STR_INDEX	16
#define ENEMY_ATK7_CHAR_STR_INDEX	17
#define ENEMY_ATK8_CHAR_STR_INDEX	18
#define LIVES_LEFT_STR_INDEX		19
#define ABDUCTED_STR_INDEX	 		20
#define DESTROYED_STR_INDEX 		21
#define LOST_STR_INDEX				22
#define YOU_WIN_STR_INDEX			23

#define MISSILE_TOP_POSITION		17
const char * strings[] = {
    "SCORE:",
	"TIME LEFT:",
	"  KOBI TYRKEL PRESENTS   ",
	"I WANT TO BELIEVE (C)2020",
	"    THEY CAME IN 1982    ",
	"        GAME OVER        ",
	"  PRESS START TO BELIEVE ",
	"                         ",
	" abbc",
	" de ",
	" fg ",
	"  hi ",
	"  jk ",
	"  lm ",
	"  nn ",
	" hnni",
	" jnnk",
	" lnnm",
	" nnnn",
	"LIVES:",
	"   ALIENS ABDUCTED YOU   ",
	"YOUR SHIPS WERE DESTROYED",
	" YOU LOST, THEY ARE HERE ",
	" YOU WON, THEY'LL BE BACK",
};

unsigned char strings_len[sizeof(strings)/sizeof(strings[0])];

unsigned char fire_triggered = 0;
unsigned char vblank_occured;
unsigned char x_e_pos = 0;
unsigned char x_p_pos = 20;
unsigned char x_m_pos = 0;
unsigned char y_m_pos = 0;
unsigned char x;

unsigned char score;
unsigned int time_left = 3600;
unsigned int lives = 2;
unsigned char clk0 = 0;
unsigned char game_over_message = BLANK_STR_INDEX;

unsigned char stick0;
unsigned char strig0;

unsigned char is_moving_right = 0;
unsigned char is_moving_left = 0;
unsigned char moving_type = MOVING_NONE;
unsigned char enemy_move_delay = 0;
unsigned char is_enemy_hit = 0;
unsigned char is_player_hit = 0;
unsigned char is_game_over = 1;
unsigned char is_game_over_control = 0;
unsigned char enemy_speed = 2;
unsigned char enemy_attack_direction;
unsigned char y_e_atk_pos;
unsigned char x_e_atk_pos; 


// 32 bytes
unsigned char antic4_display_list[] = 
{
  DL_BLK8,
  DL_BLK8,
  DL_DLI(DL_BLK8),
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
  DL_DLI(DL_CHR40x8x4),
  DL_CHR40x8x1,
  DL_CHR40x8x4,
  DL_CHR40x8x4,
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
  DL_JVB,
  0x00,
  0x40
};

unsigned char sprite_data[] = {

	// ENEMY SPACESHIP
	//0123456789ABCDEF (4 charachters - 16 pixel 4 colors)
	//            
	//   2222222222   
	//  222222222222  
	// 11111111111111 
	// 11111111111111 
	//  222222222222  
	//   1111111111     
	//  11 22  22 11   

	// abbc 
	0x00, 0x02, 0x0A, 0x15, 0x15, 0x0A, 0x01, 0x05, //a
	0x00, 0xAA, 0xAA, 0x55, 0x55, 0xAA, 0x55, 0x28, //b
	0x00, 0x80, 0xA0, 0x54, 0x54, 0xA0, 0x40, 0x50, //c

	// PLAYER SHIP (2 charachters - 8 pixel 4 colors)
	//01234567
	//   11   
	//1  11  1
	//1  11  1
	//1  11  1
	//22222222
	//1  11  1

	// de
	0x01, 0x41, 0x41, 0x41, 0xAA, 0x41, 0x00, 0x00, //d
	0x40, 0x41, 0x41, 0x41, 0xAA, 0x41, 0x00, 0x00, //e
	
	// MISSILE (2 charachters - 8 pixel 4 colors)
	//01234567
	//   11   
	//   22   
	//   33   
	//   22   
	//   11   
	//   22   
	//   33   
	//   22   

	// fg
	0x01, 0x02, 0x03, 0x02, 0x01, 0x02, 0x03, 0x02, //f
	0x40, 0x80, 0xC0, 0x80, 0x40, 0x80, 0xC0, 0x80, //g

	// ENEMY BEAM
	//0123456789ABCDEF
	//       11     
	//     
	//       22
	//     
	//       11
	//     
	//       22
	//            	

	// _hi_
	0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, //h
	0x40, 0x00, 0x80, 0x00, 0x40, 0x00, 0x80, 0x00, //i

	// ENEMY BEAM
	//0123456789ABCDEF
	//      1111    
	//     
	//      2222
	//     
	//      1111
	//     
	//      2222
	//            	

	// _jk_
	0x05, 0x00, 0x0A, 0x00, 0x05, 0x00, 0x0A, 0x00, //j
	0x50, 0x00, 0xA0, 0x00, 0x50, 0x00, 0xA0, 0x00, //k


	// ENEMY BEAM
	//0123456789ABCDEF
	//     111111   
	//     
	//     222222
	//     
	//     111111
	//     
	//     222222
	//            	

	// _lm_
	0x15, 0x00, 0x2A, 0x00, 0x15, 0x00, 0x2A, 0x00, //l
	0x54, 0x00, 0xA8, 0x00, 0x54, 0x00, 0xA8, 0x00, //m

	// ENEMY BEAM
	//0123456789ABCDEF
	//    11111111   
	//     
	//    22222222
	//     
	//    11111111
	//     
	//    22222222
	//            	

	// _nn_
	0x55, 0x00, 0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00, //p

	// ENEMY BEAM
	//0123456789ABCDEF
	//   1111111111  
	//     
	//   2222222222
	//     
	//   1111111111
	//     
	//   2222222222
	//            	

	// hnni


	// ENEMY BEAM
	//0123456789ABCDEF
	//  111111111111 
	//     
	//  222222222222
	//     
	//  111111111111
	//     
	//  222222222222
	//     

	// jnnk   	

	// ENEMY BEAM
	//0123456789ABCDEF
	// 11111111111111
	//     
	// 22222222222222
	//     
	// 11111111111111
	//     
	// 22222222222222
	//     

	// lnnm

	// ENEMY BEAM
	//0123456789ABCDEF
	//1111111111111111 
	//     
	//2222222222222222
	//     
	//1111111111111111
	//     
	//2222222222222222
	//     

	// nnnn

	// LIVES CHARACTER (8x8 1 color)
	//01234567
	//   11   
	//   11   
	// 1 11 1
	// 1 11 1
	// 111111
	// 111111
	// 1    1
	//        

	0x18, 0x18, 0x5A, 0x5A, 0x7E, 0x7E, 0x42, 0x00 // o
};

unsigned char sprite_data_alternate[] = {

	// ENEMY SPACESHIP
	//0123456789ABCDEF (4 charachters - 16 pixel 4 colors)
	//            
	//   2 22222222   
	//  222 222 2222  
	// 11111 111 1111 
	// 111111 111 111 
	//  2222 222 222  
	//   1111 111 1     
	//  11 22  22 11   

	// abbc 
	0x00, 0x02, 0x0A, 0x15, 0x15, 0x0A, 0x01, 0x05, //a
	0x00, 0x2A, 0x8A, 0x51, 0x54, 0xA2, 0x54, 0x28, //b
	0x00, 0x80, 0xA0, 0x54, 0x54, 0xA0, 0x40, 0x50, //c

	// PLAYER SHIP (2 charachters - 8 pixel 4 colors)
	//01234567
	//
	//    11   
	// 1  11 1
	// 1 11  1
	//1 11  1
	//2222 222
	// 1 11  1
	//

	// de
	0x00, 0x00, 0x10, 0x11, 0x45, 0xAA, 0x11, 0x00, //d
	0x00, 0x50, 0x51, 0x81, 0x04, 0x2A, 0x41, 0x00  //e
};

void init_strings_length();
void string_index_to_mem(unsigned char, unsigned int);
void toggle_game_over(void);
void update_sprite(void);
void draw_sprite(void);
void handle_input (void);
void wait_for_vblank_clock(void);
void wait_for_vblank(void);
void dli_routine(void);
void vbi_routine(void);
void init_vbi_dli(void);
void dli_routine_1(void);
void dli_routine_2(void);
void dli_routine_3(void);


void main(void)
{
	vblank_occured = 0;

	// CREATE SCREEN
	memcpy((void*) DLIST_MEM,antic4_display_list,sizeof(antic4_display_list));
	OS.sdlst=(void*)DLIST_MEM;

	// CREATE CHARSET
	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);
	memcpy((void*)(CHARSET_MEM+8*65), sprite_data, 8*15);
	OS.chbas = CHARSET_MEM >> 8;

	memcpy((void*)CHARSET_MEM2, (int*)0xE000, 0x400);
	memcpy((void*)(CHARSET_MEM2+8*65), sprite_data_alternate, 8*5);
	
	// UPDATE COLORS
	OS.color0 = 0x26;
	OS.color1 = 0xFF;
	OS.color2 = 0x00;
	OS.color3 = 0xE1;
	OS.color4 = 0x00;

	init_strings_length();

	string_index_to_mem(SCORE_STR_INDEX, SCREEN_MEM+2);
	string_index_to_mem(LIVES_LEFT_STR_INDEX, SCREEN_MEM+14);
	string_index_to_mem(TIME_LEFT_STR_INDEX, SCREEN_MEM+25);

	init_vbi_dli();

  	// start the DLI by adding it to the VBI with the NMIEN register
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;

	// GAME LOOP
	while (1) {
		wait_for_vblank();
		handle_input();
		update_sprite();
		draw_sprite();
	};
}

void wait_for_vblank_clock(void) {
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

void dli_routine_1(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

    clk0++;
    ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= 0xF8;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= 0xFE;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= 0x26;

  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;

    if (is_enemy_hit > 0) ANTIC.chbase = CHARSET_MEM2 >> 8; //MSB of ALTERNATE CHARSET
    else ANTIC.chbase = CHARSET_MEM >> 8; //MSB of CHARSET

  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;

  	ANTIC.chbase = CHARSET_MEM >> 8; //MSB of CHARSET

  	OS.vdslst = &dli_routine_2;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine_2(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= clk0 + 1;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= clk0 + 3;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= clk0 + 5;
  	ANTIC.wsync = 1;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colpf1 	= clk0 + 7;
  	ANTIC.wsync = 1;
  	GTIA_WRITE.colbk 	= 0x00;
  	GTIA_WRITE.colpf1 	= 0xFE;
  	GTIA_WRITE.colpf0 	= 0x22;

    OS.vdslst = &dli_routine_3;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine_3(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;
    if (is_player_hit > 0) ANTIC.chbase = CHARSET_MEM2 >> 8; //MSB of ALTERNATE CHARSET
    else ANTIC.chbase = CHARSET_MEM >> 8; //MSB of CHARSET

    GTIA_WRITE.colbk 	= 0x00;
    GTIA_WRITE.colpf0	= 0x26;
    GTIA_WRITE.colpf1	= 0xFF;
    GTIA_WRITE.colpf2	= 0x00;
    GTIA_WRITE.colpf3	= 0xE1;
    
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    GTIA_WRITE.colpf0	= clk0;
    ANTIC.wsync = 1;
    GTIA_WRITE.colpf0	= 0x26;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;

    ANTIC.chbase = CHARSET_MEM >> 8; //MSB of CHARSET

    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    
    GTIA_WRITE.colbk 	= 0x80;
    GTIA_WRITE.colpf2	= 0x80;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    GTIA_WRITE.colbk 	= 0x82;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    GTIA_WRITE.colbk 	= 0x84;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    GTIA_WRITE.colbk 	= 0x86;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;

	GTIA_WRITE.colbk 	= 0x00;
    GTIA_WRITE.colpf0 	= 0x55;
    GTIA_WRITE.colpf2	= 0x00;
    
    OS.vdslst = &dli_routine_1;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void vbi_routine(void)
{
	OS.vdslst = &dli_routine_1;

	vblank_occured = 1;

	asm("pla");
	asm("tay");
	asm("pla");
	asm("tax");
	asm("pla");

asm("rti");
}

void init_vbi_dli(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;
}

void init_strings_length(void)
{
	clk0 = 0;
	do {
		x = 0;
		do 
		{
			score = PEEK(strings[clk0] + x);
			if (score != 0) POKE(strings[clk0] + x, score - 32);
			x++;
		} while (PEEK(strings[clk0] + x) != 0);
		strings_len[clk0] = x;
		clk0++;
	} while (clk0 < sizeof(strings) / sizeof(strings[0]));
	clk0 = 0;
}
void string_index_to_mem(unsigned char str_index, unsigned int scr_pos_addr)
{
/*
	x=0;
	do 
	{
		POKE(scr_pos_addr + x, PEEK(strings[str_index] + x) - 32);
		x++;
	} while (PEEK(strings[str_index] + x) != 0);
*/	
	memcpy((unsigned int*)scr_pos_addr, (unsigned int*)strings[str_index], strings_len[str_index]);
}

void toggle_game_over()
{
	if (is_game_over == 0) {
		is_game_over = 1;
		POKE(DLIST_MEM + 16, DL_CHR40x8x1);
		if ((lives == 0) && (time_left > 0)) 
		{
			if (score > 9)
			{
				game_over_message = ABDUCTED_STR_INDEX;
			} else {
				game_over_message = DESTROYED_STR_INDEX;
			}
		} else if (score > 12) {
			game_over_message = YOU_WIN_STR_INDEX;
		} else {
			game_over_message = LOST_STR_INDEX;
		}
	} else {
		is_game_over = 0;
		POKE(DLIST_MEM + 16, DL_CHR40x8x4);
		time_left = 3600;
		score = 0;
		lives = 2;
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM+447);
		POKE(SCREEN_MEM+20, 79);
		POKE(SCREEN_MEM+21, 79);
		POKE(SCREEN_MEM+22, 79);
		//string_index_to_mem(LIVES_CHAR_STR_INDEX, SCREEN_MEM+20);
		//string_index_to_mem(LIVES_CHAR_STR_INDEX, SCREEN_MEM+21);
		//string_index_to_mem(LIVES_CHAR_STR_INDEX, SCREEN_MEM+22);
		enemy_speed = 2;
	}
}

void update_sprite() 
{
	// MISSILE LOCATION UPDATE
	if ((y_m_pos > MISSILE_TOP_POSITION) || ((enemy_attack_direction >= 10) && (y_m_pos > 16 - y_e_atk_pos) && (x_m_pos + 2 >= x_e_atk_pos) && (x_m_pos - 2 <= x_e_atk_pos))) {
		y_m_pos = 0;
		fire_triggered = 0;
	}

	if (y_m_pos > 0) y_m_pos++;
	if ((y_m_pos == 0) && (fire_triggered == 1)) y_m_pos++;

	// PLAYER LOCATION UPDATE
	if ((moving_type == MOVING_RIGHT) && (is_player_hit == 0) && (is_game_over == 0)) {
		if (x_p_pos < 38) x_p_pos++;
	} else if ((moving_type == MOVING_LEFT) && (is_player_hit == 0) && (is_game_over == 0)) {
		if (x_p_pos > 0) x_p_pos--;
	}

	if (y_m_pos == 0) x_m_pos = x_p_pos;
	
	// TIMER UPDATE	
	if ((time_left > 0) && (is_game_over == 0)) {
		time_left--;

		// ENEMY LOCATION UPDATE
		if (is_enemy_hit == 0) {
			enemy_move_delay++;
		}
		if (enemy_move_delay > enemy_speed) {
			if ((enemy_attack_direction < 10) || (enemy_attack_direction > 29)) x_e_pos++;
			enemy_move_delay = 0;
		}
		if (x_e_pos > 36) x_e_pos = 0;
		
		// ENEMY ATTACK UPDATES
		if ((x_e_pos == x_p_pos - 1) && (enemy_attack_direction == 0) && (is_player_hit == 0)) 
		{
			if (score < 10) 
			{
				enemy_attack_direction = 1;
			} else 
			{
				enemy_attack_direction = 10;
				enemy_move_delay = 35;
			}
			y_e_atk_pos = 0;
			x_e_atk_pos = x_e_pos;
		}

		if ((enemy_attack_direction == 1) && (y_e_atk_pos < 17)) 
		{
			y_e_atk_pos++;
			enemy_attack_direction++;
		}

		if ((enemy_attack_direction == 1) && (y_e_atk_pos == 17))
		{
			enemy_attack_direction = 0;
		} 

		if ((enemy_attack_direction == 10) && (y_e_atk_pos < 17))
		{
			y_e_atk_pos++;
			enemy_attack_direction++;
		}

		if ((enemy_attack_direction == 10) && (y_e_atk_pos == 17))
		{
			enemy_attack_direction = 20;
		}

		if ((enemy_attack_direction == 20) && (y_e_atk_pos > 0))
		{
			y_e_atk_pos--;
			enemy_attack_direction++;
		}

		if ((enemy_attack_direction == 20) && (y_e_atk_pos == 0))
		{
			enemy_attack_direction = 30;
			enemy_move_delay = 2;
		}

		if ((enemy_attack_direction == 30) && (x_e_pos == 0))
		{
			enemy_attack_direction = 0;
			enemy_move_delay = 0;
		}

		if (enemy_attack_direction >= 1) 
		{
			enemy_attack_direction++;
			if (enemy_attack_direction == 4) enemy_attack_direction = 1;
			if (enemy_attack_direction == 14) enemy_attack_direction = 10;
			if (enemy_attack_direction == 24) enemy_attack_direction = 20;
			if (enemy_attack_direction == 34) enemy_attack_direction = 30;
		}

		// HANDLE ENEMY HITS
		if (y_m_pos == 18) 
		{
			if ((x_m_pos + 2 >= x_e_pos) && (x_m_pos - 2 <= x_e_pos)) 
			{
				score++;
				is_enemy_hit=20;
				if (score == 10) 
				{
					enemy_speed = 1;
				}
				if (score == 20) 
				{
					enemy_speed = 0;
				}
			}
		}

		if (is_enemy_hit > 0) 
		{
			is_enemy_hit--;
			if (is_enemy_hit == 0) 
			{
				x_e_pos = 0;
				y_m_pos = 0;
			}
		}

		// HANDLE PLAYER HITS
		if (y_e_atk_pos == 17 && (enemy_attack_direction > 0)) 
		{
			if ((x_p_pos + 2 >= x_e_atk_pos) && (x_p_pos - 2 <= x_e_atk_pos)) {
				is_player_hit = 50;
			}
		}

		if (is_player_hit > 0) 
		{
			is_player_hit--;
			if (is_player_hit == 16)
			{
				if (lives == 0) 
				{
					toggle_game_over();
					POKE(SCREEN_MEM + 20 + lives, 0);
					//string_index_to_mem(BLANK_CHAR_STR_INDEX, SCREEN_MEM + 20 + lives);
				} else
				{
					POKE(SCREEN_MEM + 20 + lives, 0);
					//string_index_to_mem(BLANK_CHAR_STR_INDEX, SCREEN_MEM + 20 + lives);
					lives--;
				}

			}

		}
	}

	// HANDLE GAME OVER
	if ((time_left == 0) && (is_game_over == 0)) {
		toggle_game_over();
	}
}

void draw_sprite() 
{
	// DRAW SCORE
	POKE(SCREEN_MEM+9, 16 + score / 10); // 0
	POKE(SCREEN_MEM+10, 16 + score % 10); // 0
	POKE(SCREEN_MEM+11, 16); // 0

	// DRAW TIME LEFT
	POKE(SCREEN_MEM+36, 16 + time_left / 600); // s
	POKE(SCREEN_MEM+37, 16 + (time_left / 60) % 10); // s

	// DRAW ENEMY SHIP
	string_index_to_mem(ENEMY_SHIP_STR_INDEX, SCREEN_MEM + x_e_pos + 79);
	if ((is_enemy_hit == 1) || (x_e_pos == 36))
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_e_pos + 79);
	
	// DRAW PLAYER SHIP
	if ((is_player_hit > 0) && (enemy_attack_direction > 19))
	{
		string_index_to_mem(PLAYER_SHIP_STR_INDEX, SCREEN_MEM + x_p_pos + 79 + 40 * y_e_atk_pos);
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_p_pos + 119 + 40 * y_e_atk_pos);
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_p_pos + 799);
	} else {
		string_index_to_mem(PLAYER_SHIP_STR_INDEX, SCREEN_MEM + x_p_pos + 799);
	}
	
	// DRAW PLAYER WEAPON
	if ((y_m_pos <= MISSILE_TOP_POSITION) && (is_player_hit == 0))
		string_index_to_mem(MISSILE_STR_INDEX, SCREEN_MEM + x_m_pos + 759 - 40 * y_m_pos);
	if (fire_triggered) 
		//string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_m_pos + 799 - 40 * y_m_pos);
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_m_pos + 799 - 40 * y_m_pos);
	
	// DRAW ENEMY WEAPON - MISSILES
	if (enemy_attack_direction == 1)
	{
		if (is_player_hit == 0) string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		if (y_e_atk_pos < 17)
			string_index_to_mem(ENEMY_ATK1_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 119 + 40 * y_e_atk_pos);
	}


	// DRAW ENEMY WEAPON - ABDUCTION
	if (enemy_attack_direction == 10)
	{
		if (y_e_atk_pos < 3)
		{
			string_index_to_mem(ENEMY_ATK1_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 5)
		{
			string_index_to_mem(ENEMY_ATK2_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 7)
		{
			string_index_to_mem(ENEMY_ATK3_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 9)
		{
			string_index_to_mem(ENEMY_ATK4_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 11)
		{
			string_index_to_mem(ENEMY_ATK5_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 13)
		{
			string_index_to_mem(ENEMY_ATK6_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 15)
		{
			string_index_to_mem(ENEMY_ATK7_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		} else if (y_e_atk_pos < 17)
		{
			string_index_to_mem(ENEMY_ATK8_CHAR_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
		}
	}

	if ((enemy_attack_direction == 20) && (is_player_hit == 0))
	{
		string_index_to_mem(BLANK_STR_INDEX, SCREEN_MEM + x_e_atk_pos + 79 + 40 * y_e_atk_pos);
	}

	// DRAW MENU MESSAGES
	if (is_game_over >=1) 
	{
		if (is_game_over == 1) string_index_to_mem(game_over_message, SCREEN_MEM+447);
		if (is_game_over == 2) string_index_to_mem(CREDITS_STR_INDEX, SCREEN_MEM+447);
		if (is_game_over == 3) string_index_to_mem(GAME_NAME_STR_INDEX, SCREEN_MEM+447);
		if (is_game_over == 4) string_index_to_mem(THEY_CAME_STR_INDEX, SCREEN_MEM+447);
		if (is_game_over == 5) string_index_to_mem(PRESS_START_STR_INDEX, SCREEN_MEM+447);
		if (is_game_over_control == 0) is_game_over++;
		is_game_over_control++;
		if (is_game_over == 6) is_game_over = 1;
	}
}

void handle_input (void)
{
	strig0 = GTIA_READ.trig0;
	stick0 = (PIA.porta & 0x0f);
	
	if (stick0==STICK_RIGHT) {
		moving_type = MOVING_RIGHT;
	} else if (stick0==STICK_LEFT) {
		moving_type = MOVING_LEFT;
	} else {
		moving_type = MOVING_NONE;
	}
	
	if ((strig0 == 0) && (time_left > 0) && (is_game_over == 0) && (is_enemy_hit == 0)) fire_triggered = 1;

	if (is_game_over > 0) {
		if (CONSOL_START(GTIA_READ.consol)) {
			toggle_game_over();
		}
	}
}