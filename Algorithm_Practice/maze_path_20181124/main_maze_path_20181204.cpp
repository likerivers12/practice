//#define RUN_20181204

#ifdef RUN_20181204

// 09:55 -> 10:15
// 09:50 -> 10:34
// -> 디버깅까지 한 시간 정도 걸림.

#include <stdio.h>
#include <iostream>

using namespace std;


#define MAX_STACK_SIZE		2048
#define MAX_MAP_SIZE		20

#define M_EMPTY		0
#define M_WALL		1
#define M_VISITED	2
#define M_NOPATH	3

struct Dir
{
	int r;
	int c;
};

struct MoveInfo
{
	int r;
	int c;
	int i_dir;
};

struct Maze
{
	int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
	Dir dir8[8];

	MoveInfo stack[MAX_STACK_SIZE];
	int top;

	int NR;
	int NC;

	bool is_empty_stack()
	{
		if (top >= 0) return false;
		return true;
	}

	void init_stack()
	{
		top = -1;
	}

	int push(MoveInfo & elm)
	{
		if (++top >= MAX_STACK_SIZE) {
			printf("STACK FULL!\n");
			return -1;
		}

		stack[top] = elm;
		return 0;
	}

	MoveInfo pop()
	{
		if (top < 0) {
			printf("STACK EMPTY!\n");
		}
		return stack[top--];
	}

	void find_path()
	{
		MoveInfo elm;
		MoveInfo cur_elm;
		int cur_r, cur_c, i_dir;
		int next_r, next_c;

		bool loop_flag = true;

		elm.r = 1; elm.c = 1; elm.i_dir = 0;
		push(elm);
		map[1][1] = M_VISITED;

		while (top >= 0 && loop_flag == true) {
			elm = pop();
			cur_r = elm.r;
			cur_c = elm.c;
			i_dir = elm.i_dir;

			print_map();

			while (i_dir < 8) {
				next_r = cur_r + dir8[i_dir].r;
				next_c = cur_c + dir8[i_dir].c;

				if (map[next_r][next_c] == M_EMPTY) {
					// 현재위치, 다음 고려 방향을 푸시 (되돌아 왔을 때의 정보)
					elm.r = cur_r;
					elm.c = cur_c;
					elm.i_dir = i_dir + 1;
					push(elm);

					map[cur_r][cur_c] = M_VISITED;
					print_map();

					// 이동
					cur_r = next_r;
					cur_c = next_c;
					i_dir = 0;

					// 목적지 도착 확인:
					if (cur_r == NR - 2 && cur_c == NC - 2) {
						//도착했음!

						elm.r = cur_r;
						elm.c = cur_c;
						elm.i_dir = 0;
						push(elm);
						map[cur_r][cur_c] = M_VISITED;
						print_map();
						loop_flag = false;
						break;
					}					
				}
				else {
					i_dir++;
				}
			}

			map[cur_r][cur_c] = M_NOPATH;
		}

		return;
	}

	void print_path()
	{
		cout << "========================================" << endl;
		for (int i = 0; i <= top; i++) {
			printf("(%d, %d)\n", stack[i].r, stack[i].c);
		}
		cout << endl;
	}

	void init_dir()
	{
		dir8[0].r = 0;		dir8[0].c = 1;		// ->
		dir8[1].r = 1;		dir8[1].c = 1;		// 남동
		dir8[2].r = 1;		dir8[2].c = 0;		// v
		dir8[3].r = 1;		dir8[3].c = -1;		// 남서
		dir8[4].r = 0;		dir8[4].c = -1;		// <-
		dir8[5].r = -1;		dir8[5].c = -1;		// 북서
		dir8[6].r = -1;		dir8[6].c = 0;		// ^
		dir8[7].r = -1;		dir8[7].c = 1;		// 북동
	}

	void print_map()
	{
		int i, j;
		cout << "========================================" << endl;
		for (i = 0; i < NR; i++) {
			for (j = 0; j < NC; j++) {
				printf("%2d ", map[i][j]);
			}
			cout << endl;
		}
	}
};

Maze maze;

void run_20181204()
{
	scanf("%d", &(maze.NR));
	maze.NR += 2;
	maze.NC = maze.NR;

	int i;
	int j;
	for (i = 1; i < maze.NR - 1; i++) {
		for (j = 1; j < maze.NC - 1; j++) {
			scanf("%d", &maze.map[i][j]);
		}
	}

	for (i = 0; i < maze.NR; i++) {
		maze.map[i][0] = M_WALL;
		maze.map[i][maze.NC - 1] = M_WALL;
	}

	for (j = 0; j < maze.NC; j++) {
		maze.map[0][j] = M_WALL;
		maze.map[maze.NR - 1][j] = M_WALL;
	}


	maze.print_map();
	maze.init_dir();
	maze.init_stack();
	maze.find_path();
	maze.print_path();
}

int main(void)
{

	run_20181204();

	return 0;
}
#endif
