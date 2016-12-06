#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// https://www.acmicpc.net/problem/2178

// 실패 타임아웃
// 깊이우선이 아니라 너비우선으로 풀어야겠다.


using namespace std;

//#define SAQ	
#define ACMICPC 

#define DEBUG_LEVEL 0
#define N_TEST_CASE 1

#define MAX_MAP_SIZE	102
#define MAX_STACK_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE))

#define D_EMPTY 1
#define D_WALL 0
#define D_VISIT 2
#define D_NOPATH 3

typedef struct Move {
	int r;
	int c;
} Move;

typedef struct MoveInfo {
	int r;
	int c;
	int i_dir;
} MoveInfo;


Move dir4[4];
MoveInfo path[MAX_STACK_SIZE];
int top;

int N, M;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];

int Answer;


void init_stack(void)
{
	top = -1;
}

void push(MoveInfo const & elm)
{
	// check full //


	top++;
	path[top].r = elm.r;
	path[top].c = elm.c;
	path[top].i_dir = elm.i_dir;
}

MoveInfo pop(void)
{
	// check empty //

	return path[top--];
}

void init_move(void)
{
	dir4[0].r =  0; dir4[0].c =  1;
	dir4[1].r =  1; dir4[1].c =  0;
	dir4[2].r =  0; dir4[2].c = -1;
	dir4[3].r = -1; dir4[3].c =  0;
}

bool is_movable(int next_r, int next_c)
{
	if ( map[next_r][next_c] == D_EMPTY ) return true;
	else return false;
}

void print_map(void)
{
	for (int i = 0; i <= N+1; i++) {
		for (int j = 0; j <= M+1; j++) {
			printf("%2d", map[i][j]);
		}
		cout << endl;
	}
}

int solve(void)
{
	int loop_flag = true;
	int min_cnt = 1000000;
	int cnt;
	bool found = false;

	int cur_r, cur_c;
	int i_dir;

	int next_r, next_c;
	MoveInfo elm;

	init_move();
	init_stack();

	elm.r = 1; elm.c = 1;
	elm.i_dir = 0;

	push(elm);
	map[1][1] = D_VISIT;
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif

	while ( top >= 0  && loop_flag) {
		elm = pop();

		cur_r = elm.r; cur_c = elm.c;
		i_dir = elm.i_dir;

		while ( i_dir < 4 ) {
			next_r = cur_r + dir4[i_dir].r;
			next_c = cur_c + dir4[i_dir].c;

			if ( is_movable(next_r, next_c) ) {
				
				elm.r = cur_r; elm.c = cur_c;
				elm.i_dir = i_dir + 1;
				push(elm);

				// 이동
				cur_r = next_r; cur_c = next_c;
				i_dir = 0;
				map[cur_r][cur_c] = D_VISIT;
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
				if (found == true) {
					if (top + 2 >= min_cnt) 
						break;
				}

				if ( cur_r == N && cur_c == M ) {
					if (top + 2 < min_cnt)
						min_cnt = top + 2;

					//
					//loop_flag = false;
					map[cur_r][cur_c] = D_EMPTY;
					break;
				}
				
			} else {
				i_dir++;
			}

		}

		if (i_dir >= 4) {
			//map[cur_r][cur_c] = D_NOPATH;
			map[cur_r][cur_c] = D_EMPTY;
		}
	}

#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif

	return min_cnt;
}


void read_input_acmicpc(void)
{
	char buf[MAX_MAP_SIZE+2];
	char buf2[2];
	char ch;
	int cnt_c = 0;
	int cnt_r = 0;
	int r = 1, c = 1;
	while ( r <= N ) {
		c = 1;

		while ( c <= M ) {
			cin >> ch;

			if (ch == '0' || ch == '1') {
				buf2[0] = ch;
				buf2[1] = '\0';
				map[r][c] = atoi(buf2);
				c++;
			}
		}
		r++;
	}

	//for (int i = 1; i <= N+1; i++) {
	//	cin >> ch;

	//	if ( ch == '\n' ) continue;
	//	for (int j = 1; j <= M; j++) {
	//		buf2[0] = ch;
	//		buf2[1] = '\0';
	//		map[i][j] = atoi(buf2);
	//	}
	//}
}


int main(void)
{
	int n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;
		cin >> M;

#ifdef SAQ
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++) {
				cin >> map[i][j];
			}
		}

		// 둘레벽
		for (int r = 0 ; r <= N+1; r++) {
			map[r][0] = D_WALL;
			map[r][M+1] = D_WALL;
		}
		for (int c = 0 ; c <= M+1; c++) {
			map[0][c] = D_WALL;
			map[N+1][c] = D_WALL;
		}
#else
		read_input_acmicpc();
#endif

#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif

		Answer = solve();
#ifdef SAQ
		cout << "#" << i_tc << " ";
#endif
		cout << Answer;
		cout << endl;
	}

	return 0;
}
