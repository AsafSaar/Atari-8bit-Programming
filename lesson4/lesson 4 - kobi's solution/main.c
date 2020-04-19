////////////////////////////////////
// main.c 
////////////////////////////////////
// homework - dino game
// make a colorful dino made of 2 PMGs - changed to 3 single color PMGs
// make the screen with gradients as well - done
// make the background of characters - changed to PMG for smooth scorlling
// make a sprite as BG for gameover text with a different priority - made custom font with different colors for letters and "rainbow" effect using DLI

#include <atari.h>
#include <string.h>
#include <peekpoke.h>



#define CHARSET_MEM           0x6400   // aligned to 1K
#define CHARSET_MEM2          0x6800     // aligned to 1K
#define DLIST_MEM             0x6C00    // aligned to 1K
#define SCREEN_MEM            0x7000
#define PMG_MEM               0x7800    // aligned 2K
#define JOY_NO_MOVE           0
#define JOY_BIT_UP            1
#define JOY_BIT_DOWN          2
#define JOY_BIT_LEFT          4
#define JOY_BIT_RIGHT         8
#define JOY_BIT_TOP_RIGHT     9
#define JOY_BIT_TOP_LEFT      5

#define MOVING_NONE           0
#define MOVING_UP             1
#define MOVING_DOWN           2
#define MOVING_LEFT           3
#define MOVING_RIGHT          4
#define MOVING_DUCK           5

#define CLOUD_Y_POS           0x18 //0x30
#define CLOUD_HEIGHT          9

#define CACTUS_Y_POS          0x35 //0x8B
#define CACTUS_TYPES          2
#define CACTUS_HEIGHT         34

#define BIRD_FRAMES           2
#define BIRD_HEIGHT           8
#define BIRD_Y_POS            0x2C //0x50

#define STONE_FRAMES          4
#define STONE_HEIGHT          8
#define STONE_Y_POS           0x4F

#define SMALL_BUMP_HEIGHT     2
#define SMALL_BUMP_Y_POS      0x59 //0xB2

#define LARGE_BUMP_HEIGHT     3
#define LARGE_BUMP_Y_POS      0x60 //0xC0

#define STICK_HEIGHT          0xA
#define STICK_Y_POS           0x68 //0xD0

#define DINO_START_Y          0x3F //0x95
#define DINO_X_POS            0x25
#define DINO_HEIGHT           24
#define DINO_FRAMES           4
#define DINO_JUMP_DISTANCE    0x5B

#define OBSTACLE_CACTUS       0
#define OBSTACLE_BIRD         1
#define OBSTACLE_STONE        2

const unsigned char DINO_BACK[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0x80, 0xc3, 0xe7, 0xff, 0xff, 0x7f, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0x80, 0xc3, 0xe7, 0xff, 0xff, 0x7f, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0x80, 0xc3, 0xe7, 0xff, 0xff, 0x7f, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0x80, 0xc3, 0xe7, 0xff, 0xff, 0x7f, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x00, 0x00, 0x00
  }
};

const unsigned char DINO_MID[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 
    0x1e, 0x3e, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  },
  {
    0x0f, 0x1b, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 
    0x1e, 0x3e, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  },
  {
    0x0f, 0x1b, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 
    0x1e, 0x3e, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xce, 0x80, 0x00, 0x00, 0x00
  },
  {
    0x0f, 0x1b, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 
    0x1e, 0x3e, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  }
};

const unsigned char DINO_FRONT[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0x80, 0xf0, 0x00, 
    0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0x80, 0xf0, 0x00, 
    0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0x80, 0xf0, 0x00, 
    0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0x80, 0xf0, 0x00, 
    0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

const unsigned char DINO_DUCK_BACK[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x63, 
    0x37, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x63, 
    0x37, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x63, 
    0x37, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x02, 0x03
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x63, 
    0x37, 0x1f, 0x0f, 0x07, 0x02, 0x01, 0x00, 0x00
  }
};

const unsigned char DINO_DUCK_MID[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 
    0x1f, 0x1f, 0x1f, 0x10, 0x1f, 0x1e, 0x7f, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1b, 
    0x1f, 0x1f, 0x1f, 0x10, 0x1f, 0x1e, 0x7f, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1b, 
    0x1f, 0x1f, 0x1f, 0x10, 0x1f, 0x1e, 0x7f, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0x9c, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1b, 
    0x1f, 0x1f, 0x1f, 0x10, 0x1f, 0x1e, 0x7f, 0xfe, 
    0xfc, 0xfc, 0xf8, 0xf0, 0xb0, 0x10, 0x10, 0x18
  }
};

