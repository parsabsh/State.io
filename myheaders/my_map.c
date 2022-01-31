#include "my_map.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include <stdlib.h>

// check if the element is in the array with length i
int is_in(int element, int array[], int i){
    for(int j=0; j<i; j++){
        if(element == array[j])
            return 1;
    }
    return 0;
}

// chooses n numbers in the interval [start, end] and store them in the array
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
        castles[i].soldiers = 50;
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
    }
}