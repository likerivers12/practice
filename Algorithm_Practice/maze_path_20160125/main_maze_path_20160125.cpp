#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// 40ºÐ °É¸²

using namespace std;

#define DEBUG_LEVEL 2
#define N_TEST_CASE	1

#define MAX_MAP_SIZE	102
#define MAX_STACK_SIZE	((MAX_MAP_SIZE)*(MAX_MAP_SIZE))
#define D_EMPTY	0
#define D_WALL 1
#define D_VISIT 2
#define D_NOPATH 3

#define N_DIRECTIONS	8

typedef struct Move {
	int r;
	int c;
} Move;


typedef struct MapInfo {
	int r;
	int c;
	int i_dir;
} MapInfo;

Move dir4[4];
Move dira[N_DIRECTIONS];

MapInfo path[MAX_STACK_SIZE];
int top;

int N;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];

int Answer[MAX_MAP_SIZE*MAX_MAP_SIZE];

void init_move(void) 
{
	//dir4[0].r =  0; dir4[0].c =  1;
	//dir4[1].r =  1; dir4[1].c =  0;
	//dir4[2].r =  0; dir4[2].c = -1;
	//dir4[3].r = -1; dir4[3].c =  0;

	dira[0].r =  0; dira[0].c =  1;  // ->
	dira[1].r =  1; dira[1].c =  1;
	dira[2].r =  1; dira[2].c =  0;  // |v
	dira[3].r =  1; dira[3].c = -1;  // /
	dira[4].r =  0; dira[4].c = -1;  // <-
	dira[5].r = -1; dira[5].c = -1;
	dira[6].r = -1; dira[6].c =  0;  // |^
	dira[7].r = -1; dira[7].c =  1;
}

void init_stack(void)
{
	top = -1;
}

void push(MapInfo const & elm)
{
	// check full
	if ( top >= MAX_STACK_SIZE ) {
		cerr << "Error! stack full!" << endl;
		// exit(1);
	}

	top++;
	path[top].r = elm.r;
	path[top].c = elm.c;
	path[top].i_dir = elm.i_dir;
}

MapInfo pop(void)
{
	// check empty
	if (top < 0) {
		cerr << "Error! stack empty!" << endl;
		// exit(1);
	}

	return path[top--];
}

bool is_empty_stack(void)
{
	if (top < 0)	return true;
	else			return false;
}

bool is_movable(int next_r, int next_c)
{
	if (map[next_r][next_c] == D_EMPTY ) return true;
	else								 return false;
}

void set_Answer(void)
{
	int cnt = 0;

	for (int i = 0; i <= top; i++) {
		Answer[cnt] = path[i].r;
		cnt++;
		Answer[cnt] = path[i].c;
		cnt++;
	}
}

void print_map(void)
{
	for (int i = 0; i <= N+1; i++) {
		for (int j = 0; j <= N+1; j++) {
			printf("%2d", map[i][j]);
		}
		cout << endl;
	}
}

int solve(void)
{
	int length;
	bool loop_flag = true;

	MapInfo elm;
	int cur_r, cur_c;
	int next_r, next_c;
	int i_dir;

	init_move();
	init_stack();

	elm.r = 1; elm.c = 1;
	elm.i_dir = 0;
	push(elm);
	map[elm.r][elm.c] = D_VISIT;

	while ( !is_empty_stack() && loop_flag ) {
		elm = pop();

		cur_r = elm.r; cur_c = elm.c;
		i_dir = elm.i_dir;

		while (i_dir < N_DIRECTIONS) {

			//next_r = cur_r + dir4[i_dir].r;
			//next_c = cur_c + dir4[i_dir].c;
			next_r = cur_r + dira[i_dir].r;
			next_c = cur_c + dira[i_dir].c;
			if ( is_movable(next_r, next_c) ) {

				// 
				elm.r = cur_r; elm.c = cur_c;
				elm.i_dir = i_dir + 1;
				push(elm);

				// move
				cur_r = next_r; cur_c = next_c;
				i_dir = 0;

				map[cur_r][cur_c] = D_VISIT;
#if DEBUG_LEVEL > 1
		print_map();
		cout << endl;
#endif

				if (cur_r == N && cur_c == N) {
					// 
					elm.r = cur_r; elm.c = cur_c;
					elm.i_dir = 0;

					push(elm);
					loop_flag = false;
					break;
				}
			} else {
				i_dir++;
			}
		}

		if (i_dir >= 4) {
			map[cur_r][cur_c] = D_NOPATH;
		}
#if DEBUG_LEVEL > 1
		print_map();
		cout << endl;
#endif
	}


	set_Answer();
	length = top+1;
	return length;
}

int main(void)
{
	int const n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;

		for (int i = 1; i <= N; i++) 
			for (int j = 1; j <= N; j++) 
				cin >> map[i][j];

		for (int r = 0; r <= N+1; r++) {
			map[r][0] = D_WALL;
			map[r][N+1] = D_WALL;
		}

		for (int c = 0; c <= N+1; c++) {
			map[0][c] = D_WALL;
			map[N+1][c] = D_WALL;
		}


		int length = solve();
		cout << "#" << i_tc << " ";
		if (length > 0) {
			for (int i = 0; i < length; i++) {
				cout << Answer[i*2] << " " << Answer[i*2+1] << " ";
			}
		} else {
			cout << 0;
		}
		cout << endl;
	}


	return 0;
}
