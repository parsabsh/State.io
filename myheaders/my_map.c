#include "my_map.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "my_text.h"
#include "my_colors.h"
#include "my_audio.h"

// check if the element is in the array with length i
int is_in(int element, int array[], int i){
    for(int j=0; j<i; j++){
        if(element == array[j])
            return 1;
    }
    return 0;
}

// chooses n random numbers in the interval [start, end] and store them in the array
void random_number_range(int n, int start, int end, int array[]){
    srand(time(0));
    for(int i=0; i<n; i++){
        int random_number = (rand()%(end-start+1)) + start;
        if(!is_in(random_number, array, i)){
            array[i] = random_number;
        }else{
            i--;
        }
    }
}

// returns the number of digits of a number
int digits_of(int a){
    if(a == 0) return 1;
    int result = 0;
    while(a > 0){
        a /= 10;
        result++;
    }
    return result;
}

int default_x_coordinates[26] = {85, 230, 375, 525, 680, 827, 974,
                               150, 300, 450, 600, 780, 930,
                               85, 230, 380, 531, 690, 850, 1000,
                               160, 310, 460, 618, 765, 915};

int default_y_coordinates[26] = {100, 120, 114, 115, 120, 100, 103,
                               240, 250, 245, 241, 242, 240,
                               380, 380, 370, 367, 355, 370, 380,
                               515, 500, 490, 480, 490, 500};

castle* generate_random_map(int number_of_players, int number_of_castles){
    // choose the castles
    castle* castles = malloc(sizeof(castle) * number_of_castles);
    int selected_castles_index[number_of_castles];
    srand(time(0));
    random_number_range(number_of_castles, 0, 25, selected_castles_index);
    for(int i=0; i < number_of_castles; i++){
        castles[i].center_x = default_x_coordinates[selected_castles_index[i]];
        castles[i].center_y = default_y_coordinates[selected_castles_index[i]];
        castles[i].player = -1;
        castles[i].soldiers = START_NUMBER_OF_SOLDIERS;
        castles[i].soldiers_with_destination = 0;
        castles[i].radius = (rand()%31) + 40;
        castles[i].color = 0x88B9A8B0;
        castles[i].center_color = 0x88B3899B;
    }

    // choose the players' castles
    for(int i=0; i < number_of_players; i++){
        castles[i].player = i;
        switch(i){
            case 0:
                castles[i].color = 0xFFEABA8A;
                castles[i].center_color = 0xFF4B2600;
                break;
            case 1:
                castles[i].color = 0xFFB7C900;
                castles[i].center_color = 0xFF485400;
                break;
            case 2:
                castles[i].color = 0xFFDEFAFF;
                castles[i].center_color = 0xFF55789C;
                break;
            case 3:
                castles[i].color = 0xFFF5BBE6;
                castles[i].center_color = 0xFFBD85AE;
                break;
            default:
                break;
        }
    }
    return castles;
}

void render_map(castle* castles, int number_of_castles, SDL_Renderer* renderer){
    for(int i=0; i < number_of_castles; i++){

        filledCircleColor(renderer, castles[i].center_x, castles[i].center_y, castles[i].radius, castles[i].color);
        filledCircleColor(renderer, castles[i].center_x, castles[i].center_y, CENTER_RADIUS, castles[i].center_color);

        char number_of_soldiers[4];
        itoa(castles[i].soldiers, number_of_soldiers, 10);

        SDL_Rect rectangle;
        rectangle.y = castles[i].center_y - 10;
        rectangle.h = 20;
        rectangle.w = 10 * digits_of(castles[i].soldiers);
        rectangle.x = castles[i].center_x - (rectangle.w / 2);

        print_text(comic_font, number_of_soldiers, CASTLE_OUTLINE_COLOR, renderer, &rectangle);
    }
}

void increment_soldiers(int time, castle* castles, int fps, int rate, int number_of_castles){
    if((time % (fps/rate)) == 0){
        for(int i=0; i<number_of_castles; i++){
            if(castles[i].player != -1 && castles[i].soldiers < MAXIMUM_NUMBER_OF_SOLDIERS){
                castles[i].soldiers++;
            }
        }
    }
}

