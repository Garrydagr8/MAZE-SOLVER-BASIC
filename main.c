#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

#include "maze.h"
#ifndef PG_SOLVE_H
#define PG_SOLVE_H
int solve(struct maze * maze);
#endif
void GetMazeFromFile(char * filename, struct maze * maze);
void FreeMaze(struct maze * maze);
void PrintMaze(struct maze * maze);

int look(struct maze * maze, struct pos pos) {
    int i, n;


    /*  Set new position based on direction  */

    switch ( pos.dir ) {
    case MOVE_UP:
        pos.y -= 1;
        break;
        
    case MOVE_DOWN:
        pos.y += 1;
        break;

    case MOVE_LEFT:
        pos.x -= 1;
        break;

    case MOVE_RIGHT:
        pos.x += 1;
        break;

    default:
        break;
    }


    /*  Check new position  */

    if ( pos.y < 0 || pos.y >= maze->numrows )
        return MAZE_NOWAY;
    else if ( pos.x < 0 || pos.x >= strlen(maze->map[pos.y]) )
        return MAZE_NOWAY;
    else if ( maze->map[pos.y][pos.x] == MAZE_WALL )
        return MAZE_NOWAY;
    else if ( maze->map[pos.y][pos.x] == MAZE_EXIT )
        return MAZE_FOUNDEXIT;
    else if ( maze->map[pos.y][pos.x] == MAZE_ENTRANCE )
        return MAZE_NOEXIT;


    /*  Try all the three directions other than backwards  */

    pos.dir -= 1;
    if ( pos.dir < 0 )
        pos.dir += 4;

    for ( i = 0; i < 3; ++i ) {
        maze->map[pos.y][pos.x] = MAZE_TRAIL;          /*  Leave trail through maze  */

        n = look(maze, pos);
        if ( n ) {
            if ( n == MAZE_NOEXIT )
                maze->map[pos.y][pos.x] = MAZE_PATH;   /*  No exit, so clear trail   */
            return n;
        }

        pos.dir += 1;
        if ( pos.dir > 3 )
            pos.dir -= 4;
    }
    maze->map[pos.y][pos.x] = MAZE_PATH;

    return 0;
}


/*  Function to find a path through a maze  */
    
int solve(struct maze * maze) {
    struct pos pos;

    pos.x = maze->startx;
    pos.y = maze->starty;
    pos.dir = maze->initdir;

    return look(maze, pos);
}  

void GetMazeFromFile(char * filename, struct maze * maze) {
    char buffer[BUFFERSIZE];
    FILE * fp;
    char ** map;
    int n = 0, foundentrance = 0, foundexit = 0;



    if ( !(fp = fopen(filename, "r")) ) {
        sprintf(buffer, "Couldn't open file %s", filename);
        perror(buffer);
        exit(EXIT_FAILURE);
    }
    while ( fgets(buffer, BUFFERSIZE, fp) )
        ++n;
    if ( !(map = malloc(n * sizeof *map)) ) {
        fputs("Couldn't allocate memory for map\n", stderr);
        exit(EXIT_FAILURE);
    }
    rewind(fp);
    n = 0;
    while ( fgets(buffer, BUFFERSIZE, fp) ) {
        int i;

        if ( !(map[n] = malloc((strlen(buffer)+1) * sizeof map[n])) ) {
            fputs("Couldn't allocate memory for map\n", stderr);

            for ( i = 0; i < n; ++i )
                free(map[i]);

            free(map);

            exit(EXIT_FAILURE);
        }

        strcpy(map[n], buffer);
   

        /*  Remove trailing whitespace  */

        for ( i = strlen(map[n]) - 1; isspace(map[n][i]); --i )
            map[n][i] = 0;


        /*  Check for entrance and store position if found  */

        if ( !foundentrance ) {
            i = 0;
            while ( map[n][i] != 'I' && map[n][i++] );
            if ( map[n][i] == MAZE_ENTRANCE ) {
                maze->startx = i;
                maze->starty = n;
                foundentrance = 1;
            }
        }

        /*  Check for exit  */

        if ( !foundexit ) {
            if ( strchr(map[n], MAZE_EXIT) )
                foundexit = 1;
        }
        ++n;
    }


    /*  Fill in maze structure  */

    maze->map = map;
    maze->numrows = n;


    /*  Exit if there is no entrance or no exit */

    if ( !foundentrance ) {
        fputs("Maze has no entrance!\n", stderr);
        FreeMaze(maze);
        exit(EXIT_FAILURE);
    }

    if ( !foundexit ) {
        fputs("Maze has no exit!\n", stderr);
        FreeMaze(maze);
        exit(EXIT_FAILURE);
    }


    /*  Check for initial direction into the maze  */

    if ( maze->startx < strlen(maze->map[maze->starty]) - 1 &&
         maze->map[maze->starty][maze->startx+1] == MAZE_PATH )
        maze->initdir = MOVE_RIGHT;
    else if ( maze->startx > 0 &&
              maze->map[maze->starty][maze->startx-1] == MAZE_PATH )
        maze->initdir = MOVE_LEFT;
    else if ( maze->starty > 0 &&
         maze->map[maze->starty-1][maze->startx] == MAZE_PATH )
        maze->initdir = MOVE_UP;
    else if ( maze->starty < (maze->numrows-1) &&
              maze->map[maze->starty+1][maze->startx] == MAZE_PATH )
        maze->initdir = MOVE_DOWN;
    if ( fclose(fp) ) {
        perror("Couldn't close file");
        FreeMaze(maze);
        exit(EXIT_FAILURE);
    }
}

void FreeMaze(struct maze * maze) {
    int n;

    for ( n = 0; n < maze->numrows; ++n )
        free(maze->map[n]);

    free(maze->map);
}

void PrintMaze(struct maze * maze) {
    int n;

    for ( n = 0; n < maze->numrows; ++n )
        puts(maze->map[n]);
}

int main(int argc, char *argv[]) {
    struct maze maze;

    if ( argc < 2 ) {
        puts("You must specify the filename of your maze");
        return EXIT_FAILURE;
    }
    else if ( argc > 2 ) {
        puts("Too many command line arguments");
        return EXIT_FAILURE;
    }

    GetMazeFromFile(argv[1], &maze);

    if ( solve(&maze) == MAZE_FOUNDEXIT )
        puts("Found exit!");
    else
        puts("Can't reach exit!");

    PrintMaze(&maze);
    FreeMaze(&maze);

    return EXIT_SUCCESS;
}