const unsigned char DINO_DUCK_FRONT[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xf8, 
    0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x00, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xf8, 
    0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x00, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xf8, 
    0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x00, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xf8, 
    0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x00, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

const unsigned char DINO_COLOR[DINO_FRAMES][DINO_HEIGHT] =
{
  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

const unsigned char CLOUD_DATA[CLOUD_HEIGHT] =
{
  0x60, 0x66, 0xf7, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c
};

const unsigned char CACTUS_DATA[CACTUS_TYPES][CACTUS_HEIGHT] =
{
  {
    0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x98, 
    0xd8, 0xd8, 0xd8, 0xd9, 0xdb, 0xdb, 0xdb, 0xdb, 
    0xdb, 0xfb, 0x7b, 0x1b, 0x1b, 0x1b, 0x1f, 0x1e, 
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
    0x18, 0x10
  },
  {
    0x08, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x19, 
    0x1b, 0x1b, 0x9b, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 
    0xdb, 0xdf, 0xde, 0xd8, 0xf8, 0x78, 0x18, 0x18, 
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
    0x18, 0x08
  }
};

const unsigned char BIRD_DATA[BIRD_FRAMES][BIRD_HEIGHT] =
{
  {
    0x00, 0x20, 0x50, 0xff, 0x3e, 0x06, 0x03, 0x00
  },
  {
    0x01, 0x23, 0x56, 0xff, 0x21, 0xc0, 0x00, 0x00
  }
};

const unsigned char STONE_DATA[STONE_FRAMES][STONE_HEIGHT] =
{
  {
    0x1c, 0x36, 0x67, 0x4f, 0xdf, 0xff, 0x7e, 0x3c
  },
  {
    0x1c, 0x3e, 0x7f, 0x5f, 0xcf, 0xe7, 0x76, 0x3c
  },
  {
    0x1c, 0x3e, 0x7f, 0x7d, 0xf9, 0xf3, 0x76, 0x3c
  },
  {
    0x1c, 0x36, 0x73, 0x79, 0xfd, 0xff, 0x7e, 0x3c
  }
};

const unsigned char SMALL_BUMP[SMALL_BUMP_HEIGHT] =
{
  0x08, 0x14
};

const unsigned char LARGE_BUMP[LARGE_BUMP_HEIGHT] =
{
  0x08, 0x14, 0x22
};

const unsigned char STICK[STICK_HEIGHT] =
{
  0x06, 0x07, 0x07, 0x0f, 0x1c, 0xb8, 0xf0, 0xe0, 0xe0, 0x60
};

unsigned char st;
unsigned char vblank_occured;
unsigned char vblank_counter;
unsigned char moving_type;
unsigned char player_jump;
unsigned char player_x_position;
unsigned char player_y_position;
unsigned char player_sprite_frame;
unsigned char x;
unsigned int  score;
unsigned int  target_score;
unsigned int  highscore;
unsigned int  target_highscore;
unsigned char clk0;
unsigned char collision;
unsigned char cloud_x_pos;
unsigned char cactus_x_pos;
unsigned char cactus_type;
unsigned char bird_frame;
unsigned char stone_y_pos;
unsigned char stone_frame;
unsigned char stone_jump_direction;
unsigned char stone_jump_height;
unsigned char small_bump_x_pos;
unsigned char large_bump_x_pos;
unsigned char stick_x_pos;
unsigned char is_game_over;
unsigned char strig0;
unsigned char start_button;
unsigned char obstacle_type;
unsigned char obstacle_erase;
unsigned char seconds;
unsigned char minutes;


#define GAME_OVER_STR               0
#define PRESS_START_STR             1
#define AUTHOR_STR                  2
#define GAME_TITLE_STR              3
#define SCORE_STR                   4
#define HI_SCORE_STR                5
#define BLANK_STR                   6
#define FIRE_OR_START_STR           7
#define TIME_STR                    8

const char * strings[] = {
  "        GAme OVER        ",
  "PRESS start TO PLAY AGAIN",
  "   KoBi TyRkEl PrEsEnTs  ",
  " DInO quEst FOR http 2020",
  "SCORE: 000000",
  "HI: 0",
  "                         ",
  "    FIRE WILL ALSO DO    ",
  "TIme: 00:00",
};
 
unsigned char strings_len[sizeof(strings)/sizeof(strings[0])];

// 32 bytes
unsigned char antic4_display_list[] = 
{
  DL_DLI(DL_BLK8),
  DL_BLK8,
  DL_BLK8,
  DL_LMS(DL_CHR40x8x4),
  0x00,
  0x70,
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
  DL_JVB,
  0x00,
  0x6C
};

unsigned char sprite_dat[] = 
{
  // Space
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 1 Atascii code: 33
  0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x3C, 0x00,
  // Internal code: 2 Atascii code: 34
  0x00, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00,
  // Internal code: 3 Atascii code: 35
  0x00, 0x30, 0xFC, 0x30, 0x30, 0xFC, 0x30, 0x30,
  // Internal code: 4 Atascii code: 36
  0x00, 0x30, 0x3C, 0xC0, 0x30, 0xC, 0xFC, 0x30,
  // Internal code: 5 Atascii code: 37
  0xF, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xF,
  // Internal code: 6 Atascii code: 38
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3, 0x3F, 0xFF,
  // Internal code: 7 Atascii code: 39
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF,
  // Internal code: 8 Atascii code: 40
  0xC0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0,
  // Internal code: 9 Atascii code: 41
  0x3F, 0xF0, 0xC0, 0xC0, 0xC0, 0xF0, 0x3F, 0x3F,
  // Internal code: 10 Atascii code: 42
  0xF0, 0xC, 0xC, 0xC, 0xF0, 0x00, 0xFC, 0xF0,
  // Internal code: 11 Atascii code: 43
  0x00, 0x5, 0x5, 0x15, 0x10, 0x50, 0x40, 0x00,
  // Internal code: 12 Atascii code: 44
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x5,
  // Internal code: 13 Atascii code: 45
  0x15, 0x54, 0x50, 0x50, 0x40, 0x00, 0x00, 0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 16 Atascii code: 48
  0x30, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x30,
  // Internal code: 17 Atascii code: 49
  0x30, 0xF0, 0xF0, 0x30, 0x30, 0x30, 0xFC, 0xFC,
  // Internal code: 18 Atascii code: 50
  0x30, 0xFC, 0xCC, 0xC, 0x30, 0xC0, 0xFC, 0xFC,
  // Internal code: 19 Atascii code: 51
  0xFC, 0xFC, 0xC, 0x30, 0xC, 0xCC, 0xFC, 0x30,
  // Internal code: 20 Atascii code: 52
  0xC, 0x3C, 0xFC, 0xCC, 0xFC, 0xFC, 0xC, 0xC,
  // Internal code: 21 Atascii code: 53
  0xFC, 0xFC, 0xC0, 0xFC, 0xC, 0xCC, 0xFC, 0x30,
  // Internal code: 22 Atascii code: 54
  0x3C, 0xFC, 0xC0, 0xFC, 0xCC, 0xCC, 0xFC, 0x3C,
  // Internal code: 23 Atascii code: 55
  0xFC, 0xFC, 0xC, 0x3C, 0x30, 0xF0, 0xC0, 0xC0,
  // Internal code: 24 Atascii code: 56
  0x30, 0xFC, 0xCC, 0xFC, 0xCC, 0xCC, 0xFC, 0x30,
  // Internal code: 25 Atascii code: 57
  0x30, 0xFC, 0xCC, 0xFC, 0xC, 0xC, 0xFC, 0xF0,
  // Internal code: 26 Atascii code: 58
  0x00, 0x00, 0x3C, 0x3C, 0x00, 0x3C, 0x3C, 0x00,
  // Internal code: 27 Atascii code: 59
  0x00, 0x00, 0x3C, 0x3C, 0x00, 0x3C, 0x3C, 0xF0,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 33 Atascii code: 65
  0x30, 0xFC, 0xCC, 0xFC, 0xFC, 0xCC, 0xCC, 0xCC,
  // Internal code: 34 Atascii code: 66
  0xF0, 0xFC, 0xCC, 0xFC, 0xCC, 0xCC, 0xFC, 0xF0,
  // Internal code: 35 Atascii code: 67
  0x30, 0xFC, 0xCC, 0xC0, 0xC0, 0xCC, 0xFC, 0x30,
  // Internal code: 36 Atascii code: 68
  0xF0, 0xF0, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0xF0,
  // Internal code: 37 Atascii code: 69
  0xFC, 0xFC, 0xC0, 0xFC, 0xC0, 0xC0, 0xFC, 0xFC,
  // Internal code: 38 Atascii code: 70
  0xFC, 0xFC, 0xC0, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0,
  // Internal code: 39 Atascii code: 71
  0x3C, 0xFC, 0xC0, 0xC0, 0xCC, 0xCC, 0xFC, 0x3C,
  // Internal code: 40 Atascii code: 72
  0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC,
  // Internal code: 41 Atascii code: 73
  0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0xFC, 0xFC,
  // Internal code: 42 Atascii code: 74
  0xC, 0xC, 0xC, 0xC, 0xCC, 0xCC, 0xFC, 0x3C,
  // Internal code: 43 Atascii code: 75
  0xC0, 0xCC, 0xCC, 0xF0, 0xF0, 0xCC, 0xCC, 0xCC,
  // Internal code: 44 Atascii code: 76
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC,
  // Internal code: 45 Atascii code: 77
  0xC3, 0xC3, 0xCF, 0xFF, 0xF3, 0xC3, 0xC3, 0xC3,
  // Internal code: 46 Atascii code: 78
  0xC3, 0xC3, 0xF3, 0xF3, 0xCF, 0xCF, 0xC3, 0xC3,
  // Internal code: 47 Atascii code: 79
  0x30, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x30,
  // Internal code: 48 Atascii code: 80
  0xF0, 0xFC, 0xCC, 0xFC, 0xF0, 0xC0, 0xC0, 0xC0,
  // Internal code: 49 Atascii code: 81
  0x30, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xF0, 0x3C,
  // Internal code: 50 Atascii code: 82
  0xF0, 0xFC, 0xCC, 0xF0, 0xFC, 0xCC, 0xCC, 0xCC,
  // Internal code: 51 Atascii code: 83
  0x3C, 0xFC, 0xC0, 0xF0, 0x3C, 0xC, 0xFC, 0xF0,
  // Internal code: 52 Atascii code: 84
  0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
  // Internal code: 53 Atascii code: 85
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC,
  // Internal code: 54 Atascii code: 86
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x30, 0x30,
  // Internal code: 55 Atascii code: 87
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xCC,
  // Internal code: 56 Atascii code: 88
  0xCC, 0xCC, 0x30, 0x30, 0x30, 0xCC, 0xCC, 0xCC,
  // Internal code: 57 Atascii code: 89
  0xCC, 0xCC, 0xCC, 0x30, 0x30, 0x30, 0x30, 0x30,
  // Internal code: 58 Atascii code: 90
  0xFC, 0xFC, 0xC, 0x30, 0x30, 0xC0, 0xFC, 0xFC,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 64 Atascii code: 0
  0x55, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
  // Internal code: 65 Atascii code: 1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  // Internal code: 66 Atascii code: 2
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xFF,
  // Internal code: 67 Atascii code: 3
  0x00, 0x00, 0x11, 0x4, 0x00, 0x00, 0x00, 0x00,
  // Internal code: 68 Atascii code: 4
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0x4,
  // Internal code: 69 Atascii code: 5
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
  // Internal code: 70 Atascii code: 6
  0xA8, 0xA8, 0xAA, 0xA8, 0x20, 0x00, 0x00, 0x00,
  // Internal code: 71 Atascii code: 7
  0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  // Internal code: 72 Atascii code: 8
  0x20, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  // Internal code: 73 Atascii code: 9
  0xA8, 0x88, 0x88, 0x88, 0xA8, 0xA8, 0xA8, 0xA8,
  // Internal code: 74 Atascii code: 10
  0x88, 0x88, 0x88, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
  // Internal code: 75 Atascii code: 11
  0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0x88, 0x88, 0x88,
  // Internal code: 76 Atascii code: 12
  0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  // Internal code: 77 Atascii code: 13
  0xA8, 0xA8, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
  // Internal code: 78 Atascii code: 14
  0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0xC,
  // Internal code: 79 Atascii code: 15
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C,
  // Internal code: 80 Atascii code: 16
  0xC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  // Internal code: 81 Atascii code: 17
  0x30, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  // Internal code: 82 Atascii code: 18
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  // Internal code: 83 Atascii code: 19
  0xC, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  // Internal code: 84 Atascii code: 20
  0x00, 0xF, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
  // Internal code: 86 Atascii code: 22
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xC0,
  // Internal code: 87 Atascii code: 23
  0x00, 0xC0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF,
  // Internal code: 88 Atascii code: 24
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xF0,
  // Internal code: 89 Atascii code: 25
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xC0,
  // Internal code: 90 Atascii code: 26
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 92 Atascii code: 28
  0x00, 0x8, 0x22, 0xAA, 0xA, 0x00, 0x00, 0x00,
  // Internal code: 93 Atascii code: 29
  0x00, 0x00, 0x00, 0xAA, 0xA8, 0x28, 0xA, 0x00,
  // Internal code: 94 Atascii code: 30
  0x00, 0x8, 0x22, 0xAA, 0xA, 0xA0, 0x00, 0x00,
  // Internal code: 95 Atascii code: 31
  0x2, 0xA, 0x28, 0xAA, 0xA, 0x00, 0x00, 0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // Internal code: 97 Atascii code: 97
  0x10, 0x54, 0x44, 0x54, 0x44, 0x44, 0x44, 0x44,
  // Internal code: 98 Atascii code: 98
  0x50, 0x54, 0x44, 0x54, 0x44, 0x44, 0x54, 0x50,
  // Internal code: 99 Atascii code: 99
  0x10, 0x54, 0x44, 0x40, 0x40, 0x44, 0x54, 0x10,
  // Internal code: 100 Atascii code: 100
  0x50, 0x50, 0x44, 0x44, 0x44, 0x44, 0x54, 0x50,
  // Internal code: 101 Atascii code: 101
  //0x54, 0x54, 0x40, 0x54, 0x40, 0x40, 0x54, 0x54,
  0xCF, 0xCC, 0xCC, 0xCF, 0xCC, 0xCC, 0xCC, 0xCF, // e THAT WORKS AFTER a wide m
  // Internal code: 102 Atascii code: 102
  0x54, 0x54, 0x40, 0x54, 0x40, 0x40, 0x40, 0x40,
  // Internal code: 103 Atascii code: 103
  0x14, 0x54, 0x40, 0x40, 0x44, 0x44, 0x54, 0x14,
  // Internal code: 104 Atascii code: 104
  0x44, 0x44, 0x44, 0x54, 0x44, 0x44, 0x44, 0x44,
  // Internal code: 105 Atascii code: 105
  0x54, 0x54, 0x10, 0x10, 0x10, 0x10, 0x54, 0x54,
  // Internal code: 106 Atascii code: 106
  0x4, 0x4, 0x4, 0x4, 0x44, 0x44, 0x54, 0x14,
  // Internal code: 107 Atascii code: 107
  0x40, 0x44, 0x44, 0x50, 0x50, 0x44, 0x44, 0x44,
  // Internal code: 108 Atascii code: 108
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x54, 0x54,
  // Internal code: 109 Atascii code: 109
  //0x41, 0x41, 0x45, 0x55, 0x51, 0x41, 0x41, 0x41,
  0xC0, 0xF3, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, // WIDE m THAT WORKS WITH e
  // Internal code: 110 Atascii code: 110
  0x41, 0x41, 0x51, 0x51, 0x45, 0x45, 0x41, 0x41,
  // Internal code: 111 Atascii code: 111
  0x10, 0x54, 0x44, 0x44, 0x44, 0x44, 0x54, 0x10,
  // Internal code: 112 Atascii code: 112
  0x50, 0x54, 0x44, 0x54, 0x50, 0x40, 0x40, 0x40,
  // Internal code: 113 Atascii code: 113
  0x10, 0x54, 0x44, 0x44, 0x44, 0x44, 0x50, 0x14,
  // Internal code: 114 Atascii code: 114
  0x50, 0x54, 0x44, 0x50, 0x54, 0x44, 0x44, 0x44,
  // Internal code: 115 Atascii code: 115
  0x14, 0x54, 0x40, 0x50, 0x14, 0x4, 0x54, 0x50,
  // Internal code: 116 Atascii code: 116
  0x54, 0x54, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  // Internal code: 117 Atascii code: 117
  0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x54,
  // Internal code: 118 Atascii code: 118
  0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x10, 0x10,
  // Internal code: 119 Atascii code: 119
  0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x54, 0x44,
  // Internal code: 120 Atascii code: 120
  0x44, 0x44, 0x10, 0x10, 0x10, 0x44, 0x44, 0x44,
  // Internal code: 121 Atascii code: 121
  0x44, 0x44, 0x44, 0x10, 0x10, 0x10, 0x10, 0x10,
  // Internal code: 122 Atascii code: 122
  0x54, 0x54, 0x4, 0x10, 0x10, 0x40, 0x54, 0x54,
  // Internal code: 123 Atascii code: 123
  0x55, 0x15, 0x15, 0x5, 0x5, 0x5, 0x1, 0x1,
  // Internal code: 124 Atascii code: 124
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
  // Internal code: 125 Atascii code: 125
  0x55, 0x54, 0x54, 0x50, 0x50, 0x50, 0x40, 0x40,
  // Internal code: 126 Atascii code: 126
  0x1, 0x41, 0x44, 0x55, 0x15, 0x11, 0x51, 0x41,
  // Internal code: 127 Atascii code: 127
  0x4, 0x15, 0x51, 0x41, 0x45, 0x55, 0x15, 0x4
};

void init_strings_length(void);
void string_index_to_mem(unsigned char, unsigned int);
void wait_for_vblank_clock(void);
void wait_for_vblank(void);
void erase_sprite(void);
void update_sprite(void);
void draw_sprite(void);
void handle_input(void);
void dli_routine(void);
void dli_routine1(void);
void dli_routine2(void);
void vbi_routine(void);
void init_vbi_dli(void);
void setup_pmg(void);
void vbi_copy_shadow_to_hw_registers(void);
void check_collision(void);
void update_cactus_type(void);
void update_dino(void);
void update_player_objects(void);
void init_new_game(void);
void print_score(void);
void print_highscore(void);
void update_bird_animation(void);
void update_stone_animation(void);
void print_time(void);

void main(void)
{

  memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
  bzero((void*)SCREEN_MEM, 960);

  OS.sdlst = (void*)DLIST_MEM;

  OS.color0 = 0x55;   // playerfield 0
  OS.color1 = 0xFF;   // playerfield 1
  OS.color2 = 0X26;   // playerfield 2
  OS.color3 = 0xE1;   // playerfield 3
  OS.color4 = 0x00;   // background

  // [CHARSETADDRESS] + 8 * charcode
  memcpy((void*) (CHARSET_MEM) , sprite_dat, 8*128);

  OS.chbas = CHARSET_MEM >> 8; // MSB / 256
  
  init_strings_length();
  string_index_to_mem(SCORE_STR, SCREEN_MEM + 2);
  string_index_to_mem(HI_SCORE_STR, SCREEN_MEM + 28);
  string_index_to_mem(TIME_STR, SCREEN_MEM + 16);
  highscore = 0;
  target_highscore = 0;
  setup_pmg();
  init_new_game();
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
    check_collision();
  };


  // stop DLI by setting NMIEN to point only on VBI
  ANTIC.nmien = NMIEN_VBI; 
}

void init_new_game(void)
{
  moving_type = MOVING_NONE;
  vblank_occured = 0;
  is_game_over = 0;
  score = 0;
  target_score = 0;
  collision = 0;
  cactus_x_pos = 255;
  
  bird_frame = 0;

  stone_frame = 0;
  stone_y_pos = STONE_Y_POS;
  stone_jump_direction = 0;
  stone_jump_height = 0;
  st = 0;
  vblank_occured = 0;
  vblank_counter = 0;
  moving_type = MOVING_NONE;
  player_jump = 0;

  obstacle_type = OBSTACLE_CACTUS;
  obstacle_erase = 1;

  seconds = 0;
  minutes = 0;

  setup_pmg();
  string_index_to_mem(BLANK_STR, SCREEN_MEM + 248);
  GTIA_WRITE.hitclr = 0;
  update_cactus_type();
  print_score();
  print_highscore();
}

void print_score(void)
{
    POKE(SCREEN_MEM+10, (score / 10000) % 10 + 16);
    POKE(SCREEN_MEM+11, (score / 1000) % 10 + 16);
    POKE(SCREEN_MEM+12, (score / 100) % 10 + 16);
    POKE(SCREEN_MEM+13, (score / 10) % 10 + 16);
    POKE(SCREEN_MEM+14, score % 10 + 16);
}

void print_highscore(void)
{
    POKE(SCREEN_MEM+32, (highscore / 10000) % 10 + 16);
    POKE(SCREEN_MEM+33, (highscore / 1000) % 10 + 16);
    POKE(SCREEN_MEM+34, (highscore / 100) % 10 + 16);
    POKE(SCREEN_MEM+35, (highscore / 10) % 10 + 16);
    POKE(SCREEN_MEM+36, highscore % 10 + 16);
}

void print_time(void)
{
    POKE(SCREEN_MEM+22, (minutes / 10) % 10 + 16);
    POKE(SCREEN_MEM+23, (minutes) % 10 + 16);
    POKE(SCREEN_MEM+25, (seconds / 10) % 10 + 16);
    POKE(SCREEN_MEM+26, seconds % 10 + 16);
}

void init_strings_length(void)
{
  clk0 = 0;
  do {
    x = 0;
    do 
    {
      score = PEEK(strings[clk0] + x);
      if ((score != 0) && (score < 97)) POKE(strings[clk0] + x, score - 32);
      x++;
    } while (PEEK(strings[clk0] + x) != 0);
    strings_len[clk0] = x;
    clk0++;
  } while (clk0 < sizeof(strings) / sizeof(strings[0]));
  clk0 = 0;
}

void string_index_to_mem(unsigned char str_index, unsigned int scr_pos_addr)
{
  memcpy((unsigned int*)scr_pos_addr, (unsigned int*)strings[str_index], strings_len[str_index]);
}

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
  vblank_counter++;
  if (vblank_counter == 240) vblank_counter = 0;
}

