
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    TILE_VALUE_FIVE, TILE_VALUE_SIX, TILE_VALUE_SEVEN, TILE_VALUE_EIGHT
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

void gBoard_dead(gameBoard* gBoard)
{
    for (int i = 0; i < gBoard->bBombAmount; i++)
    {
        gBoard->grid[gBoard->bombLocation[i * 2]][gBoard->bombLocation[(i * 2) + 1]].state = TILE_STATE_SHOWN;
    }
}

int gBoard_boundary_check(gameBoard* gBoard, int x, int y)
{
    /*
        A Function to check if position is in game board
        Returns 1 if position is out of board
        Returns 0 if position is in board
    */
    if (x < 0 || x >= gBoard->bWidth || y < 0 || y >= gBoard->bHeight)
    {
        //printf("{%d, %d}: Out of bounds\n", x, y);
        return 1;
    }

    return 0;
}

int gBoard_check_flag(gameBoard* gBoard, int x, int y, int neighbour_flag_array[16], int* count)
{
    /*
        A Function to be used to check for chord clicks
        Returns 1 if tile is a flag
        Returns 0 if tile isn't flag

        If neighbour has a flag then add them to neighbour_flag_array
    */
    int flag = 1;

    // Boundary Check
    if (gBoard_boundary_check(gBoard, x, y))
    {
        flag = 0;
    }
    else if (!(gBoard->grid[x][y].flag))
    {
        //printf("count: %d\n", *count);
        // This neighbour is valid and doesn't have a flag so add it to the array
        neighbour_flag_array[(*count) * 2] = x;
        neighbour_flag_array[((*count) * 2) + 1] = y;

        *count += 1;
        flag = 0;
    }

    return flag;
}

void gBoard_user_click_chord(gameBoard* gBoard, int x, int y, int neighbour_flag_array[16], int count)
{
    /*
        Do a chord click on tile x, y
        Check neighbours around and see if they're hidden or shown
        Only reveal neighbours that don't have flags
    */
    //printf("{%d, %d}: Chord Click\n", x, y);

    for (int i = 0; i < count; i++)
    {
        gBoard_reveal(gBoard, neighbour_flag_array[i * 2], neighbour_flag_array[(i * 2) + 1]);
    }
}

