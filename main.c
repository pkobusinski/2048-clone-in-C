#include "global.h"

int main() {

    srand(time(NULL));

    al_config();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) { fprintf(stderr, "Failed to create display.\n"); return 1; }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    if (!timer) { fprintf(stderr, "Failed to create timer.\n"); return 1; }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) { fprintf(stderr, "Failed to create queue.\n"); return 1; }

    font_tiles1 = al_load_font("font2.ttf", 48, 0);
    font_tiles2 = al_load_font("font2.ttf", 40, 0);
    font_tiles3 = al_load_font("font2.ttf", 35, 0);
    font_small_text = al_load_font("font2.ttf", 25, 0);
    font_large_text = al_load_font("font1.ttf", 55, 0);
    font_title_text = al_load_font("font1.ttf", 125, 0);
    font_title1_text = al_load_font("font1.ttf", 75, 0);

    swish = al_load_sample("slash.mp3");
    music = al_load_sample("music.mp3");
    al_reserve_samples(3);
    songinstance = al_create_sample_instance(music);
    al_set_sample_instance_playmode(songinstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(songinstance, al_get_default_mixer());


    //al_set_window_title(display, "2048");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int board[4][4] = { 0 };
    draw_board(&board);
    

    bool game_over = false;
    bool did_win = false;
    bool music = true;
    bool sound_effects = true;

    int active_choice[3] = { 1,2,3 };
    int current_option = active_choice[0];

    score = 0;
    
    int game_state = STATE_START_SCREEN;

    al_start_timer(timer);

    while (game_state != STATE_QUIT) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        switch (game_state) {
        case STATE_GAME:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                if (music) al_play_sample_instance(songinstance);
                else
                    al_stop_sample_instance(songinstance);

                redraw_board(&board);
                draw_score();

                if (is_game_over(&board, &did_win)) {
                    game_state = STATE_AFTER_GAME;
                    draw_after_game_screen(did_win);
                }
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    if (sound_effects) al_play_sample(swish, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    move_up(&board);
                    break;

                case ALLEGRO_KEY_DOWN:
                    if (sound_effects) al_play_sample(swish, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    move_down(&board);
                    break;

                case ALLEGRO_KEY_LEFT:
                    if (sound_effects) al_play_sample(swish, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    move_left(&board);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (sound_effects) al_play_sample(swish, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    move_right(&board);
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    game_state = STATE_START_SCREEN;
                    break;
                case ALLEGRO_KEY_SPACE:
                    game_state = STATE_PAUSE;
                    draw_static_pause_screen();
                    break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }
            break;

        case STATE_SETTINGS:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                if (music) al_play_sample_instance(songinstance);
                else
                    al_stop_sample_instance(songinstance);

                draw_settings(current_option, music, sound_effects);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_DOWN:
                    next_choice(&current_option, active_choice);
                    break;
                case ALLEGRO_KEY_UP:
                    previous_choice(&current_option, active_choice);
                    break;

                case ALLEGRO_KEY_ENTER:
                    switch (current_option) {
                    case 1:
                        if (music == true)
                            music = false;
                        else if (music == false)
                            music = true;
                        break;
                    case 2:
                        if (sound_effects == true)
                            sound_effects = false;
                        else if (sound_effects == false)
                            sound_effects = true;
                        break;
                    case 3:
                        game_state = STATE_START_SCREEN;
                        break;
                    }
                    break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }


            break;
        case STATE_START_SCREEN:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                if (music) al_play_sample_instance(songinstance);
                else
                    al_stop_sample_instance(songinstance);
                draw_start_screen(current_option);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_DOWN:
                    next_choice(&current_option, active_choice);
                    break;
                case ALLEGRO_KEY_UP:
                    previous_choice(&current_option, active_choice);
                    break;

                case ALLEGRO_KEY_ENTER:
                    switch (current_option) {
                    case 1:
                        game_state = STATE_GAME;
                        clear_board(&board);
                        score = 0;
                        spawn_field(&board);
                        draw_board(&board);
                        break;
                    case 2:
                        game_state = STATE_LEADERBOARD;
                        
                        break;
                    case 3:
                        game_state = STATE_SETTINGS;
                        //draw_settings(current_option);
                        break;
                    }
                    break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }
            break;
        case STATE_AFTER_GAME:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    game_state = STATE_START_SCREEN;
                    break;

      
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }
            break;

        case STATE_LEADERBOARD:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                if (music) al_play_sample_instance(songinstance);
                else
                    al_stop_sample_instance(songinstance);

                draw_leaderboard(current_option);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_ENTER:
                    game_state = STATE_START_SCREEN;
                    break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }

            break;
        case STATE_PAUSE:
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                break;

            case ALLEGRO_EVENT_KEY_DOWN:

                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_SPACE:
                    game_state = STATE_GAME;
                    draw_board(&board);
                    break;
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_state = STATE_QUIT;
                break;
            }
            break;

        }
    }
    // zwolnienie zasobów Allegro
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(font_tiles1);
    al_destroy_font(font_tiles2);
    al_destroy_font(font_tiles3);
    al_destroy_sample(swish);

    return 0;
}
void al_config()
{
    // właściwości allegro (config)
    if (!al_init()) { fprintf(stderr, "Failed to initialize Allegro.\n"); return 1; }
    if (!al_install_keyboard()) { fprintf(stderr, "Failed to initialize keyboard.\n"); return 1; }
    if (!al_init_primitives_addon()) { fprintf(stderr, "Failed to initialize primitives addon.\n"); return 1; }
    if (!al_init_font_addon()) { fprintf(stderr, "Failed to initialize font addon.\n"); return 1; }
    if (!al_init_ttf_addon()) { fprintf(stderr, "Failed to initialize font ttf addon.\n"); return 1; }
    if (!al_install_audio()) { fprintf(stderr, "Failed to initialize audio addon.\n"); return 1; }
    al_init_acodec_addon();
}