void erase_sprite(void)
{
  if ((obstacle_erase == 1) && (is_game_over == 0))
  {
    memset((void*)(PMG_MEM+0x0380), 0, 0x59);
    obstacle_erase = 2;
  }
}

void update_sprite(void)
{
  // handle game over timing and init new game on START / FIRE button press
  if (is_game_over > 0) {
    if (highscore < score) target_highscore = score;
    if (vblank_counter % 128 == 0)
    {
      is_game_over++;
      if (is_game_over > 10) is_game_over = 1;
    }

    if ((strig0 == 0) || (start_button > 0)) 
    {
      init_new_game();
    }
    return;
  }

  // update game time
  if (vblank_counter % 60 == 0) seconds++;
  if (seconds == 60)
  {
    minutes++;
    seconds = 0;
  }

  // handle cloud missile movement
  if (vblank_counter % 32 == 0) cloud_x_pos -= 1;

  // handle obstacle type changes and movement
  if (score > 120)
  {
    if (vblank_counter % 3) cactus_x_pos-=3;
    else cactus_x_pos-=2;
  }
  else if (score > 30) cactus_x_pos-=2;
  else cactus_x_pos--;
  if (cactus_x_pos <=4) 
  {
    cactus_type++;
    obstacle_erase = 1;
    target_score += 10;
    cactus_x_pos = 255;
    if (cactus_type == CACTUS_TYPES)
    {
      if (score > 60)
      {
        obstacle_type = OBSTACLE_BIRD;
        obstacle_erase = 1;
      }
      else
      {
        cactus_type = 0;
      }
    }
    else if (cactus_type == CACTUS_TYPES + 1)
    {
      obstacle_type = OBSTACLE_STONE;
      obstacle_erase = 1;
    }
    else if (cactus_type > CACTUS_TYPES + 1)
    {
      obstacle_type = OBSTACLE_CACTUS;
      obstacle_erase = 1;
      cactus_type = 0;
    }
  }

  // handle bird frame animation
  if ((obstacle_type == OBSTACLE_BIRD) && (vblank_counter % 128))
  {
    clk0++;
    if (clk0 % 32 == 0) bird_frame = 1 - bird_frame;
  }

  // handle stone animation and position
  if ((obstacle_type == OBSTACLE_STONE) && (vblank_counter % 128))
  {
    clk0++;
    if (clk0 % 16 == 0)
    {
      stone_frame++;
      if (stone_frame >= STONE_FRAMES) stone_frame = 0;
      if (stone_jump_direction == 1) stone_jump_height++;
      else stone_jump_height--;
      if (stone_jump_height > 5) stone_jump_direction = 2;
      if ((stone_jump_height == 0) || (stone_jump_height > 6)) stone_jump_direction = 1;
      obstacle_erase = 1;
    }
  }

  // handle non-obstacle ground artifact position 
  small_bump_x_pos -= 3;
  large_bump_x_pos -= 4;
  stick_x_pos -= 5;

  // handle player frame animation
  if (vblank_counter % 4 == 0) {
    player_sprite_frame++;
    if (player_sprite_frame>=DINO_FRAMES) {
      if (vblank_counter % 128 == 0) 
      {
        player_sprite_frame = 0;
      }
      else
      {
         player_sprite_frame = 1;
      } 
    }
  }

  // handle player movement
  if (moving_type == MOVING_UP)
  {
    if (vblank_counter % 8 == 0)
    {
      player_y_position-=(6-player_jump)*(5-player_jump);
      player_jump++;
      if (player_jump > 5) moving_type = MOVING_DOWN;
      return;
    }
  }
  
  if (moving_type == MOVING_DOWN)
  {
    if (vblank_counter % 8 == 0)
    {
      player_y_position+=(6-player_jump)*(5-player_jump);
      if (player_jump == 0) moving_type = MOVING_NONE;
      if (player_jump > 0) player_jump--;
    }
    if (player_y_position > DINO_START_Y)
    {
      player_y_position = DINO_START_Y;
      moving_type = MOVING_NONE;
    }
    return;
  }
}