int click_in_rect(SDL_Event event, SDL_Rect rect){
    if(event.button.x > rect.x && event.button.x < rect.x+rect.w
    && event.button.y > rect.y && event.button.y < rect.y+rect.h){
        return 1;
    }else{
        return 0;
    }
}

void sort(int points_of_players[4][2]){
    for (int pass = 1; pass < 4; pass++) {
        for (int i = 0; i < 3; i++){
            if (points_of_players[i][1] < points_of_players[i + 1][1]) {
                int hold[2];
                hold[0] = points_of_players[i][0];
                hold[1] = points_of_players[i][1];
                points_of_players[i][0] = points_of_players[i + 1][0];
                points_of_players[i][1] = points_of_players[i + 1][1];
                points_of_players[i + 1][0] = hold[0];
                points_of_players[i + 1][1] = hold[1];
            }
        }
    }
}

void unsort(int points_of_players[4][2]){
    for (int pass = 0; pass < 3; pass++){
        for (int i = 0; i < 3; i++){
            if (points_of_players[i][0] > points_of_players[i + 1][0]) {
                int hold[2];
                hold[0] = points_of_players[i][0];
                hold[1] = points_of_players[i][1];
                points_of_players[i][0] = points_of_players[i + 1][0];
                points_of_players[i][1] = points_of_players[i + 1][1];
                points_of_players[i + 1][0] = hold[0];
                points_of_players[i + 1][1] = hold[1];
            }
        }
    }
}

void show_scoreboard(int points_of_players[4][2], SDL_Renderer* renderer, char username[]){
    sort(points_of_players);
    SDL_Surface* surface = SDL_LoadBMP("images/scoreboard_background.bmp");
    SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = NULL;
    SDL_Rect quit_rect = {903, 548, 177, 92};
    char first_text[20];
    char second_text[20];
    char third_text[20];
    char fourth_text[20];
    SDL_Rect first_place, second_place, third_place, fourth_place;
    first_place.y = 176;
    second_place.y = 276;
    third_place.y = 376;
    fourth_place.y = 476;
    first_place.x = 250;
    second_place.x = 250;
    third_place.x = 250;
    fourth_place.x = 250;
    first_place.h = 40;
    second_place.h = 40;
    third_place.h = 40;
    fourth_place.h = 40;
    switch(points_of_players[0][0]){
        case 0:
            sprintf(first_text, "%s (blue castles) : %d", username, points_of_players[0][1]);
            first_place.w = strlen(first_text)*17;
            break;
        case 1:
            sprintf(first_text, "Player 1 (green castles) : %d", points_of_players[0][1]);
            first_place.w = strlen(first_text)*17;
            break;
        case 2:
            sprintf(first_text, "Player 2 (cream castles) : %d", points_of_players[0][1]);
            first_place.w = strlen(first_text)*17;
            break;
        case 3:
            sprintf(first_text, "Player 3 (pink castles) : %d", points_of_players[0][1]);
            first_place.w = strlen(first_text)*17;
            break;
    }
    switch(points_of_players[1][0]){
        case 0:
            sprintf(second_text, "%s (blue castles) : %d", username, points_of_players[1][1]);
            first_place.w = strlen(second_text)*17;
            break;
        case 1:
            sprintf(second_text, "Player 1 (green castles) : %d", points_of_players[1][1]);
            second_place.w = strlen(second_text)*17;
            break;
        case 2:
            sprintf(second_text, "Player 2 (cream castles) : %d", points_of_players[1][1]);
            second_place.w = strlen(second_text)*17;
            break;
        case 3:
            sprintf(second_text, "Player 3 (pink castles) : %d", points_of_players[1][1]);
            second_place.w = strlen(second_text)*17;
            break;
    }
    switch(points_of_players[2][0]){
        case 0:
            sprintf(third_text, "%s (blue castles) : %d", username, points_of_players[2][1]);
            third_place.w = strlen(third_text)*17;
            break;
        case 1:
            sprintf(third_text, "Player 1 (green castles) : %d", points_of_players[2][1]);
            third_place.w = strlen(third_text)*17;
            break;
        case 2:
            sprintf(third_text, "Player 2 (cream castles) : %d", points_of_players[2][1]);
            third_place.w = strlen(third_text)*17;
            break;
        case 3:
            sprintf(third_text, "Player 3 (pink castles) : %d", points_of_players[2][1]);
            third_place.w = strlen(third_text)*17;
            break;
    }
    switch(points_of_players[3][0]){
        case 0:
            sprintf(fourth_text, "%s (blue castles) : %d", username, points_of_players[3][1]);
            fourth_place.w = strlen(fourth_text)*17;
            break;
        case 1:
            sprintf(fourth_text, "Player 1 (green castles) : %d", points_of_players[3][1]);
            fourth_place.w = strlen(fourth_text)*17;
            break;
        case 2:
            sprintf(fourth_text, "Player 2 (cream castles) : %d", points_of_players[3][1]);
            fourth_place.w = strlen(fourth_text)*17;
            break;
        case 3:
            sprintf(fourth_text, "Player 3 (pink castles) : %d", points_of_players[3][1]);
            fourth_place.w = strlen(fourth_text)*17;
            break;
    }
    SDL_Event event;
    SDL_bool is_open = SDL_TRUE;
    while(is_open){
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                is_open = SDL_FALSE;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (click_in_rect(event, quit_rect)) {
                    is_open = SDL_FALSE;
                    break;
                }
            }
        }
        check_music_finished();
        SDL_RenderCopy(renderer, background, NULL, NULL);
        print_text(comic_font, first_text, GREEN_CASTLE_COLOR, renderer, &first_place);
        print_text(comic_font, second_text, GREEN_CASTLE_COLOR, renderer, &second_place);
        print_text(comic_font, third_text, GREEN_CASTLE_COLOR, renderer, &third_place);
        print_text(comic_font, fourth_text, GREEN_CASTLE_COLOR, renderer, &fourth_place);
        SDL_RenderPresent(renderer);
    }
    unsort(points_of_players);
}

