#include <iostream>
#include <stdio.h>

using namespace std;

#define DEBUG_LEVEL 1

#define N_TEST_CASE 6

#define MAX_MAP_SIZE	(100+2)
#define MAX_Q_SIZE		((MAX_MAP_SIZE)*(MAX_MAP_SIZE)*10)

#define D_EMPTY	0
#define D_RED	1
#define D_BLUE	2
#define D_WALL  3
#define D_EXIT	4

typedef struct Move {
	int r;
	int c;
} Move;
Move dira[4];

void init_move(void)
{
	dira[0].r =  0;		dira[0].c =  1;		// E
	dira[1].r =  1;		dira[1].c =  0;		// S
	dira[2].r =  0;		dira[2].c = -1;		// W
	dira[3].r = -1;		dira[3].c =  0;		// N

}

typedef struct Position {
	int r;
	int c;
} Position;


typedef struct PositionInfo {
	Position red;
	Position blue;
	int n_move;
} PositionInfo;


PositionInfo Q[MAX_Q_SIZE];
int front = 0;
int rear = 0;
int n_Qelm = 0;


int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
Position red;
Position blue;
int N;
int M;


void init_Q(void)
{
	front = 0;
	rear = 0;
	n_Qelm = 0;
}

void addQ(PositionInfo const & elm)
{
	if (n_Qelm >= MAX_Q_SIZE) {
		cerr << "Error! Full Queue!" << endl;
		exit(1);
	}

	Q[rear].red.r = elm.red.r;
	Q[rear].red.c = elm.red.c;
	Q[rear].blue.r = elm.blue.r;
	Q[rear].blue.c = elm.blue.c;
	Q[rear].n_move = elm.n_move;

	rear = (rear + 1) % MAX_Q_SIZE;
	n_Qelm++;
}

PositionInfo delQ(void)
{
	if (n_Qelm < 0) {
		cerr << "Error! Empty Queue!" << endl;
		exit(1);
	}

	int tmp = front;
	front = (front + 1) % MAX_Q_SIZE;
	n_Qelm--;

	return Q[tmp];
}

bool is_emptyQ(void)
{
	if (n_Qelm > 0) return false;
	return true;
}

void print_map(void)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (i == red.r && j == red.c) 
				printf("%2d ", D_RED);
			else if (i == blue.r && j == blue.c)
				printf("%2d ", D_BLUE);
			else
				printf("%2d ", map[i][j]);
		}
		cout << endl;
	}
	cout << endl;
}

//bool take_amove(Position * pos, int i_dir)
//{
//	int next_r;
//	int next_c;
//
//	next_r = pos->r + dira[i_dir].r;
//	next_c = pos->c + dira[i_dir].c;
//
//	if (map[next_r][next_c] == D_WALL) return false;
//	
//	pos->r = next_r;
//	pos->c = next_c;
//	return true;
//}

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

//bool is_movable(PositionInfo const & elm, int i_dir)
//{
//	Position red = elm.red;
//	Position blue = elm.blue;
//	bool flag_red;
//	bool flag_blue;
//
//	flag_red = take_amove(&red, i_dir);
//	flag_blue = take_amove(&blue, i_dir);
//
//	if (flag_red == false && flag_blue == false)
//		return false;		// 둘다 이동 없음.
//	if (red.r == blue.r && red.c == blue.c)
//		return false;		// 충돌
//	if (map[blue.r][blue.c] == D_EXIT)
//		return false;
//
//	return true;
//}
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

int solve(void)
{
	init_Q();

	int n_move = 0;
	PositionInfo elm;
	PositionInfo next_elm;

	elm.red = red;
	elm.blue = blue;
	elm.n_move = n_move;
	addQ(elm);

	while (!is_emptyQ()) {
		elm = delQ();
		//n_move = elm.n_move;

#if DEBUG_LEVEL >= 2
		print_map();
#endif

		if (n_move >= 10)
			continue;

		for (int i_dir = 0; i_dir < 4; i_dir++) {
			red = elm.red;
			blue = elm.blue;
			n_move = elm.n_move;

			//if (is_movable(elm, i_dir)) {
			if (is_movable(red, blue, i_dir)) {
				take_amove(&red, i_dir);
				take_amove(&blue, i_dir);
				n_move++;

#if DEBUG_LEVEL >= 2
				print_map();
#endif
				next_elm.red = red;
				next_elm.blue = blue;
				next_elm.n_move = n_move;

				addQ(next_elm);

				if (map[red.r][red.c] == D_EXIT) {
					return n_move;
				}
			}
		}
	}

	return -1;
}


int main(void)
{
	int const n_case = N_TEST_CASE;

	init_move();

	for (int i_case = 0; i_case < n_case; i_case++) {
		cin >> N;
		cin >> M;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> map[i][j];

				if (map[i][j] == D_RED) {
					red.r = i;
					red.c = j;
					map[i][j] = D_EMPTY;
				}
				else if (map[i][j] == D_BLUE) {
					blue.r = i;
					blue.c = j;
					map[i][j] = D_EMPTY;
				}
			}
		}


#if DEBUG_LEVEL >= 1
		print_map();
#endif

		int n_move = solve();

#if DEBUG_LEVEL >= 1
		print_map();
#endif
		cout << "#" << i_case + 1 << " " << n_move << endl;

	}	

	return 0;
}