bool is_game_over(int board[][4], bool* did_win)
{
    bool is_full = true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 2048) {
                *did_win = true;
                return true;
            }
            else if (board[i][j] == 0)
                is_full = false;
            else if (j > 0 && board[i][j] == board[i][j - 1])
                return false;
            else if (j < 3 && board[i][j] == board[i][j + 1])
                return false;
            else if (i > 0 && board[i][j] == board[i - 1][j])
                return false;
            else if (i < 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    }
    return is_full;
}

void draw_score()
{
    char score_value[20];
    sprintf_s(score_value, sizeof(score_value), "%d", score, score_value);

    al_draw_filled_rounded_rectangle(600,OFFSETY-10,800,165,5,5, al_map_rgb(185, 173, 163));
    al_draw_text(font_small_text, al_map_rgb(238, 229, 219), 700, OFFSETY-10, ALLEGRO_ALIGN_CENTER, "SCORE");
    al_draw_text(font_tiles1, al_map_rgb(255,255,255), 700, OFFSETY + 20, ALLEGRO_ALIGN_CENTER, score_value);

    al_flip_display();
}

void draw_board(int board[][4]) {
    // rysowanie tła
    al_clear_to_color(al_map_rgb(251, 248, 246));
    //rysowanie tla planszy
    al_draw_filled_rectangle(OFFSETX-10, OFFSETY-10, 4.7*(TILE_SIZE + SPACING), 4.7*(TILE_SIZE + SPACING), al_map_rgb(185, 173, 163));
    // rysowanie pól
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = j * (TILE_SIZE + SPACING) + (TILE_SIZE + 1) / 2 + OFFSETX;
            int y = i * (TILE_SIZE + SPACING) + (TILE_SIZE + 1) / 2 + OFFSETY;

            al_draw_filled_rounded_rectangle(x - TILE_SIZE / 2, y - TILE_SIZE / 2, x + TILE_SIZE / 2, y + TILE_SIZE / 2, 10, 10, al_map_rgb(205, 192, 181, 255));

        }
    }

    // aktualizacja ekranu
    al_flip_display();
}

