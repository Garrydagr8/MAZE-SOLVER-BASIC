#ifndef PG_MAZE_H
#define PG_MAZE_H
struct maze {
    char ** map;
    int startx, starty;
    int numrows;
    int initdir;
};

struct pos {
    int x, y, dir;
};
#define BUFFERSIZE (1000)

#define MAZE_ENTRANCE 'I'
#define MAZE_EXIT     'O'
#define MAZE_WALL     '0'
#define MAZE_PATH     '1'
#define MAZE_TRAIL    '*'

#define MOVE_LEFT  (0)
#define MOVE_UP    (1)
#define MOVE_RIGHT (2)
#define MOVE_DOWN  (3)

#define MAZE_NOWAY      (0)
#define MAZE_FOUNDEXIT  (1)
#define MAZE_NOEXIT    (-1)

#endif