void gBoard_user_click(gameBoard* gBoard, int x, int y)
{
    /*
        User clicks on a tile
        Check to see if its a chord or normal click

        Normal Click Conditions
        - tile is hidden

        Chord Click Conditions
        - tile is shown
        - tile has a value
        - tiles value == flags around it
    */

    if (gBoard->grid[x][y].state == TILE_STATE_HIDDEN)
    {
        // Tile is hidden so normal click
        gBoard_reveal(gBoard, x, y);
    }
    else
    {
        /*
            Possible chord click so check for neighbours
            Shown tiles cannot be bombs or flags so no need to check
        */
        int neighbour_flag_count = 0;
        int neighbour_no_flag_count = 0;
        int neighbours_no_flag_pos[16];

        neighbour_flag_count += gBoard_check_flag(gBoard, x, y - 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // top
        neighbour_flag_count += gBoard_check_flag(gBoard, x, y + 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // bottom
        neighbour_flag_count += gBoard_check_flag(gBoard, x - 1, y, neighbours_no_flag_pos, &neighbour_no_flag_count); // left
        neighbour_flag_count += gBoard_check_flag(gBoard, x + 1, y, neighbours_no_flag_pos, &neighbour_no_flag_count); // right

        neighbour_flag_count += gBoard_check_flag(gBoard, x + 1, y - 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // top-right
        neighbour_flag_count += gBoard_check_flag(gBoard, x - 1, y - 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // top-left
        neighbour_flag_count += gBoard_check_flag(gBoard, x + 1, y + 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // bottom-right
        neighbour_flag_count += gBoard_check_flag(gBoard, x - 1, y + 1, neighbours_no_flag_pos, &neighbour_no_flag_count); // bottom-left

        //printf("flags around: %d\n", neighbour_flag_count);
        if (neighbour_flag_count == gBoard->grid[x][y].value)
        {
            //printf("Chord Click!\n");
            gBoard_user_click_chord(gBoard, x, y, neighbours_no_flag_pos, neighbour_no_flag_count);
        }

    }

}

void gBoard_user_place_flag(gameBoard* gBoard, int x, int y)
{
    /*
        User requests to flag a tile
    */
    if (gBoard->grid[x][y].state == TILE_STATE_SHOWN)
    {
        // If tile is already shown
        return;
    }
    if (gBoard->grid[x][y].flag)
    {
        // If tile is flagged
        gBoard->grid[x][y].flag = false;
        return;
    }

    gBoard->grid[x][y].flag = true;
}

void gBoard_reveal(gameBoard* gBoard, int x, int y)
{
    /*
        Base Case
        Check for out of bounds.
        Check to see if its shown
        Check to see tile value
    */
    if (gBoard_boundary_check(gBoard, x, y))
    {
        return;
    }

    if (gBoard->grid[x][y].flag)
    {
        //printf("{%d, %d}: Flag\n", x, y);
        return;
    }
    else if (gBoard->grid[x][y].bomb)
    {
        //printf("{%d, %d}: Bomb\n", x, y);
        gBoard->GAME_STATE = GAME_STATE_DEAD;
        return;
    }
    else if (gBoard->grid[x][y].state == TILE_STATE_SHOWN)
    {
        //printf("{%d, %d}: Already visited\n", x, y);
        return;
    }

    if (gBoard->grid[x][y].value != TILE_VALUE_NOTHING)
    {
        gBoard->grid[x][y].state = TILE_STATE_SHOWN;
        gBoard->GAME_TILES_SHOWN += 1;
        //printf("{%d, %d}: Revealed\n", x, y);
        return;
    }

    // General Case
    gBoard->grid[x][y].state = TILE_STATE_SHOWN;
    gBoard->GAME_TILES_SHOWN += 1;
    //printf("{%d, %d}: Revealed\n", x, y);

    gBoard_reveal(gBoard, x, y - 1); // top
    gBoard_reveal(gBoard, x, y + 1); // bottom
    gBoard_reveal(gBoard, x - 1, y); // left
    gBoard_reveal(gBoard, x + 1, y); // right

    gBoard_reveal(gBoard, x + 1, y - 1); // top-right
    gBoard_reveal(gBoard, x - 1, y - 1); // top-left
    gBoard_reveal(gBoard, x + 1, y + 1); // bottom-right
    gBoard_reveal(gBoard, x - 1, y + 1); // bottom-left
}

void gBoard_print(gameBoard* gBoard, int bWidth, int bHeight)
{
    printf("--Board--");
    for (int i = 0; i < bHeight; i++)
    {
        printf("\n");
        for (int j = 0; j < bWidth; j++)
        {
            if (gBoard->grid[j][i].bomb)
            {
                printf(" B ");
            }
            else
            {
                printf(" %d ", gBoard->grid[j][i].value);
            }
        }
    }
    printf("\n");
}

void gBoard_tile_value_set(gameBoard* gBoard)
{
    /*
        Update tiles nearby bombs with the correct value
        Iterate through bombList and for tiles neighbouring coordinates increment their value by one
    */
    int bWidth_array = gBoard->bWidth - 1;
    int bHeight_array = gBoard->bHeight - 1;

    for (int i = 0; i < gBoard->bBombAmount; i++)
    {
        // Position of i'th bomb
        int b_PosX = gBoard->bombLocation[i * 2]; int b_PosY = gBoard->bombLocation[(i * 2) + 1];
        //printf("Stage 3: {%d, %d} ", b_PosX, b_PosY);

        // check for 9 cases
        if ((b_PosX + 1 > bWidth_array) && (b_PosY - 1 < 0))
        {
            //printf("Case 1\n");
            /*
                0    0    0
                --------|
                0    B  | 0
                        |
                0    0  | 0
            */
            gBoard->grid[b_PosX - 1][b_PosY].value++;
            gBoard->grid[b_PosX - 1][b_PosY + 1].value++;
            gBoard->grid[b_PosX][b_PosY + 1].value++;
            continue;
        }

        if ((b_PosX - 1 < 0) && (b_PosY - 1 < 0))
        {
            //printf("Case 2\n");
            /*
                0    0   0
                  |-------
                0 |  B   0
                  |
                0 |  0   0
            */

            gBoard->grid[b_PosX + 1][b_PosY].value++;
            gBoard->grid[b_PosX + 1][b_PosY + 1].value++;
            gBoard->grid[b_PosX][b_PosY + 1].value++;
            continue;
        }

        if ((b_PosX + 1 > bWidth_array) && (b_PosY + 1 > bHeight_array))
        {
            //printf("Case 3\n");
            /*
                0    0  | 0
                        |
                0    B  | 0
                --------|
                0    0    0
            */

            gBoard->grid[b_PosX - 1][b_PosY].value++;
            gBoard->grid[b_PosX - 1][b_PosY - 1].value++;
            gBoard->grid[b_PosX][b_PosY - 1].value++;
            continue;
        }

        if ((b_PosX - 1 < 0) && (b_PosY + 1 > bHeight_array))
        {
            //printf("Case 4\n");
            /*
                0 |  0    0
                  |
                0 |  B    0
                  |-------
                0    0    0
            */

            gBoard->grid[b_PosX + 1][b_PosY].value++;
            gBoard->grid[b_PosX + 1][b_PosY - 1].value++;
            gBoard->grid[b_PosX][b_PosY - 1].value++;
            continue;
        }

        if (b_PosY - 1 < 0)
        {
            //printf("Case 5\n");
            /*
                0    0    0
               ------------
                0    B    0

                0    0    0
            */
            gBoard->grid[b_PosX + 1][b_PosY].value++;
            gBoard->grid[b_PosX - 1][b_PosY].value++;
            gBoard->grid[b_PosX][b_PosY + 1].value++;
            gBoard->grid[b_PosX + 1][b_PosY + 1].value++;
            gBoard->grid[b_PosX - 1][b_PosY + 1].value++;
            continue;
        }

        if (b_PosY + 1 > bHeight_array)
        {
            //printf("Case 6\n");
            /*
                0    0    0

                0    B    0
               ------------
                0    0    0
            */

            gBoard->grid[b_PosX + 1][b_PosY].value++;
            gBoard->grid[b_PosX - 1][b_PosY].value++;
            gBoard->grid[b_PosX][b_PosY - 1].value++;
            gBoard->grid[b_PosX + 1][b_PosY - 1].value++;
            gBoard->grid[b_PosX - 1][b_PosY - 1].value++;
            continue;
        }

        if (b_PosX - 1 < 0)
        {
            //printf("Case 7\n");
            /*
                0  | 0    0
                   |
                0  | B    0
                   |
                0  | 0    0
            */

            gBoard->grid[b_PosX][b_PosY - 1].value++;
            gBoard->grid[b_PosX][b_PosY + 1].value++;
            gBoard->grid[b_PosX + 1][b_PosY].value++;
            gBoard->grid[b_PosX + 1][b_PosY - 1].value++;
            gBoard->grid[b_PosX + 1][b_PosY + 1].value++;
            continue;
        }

        if (b_PosX + 1 > bWidth_array)
        {
            //printf("Case 8\n");
            /*
                0    0  | 0
                        |
                0    B  | 0
                        |
                0    0  | 0
            */

            gBoard->grid[b_PosX][b_PosY - 1].value++;
            gBoard->grid[b_PosX][b_PosY + 1].value++;
            gBoard->grid[b_PosX - 1][b_PosY].value++;
            gBoard->grid[b_PosX - 1][b_PosY - 1].value++;
            gBoard->grid[b_PosX - 1][b_PosY + 1].value++;
            continue;
        }

        /*
            Last Case:
            0    0    0

            0    B    0

            0    0    0
        */
        gBoard->grid[b_PosX + 1][b_PosY].value++;
        gBoard->grid[b_PosX - 1][b_PosY].value++;
        gBoard->grid[b_PosX][b_PosY - 1].value++;
        gBoard->grid[b_PosX][b_PosY + 1].value++;

        gBoard->grid[b_PosX + 1][b_PosY - 1].value++;
        gBoard->grid[b_PosX + 1][b_PosY + 1].value++;
        gBoard->grid[b_PosX - 1][b_PosY - 1].value++;
        gBoard->grid[b_PosX - 1][b_PosY + 1].value++;
    }
}

void gBoard_bomb_set(gameBoard* gBoard)
{
    // Populate gameBoard with bombs
    // Inits gameBoard.bombList

    if (gBoard->bBombAmount >= (gBoard->bWidth * gBoard->bHeight))
    {
        // More bombs then there are tiles
        gBoard->bBombAmount = rand() % (gBoard->bWidth * gBoard->bHeight) - 1;
        printf("More bombs then there are tiles! Setting bomb amount to %d\n", gBoard->bBombAmount);


    }

    tile** grid = gBoard->grid;
    int bBombAmount = gBoard->bBombAmount;
    int bWidth = gBoard->bWidth; int bHeight = gBoard->bHeight;

    int bombsPlaced = 0;
    int* bombList = malloc(2 * bBombAmount * sizeof(int)); // allocate space for bombList
    srand(time(NULL));
    while (bombsPlaced != bBombAmount)
    {
        // Get random board position
        int randX = rand() % bWidth;
        int randY = rand() % bHeight;

        if (!grid[randX][randY].bomb)
        {
            grid[randX][randY].bomb = true;

            // Place bomb locations in bombList
            bombList[bombsPlaced * 2] = randX;
            bombList[(bombsPlaced * 2) + 1] = randY;

            bombsPlaced++;

            //printf("Bomb placed at {%d, %d}\n", randX, randY);
        }
    }
    gBoard->bombLocation = bombList;

}

void gBoard_free(gameBoard* gBoard)
{
    // Free gameboard and NULL all the contents
    for (int i = 0; i < gBoard->bWidth; i++)
    {
        for (int j = 0; j < gBoard->bHeight; j++)
        {
            gBoard->grid[i][j].pos_x = NULL;
            gBoard->grid[i][j].pos_y = NULL;
            gBoard->grid[i][j].state = 0;
            gBoard->grid[i][j].value = 0;
            gBoard->grid[i][j].flag = NULL;
            gBoard->grid[i][j].bomb = NULL;
        }
        gBoard->grid[i] = NULL;
        free(gBoard->grid[i]);
    }
    gBoard->grid = NULL;
    gBoard->bBombAmount = NULL;
    gBoard->bWidth = NULL;
    gBoard->bHeight = NULL;

    free(gBoard->grid);
    free(gBoard->bBombAmount);
    free(gBoard->bWidth);
    free(gBoard->bHeight);
    free(gBoard->bombLocation);

    gBoard == NULL;
}

gameBoard gBoard_init(int bWidth, int bHeight, int bBombAmount)
{
    /*
        Init the gameboard
        Gameboard is a bWidth x bHeight 2D array of tile structs
    */
    gameBoard gBoard;
    gBoard.bWidth = bWidth; gBoard.bHeight = bHeight; gBoard.bBombAmount = bBombAmount;
    gBoard.GAME_STATE = GAME_STATE_ALIVE;
    gBoard.GAME_TILES_SHOWN = 0;
    // Create grid
    tile** grid = malloc(bWidth * sizeof(tile*));

    for (int i = 0; i < bWidth; i++)
    {
        grid[i] = malloc(bHeight * sizeof(tile));

        for (int j = 0; j < bHeight; j++)
        {
            grid[i][j].pos_x = i;
            grid[i][j].pos_y = j;
            grid[i][j].state = TILE_STATE_HIDDEN;
            grid[i][j].value = TILE_VALUE_NOTHING;
            grid[i][j].flag = false;
            grid[i][j].bomb = false;
        }
    }
    gBoard.grid = grid;

    gBoard_bomb_set(&gBoard);
    gBoard_tile_value_set(&gBoard);
    gBoard_print(&gBoard, bWidth, bHeight);
    return gBoard;
}
