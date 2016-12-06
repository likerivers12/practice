#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// 1시간 30분 걸림
// 주의 할 것:
// 1. 지도 읽기
// 2. 이동 가능 확인
// 3. 큐에 정보 삽입

using namespace std;

#define DEBUG_LEVEL 1
#define N_TEST_CASE 6

#define N_DIRECTIONS 4

#define MAX_MAP_SIZE	1002
#define MAX_Q_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE))

#define D_EMPTY 0
#define D_WALL	3
#define D_RED   1
#define D_BLUE  2
#define D_EXIT  4

typedef struct Move {
	int r;
	int c;
} Move;
Move dira[N_DIRECTIONS];

typedef struct Position {
	int r;
	int c;
} Position;

typedef struct PositionInfo {
	Position red;
	Position blue;
	int n_move;
	int i_dir;
} PositionInfo;

PositionInfo Q[MAX_Q_SIZE];
int front;
int rear;
int n_Qelm;

Position red;
Position blue;

int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int N, M;


void init_move(void)
{
	dira[0].r =  0; dira[0].c =  1;
	dira[1].r =  1; dira[1].c =  0;
	dira[2].r =  0; dira[2].c = -1;
	dira[3].r = -1; dira[3].c =  0;
}

void initQ(void)
{
	front = 0;
	rear = 0;
	n_Qelm = 0;
}

void addQ(PositionInfo const & elm)
{
	// check full
	if (n_Qelm > MAX_Q_SIZE) {
		cerr << "Error! full Q!" << endl;
	}

	Q[rear].red.r = elm.red.r;
	Q[rear].red.c = elm.red.c;
	Q[rear].blue.r = elm.blue.r;
	Q[rear].blue.c = elm.blue.c;
	Q[rear].i_dir = elm.i_dir;
	Q[rear].n_move = elm.n_move;
	n_Qelm++;

	rear = (rear + 1) % MAX_Q_SIZE;
}

PositionInfo delQ(void)
{
	int tmp = front;

	front = (front + 1) % MAX_Q_SIZE;
	n_Qelm--;

	return Q[tmp];
}

bool is_emptyQ(void)
{
	if (n_Qelm <= 0) return true;	// empty
	else			 return false;  // not empty
}

void add4Q(PositionInfo elm)
{
	for (int i = 0; i < 4; i++) {
		elm.i_dir = i;
		addQ(elm);
	}
}

bool take_amove(Position *pos, int i_dir)
{
	int next_r;
	int next_c;
	next_r = pos->r + dira[i_dir].r;
	next_c = pos->c + dira[i_dir].c;
	
	if (map[next_r][next_c] == D_EMPTY || map[next_r][next_c] == D_EXIT) {
		pos->r = next_r;
		pos->c = next_c;
		return true;
	}
	else 
		return false;
}

bool is_movable(Position red, Position blue, int i_dir)
{
	bool flag_red;
	bool flag_blue;

	flag_red = take_amove(&red, i_dir);
	flag_blue = take_amove(&blue, i_dir);

	// 둘다 이동 없으면 return false;
	if (flag_red == false && flag_blue == false) 
		return false;

	// 파랑과 빨강이 충돌하면 
	if (red.r == blue.r && red.c == blue.c) 
		return false;

	// 파랑이 출구에 도착하면
	if (map[blue.r][blue.c] == D_EXIT)
		return false;

	return true;
}

void print_map(void)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if ( i == red.r && j == red.c ) {
				printf("%2d", D_RED);
			} else if ( i == blue.r && j == blue.c ) {
				printf("%2d", D_BLUE);
			} else {
				printf("%2d", map[i][j]);
			}
		}
		cout << endl;
	}
}

char get_dir_char(int i_dir)
{
	switch (i_dir) {
	case 0: return 'R';
	case 1: return 'D';
	case 2: return 'L';
	case 3: return 'U';
	}
}