castle* random_map_menu(SDL_Renderer* renderer, int* number_of_players, int* number_of_castles, SDL_Texture* background){
    *number_of_players = 2;
    *number_of_castles = 3;
    SDL_Rect players_rect = {573, 228, 20, 40};
    SDL_Rect castles_rect = {573, 375, 20, 40};

    SDL_Rect quit_rect = {903, 548, 177, 92};
    SDL_Rect player_minus = {857, 217, 52, 52};
    SDL_Rect player_plus = {925, 217, 52, 52};
    SDL_Rect castle_minus = {857, 367, 52, 52};
    SDL_Rect castle_plus = {925, 367, 52, 52};
    SDL_Rect generate_rect = {350, 500, 378, 88};

    SDL_Event event;
    while(1){
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return NULL;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(click_in_rect(event, quit_rect)){
                    return NULL;
                }
                if(click_in_rect(event, player_minus)){
                    if(*number_of_players > 2)
                        (*number_of_players)--;
                }
                if(click_in_rect(event, player_plus)){
                    if(*number_of_players < 4)
                        (*number_of_players)++;
                    if(*number_of_castles == *number_of_players)
                        (*number_of_castles)++;
                }
                if(click_in_rect(event, castle_minus)){
                    if(*number_of_castles > (*number_of_players + 1))
                        (*number_of_castles)--;
                }
                if(click_in_rect(event, castle_plus)){
                    if(*number_of_castles < 26)
                        (*number_of_castles)++;
                }
                if(click_in_rect(event, generate_rect)){
                    castle* castles = generate_random_map(*number_of_players, *number_of_castles);
                    if(castles != NULL){
                        return castles;
                    }
                }
            }
        }
        check_music_finished();
        SDL_RenderCopy(renderer, background, NULL, NULL);
        castles_rect.w = digits_of(*number_of_castles)*20;
        char players[2];
        itoa(*number_of_players, players, 10);
        char castles[3];
        itoa(*number_of_castles, castles, 10);
        print_text(comic_font, players, PINK_CASTLE_COLOR, renderer, &players_rect);
        print_text(comic_font, castles, PINK_CASTLE_COLOR, renderer, &castles_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/20);
    }
}

