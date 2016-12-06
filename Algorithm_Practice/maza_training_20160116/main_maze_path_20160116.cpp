#include <iostream>
#include <stdio.h>

#include "maze_path_20160116.h"

// 1시간 10분 걸림
using namespace std;

int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int N;
int AnswerLength;
int Answer[MAX_PATH_SIZE*2];


Move dir[8];
MapElement path[MAX_PATH_SIZE];		// stack
int top = -1;

int main(void)
{
	int const test_case = N_TEST_CASE;

	for (int t = 1; t <= test_case; t++) {
		// 입력:
		cin >> N;

		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				cin >> map[i][j];
			}
		}

		// 벽을 설치
		for (int i = 0; i <= N+1; i++) {
			map[0][i] = D_WALL;
			map[N+1][i] = D_WALL;
			map[i][0] = D_WALL;
			map[i][N+1] = D_WALL;

		}
#if DEBUG_LEVEL > 0
		print_map();
		cout << endl;
#endif

		//======================================
		AnswerLength = solve(Answer);
		cout << "#" << t << " ";
		for (int i = 0; i < AnswerLength; i++) {
			cout << Answer[i*2] << " " << Answer[i*2 + 1] << " ";
		}
		cout << endl;
	}

	return 0;
}

int solve(int Answer[MAX_PATH_SIZE])
{

	init_move();
	init_stack();
	//----------------------------------
	bool loop_flag = true;
	MapElement elm;
	int cur_r, cur_c, i_dir;
	int next_r, next_c;


	path[0].r = 1; path[0].c = 1; path[0].dir = 0;
	top = 0;

	map[1][1] = D_VISIT;

	while ( top >= 0 && loop_flag ) {
		elm = pop();
#if DEBUG_LEVEL > 2
		print_map();
		cout << endl;
#endif
		cur_r = elm.r; cur_c = elm.c;
		i_dir = elm.dir;

		while (i_dir < 8 && loop_flag) {
			// 다음 방향 확인.
			next_r = cur_r + dir[i_dir].r;
			next_c = cur_c + dir[i_dir].c;

			// 갈수 있나?
			if (map[next_r][next_c] == D_EMPTY) {
				/// yes:

				// 돌아왔을 때 가볼 방향을 스택에 넣는다.
				elm.r = cur_r; elm.c = cur_c;
				elm.dir = i_dir + 1;
				push(elm);

				// 이동
				cur_r = next_r; cur_c = next_c;
				i_dir = 0;
				map[cur_r][cur_c] = D_VISIT;
#if DEBUG_LEVEL > 2
		print_map();
		cout << endl;
#endif
				// 만약 이동한 곳이 출구라면:
				if ( next_r == N && next_c == N ) {
					// 끝!
					elm.r = next_r; elm.c = next_c;
					elm.dir = 0;
					
					push(elm);

					loop_flag = false;
					break;
				}

			} else {
				i_dir++; // next direction
			}
		}

		if (i_dir >= 8 ) map[cur_r][cur_c] = D_NOPATH;
	}

#if DEBUG_LEVEL > 0
		print_map();
		cout << endl;
#endif

	set_answer();
	return top+1;
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





void init_move()
{
	dir[0].r =  0; dir[0].c =  1;   // ->
	dir[1].r =  1; dir[1].c =  1;   // 
	dir[2].r =  1; dir[2].c =  0;   // |v
	dir[3].r =  1; dir[3].c = -1;   // 
	dir[4].r =  0; dir[4].c = -1;   // <-
	dir[5].r = -1; dir[5].c = -1;   //
	dir[6].r = -1; dir[6].c =  0;   // |^
	dir[7].r = -1; dir[7].c =  1;   //
}

void init_stack()
{
	top = -1;
}

void push(MapElement const &elm)
{
	top++;

	// Error check!


	path[top].r = elm.r;
	path[top].c = elm.c;
	path[top].dir = elm.dir;
}

MapElement pop()
{
	//if (top < 0) {
	//	cerr << "Error: Stack Empty!" << endl;
	//	exit(1);
	//}
	return path[top--];
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

 