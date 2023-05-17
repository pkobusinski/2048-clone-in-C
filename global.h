#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


// program states 

enum State {STATE_QUIT,  STATE_PAUSE, STATE_GAME , STATE_START_SCREEN, STATE_AFTER_GAME};



// game data 
const int SCREEN_WIDTH = 850;
const int SCREEN_HEIGHT =580;
const int TILE_SIZE= 100;
const int OFFSETX =75;
const int OFFSETY =75;
const int SPACING = 10;

int score;

ALLEGRO_FONT* font_tiles1;
ALLEGRO_FONT* font_tiles2;
ALLEGRO_FONT* font_tiles3;
ALLEGRO_FONT* font_small_text;
ALLEGRO_FONT* font_large_text;
ALLEGRO_FONT* font_title_text;

void al_config();
void clear_board(int board[][4]);
void draw_after_screen(bool _did_win);
void draw_score();
void draw_board(int board[][4]);
void spawn_field(int board[][4]);
void redraw_board(int board[][4]);
void move_right(int board[][4]);
void move_left(int board[][4]);
void move_up(int board[][4]);
void move_down(int board[][4]);
bool is_game_over(int board[][4], bool* did_win);

void draw_start_screen(int *current);
void next_choice(int* current, int* active_choice);
void previous_choice(int* current, int active_choice[]);

void draw_after_game_screen(bool _did_win);
void  draw_static_pause_screen();