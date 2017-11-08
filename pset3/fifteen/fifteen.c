/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

// my function prototypes
void swaptile(int tile);

// my declarations
int maxrow;
int maxcol;
int tilerow;
int tilecol;
int blankrow;
int blankcol;


int main(int argc, string argv[])
{
    
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    //declare for use
    int tileval = 1;
    
    // iterate over the board
    for (int row = 0; row < d; row++) // choose row
    {
        for (int col = 0; col < d; col++) // choose col
        {
            board[row][col] = (d * d) - tileval;
            tileval++;
        }
    }
    
    // if d is even swap the last two arraay values
    if (d % 2 == 0)
    {
        // declar new var to act as as the highest col / row
        int e = d - 1;
        
        int temp = board[e][e - 1];
        board[e][e - 1] = board[e][e - 2];
        board[e][e - 2] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int row = 0; row < d; row++) // choose row
    {
        for (int col = 0; col < d; col++) // choose col
        {
            if (board[row][col] == 0)
            {
                printf("  _ "); // print underscore in place of 0
            }
            else
            {
            printf(" %2i ", board[row][col]); // print value
            }
        }
        printf("\n\n"); // print new line
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    maxrow = d - 1;
    maxcol = d - 1;
    
    // check tile exists
    if (tile > (d * d) - 1 || tile < 1)
    {
        return false;
    }
    
    // find the tile row and column index
    for (int row = 0; row < d; row++) // choose row
    {
        for (int col = 0; col < d; col++) // choose col
        {
            if (board[row][col] == tile)
            {
                tilerow = row;
                tilecol = col;
            }
        }
    }
    
    // find blank location
    for (int row = 0; row < d; row++) // choose row
    {
        for (int col = 0; col < d; col++) // choose col
        {
            if (board[row][col] == 0)
            {
                blankrow = row;
                blankcol = col;
            }
        }
    }
    
    // check if the move is legal
    
    // check if tile is directly above blanktile
    if (blankrow + 1 == tilerow && blankcol == tilecol)
    {
        swaptile(tile);
        return true;
    }
    
    // check if tile is directly below blanktile
    if (blankrow - 1 == tilerow && blankcol == tilecol)
    {
        swaptile(tile);
        return true;
    }
    
    // check if tile is to left of blanktile
    if (blankcol - 1 == tilecol && blankrow == tilerow)
    {
        swaptile(tile);
        return true;
    }
    
    // check if tile is to right of blanktile
    if (blankcol + 1 == tilecol && blankrow == tilerow)
    {
        swaptile(tile);
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // have a checker that checks that each iteration over the array has an index value 1 higher than the last
    int checker = 1;
    
    // iterate over board incrementing checker
    for (int row = 0; row < d; row++) // choose row
    {
        for (int col = 0; col < d; col++) // choose col
        {
            if (board[row][col] == checker)
            {
                checker++;
            }
        }
    }
    
    // check for the condition that the 0 and the max value are in their final position
    if (checker == d * d && board[maxrow][maxcol] == 0)
    {
        return true;
    }
    return false;
}

void swaptile(tile)
{
    int temp = board[tilerow][tilecol];
    board[tilerow][tilecol] = board[blankrow][blankcol];
    board[blankrow][blankcol] = temp;
}