void redraw_board(int board[][4]) {

    char tile_label[10];
    ALLEGRO_FONT* tile_font;
    ALLEGRO_COLOR tile_color;
    ALLEGRO_COLOR font_color;
    // rysowanie kafli
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = j * (TILE_SIZE + SPACING) + (TILE_SIZE + 1) / 2 + OFFSETX;
            int y = i * (TILE_SIZE + SPACING) + (TILE_SIZE + 1) / 2 + OFFSETY;

            

            if (board[i][j] != 0) {
                //_itoa_s(abs(board[i][j]), tile_label, 10);
                sprintf_s(tile_label, sizeof(tile_label), "%d", abs(board[i][j]), tile_label);
                tile_font = (abs(board[i][j]) < 100) ? font_tiles1 : (abs(board[i][j]) < 1000) ? font_tiles2 : font_tiles3;
                font_color = (abs(board[i][j]) < 8) ? al_map_rgb(118, 111, 101) : al_map_rgb(255, 255, 255);
                
                switch (abs(board[i][j]))
                {
                case 2:
                    tile_color = al_map_rgb(238, 229, 219);
                    break;
                case 4: 
                    tile_color = al_map_rgb(238, 225, 201);
                    break;
                case 8:
                    tile_color = al_map_rgb(242, 178, 123);
                    break;
                case 16:
                    tile_color = al_map_rgb(247, 150, 101);
                    break;
                case 32: 
                    tile_color = al_map_rgb(247, 125, 94);
                    break;
                case 64:
                    tile_color = al_map_rgb(246, 94, 59);
                    break;
                case 128:
                    tile_color = al_map_rgb(236, 208, 114);
                    break;
                case 256: 
                    tile_color = al_map_rgb(236, 204, 99);
                    break;
                case 512: 
                    tile_color = al_map_rgb(237, 201, 81);
                    break;
                case 1024: 
                    tile_color = al_map_rgb(236, 197, 62);
                    break;
                case 2048: 
                    tile_color = al_map_rgb(236, 195, 45);
                    break;
                }

                tile_color = board[i][j] < 0 ? al_map_rgb(0, 255, 0) : tile_color;
                
                al_draw_filled_rounded_rectangle(x - TILE_SIZE / 2, y - TILE_SIZE / 2, x + TILE_SIZE / 2, y + TILE_SIZE / 2, 10, 10, tile_color);
                al_draw_text(tile_font, font_color, x, y - TILE_SIZE / 2 + 15, ALLEGRO_ALIGN_CENTER, tile_label);
            }
            else
                al_draw_filled_rounded_rectangle(x - TILE_SIZE / 2, y - TILE_SIZE / 2, x + TILE_SIZE / 2, y + TILE_SIZE / 2, 10, 10, al_map_rgb(205, 192, 181, 255));
        }
    }

    // aktualizacja ekranu
    al_flip_display();
}

void spawn_field(int board[][4])
{
    /*printf("spawn \n");
    fflush(stdout);*/
    int x, y, z;
    do {
        x = rand() % 4;
        y = rand() % 4;
    } while (board[x][y] != 0);
    
    board[x][y] = (rand() % 5 == 1) ? 4 : 2;

    //board[0][0] = 2;
    //board[0][1] = 4;
    //board[0][2] = 8;
    //board[0][3] = 16;
    //board[1][0] = 32;
    //board[1][1] = 64;
    //board[1][2] = 128;
    //board[1][3] = 256;
    //board[2][0] = 512;
    //board[2][1] = 1024;
    //board[2][2] = 2048;

}

void move_right(int board[][4])
{
    bool moved;
    bool add_new = false;
    do {
        moved = false;
        for (int i = 0; i < 4; i++) {
            for (int j = 2; j >= 0; j--) {
                if (board[i][j] != 0) {
                    int k = j + 1;
                    if (board[i][k] == 0) {
                        board[i][k] = board[i][k - 1];
                        board[i][k - 1] = 0;
                        moved = true;
                        add_new = true;

                    }
                    else if (board[i][k] > 0 && board[i][k] == board[i][k - 1]) {
                        board[i][k] *= -2;
                        score += abs(board[i][k]);
                        board[i][k - 1] = 0;
                        moved = true;
                        add_new = true;

                    }


                }
            }
        }
        redraw_board( board);
        Sleep(50);
    } while (moved);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = abs(board[i][j]);
        }
    redraw_board(board);
    if (add_new) {
        spawn_field(board);
    }
}