void draw_sprite(void)
{
  if ((vblank_counter % 4 == 0) && (target_score > score))
  {
    score++;
    print_score();
  }

  if (target_highscore > highscore)
  {
    highscore++;
    print_highscore();
  }

  if (is_game_over > 0)
  {
    if (is_game_over == 1) string_index_to_mem(GAME_OVER_STR, SCREEN_MEM + 248);
    else if (is_game_over == 3) string_index_to_mem(PRESS_START_STR, SCREEN_MEM + 248);
    else if (is_game_over == 5) string_index_to_mem(FIRE_OR_START_STR, SCREEN_MEM + 248);
    else if (is_game_over == 7) string_index_to_mem(AUTHOR_STR, SCREEN_MEM + 248);
    else if (is_game_over == 9) string_index_to_mem(GAME_TITLE_STR, SCREEN_MEM + 248);
    return;
  }

  print_time();
  
  if ((obstacle_erase == 2) && (obstacle_type == OBSTACLE_CACTUS)) update_cactus_type();
  if (obstacle_erase == 2) obstacle_erase = 0;
  if (obstacle_type == OBSTACLE_BIRD) update_bird_animation();
  if (obstacle_type == OBSTACLE_STONE) update_stone_animation();
  if (moving_type != MOVING_NONE) 
  {
    memset((void*)(PMG_MEM+0x200+player_y_position-DINO_HEIGHT), 0, DINO_HEIGHT);
    memset((void*)(PMG_MEM+0x280+player_y_position-DINO_HEIGHT), 0, DINO_HEIGHT);
    memset((void*)(PMG_MEM+0x300+player_y_position-DINO_HEIGHT), 0, DINO_HEIGHT);
    memset((void*)(PMG_MEM+0x200+player_y_position+DINO_HEIGHT), 0, DINO_HEIGHT);
    memset((void*)(PMG_MEM+0x280+player_y_position+DINO_HEIGHT), 0, DINO_HEIGHT);
    memset((void*)(PMG_MEM+0x300+player_y_position+DINO_HEIGHT), 0, DINO_HEIGHT);
  }
  update_dino();
}


