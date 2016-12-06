#include <iostream>
#include <stdio.h>

// 35분 걸림
using namespace std;

#define DEBUG_LEVEL 0
#define N_TEST_CASE 8

#define MAX_STACK_SIZE (102*102)
#define MAX_MAP_SIZE 102
typedef struct Move {
	int r;
	int c;
} Move;

Move dir8[8];



#define D_EMPTY 0
#define D_WALL 1
#define D_VISIT 2
#define D_NOPATH 3

typedef struct MapElement {
	int r;
	int c;
	int i_dir;
} MapElement;

MapElement path[MAX_STACK_SIZE];
int top = -1;

int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int N;

int AnswerLength;
int Answer[MAX_MAP_SIZE*2];


void init_move(void)
{
	dir8[0].r =  1; dir8[0].c =  0;  // |v
	dir8[1].r =  1; dir8[1].c = -1;  // 
	dir8[2].r =  0; dir8[2].c = -1;  // <-
	dir8[3].r = -1; dir8[3].c = -1;  // 
	dir8[4].r = -1; dir8[4].c =  0;  // |^
	dir8[5].r = -1; dir8[5].c =  1;  // /
	dir8[6].r =  0; dir8[6].c =  1;  // ->
	dir8[7].r =  1; dir8[7].c =  1;  // 

	//dir8[0].r =  0; dir8[0].c =  1;	// ->
	//dir8[1].r =  1; dir8[1].c =  1; // 
	//dir8[2].r =  1; dir8[2].c =  0; // |v
	//dir8[3].r =  1; dir8[3].c = -1; // /
	//dir8[4].r =  0; dir8[4].c = -1; // <-
	//dir8[5].r = -1; dir8[5].c = -1; // 
	//dir8[6].r = -1; dir8[6].c =  0; // |^
	//dir8[7].r = -1; dir8[7].c =  1; // /

}

void init_stack(void)
{
	top = -1;
}

void push(MapElement const & elm)
{
	top++;
	// full?

	path[top].r = elm.r;
	path[top].c = elm.c;
	path[top].i_dir = elm.i_dir;
}

MapElement pop(void)
{
	// empty?

	int tmp = top;
	top--;

	return path[tmp];
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

void set_answer(void)
{
	int c = 0;

	for (int i = 0; i <= top; i++) {
		Answer[c] = path[i].r;
		c++;
		Answer[c] = path[i].c;
		c++;
	}
}

int solve(void)
{

	MapElement elm;
	bool loop_flag = true;
	int cur_r, cur_c, i_dir;
	int next_r, next_c;

	init_move();

#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif

	// 첫번째 방문
	elm.r = 1; elm.c = 1; elm.i_dir = 0;  // 현재위치 다음 방향.
	push(elm);
	map[1][1] = D_VISIT;

	while ( top >= 0 && loop_flag ) {
		elm = pop();

		cur_r = elm.r; cur_c = elm.c; i_dir = elm.i_dir;
		while ( i_dir < 8 && loop_flag ) {
			next_r = cur_r + dir8[i_dir].r;
			next_c = cur_c + dir8[i_dir].c;

			if (map[next_r][next_c] == D_EMPTY) {
				// 갈수 있다.
				// 되돌아 왔을 때 이동 정보를 스택에 넣는다.
				elm.r = cur_r; elm.c = cur_c; elm.i_dir = i_dir + 1;
				push(elm);

				// 이동
				cur_r = next_r; cur_c = next_c; i_dir = 0;
				map[cur_r][cur_c] = D_VISIT;
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
				// 만약 목적지에 도착했으면:
				if (cur_r == N && cur_c == N) {
					// 목적지를 path에 추가
					elm.r = cur_r; elm.c = cur_c; elm.i_dir = 0;
					push(elm);

					loop_flag = false;
				}

			} else {
				// 갈수 없으면 다음 방향
				i_dir++;
			}
		}

		//
		if (i_dir == 8) {
			map[cur_r][cur_c] = D_NOPATH;
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
		}
	}

#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif

	set_answer();
	return top+1;
}

int main(void)
{
	int const n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;

		for (int i = 1; i <= N; i++) 
			for (int j = 1; j <= N; j++) 
				cin >> map[i][j];

		// 둘레 벽
		for (int i = 0; i <= N+1; i++) {
			map[i][0]	= D_WALL;
			map[i][N+1] = D_WALL;
			map[0][i]	= D_WALL;
			map[N+1][i] = D_WALL;
		}

		AnswerLength = solve();


		cout << "#" << i_tc << " ";
		if (AnswerLength > 0) {
			for (int i = 0; i < AnswerLength; i++) {
				cout << Answer[i*2] << " " << Answer[i*2 + 1] << " ";
			}
		} else {
			cout << 0;
		}
		cout << endl;
	}

	return 0;
}
