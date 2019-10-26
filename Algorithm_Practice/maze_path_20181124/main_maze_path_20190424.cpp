// 10:40 시작

#define RUN_20190424
#ifdef RUN_20190424
#define DEBUG

#include <stdio.h>
#include <iostream>

using namespace std;


#define EMPTY 0
#define WALL 1
#define VISIT 2
#define NOPATH 3


#define MAX_MAP_SIZE 100
#define MAX_STACK_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE))


struct Dir
{
	int r;
	int c;
};

struct Position
{
	int r;
	int c;
	int next_dir;
};

struct Maze
{
	int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
	int NR;
	int NC;

	Dir dir[8];
	Position stack[MAX_STACK_SIZE];
	int top;


	void init()
	{
		dir[0].r = 0;		dir[0].c = 1;		// ->
		dir[1].r = 1;		dir[1].c = 1;		// \.
		dir[2].r = 1;		dir[2].c = 0;		// v
		dir[3].r = 1;		dir[3].c = -1;		// ./
		dir[4].r = 0;		dir[4].c = -1;		// <-
		dir[5].r = -1;		dir[5].c = -1;		// 
		dir[6].r = -1;		dir[6].c = 0;		// ^
		dir[7].r = -1;		dir[7].c = 1;		// /'

	}

	void init_stack()
	{
		top = -1;
	}

	void push(Position &elm)
	{
		if (top >= MAX_STACK_SIZE - 1) {
			printf("STACK FULL!!!!!!!!!!\n");
		}
		stack[++top] = elm;
	}

	bool is_stack_empty()
	{
		if (top >= 0) return false;
		return true;
	}

	Position pop()
	{
		if (top < 0) {
			printf("STACK EMPTY!!!!!!!!!!!\n");
		}
		return stack[top--];
	}


	void read_map()
	{
		scanf("%d", &NR);
		NR += 2;
		NC = NR;
		
		int i;
		int j;
		for (i = 1; i < NR - 1; i++) {
			for (j = 1; j < NC - 1; j++) {
				scanf("%d", &(this->map[i][j]));
			}
		}

		// WALL L/R
		for (i = 0; i < NR; i++) {
			this->map[i][0] = WALL;
			this->map[i][NC - 1] = WALL;
		}

		// WALL T/B
		for (i = 0; i < NC; i++) {
			this->map[0][i] = WALL;
			this->map[NR - 1][i] = WALL;
		}

	}

	void print_map()
	{
		int i, j;
		for (i = 0; i < NR; i++) {
			for (j = 0; j < NC; j++) {
				printf("%2d ", this->map[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}


	void find_path()
	{
		Position elm;
		int cur_r, cur_c, i_dir;
		int next_r, next_c;

		bool	loop_flag = true;

		elm.r = 1;	elm.c = 1; elm.next_dir = 0;
		map[1][1] = VISIT;
		push(elm);

		while (!is_stack_empty() && loop_flag) {
			elm = pop();
			cur_r = elm.r;
			cur_c = elm.c;
			i_dir = elm.next_dir;

#ifdef DEBUG
			print_map();
#endif
			while( i_dir < 8) {
				next_r = cur_r + dir[i_dir].r;
				next_c = cur_c + dir[i_dir].c;

				if (map[next_r][next_c] == EMPTY) {
					// 이동가능
					
					// 현재위치, 다음 이동방향을 스택에 넣고
					elm.r = cur_r;
					elm.c = cur_c; 
					elm.next_dir = i_dir + 1;
					push(elm);

					// 이동
					cur_r = next_r;
					cur_c = next_c;
					i_dir = 0;

					map[cur_r][cur_c] = VISIT;		//<------
#ifdef DEBUG
					print_map();
#endif
					if (cur_r == NR - 2 && cur_c == NC - 2) {
						// 도착
						loop_flag = false;
						elm.r = cur_c;
						elm.c = cur_c;
						elm.next_dir = 0;
						push(elm);
						map[cur_r][cur_c] = VISIT;
#ifdef DEBUG
						print_map();
#endif

						break;
					}

				}
				else {
					// 이동불가
					i_dir++;
				}
			}

			map[cur_r][cur_c] = NOPATH;

		}


		this->print_map();
		for (int i = 0; i <= top; i++) {
			printf("(%2d, %2d)\n", stack[i].r, stack[i].c);
		}
		printf("\n");
	}

};

int main(void)
{

	//printf("Hello\n");

	Maze m;
	m.init();
	m.init_stack();
	m.read_map();
	m.print_map();

	m.find_path();


	return 0;
}
#endif