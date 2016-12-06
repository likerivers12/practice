#ifndef MAZE_MOVING_H
#define MAZE_MOVING_H

#define DEBUG_LEVEL 2
#define N_TEST_CASE 3

#define MAX_Q_SIZE (102*102)
#define MAX_MAP_SIZE 102
#define N_MAX_MOVE 10

#define D_EMPTY 0
#define D_RED 1
#define D_BLUE 2
#define D_WALL 3
#define D_EXIT 4

typedef struct Move {
	int r;
	int c;
} Move;

typedef struct Position {
	int r;
	int c;
} Position;

typedef struct MapElement {
	Position red;
	Position blue;
	int i_dir;
	int n_move;
} MapElement;




int solve(Position red, Position blue);

bool is_emptyQ(void);
MapElement deleteQ(void);
void add4Q(Position red, Position blue, int n_move);
void addQ(MapElement const &elm);
void initQ(void);

void init_move(void);
int take_move1(Position *pos, int i_dir);
void take_move(Position *red, Position *blue, int i_dir, int *n_move);
//bool is_movable(MapElement elm);
bool is_movable(Position red, Position blue, int i_dir);

void print_map(Position red, Position blue);

#endif

