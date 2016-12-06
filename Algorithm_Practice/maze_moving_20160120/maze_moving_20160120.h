#ifndef MAZE_MOVING_20160120_H
#define MAZE_MOVING_20160120_H

#define DEBUG_LEVEL 2
#define N_TEST_CASE 6

#define MAX_MAP_SIZE 102
#define MAX_Q_SIZE	(MAX_MAP_SIZE * MAX_MAP_SIZE * MAX_MAP_SIZE)

#define D_EMPTY 0
#define D_RED	1
#define D_BLUE  2
#define D_WALL  3
#define D_EXIT  4

typedef struct Position {
	int r;
	int c;
} Position;

typedef struct PositionInfo {
	Position red;
	Position blue;
	int i_dir;
	int n_move;
} PositionInfo;

typedef struct Move {
	int r;
	int c;
} Move;

#endif