int solve(void)
{
	PositionInfo elm;
	PositionInfo next_elm;

	//Position cur_red;
	//Position cur_blue;
	int i_dir;
	int n_move = 0;

	init_move();
	initQ();

	//elm.red.r = red.r; elm.red.c = red.c;
	//elm.blue.r = blue.r; elm.blue.c = blue.c;
	elm.red = red;
	elm.blue = blue;
	elm.n_move = 0;
	//add4Q(elm);
	addQ(elm);

	while ( !is_emptyQ() && n_move < 10) {
		elm = delQ();

		for (int i_dir = 0; i_dir < 4; i_dir++) {
			red = elm.red;
			blue = elm.blue;
			//i_dir = elm.i_dir;
			n_move = elm.n_move;

#if DEBUG_LEVEL > 1
			print_map();
			cout << get_dir_char(i_dir);
			cout << endl;
#endif

			if ( is_movable(red, blue, i_dir) ) {
				// 이동
				take_amove(&red, i_dir);
				take_amove(&blue, i_dir);
				n_move++;

	#if DEBUG_LEVEL > 1
				print_map();
				cout << endl;
	#endif

				next_elm.red = red;
				next_elm.blue = blue;
				next_elm.n_move = n_move;

				addQ(next_elm);

				// 도착 확인
				if (map[red.r][red.c] == D_EXIT) {
	#if DEBUG_LEVEL > 0
				print_map();
				cout << endl;
	#endif				
					return n_move;
				}
			}
		}
	}

#if DEBUG_LEVEL > 0
			print_map();
			cout << endl;
#endif	
	return 0;
}

int solve_old(void)
{
	PositionInfo elm;

	//Position cur_red;
	//Position cur_blue;
	int i_dir;
	int n_move = 0;

	init_move();
	initQ();

	//elm.red.r = red.r; elm.red.c = red.c;
	//elm.blue.r = blue.r; elm.blue.c = blue.c;
	elm.red = red;
	elm.blue = blue;
	elm.n_move = 0;
	add4Q(elm);

	while ( !is_emptyQ() && n_move < 10) {
		elm = delQ();

		red = elm.red;
		blue = elm.blue;
		i_dir = elm.i_dir;
		n_move = elm.n_move;

#if DEBUG_LEVEL > 1
			print_map();
			cout << get_dir_char(i_dir);
			cout << endl;
#endif
		if ( is_movable(red, blue, i_dir) ) {
			// 이동
			take_amove(&red, i_dir);
			take_amove(&blue, i_dir);
			n_move++;

#if DEBUG_LEVEL > 1
			print_map();
			cout << endl;
#endif

			elm.red = red;
			elm.blue = blue;
			elm.n_move = n_move;

			add4Q(elm);

			// 도착 확인
			if (map[red.r][red.c] == D_EXIT) {
#if DEBUG_LEVEL > 0
			print_map();
			cout << endl;
#endif				
				return n_move;
			}
		}
	}

#if DEBUG_LEVEL > 0
			print_map();
			cout << endl;
#endif	
	return 0;
}

int main(void)
{
	int const n_test_case = N_TEST_CASE;
	
	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;
		cin >> M;

		for (int i = 0; i < N; i++) 
			for (int j = 0; j < M; j++) {
				cin >> map[i][j];

				if (map[i][j] == D_RED) {
					map[i][j] = D_EMPTY;
					red.r = i;
					red.c = j;
				} else if (map[i][j] == D_BLUE) {
					map[i][j] = D_EMPTY;
					blue.r = i;
					blue.c = j;
				}
			}

		//for (int r = 0; r <= N+1; r++) {
		//	map[r][0] = D_WALL;
		//	map[r][M+1] = D_WALL;
		//}
		//for (int c = 0; c <= M+1; c++) {
		//	map[0][c] = D_WALL;
		//	map[N+1][c] = D_WALL;
		//}

#if DEBUG_LEVEL > 0
			print_map();
			cout << endl;
#endif

		int n_move = solve();
		cout << "#" << i_tc << " ";
		cout << n_move;
		cout << endl;
	}

	return 0;
}
