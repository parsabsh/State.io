#include <SDL2/SDL.h>
#define CENTER_RADIUS 18

typedef struct castle{
    // coordinates of the castle (these will be selected randomly)
    int center_x;
    int center_y;
    int radius;

    /*
    -1 : neutral (gray)
     0 : user (blue)
     1 : player 1 (green)
     2 : player 2 (cream)
     3 : player 3 (pink)
    */
    int player;
    Uint32 color, center_color;

    // the number of soldiers in the castle
    int soldiers;
} castle;

// default coordinates of castles' center
extern int default_x_coordinates[26];
extern int default_y_coordinates[26];

castle* generate_random_map(int number_of_players, int number_of_castles);

void render_map(castle* castles, int number_of_castles, SDL_Renderer* renderer);