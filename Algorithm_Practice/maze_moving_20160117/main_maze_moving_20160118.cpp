#include <iostream>
#include <stdio.h>

#include "maze_moving_20160118.h"

// 2시간 10분 걸림

using namespace std;

Move dir4[4];
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];

MapElement Q[MAX_Q_SIZE];
int head;
int tail;
int n_Qelm;

int N_R, N_C;
int Answer;

int main(void)
{
	int const test_case = N_TEST_CASE;
	Position red;
	Position blue;

	init_move();

	for (int i_test_case = 1; i_test_case <= test_case; i_test_case++) {
		// 입력:
		cin >> N_R;
		cin >> N_C;

		for (int i = 0; i < N_R; i++) {
			for (int j = 0; j < N_C; j++) {
				cin >> map[i][j];
				if (map[i][j] == D_RED) {
					red.r = i;
					red.c = j;
					map[i][j] = D_EMPTY;

				} else if (map[i][j] == D_BLUE) {
					blue.r = i;
					blue.c = j;
					map[i][j] = D_EMPTY;

				}
			}
		}	// end of reading map

		/// 
		Answer = solve(red, blue);

		cout << "#" << i_test_case << " ";
		cout << Answer;
		cout << endl;

	}

	return 0;
}


int solve(Position red, Position blue)
{
	int n_move = 0;
	int i_dir;
	bool loop_flag = true;
	MapElement elm;

	initQ();

	add4Q(red, blue, n_move);

	while ( !is_emptyQ() ) {
		elm = deleteQ();

		red = elm.red;
		blue = elm.blue;
		i_dir = elm.i_dir;
		n_move = elm.n_move;

#if DEBUG_LEVEL > 1
	print_map(red, blue);
	cout << "step:" << n_move;
	cout << endl;
#endif

		if ( is_movable(red, blue, i_dir) == true && n_move < N_MAX_MOVE) {
			take_move(&red, &blue, i_dir, &n_move);
#if DEBUG_LEVEL > 1
	print_map(red, blue);
	cout << "step:" << n_move;
	cout << endl;
#endif

			// 도착 확인
			if (map[red.r][red.c] == D_EXIT) {
				loop_flag = false;
				//break;
				return n_move;
			}

			add4Q(red, blue, n_move);
		}
	}

	return 0;
}

//bool is_movable(MapElement elm) 
//{
//	bool flag;
//	int flag_red;
//	int flag_blue;
//
//	Position red, blue;
//	int i_dir;
//
//	red = elm.red;
//	blue = elm.blue;
//	i_dir = elm.i_dir;
//
//	flag_red = take_move1(&red, i_dir);
//	flag_blue = take_move1(&blue, i_dir);
//	if (flag_red == 0 && flag_blue == 0) {
//		// 둘다 이동 없음.
//		return false;
//	}
//
//	if (map[blue.r][blue.c] == D_EXIT) {
//		// 파란공이 출구에 도착하면
//		return false;
//	}
//
//	if (blue.r == red.r && blue.c == red.c) {
//		// 충돌
//		return false;
//	}
//	return true;
//}
bool is_movable(Position red, Position blue, int i_dir)
{
	bool flag;
	int flag_red;
	int flag_blue;

	flag_red = take_move1(&red, i_dir);
	flag_blue = take_move1(&blue, i_dir);
	if (flag_red == 0 && flag_blue == 0) {
		// 둘다 이동 없음.
		return false;
	}

	if (map[blue.r][blue.c] == D_EXIT) {
		// 파란공이 출구에 도착하면
		return false;
	}

	if (blue.r == red.r && blue.c == red.c) {
		// 충돌
		return false;
	}
	return true;
}

int take_move1(Position *pos, int i_dir)
{
	int next_r, next_c;
	next_r = pos->r + dir4[i_dir].r;
	next_c = pos->c + dir4[i_dir].c;

	if (map[next_r][next_c] == D_EMPTY || map[next_r][next_c] == D_EXIT) {
		pos->r = next_r;
		pos->c = next_c;

		return 1;
	}
	else {
		return 0;
	}
}

void take_move(Position *red, Position *blue, int i_dir, int *n_move)
{

	//int next_r, next_c;

	//next_r = red->r + dir4[i_dir].r;
	//next_c = red->c + dir4[i_dir].c;
	//if (map[next_r][next_c] == D_EMPTY) {
	//	red->r = next_r;
	//	red->c = next_c;
	//}

	//next_r = blue->r + dir4[i_dir].r;
	//next_c = blue->c + dir4[i_dir].c;
	//if (map[next_r][next_c] == D_EMPTY) {
	//	blue->r = next_r;
	//	blue->c = next_c;
	//}
	take_move1(red, i_dir);
	take_move1(blue, i_dir);
	(*n_move)++;
}

void print_map(Position red, Position blue)
{
	for (int i = 0; i < N_R; i++) {
		for (int j = 0; j < N_C; j++) {
			if (i == red.r && j == red.c) {
				printf("%2d", D_RED);
			} else if (i == blue.r && j == blue.c) {
				printf("%2d", D_BLUE);
			} else {
				printf("%2d", map[i][j]);
			}
		}
		cout << endl;
	}
}

void init_move(void)
{
	dir4[0].r =  0; dir4[0].c =  1;	// ->
	dir4[1].r =  1; dir4[1].c =  0;	// |v
	dir4[2].r =  0; dir4[2].c = -1;	// <-
	dir4[3].r = -1; dir4[3].c =  0;	// |^
}

void initQ(void)
{
	head = 0;
	tail = 0;
	n_Qelm = 0;
}

void add4Q(Position red, Position blue, int n_move)
{
	MapElement elm;

	elm.red = red;
	elm.blue = blue;
	elm.n_move = n_move;

	for (int i = 0; i < 4; i++) {
		elm.i_dir = i;
		addQ(elm);
	}
}

void addQ(MapElement const &elm)
{
	// 에러체크:
	if ( n_Qelm > MAX_Q_SIZE) {
		// Error!
		cerr << "Error: MAX Q!" << endl;
	}

	Q[tail].red = elm.red;
	Q[tail].blue = elm.blue;
	Q[tail].i_dir = elm.i_dir;
	Q[tail].n_move = elm.n_move;

	tail = (tail + 1) % MAX_Q_SIZE;

	n_Qelm++;
}

MapElement deleteQ(void)
{
	int tmp_head = head;

	if ( n_Qelm <= 0 ) {
		cerr << "Error: Empty Q!" << endl;
	}

	n_Qelm--;
	head = (head + 1) % MAX_Q_SIZE;
	return Q[tmp_head];
}

bool is_emptyQ(void)
{
	if (n_Qelm > 0) {
		return false; // Not Empty
	} else {
		return true;
	}
}


