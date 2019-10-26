#include <iostream>
#include <stdio.h>

using namespace std;

#define M_EMPTY 0
#define M_RED 1
#define M_BLUE 2
#define M_WALL 3
#define M_EXIT 4


#define MAX_MAP_SIZE	100
#define MAX_Q_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE)*100)

typedef struct Dir4 {
	int r;
	int c;
} Dir4;
Dir4 dir4[4];

typedef struct Position {
	int r;
	int c;
} Position;
Position red;
Position blue;

int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int N, M;

typedef struct PositionInfo {
	Position red;
	Position blue;
	int n_move;
} PositionInfo;

PositionInfo Q[MAX_Q_SIZE];
int front;
int rear;
int n_Qelm;

void initQ()
{
	front = 0;
	rear = 0;
	n_Qelm = 0;
}

void addQ(PositionInfo & elm)
{
	if (n_Qelm >= MAX_Q_SIZE) {
		printf("ERROR! FULL Q\n");
	}

	Q[rear].red.r = elm.red.r;
	Q[rear].red.c = elm.red.c;
	Q[rear].blue.r = elm.blue.r;
	Q[rear].blue.c = elm.blue.c;
	Q[rear].n_move = elm.n_move;
	
	n_Qelm++;
	rear = (rear + 1) % MAX_Q_SIZE;
}

PositionInfo delQ()
{
	if (n_Qelm == 0) {
		printf("ERROR! EMPRY Q\n");
	}

	int tmp = front;

	n_Qelm--;
	front = (front + 1) % MAX_Q_SIZE;
	return Q[tmp];
}

bool isEmptyQ() 
{
	if (n_Qelm == 0) return true;
	return false;
}


void init_dir();
void print_map();

bool take_move(Position * ball, int i)
{
	int r = ball->r + dir4[i].r;
	int c = ball->c + dir4[i].c;

	if (map[r][c] == M_WALL) {
		return false;
	}
	else {
		ball->r = r;
		ball->c = c;
		return true;
	}
}

bool is_movable(Position red, Position blue, int i)
{
	// red 이동
	// blue 이동
	bool flag_red = take_move(&red, i);
	bool flag_blue = take_move(&blue, i);

	// 조건 확인
	if (flag_red == false && flag_blue == false) return false;	// 둘다 이동 없음.
	if (red.r == blue.r && red.c == blue.c) return false;	// 충돌
	if (map[blue.r][blue.c] == M_EXIT) return false;		// blue가 먼저 도착
	
	return true;
}


int solve()
{
	initQ();
	print_map();
	PositionInfo elm, next_elm;
	int n_move;
	elm.red = red;	
	elm.blue = blue; 
	elm.n_move = 0;

	addQ(elm);

	while ( !isEmptyQ()) {
		elm = delQ();

		if (map[elm.red.r][elm.red.c] == M_EXIT || elm.n_move > 20) {
			continue;
		}

		for (int i = 0; i < 4; i++) {
			red = elm.red;
			blue = elm.blue;
			n_move = elm.n_move;

			// i방향으로 이동가능하면 (조건에 맞는 이동)
			if ( is_movable(red, blue, i) ) {
				// 이동
				take_move(&red, i);
				take_move(&blue, i);

				next_elm.red = red;
				next_elm.blue = blue;
				next_elm.n_move = n_move + 1;
				addQ(next_elm);
				// red가 도착했으면 끝
				if (map[red.r][red.c] == M_EXIT) {
					print_map();
					return next_elm.n_move;
				}

			}
		}
	}
	return 0;
}

int main(void)
{
	int N_TEST_CASE;
	int val;

	init_dir();

	cin >> N_TEST_CASE;


	for (int i_case = 0; i_case < N_TEST_CASE; i_case++) {
		cin >> N >> M;

		for (int r = 0; r < N; r++) {
			for (int c = 0; c < M; c++) {
				cin >> val;

				if (val == M_RED) {
					red.r = r;
					red.c = c;
					map[r][c] = M_EMPTY;
				}
				else if (val == M_BLUE) {
					blue.r = r;
					blue.c = c;
					map[r][c] = val;
					map[r][c] = M_EMPTY;
				}
				else {
					map[r][c] = val;
				}
			}
		}


		int n_move = solve();
		cout << "#" << i_case + 1 << " " << n_move << endl;
	}

	return 0;
}


void init_dir()
{
	dir4[0].r = 0;	dir4[0].c = 1;
	dir4[1].r = 1;	dir4[1].c = 0;
	dir4[2].r = 0;	dir4[2].c = -1;
	dir4[3].r = -1;	dir4[3].c = 0;
}

void print_map()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (i == red.r && j == red.c) {
				printf("%2d ", M_RED);
			}
			else if (i == blue.r && j == blue.c) {
				printf("%2d ", M_BLUE);
			}
			else {
				printf("%2d ", map[i][j]);
			}
		}
		cout << endl;
	}
}