void move_left(int board[][4])
{
    bool moved;
    bool add_new = false;
    do {
        moved = false;
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                if (board[i][j] != 0) {
                    int k = j - 1;
                    if (board[i][k] == 0) {
                        board[i][k] = board[i][k + 1];
                        board[i][k + 1] = 0;
                        moved = true;
                        add_new = true;

                    }
                    else if (board[i][k] >0 && board[i][k] == board[i][k + 1]) {
                        board[i][k] *= -2;
                        score += abs(board[i][k]);
                        board[i][k + 1] = 0;
                        moved = true;
                        add_new = true;

                    }


                }
            }
        }
        redraw_board(board);
        Sleep(50);
    } while (moved);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = abs(board[i][j]);
        }
    redraw_board(board);
    if (add_new) {
        spawn_field(board);
    }
}

void move_up(int board[][4])
{
    bool moved;
    bool add_new = false;
    do {
        moved = false;
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] != 0) {
                    int k = i - 1;
                    if (board[k][j] == 0) {
                        board[k][j] = board[k + 1][j];
                        board[k + 1][j] = 0;
                        moved = true;
                        add_new = true;

                    }
                    else if (board[k][j] > 0 && board[k][j] == board[k + 1][j]) {
                        board[k][j] *= -2;
                        score += abs(board[k][j]);
                        board[k + 1][j] = 0;
                        moved = true;
                        add_new = true;

                    }


                }
            }
        }
        redraw_board(board);
        Sleep(50);
    } while (moved);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = abs(board[i][j]);
        }
    redraw_board(board);
    if (add_new) {
        spawn_field(board);
    }
}

void move_down(int board[][4])
{
    bool moved;
    bool add_new = false;
    do {
        moved = false;
        for (int i = 2; i >= 0; i--) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] != 0) {
                    int k = i + 1;
                    if (board[k][j] == 0) {
                        board[k][j] = board[k - 1][j];
                        board[k - 1][j] = 0;
                        moved = true;
                        add_new = true;

                    }
                    else if (board[k][j] >0 && board[k][j] == board[k - 1][j]) {
                        board[k][j] *= -2;
                        score += abs(board[k][j]);
                        board[k - 1][j] = 0;
                        moved = true;
                        add_new = true;

                    }


                }
            }
        }
        redraw_board(board);
        Sleep(50);
    } while (moved);

    for(int i = 0; i<4; i++)
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = abs(board[i][j]);
        }
    redraw_board(board);
    if (add_new) {
        spawn_field(board);
    }
}

void draw_start_screen(int *current)
{

    al_draw_filled_rounded_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,10,10, al_map_rgb(251, 248, 246));
    al_draw_text(font_title_text, al_map_rgb(118, 111, 101), 425, -20, ALLEGRO_ALIGN_CENTER, "2048");
    al_draw_filled_rounded_rectangle(250, 150, 600, 250,10,10, (current == 1) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles1, al_map_rgb(255,255,255), 425, 165, ALLEGRO_ALIGN_CENTER, "PLAY");
    al_draw_filled_rounded_rectangle(250, 275, 600, 375,10,10, (current == 2) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles1, al_map_rgb(255, 255, 255), 425, 290, ALLEGRO_ALIGN_CENTER, "INSTRUCTIONS");
    al_draw_filled_rounded_rectangle(250, 400, 600, 500,10,10, (current == 3) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles1, al_map_rgb(255, 255, 255), 425, 415, ALLEGRO_ALIGN_CENTER, "OPTIONS");

    al_flip_display();
}

void previous_choice(int* current, int active_choice[])
{
    if (*current == 1)
        *current = active_choice[sizeof(active_choice) / sizeof(active_choice[0])];
    else
        *current = active_choice[*current-2];
}

void next_choice(int *current, int *active_choice)
{
    if (*current == sizeof(active_choice) / sizeof(active_choice[0])+1)
        *current = active_choice[0];
    else
        *current = active_choice[*current];
}

