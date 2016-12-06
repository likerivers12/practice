#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// 1시간 00분 걸림
// 그런데 되돌아갈 때 뭔가 조치가 필요할 듯...
// : 벽으로 표시?

using namespace std;


#define DEBUG_LEVEL 2
 
//#define ACMICPC
//#define SAQ		

#define D_EMPTY	0
#define D_WALL  1
#define D_VISIT_TO_EMPTY 2
#define D_VISIT_TO_WALL  3
#define D_NOPATH 9

#define N_TEST_CASE	1
#define MAX_MAP_SIZE	1000
#define MAX_STACK_SIZE	(MAX_MAP_SIZE * MAX_MAP_SIZE)

int N, M;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int Answer;

typedef struct Move {
	int r;
	int c;
} Move;

Move dir4[4];

typedef struct MoveInfo {
	int r;
	int c;
	int i_dir;
	bool destroyable;	// destroy_p: true (깰 수 있다), false (깰 수 없다)
} MoveInfo;

MoveInfo path[MAX_STACK_SIZE];
int top;

void init_stack(void)
{
	top = -1;
}

void push(MoveInfo const & elm)
{
	// check full:
	if (top > MAX_STACK_SIZE) {
		cerr << "Error!: full stack" << endl;
		exit(1);
	}

	top++;
	path[top].r			= elm.r;
	path[top].c			= elm.c;
	path[top].destroyable = elm.destroyable;
	path[top].i_dir		= elm.i_dir;	
}

MoveInfo pop(void)
{
	// check empty:
	if (top < 0) {
		cerr << "Error!: empty stack" << endl;
		exit(1);
	}

	return path[top--];
}

void init_move(void)
{
	dir4[0].r =  0; dir4[0].c =  1;
	dir4[1].r =  1; dir4[1].c =  0;
	dir4[2].r =  0; dir4[2].c = -1;
	dir4[3].r = -1; dir4[3].c =  0;
}

bool is_movable(int next_r, int next_c, bool destroyable)
{
	if ( next_r < 1 || next_r > N ) return false;
	if ( next_c < 1 || next_c > M ) return false;

	if ( map[next_r][next_c] == D_EMPTY ) return true;
	if ( map[next_r][next_c] == D_WALL && destroyable ) return true;

	return false;
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
}


int solve(void)
{
	MoveInfo elm;
	int shortest_dist = MAX_MAP_SIZE*MAX_MAP_SIZE;
	bool found = false;

	int cur_r;
	int cur_c;
	int i_dir;
	bool cur_destroyable;

	int next_r;
	int next_c;
	bool next_destroyable;

	init_move();
	init_stack();

	elm.r = 1; elm.c = 1;
	elm.i_dir = 0;
	elm.destroyable = true;

	push(elm);
	map[1][1] = D_VISIT_TO_EMPTY;

	while ( top >= 0 ) {
		elm = pop();

#if DEBUG_LEVEL > 1
		print_map();
		cout << endl;
#endif
		cur_r = elm.r; cur_c = elm.c;
		cur_destroyable = elm.destroyable;
		i_dir = elm.i_dir;

		while (i_dir < 4) {

			next_r = cur_r + dir4[i_dir].r;
			next_c = cur_c + dir4[i_dir].c;

			if ( is_movable(next_r, next_c, cur_destroyable) ) {
				// 현재 정보를 스택에 넣고
				elm.r = cur_r; elm.c = cur_c;
				elm.destroyable = cur_destroyable;
				elm.i_dir = i_dir + 1;
				push(elm);

				// 이동
				cur_r = next_r;
				cur_c = next_c;
				i_dir = 0;
				cur_destroyable;
				if (map[cur_r][cur_c] == D_WALL) {
					cur_destroyable = false;
					map[cur_r][cur_c] = D_VISIT_TO_WALL;
				} else {
					map[cur_r][cur_c] = D_VISIT_TO_EMPTY;
				}

#if DEBUG_LEVEL > 1
		print_map();
		cout << endl;
#endif
				// 목적지 도착 확인
				if (cur_r == N && cur_c == M) {

					if (top+1 +1 < shortest_dist)
						shortest_dist = top+1 +1;

					found = true;
					break;
				}

			} else {
				i_dir++;
			}
		}

		if (i_dir == 4)
			map[cur_r][cur_c] = D_NOPATH;

	}

	if (found == true)	return shortest_dist;
	else				return -1;
}


int main(void)
{
	int n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;
		cin >> M;

#ifdef SAQ
		for (int i = 1; i <= N; i++) 
			for (int j = 1; j <= M; j++) 
				cin >> map[i][j];
#else
		read_input_acmicpc();
#endif

		// 테두리
		for (int r = 0; r <= N+1; r++) {
			map[r][0] = D_WALL;
			map[r][M+1] = D_WALL;
		}

		for (int c = 0; c <= M+1; c++) {
			map[0][c] = D_WALL;
			map[N+1][c] = D_WALL;
		}

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