castle* show_random_map_menu(SDL_Renderer* renderer, int* number_of_players, int* number_of_castles){
    SDL_Surface* surface = SDL_LoadBMP("images/choose_map_background.bmp");
    SDL_Texture* choose_map_background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = NULL;
    SDL_Surface* surface1 = SDL_LoadBMP("images/random_map_background.bmp");
    SDL_Texture* random_map_background = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_FreeSurface(surface1); surface1 = NULL;
    SDL_Rect quit_rect = {903, 548, 177, 92};
    SDL_Rect default_map1_rect = {374, 177, 330, 60};
    SDL_Rect default_map2_rect = {374, 267, 330, 60};
    SDL_Rect default_map3_rect = {374, 357, 330, 60};
    SDL_Rect random_map_rect = {352, 460, 374, 58};
    SDL_Event event;
    while(1){
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return NULL;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(click_in_rect(event, quit_rect)){
                    return NULL;
                }
                if(click_in_rect(event, default_map1_rect)){
                    *number_of_players = 2;
                    *number_of_castles = 12;
                    castle* castles = malloc(sizeof(castle) * (*number_of_castles));
                    for(int i=0; i<11; i++){
                        castles[i].player = 0;
                        castles[i].soldiers = START_NUMBER_OF_SOLDIERS;
                        castles[i].soldiers_with_destination = 0;
                        castles[i].radius = (5*i)%31 + 40;
                        castles[i].center_x = default_x_coordinates[2*i];
                        castles[i].center_y = default_y_coordinates[2*i];
                        castles[i].color = 0xFFEABA8A;
                        castles[i].center_color = 0xFF4B2600;
                    }
                    castles[11].player = 1;
                    castles[11].soldiers = START_NUMBER_OF_SOLDIERS;
                    castles[11].soldiers_with_destination = 0;
                    castles[11].radius = 65;
                    castles[11].center_x = default_x_coordinates[7];
                    castles[11].center_y = default_y_coordinates[7];
                    castles[11].color = 0xFFB7C900;
                    castles[11].center_color = 0xFF485400;
                    return castles;
                }
                if(click_in_rect(event, default_map2_rect)){
                    *number_of_players = 3;
                    *number_of_castles = 15;
                    castle* castles = generate_random_map(*number_of_players, *number_of_castles);
                    if(castles != NULL){
                        return castles;
                    }
                }
                if(click_in_rect(event, default_map3_rect)){
                    *number_of_players = 2;
                    *number_of_castles = 10;
                    castle* castles =  generate_random_map(*number_of_players, *number_of_castles);
                    if(castles != NULL){
                        return castles;
                    }
                }
                if(click_in_rect(event, random_map_rect)){
                    castle* castles = random_map_menu(renderer, number_of_players, number_of_castles, random_map_background);
                    if(castles != NULL){
                        return castles;
                    }
                }
            }
        }
        check_music_finished();
        SDL_RenderCopy(renderer, choose_map_background, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/20);
    }
}

castle* show_menu(SDL_Renderer* renderer, SDL_Rect* quick_game_rect,SDL_Rect* scoreboard_rect, SDL_Rect* quit_rect, SDL_Texture* background, int* number_of_players, int* number_of_castles, int points_of_players[4][2], char username[]){
    SDL_Event event;
    while(1){
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return NULL;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(click_in_rect(event, *quit_rect)){
                    return NULL;
                }
                if(click_in_rect(event, *scoreboard_rect)){
                    show_scoreboard(points_of_players, renderer, username);
                }
                if(click_in_rect(event, *quick_game_rect)){
                    castle* castles = show_random_map_menu(renderer, number_of_players, number_of_castles);
                    if(castles != NULL){
                        return castles;
                    }
                }
            }
        }
        check_music_finished();
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/20);
    }
}

void evaluate_menu_rects(SDL_Rect* quick_game_rect, SDL_Rect* scoreboard_rect, SDL_Rect* quit_rect){
    quick_game_rect->y = 228;
    quick_game_rect->h = 92;
    quick_game_rect->x = 262;
    quick_game_rect->w = 598;

    scoreboard_rect->y = 366;
    scoreboard_rect->h = 70;
    scoreboard_rect->x = 358;
    scoreboard_rect->w = 405;

    quit_rect->y = 484;
    quit_rect->h = 82;
    quit_rect->x = 477;
    quit_rect->w = 157;
}

