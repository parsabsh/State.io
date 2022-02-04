#include <SDL2/SDL.h>
#define CENTER_RADIUS 18
#define START_NUMBER_OF_SOLDIERS 30
#define MAXIMUM_NUMBER_OF_SOLDIERS 150

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

void increment_soldiers(int time, castle* castles, int fps, int rate, int number_of_castles);

castle* show_random_map_menu(SDL_Renderer* renderer, int* number_of_players, int* number_of_castles);

castle* show_menu(SDL_Renderer* renderer,SDL_Rect* quick_game_rect, SDL_Rect* Scoreboard_rect, SDL_Rect* quit_rect,
              SDL_Texture* background, int* number_of_players, int* number_of_castles, int points_of_players[4][2], char username[]);

void evaluate_menu_rects(SDL_Rect* quick_game_rect, SDL_Rect* Scoreboard_rect, SDL_Rect* quit_rect);

void show_scoreboard(int points_of_players[4][2], SDL_Renderer* renderer, char username[]);

castle* random_map_menu(SDL_Renderer* renderer, int* number_of_players, int* number_of_castles, SDL_Texture* background);