void check_collision(void)
{
  collision = GTIA_READ.p2pl || GTIA_READ.p1pl || GTIA_READ.p0pl;
  if ((collision != 0) && (is_game_over == 0))
  {
    is_game_over = 1;
  }
}

void handle_input(void)
{
  // READ START BUTTON
  start_button = CONSOL_START(GTIA_READ.consol);
  
  // READ STICK TRIGGER PORT 1 (strig(0))
  strig0 = GTIA_READ.trig0;

  // READ JOYSTICK PORT 1 (stick(0))
  st = (PIA.porta & 0x0f);
    st = st ^ 0x0f;

  // ASSIGN VALUE TO moving_type
  if (st == JOY_BIT_RIGHT)
  {
    moving_type = MOVING_RIGHT;
    return;
  }
  
  if (player_jump > 0) return;

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
    player_jump = 1;
    return;
  }
  
  if (st == JOY_BIT_DOWN)
  {
    if (player_y_position == DINO_START_Y) moving_type = MOVING_DUCK;
    else moving_type = MOVING_DOWN;
  }

}

void dli_routine(void)
{
  asm("pha");
  asm("txa");
  asm("pha");
  asm("tya");
  asm("pha");


  ANTIC.wsync = 1;

  GTIA_WRITE.colbk = 0x80;
  ANTIC.wsync = 1;
  // UPDATE MISSILE WITH CLOUD POSITION AND COLOR
  GTIA_WRITE.colpf3 = 0xFF;
  GTIA_WRITE.hposm0 = cloud_x_pos + 16;
  GTIA_WRITE.hposm1 = cloud_x_pos + 14;
  GTIA_WRITE.hposm2 = cloud_x_pos + 12;
  GTIA_WRITE.hposm3 = cloud_x_pos + 10;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  // UPDATE PLAYER 3 WITH TOP BIRD POSITION AND COLOR
  GTIA_WRITE.colpm3 = 0x00;
  GTIA_WRITE.hposp3 = cactus_x_pos;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0x82;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0x84;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0x86;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0x88;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0x8A;
  ANTIC.wsync = 1;

  GTIA_WRITE.colpf0 = 0x55;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;

  GTIA_WRITE.colpf0 = 0x40 + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x42 + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x44 + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x4C + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x4F + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x50 + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x52 + vblank_counter;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpf0 = 0x54 + vblank_counter;
  ANTIC.wsync = 1;

  OS.vdslst = &dli_routine1;

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
  asm("txa");
  asm("pha");
  asm("tya");
  asm("pha");


  ANTIC.wsync = 1;
  
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  // UPDATE PLAYER 3 WITH CACTUS / SECOND_BIRD / STONE POSITION AND COLOR
  GTIA_WRITE.hposp3 = cactus_x_pos+6;

  if (obstacle_type == OBSTACLE_BIRD) GTIA_WRITE.colpm3 = 0x00;
  else if (obstacle_type == OBSTACLE_STONE) GTIA_WRITE.colpm3 = 0xE0;
  else GTIA_WRITE.colpm3 = 0xC4;

  ANTIC.wsync = 1;
 
  //ANTIC.chbase = CHARSET_MEM >> 8; // MSB / 256
  
  GTIA_WRITE.colpf0 = 0x55;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;

  GTIA_WRITE.colpf3 = 0x4A;
  ANTIC.wsync = 1;
  OS.vdslst = &dli_routine2;

asm("pla");
  asm("tay");
  asm("pla");
  asm("tax");
  asm("pla");

  asm("rti");
}