castle* click_on_castle(SDL_Event event, castle* castles, int number_of_castles){
    for(int i=0; i<number_of_castles; i++){
        if(event.button.x < castles[i].center_x + CENTER_RADIUS && event.button.x > castles[i].center_x - CENTER_RADIUS
        && event.button.y < castles[i].center_y + CENTER_RADIUS && event.button.y > castles[i].center_y - CENTER_RADIUS){
            return &castles[i];
        }
    }
    return NULL;
}

void create_new_soldier(castle* source, castle* destination, soldier** soldiers, int number_of_moving_soldiers, int speed){
    *soldiers = realloc(*soldiers, (number_of_moving_soldiers + 1) * sizeof(soldier));
    (*soldiers)[number_of_moving_soldiers].destination = destination;
    (*soldiers)[number_of_moving_soldiers].radius = SOLDIER_RADIUS;
    (*soldiers)[number_of_moving_soldiers].color = source->center_color;
    (*soldiers)[number_of_moving_soldiers].source_color = source->color;
    (*soldiers)[number_of_moving_soldiers].x = source->center_x;
    (*soldiers)[number_of_moving_soldiers].y = source->center_y;
    (*soldiers)[number_of_moving_soldiers].speed = speed;
    (*soldiers)[number_of_moving_soldiers].player = source->player;
    if (source->soldiers_with_destination != 0) {
        source->soldiers_with_destination--;
    }
    source->soldiers--;
}

void send_one_soldier(soldier* the_soldier, soldier* soldiers, castle** source_castles, castle** destination_castles, int number_pf_destinations){
    if(the_soldier->radius != 0){
        int x_distance = (the_soldier->destination->center_x - the_soldier->x);
        int y_distance = (the_soldier->destination->center_y - the_soldier->y);
        double length = sqrt((the_soldier->destination->center_y - the_soldier->y)*(the_soldier->destination->center_y - the_soldier->y)
                              + (the_soldier->destination->center_x - the_soldier->x)*(the_soldier->destination->center_x - the_soldier->x));
        the_soldier->x += (int)((x_distance / length) * the_soldier->speed);
        the_soldier->y += (int)((y_distance / length) * the_soldier->speed);

        // if it arrives
        if (the_soldier->x < the_soldier->destination->center_x + CENTER_RADIUS && the_soldier->x > the_soldier->destination->center_x - CENTER_RADIUS
        && the_soldier->y < the_soldier->destination->center_y + CENTER_RADIUS && the_soldier->y > the_soldier->destination->center_y - CENTER_RADIUS) {
            if (the_soldier->player == the_soldier->destination->player) {
                the_soldier->destination->soldiers++;
            } else {
                if (the_soldier->destination->soldiers != 0) {
                    if(the_soldier->destination->soldiers_with_destination != 0)
                        the_soldier->destination->soldiers_with_destination--;
                    the_soldier->destination->soldiers--;
                }else{
                    the_soldier->destination->player = the_soldier->player;
                    the_soldier->destination->color = the_soldier->source_color;
                    the_soldier->destination->center_color = the_soldier->color;
                    the_soldier->destination->soldiers = 1;
                    the_soldier->destination->soldiers_with_destination = 0;
                    for(int i=0; i<number_pf_destinations; i++){
                        if(the_soldier->destination == source_castles[i]){
                            source_castles[i] = NULL;
                            destination_castles[i] = NULL;
                        }
                    }
                }
            }
            the_soldier->radius = 0;
        }

        // check for collisions
        for(soldier* i=soldiers; i<the_soldier; i++){
            if(i->player != the_soldier->player && i != the_soldier){
                int xDistance = i->x - the_soldier->x;
                int yDistance = i->y - the_soldier->y;
                if(xDistance < 2*i->radius && xDistance > -2*i->radius
                && yDistance < 2*i->radius && yDistance > -2*i->radius){
                    i->radius = 0;
                    the_soldier->radius = 0;
                }
            }
        }
    }
}

void render_soldiers(SDL_Renderer * renderer, soldier* soldiers, int number_of_moving_soldiers){
    for(int i=0; i<number_of_moving_soldiers; i++){
        if(soldiers[i].radius != 0){
            filledCircleColor(renderer, soldiers[i].x, soldiers[i].y, soldiers[i].radius, soldiers[i].color);
        }
    }
}