void draw_after_game_screen(bool _did_win)
{
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    if (_did_win == true) {
        al_draw_filled_rectangle(OFFSETX - 10, OFFSETY - 10, 4.7 * (TILE_SIZE + SPACING), 4.7 * (TILE_SIZE + SPACING), al_map_rgba(213,	137, 16, 128));
        al_draw_text(font_large_text, al_map_rgb(255,255,255), OFFSETX + 215, OFFSETY + 150, ALLEGRO_ALIGN_CENTER, "YOU WIN");
    }
    else {
        al_draw_filled_rectangle(OFFSETX - 10, OFFSETY - 10, 4.7 * (TILE_SIZE + SPACING), 4.7 * (TILE_SIZE + SPACING), al_map_rgba(99, 95, 87, 128));
        al_draw_text(font_large_text, al_map_rgb(118, 111, 101), OFFSETX + 215, OFFSETY + 150, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    }

    // tutaj może być okienko na wpisanie imienia do leader boardu 

    al_flip_display();
}

void clear_board(int board[][4]) {
    for(int i =0; i<4; i++)
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = 0;
        }
}

void draw_static_pause_screen() {
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    al_draw_filled_rounded_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 10, al_map_rgba(185, 173, 163, 190));
    al_draw_text(font_large_text, al_map_rgb(118, 111, 101), 425, 200, ALLEGRO_ALIGN_CENTER, "GAME PAUSED");
    al_flip_display();
}

void draw_leaderboard(int* current) {

    al_draw_filled_rounded_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 10, al_map_rgb(251, 248, 246));
    al_draw_text(font_title1_text, al_map_rgb(118, 111, 101), 425, 25, ALLEGRO_ALIGN_CENTER, "INSTRUCTIONS");
    al_draw_text(font_tiles1, al_map_rgb(118, 111, 101), 425, 170, ALLEGRO_ALIGN_CENTER, "Use your arrow keys to move the");
    al_draw_text(font_tiles1, al_map_rgb(118, 111, 101), 425, 235, ALLEGRO_ALIGN_CENTER, "tiles. Tiles with the same number");
    al_draw_text(font_tiles1, al_map_rgb(118, 111, 101), 425, 300, ALLEGRO_ALIGN_CENTER, "merge into one when they touch.");
    al_draw_text(font_tiles1, al_map_rgb(118, 111, 101), 425, 365, ALLEGRO_ALIGN_CENTER, "Add them up to reach 2048!");
    al_draw_filled_rounded_rectangle(250, 490, 600, 540, 10, 10, al_map_rgb(118, 111, 101));
    al_draw_text(font_tiles1, al_map_rgb(255, 255, 255), 425, 480, ALLEGRO_ALIGN_CENTER, "EXIT");

    al_flip_display();
}

void draw_settings(int *current, bool *music, bool *sound_effects) {

    al_draw_filled_rounded_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 10, al_map_rgb(251, 248, 246));
    al_draw_text(font_title1_text, al_map_rgb(118, 111, 101), 425, 30, ALLEGRO_ALIGN_CENTER, "OPTIONS");
    al_draw_filled_rounded_rectangle(250, 150, 600, 250, 10, 10, (current == 1) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles3, (music == true) ? al_map_rgb(0, 255, 0): al_map_rgb(255, 0, 0), 425, 175, ALLEGRO_ALIGN_CENTER, "MUSIC");
    al_draw_filled_rounded_rectangle(250, 275, 600, 375, 10, 10, (current == 2) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles3, (sound_effects == true) ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 0, 0), 425, 300, ALLEGRO_ALIGN_CENTER, "SOUND EFFECTS");
    al_draw_filled_rounded_rectangle(250, 400, 600, 500, 10, 10, (current == 3) ? al_map_rgb(118, 111, 101) : al_map_rgb(185, 173, 163));
    al_draw_text(font_tiles3, al_map_rgb(255, 255, 255), 425, 425, ALLEGRO_ALIGN_CENTER, "EXIT");

    al_flip_display();
}