void dli_routine2(void)
{
  asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0xF0;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0xF2;
  ANTIC.wsync = 1;
  GTIA_WRITE.hposp3 = small_bump_x_pos;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpm3 = 0xF6;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0xF4;
  ANTIC.wsync = 1;
  GTIA_WRITE.hposp3 = large_bump_x_pos;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpm3 = 0xF0;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  ANTIC.wsync = 1;
  GTIA_WRITE.colbk = 0xF6;
  ANTIC.wsync = 1;
  GTIA_WRITE.hposp3 = stick_x_pos;
  ANTIC.wsync = 1;
  GTIA_WRITE.colpm3 = 0xF2;
  ANTIC.wsync = 1;

  OS.vdslst = &dli_routine;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}


void vbi_copy_shadow_to_hw_registers(void)
{
    ANTIC.dmactl = OS.sdmctl;

    ANTIC.dlistl = OS.sdlstl;
    ANTIC.dlisth = OS.sdlsth;

    ANTIC.chbase = OS.chbas;

    GTIA_WRITE.prior = OS.gprior;

    GTIA_WRITE.colpf0 = OS.color0;
    GTIA_WRITE.colpf1 = OS.color1;
    GTIA_WRITE.colpf2 = OS.color2;
    GTIA_WRITE.colpf3 = OS.color3;
    GTIA_WRITE.colbk  = OS.color4;
    
    GTIA_WRITE.colpm0 = OS.pcolr0;
    GTIA_WRITE.colpm1 = OS.pcolr1;
    GTIA_WRITE.colpm2 = OS.pcolr2;
    GTIA_WRITE.colpm3 = OS.pcolr3;  
}