void AI_moves(castle* castles, int number_of_castles, castle*** source_castles, int* number_of_sources, castle*** destination_castles, int* number_of_destinations, int number_of_done_motions){
    for(int i=0; i<number_of_castles; i++){
        if(castles[i].player > 0 && castles[i].soldiers_with_destination == 0 && castles[i].soldiers > 20){
            for(int j=0; j<number_of_castles; j++){
                // attack other players
                if(castles[i].soldiers - castles[j].soldiers > 10 && castles[j].player != castles[i].player && castles[j].player != -1){
                    (*source_castles)[*number_of_sources] = castles+i;
                    (*number_of_sources)++;
                    *source_castles = realloc(*source_castles, (*number_of_sources+1) * sizeof(castle*));
                    (*destination_castles)[*number_of_destinations] = castles+j;
                    (*number_of_destinations)++;
                    *destination_castles = realloc(*destination_castles, (*number_of_destinations+1) * sizeof(castle *));
                    castles[i].soldiers_with_destination = castles[i].soldiers;
                    return;
                }
                // attack Neutral castles
                else if(castles[i].soldiers > castles[j].soldiers && castles[j].player == -1){
                    (*source_castles)[*number_of_sources] = castles+i;
                    (*number_of_sources)++;
                    *source_castles = realloc(*source_castles, (*number_of_sources+1) * sizeof(castle*));
                    (*destination_castles)[*number_of_destinations] = castles+j;
                    (*number_of_destinations)++;
                    *destination_castles = realloc(*destination_castles, (*number_of_destinations+1) * sizeof(castle *));
                    castles[i].soldiers_with_destination = castles[i].soldiers;
                    return;
                }
                // support other castles of its own team (castles with the same player)
                else if(5 > castles[j].soldiers && castles[j].player == castles[i].player){
                    (*source_castles)[*number_of_sources] = castles+i;
                    (*number_of_sources)++;
                    *source_castles = realloc(*source_castles, (*number_of_sources+1) * sizeof(castle*));
                    (*destination_castles)[*number_of_destinations] = castles+j;
                    (*number_of_destinations)++;
                    *destination_castles = realloc(*destination_castles, (*number_of_destinations+1) * sizeof(castle *));
                    castles[i].soldiers_with_destination = castles[i].soldiers;
                    return;
                }
            }
        }
    }
}

int check_for_winner(int points_of_players[4][2], castle* castles, int number_of_players, int number_of_castles, int is_lost[], SDL_Renderer* renderer){
    int is_lost_copy[number_of_players];
    for(int i=0; i<number_of_players; i++){
        is_lost_copy[i] = 1;
    }

    for(int i=0; i<number_of_castles; i++){
        if(castles[i].player >= 0){
            is_lost_copy[castles[i].player] = 0;
        }
    }
    int is_all_lost = 1;

    // if an enemy loses
    for(int i=1; i<number_of_players; i++){
        if(is_lost_copy[i]){
            if(!is_lost[i])
                points_of_players[i][1] -= 5;
        }else{
            is_all_lost = 0;
        }
    }
    for(int i=0; i<number_of_players; i++){
        is_lost[i] = is_lost_copy[i];
    }
    // if the user loses
    if(is_lost[0]){
        points_of_players[0][1] -= 5;
        for(int i=1; i<number_of_players; i++){
            if(!is_lost[i])
                points_of_players[i][1] += 5;
        }
        SDL_Surface* surface = SDL_LoadBMP("images/you_lost.bmp");
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); // free the surface because we don't need it anymore
        SDL_Rect quit_rect = {825, 550, 255, 90};
        SDL_Event event;
        while(1){
            SDL_RenderClear(renderer);
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    return 1;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (click_in_rect(event, quit_rect)) {
                        return 1;
                    }
                }
            }
            check_music_finished();
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000/20);
        }
    }

    // if the user wins
    if((!is_lost[0]) && is_all_lost){
        points_of_players[0][1] += 5;
        SDL_Surface* surface = SDL_LoadBMP("images/you_won.bmp");
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); // free the surface because we don't need it anymore
        SDL_Rect quit_rect = {825, 550, 255, 90};
        SDL_Event event;
        while(1){
            SDL_RenderClear(renderer);
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    return 1;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (click_in_rect(event, quit_rect)) {
                        return 1;
                    }
                }
            }
            check_music_finished();
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000/20);
        }
    }
    return 0;
}