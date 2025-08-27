#ifndef game_h
#define game_h
typedef struct tile tile;
typedef struct gameBoard gameBoard;

enum GAME_STATE {
    GAME_STATE_DEAD,
    GAME_STATE_ALIVE
};

enum TILE_STATE {
    TILE_STATE_HIDDEN,
    TILE_STATE_SHOWN
};

enum TILE_VALUE {
    TILE_VALUE_NOTHING,
    TILE_VALUE_ONE, TILE_VALUE_TWO, TILE_VALUE_THREE, TILE_VALUE_FOUR,
    TILE_VALUE_FIVE, TILE_VALUE_SIX, TILE_VALUE_SEVEN, TILE_VALUE_EIGHT,
};

// Structs
struct tile {
    enum TILE_STATE state;
    enum TILE_VALUE value;
    bool flag;
    bool bomb;
    int pos_x, pos_y;
};

struct gameBoard {
    int bBombAmount, bWidth, bHeight;

    /*
        grid:
        Contains a 2d array of tile structs. This is the variable that holds the board
    */
    tile** grid;
    /*
        bombLocation:
        Contains an array which consits of bomb locations.
        Each bomb takes up two indexes. The first index being the x position while the second index is the y position
    */
    int* bombLocation;
    /*
        Game State
        0 = alive
        1 = dead
    */
    int GAME_STATE;
    /*
        Tiles shown
        if (bWidth * BHeght) - bBombAmount == GAMES_TILES_SHOWN
        won!
    */
    int GAME_TILES_SHOWN;
};

gameBoard gBoard_init(int bWidth, int bHeight, int bBombAmount);
void gBoard_tile_value_set(gameBoard* gBoard);
void gBoard_free(gameBoard* gBoard);
void gBoard_bomb_set(gameBoard* gBoard);
void gBoard_print(gameBoard* gBoard, int bWidth, int bHeight);
void gBoard_reveal(gameBoard* gBoard, int x, int y);
void gBoard_user_place_flag(gameBoard* gBoard, int x, int y);
void gBoard_user_click(gameBoard* gBoard, int x, int y);
int gBoard_boundary_check(gameBoard* gBoard, int x, int y);
int gBoard_check_flag(gameBoard* gBoard, int x, int y, int neighbour_flag_array[16], int* count);
void gBoard_user_click_chord(gameBoard* gBoard, int x, int y, int neighbour_flag_array[16], int count);
void gBoard_dead(gameBoard* gBoard);
#endif