void vbi_routine(void)
{
  vbi_copy_shadow_to_hw_registers();

  OS.vdslst = &dli_routine;

  vblank_occured = 1;

  // SFX Player jump, Score up

  if ((target_score > score + 8) && (is_game_over == 0)) 
  {
    POKEY_WRITE.audf3 = 121 + target_score - score;
    POKEY_WRITE.audc3 = AUDC_POLYS_NONE | 0x0A;
  } else
  {
    POKEY_WRITE.audf3 = 0;
    POKEY_WRITE.audc3 = AUDC_POLYS_NONE | 0x00;
  }

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

void update_dino(void)
{
  // draw dino frame on sprites 0-2 and missiles
 // memcpy((void*)(PMG_MEM+0x0180+player_y_position), &DINO_COLOR[player_sprite_frame], DINO_HEIGHT);
  if (moving_type != MOVING_DUCK)
  {
    memcpy((void*)(PMG_MEM+0x0200+player_y_position), &DINO_BACK[player_sprite_frame], DINO_HEIGHT);
    memcpy((void*)(PMG_MEM+0x0280+player_y_position), &DINO_MID[player_sprite_frame], DINO_HEIGHT);
    memcpy((void*)(PMG_MEM+0x0300+player_y_position), &DINO_FRONT[player_sprite_frame], DINO_HEIGHT);
  }
  else 
  {
    memcpy((void*)(PMG_MEM+0x0200+player_y_position), &DINO_DUCK_BACK[player_sprite_frame], DINO_HEIGHT);
    memcpy((void*)(PMG_MEM+0x0280+player_y_position), &DINO_DUCK_MID[player_sprite_frame], DINO_HEIGHT);
    memcpy((void*)(PMG_MEM+0x0300+player_y_position), &DINO_DUCK_FRONT[player_sprite_frame], DINO_HEIGHT);
  }
}

void update_cactus_type(void)
{
  // draw cactus on missile
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+CACTUS_Y_POS), &CACTUS_DATA[cactus_type], CACTUS_HEIGHT);
}

