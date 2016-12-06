#include <stdio.h>
#include <iostream>

// 10:20 시작

using namespace std;


#define MAX_MAP_SIZE 100
#define MAX_Q_SIZE (MAX_MAP_SIZE*MAX_MAP_SIZE)


#define M_EMPTY 0
#define M_WALL 3
#define M_RED 1
#define M_BLUE 2
#define M_EXIT 4

typedef struct Move {
	int r;
	int c;
} Move;

Move dir4[4];

typedef struct Position {
	int r;
	int c;
} Position;

Position blue;
Position red;

typedef struct PositionInfo {
	Position blue;
	Position red;
	int n_move;		// 현재까지 이동 수
	int i_dir;		// 다음 갈 방향
};

int N, M;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];


void init_dir()
{
	dir4[0].r =  0;	 dir4[0].c =  1;		// E
	dir4[1].r =  1;  dir4[1].c =  0;		// S
	dir4[2].r =  0;  dir4[2].c = -1;		// W
	dir4[3].r = -1;  dir4[3].c =  0;		// N
}

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

void addQ(PositionInfo const & elm) 
{
	if (n_Qelm >= MAX_Q_SIZE) {
		cerr << "Full Q!!!" << endl;
	}

	Q[rear] = elm;
	n_Qelm++;

	rear = (rear + 1) % MAX_Q_SIZE;
}

PositionInfo delQ()
{
	if (n_Qelm <= 0) {
		cerr << "Empty Q!!!" << endl;
	}

	int tmp = front;

	front = (front + 1) % MAX_Q_SIZE;
	n_Qelm--;

	return Q[tmp];
}

bool isEmptyQ()
{
	return n_Qelm > 0 ? false : true;
}

bool is_movable(Position const & red, Position const & blue, int i_dir)
{
	if (map[red.r][red.c] == M_WALL && map[blue.r][blue.c] == M_WALL)
		return false;
	else
		return true;
}

void take_move(Position * pos, int i_dir)
{
	int next_r = pos->r + dir4[i_dir].r;
	int next_c = pos->c + dir4[i_dir].c;
	if (map[next_r][next_c] == M_WALL)
		return;

	pos->r = next_r;
	pos->c = next_c;
};\


bool is_valid(Position const & red, Position const & blue)
{
	if (red.r == blue.r && red.c == blue.c)
		return false;

	if (map[blue.r][blue.c] == M_EXIT)
		return false;

	return true;
}

void print_map(void)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (i == red.r && j == red.c)
				printf("%2d ", M_RED);
			else if (i == blue.r && j == blue.c)
				printf("%2d ", M_BLUE);
			else
				printf("%2d ", map[i][j]);
		}
		cout << endl;
	}
	cout << endl;
}

int solve()
{
	initQ();
	//Position cur_red;
	//Position cur_blue;
	//int cur_n_move;

	//Position next_red;
	//Position next_blue;
	//int next_n_move;

	int n_move = 0;
	PositionInfo elm;
	PositionInfo next_elm;

	elm.red = red;
	elm.blue = blue;
	elm.n_move = 0;
	addQ(elm);

	while (isEmptyQ() != true && n_move < 10) {
		elm = delQ();

		if (elm.n_move >= 10)
			continue;

		for (int i = 0; i < 4; i++) {
			red = elm.red;
			blue = elm.blue;
			n_move = elm.n_move;

			printf("----------------------------------\n");
			print_map();

			if (is_movable(red, blue, i)) {
				take_move(&red, i);
				take_move(&blue, i);
				n_move++;

				print_map();
				if (is_valid(red, blue)) {
					next_elm.red = red;
					next_elm.blue = blue;
					next_elm.n_move = n_move;

					if (map[red.r][red.c] == M_EXIT) {
						return n_move;
					}

					addQ(next_elm);
				}
			}

		} // end fo
		printf("----------------------------------\n");
	} // end while

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
				}
				else if (val == M_BLUE) {
					blue.r = r;
					blue.c = c;
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