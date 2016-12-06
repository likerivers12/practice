#ifndef MAZE_20160116_H
#define MAZE_20160116_H

#define DEBUG_LEVEL 3
#define N_TEST_CASE 1

#define MAX_MAP_SIZE 102
#define MAX_PATH_SIZE (102*102)

#define D_EMPTY 0
#define D_WALL 1
#define D_VISIT 2
#define D_NOPATH 3


typedef struct Move{
	int r;
	int c;
} Move;

typedef struct MapElement
{
	int r;   // 현재 row
	int c;   // 현재 col
	int dir; // 다음 방향
} MapElement;


int solve(int []);
void print_map(void);

void set_answer(void);

void init_move();
void init_stack();
void push(MapElement const &elm);
MapElement pop();

#endif