void update_bird_animation(void)
{
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+BIRD_Y_POS), &BIRD_DATA[bird_frame], BIRD_HEIGHT);
  memcpy((void*)(PMG_MEM+/*0x300*/0x390+BIRD_Y_POS), &BIRD_DATA[bird_frame], BIRD_HEIGHT);
}

void update_stone_animation(void)
{
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+stone_y_pos-stone_jump_height*stone_jump_height), &STONE_DATA[stone_frame], STONE_HEIGHT);
}

void update_player_objects(void)
{
  memcpy((void*)(PMG_MEM+/*0x300*/0x180+CLOUD_Y_POS), &CLOUD_DATA, CLOUD_HEIGHT);
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+SMALL_BUMP_Y_POS), &SMALL_BUMP, SMALL_BUMP_HEIGHT);
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+LARGE_BUMP_Y_POS), &LARGE_BUMP, LARGE_BUMP_HEIGHT);
  memcpy((void*)(PMG_MEM+/*0x300*/0x380+STICK_Y_POS), &STICK, STICK_HEIGHT);
}
void setup_pmg(void)
{ 
  // tell ANTIC what is the memory address of the PMG memory
    ANTIC.pmbase = PMG_MEM >> 8;

    // clear PMG memory
  bzero((void*)(PMG_MEM+/*0x300*/0x180),0x500);
  update_player_objects();

    // tell ANTIC do we work with single line or double line resolution 
    OS.sdmctl = 0x2E;//0x3E; // single line

  // tell GTIA what type of PMG do we want?
  GTIA_WRITE.gractl = GRACTL_MISSLES + GRACTL_PLAYERS;

    // set the priority register (order of objects appear on screen)
  // PRIOR_P03_PF03 | PRIOR_5TH_PLAYER | PRIOR_OVERLAP_3RD_COLOR
    OS.gprior = PRIOR_P03_PF03 | PRIOR_OVERLAP_3RD_COLOR | PRIOR_5TH_PLAYER;

    // player colors , set shadows
    OS.pcolr0 = 0xB6;
    OS.pcolr1 = 0xB6;
    OS.pcolr2 = 0xB6;
    OS.pcolr3 = 0xCC;

    // missiles combining the 5th player get their color from location 711
    // which is playfield 3
  OS.color3 = 0x26; 

    // player size (normal, double, quad)
    GTIA_WRITE.sizem  = PMG_SIZE_DOUBLE;  
    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep2 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep3 = PMG_SIZE_DOUBLE;

    // player horizontal position
    GTIA_WRITE.hposm0 = DINO_X_POS + 34;
    GTIA_WRITE.hposm1 = DINO_X_POS + 32;
    GTIA_WRITE.hposm2 = DINO_X_POS + 28;
    GTIA_WRITE.hposm3 = DINO_X_POS + 26;


    GTIA_WRITE.hposp0 = DINO_X_POS;
    GTIA_WRITE.hposp1 = DINO_X_POS + 16;
    GTIA_WRITE.hposp2 = DINO_X_POS + 32;
    GTIA_WRITE.hposp3 = 0x00;

    player_y_position = DINO_START_Y;

    memset((void*)(PMG_MEM + 0x400 + 0x90),255,8);
}
