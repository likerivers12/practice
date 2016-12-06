#include <iostream>
#include <stdio.h>

#include "maze_moving_20160120.h"

// 1�ð� 10�� �ɸ�
using namespace std;


Move dir4[4];
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int N, M;	//  N x M

int Answer;

PositionInfo Q[MAX_Q_SIZE];
int head;
int tail;
int n_Qelm;


void init_move(void)
{
	dir4[0].r =  0; dir4[0].c =  1;  // ->
	dir4[1].r =  1; dir4[1].c =  0;  // |v
	dir4[2].r =  0; dir4[2].c =  1;  // <-
	dir4[3].r = -1; dir4[3].c =  0;  // |^
}

void initQ(void)
{
	head = 0;
	tail = 0;
	n_Qelm = 0;
}

void addQ(PositionInfo const & elm) 
{
	// check full,
	if (n_Qelm > MAX_Q_SIZE) {
		cerr << "Error: Q is full!" << endl;
		exit(1);
	}

	Q[tail].blue = elm.blue;
	Q[tail].red = elm.red;
	Q[tail].i_dir = elm.i_dir;
	Q[tail].n_move = elm.n_move;
	n_Qelm++;

	tail = (tail + 1) % MAX_Q_SIZE;
}

PositionInfo delQ(void)
{
	int tmp = head;

	// check empty,
	if (n_Qelm <= 0) {
		cerr << "Error: Q is empty!" << endl;
		exit(1);
	}

	n_Qelm--;
	head = (head + 1) % MAX_Q_SIZE;

	return Q[tmp];
}

bool is_emptyQ(void) 
{
	if (n_Qelm <= 0)	return true;	// Empty
	else				return false;
}

void add4Q(Position const & red, Position const & blue, int n_move)
{
	PositionInfo elm;
	for (int i_dir = 0; i_dir < 4; i_dir++) {
		elm.red = red;
		elm.blue = blue;
		elm.n_move = n_move;
		elm.i_dir = i_dir;

		addQ(elm);
	}
}

void print_map(Position const & red, Position const & blue)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
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

void get_position(Position * red, Position * blue) 
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (map[i][j] == D_RED) {
				red->r = i;
				red->c = j;
			}

			if (map[i][j] == D_BLUE) {
				blue->r = i;
				blue->c = j;
			}
		}
	}
}

//void init_map(Position const & red, Position const & blue) 
//{
//	map[red.r][red.c] = D_EMPTY;
//	map[blue.r][blue.c] = D_EMPTY;
//}

bool take_amove(Position * pos, int i_dir)
{
	int next_r, next_c;
	next_r = pos->r + dir4[i_dir].r;
	next_c = pos->c + dir4[i_dir].c;

	if (map[next_r][next_c] != D_WALL) {
		// D_EMPTY or D_EXIT
		pos->r = next_r;
		pos->c = next_c;
		return true;
	} else 
		return false;
}

bool is_movable(Position red, Position blue, int i_dir)
{
	bool flag_red, flag_blue;
	// red�� �̵� �����Ѱ�?
	flag_red = take_amove(&red, i_dir);

	// blue�� �̵� �����Ѱ�?
	flag_blue = take_amove(&blue, i_dir);

	if (!flag_red && !flag_blue) {
		// �Ѵ� �̵��� ����.
		return false;
	}

	// �浹���� �ʴ°�?
	if (red.r == blue.r && red.c == blue.c) 
		return false;	// �浹
	else if (map[blue.r][blue.c] == D_EXIT)
		return false;   // �Ķ����� �ⱸ��
	else 
		return true;
}

void print_direction(int i_dir)
{
	switch (i_dir) {
	case 0:
		cout << "->" << endl;
		break;
	case 1:
		cout << "D" << endl;
		break;
	case 2:
		cout << "<-" << endl;
		break;
	case 3:
		cout << "U" << endl;
		break;
	}
}
int solve(void)
{
	PositionInfo elm;

	Position red;
	Position blue;
	int i_dir;
	int n_move = 0;

	init_move();
	initQ();

	get_position(&red, &blue);

	map[red.r][red.c] = D_EMPTY;
	map[blue.r][blue.c] = D_EMPTY;

	add4Q(red, blue, 0);

#if DEBUG_LEVEL > 1
	print_map(red, blue);
	cout << endl;
#endif

	//while ( !is_emptyQ() && n_move < 10 ) {
	while ( n_Qelm > 0 && n_move < 10 ) {
		elm = delQ();

		red = elm.red;
		blue = elm.blue;
		i_dir = elm.i_dir;
		n_move = elm.n_move;

		// �̵� ���� Ȯ��
		if (is_movable(red, blue, i_dir)) {
			// �̵� ����
#if DEBUG_LEVEL > 2
	print_map(red, blue);
	cout << endl;
#endif
			take_amove(&red, i_dir);
			take_amove(&blue, i_dir);
			n_move++;

#if DEBUG_LEVEL > 2
	print_direction(i_dir);
	print_map(red, blue);
	cout << endl;
#endif
			// ���� Ȯ��
			if (map[red.r][red.c] == D_EXIT) {
				// ����
				break;
			}

			// 
			add4Q(red, blue, n_move);

		} else {
			// 
		}		
	}

#if DEBUG_LEVEL > 1
	print_map(red, blue);
	cout << endl;
#endif

	return n_move;
}


int main(void)
{
	int n_test_case = N_TEST_CASE;

	for (int i_case = 1; i_case <= n_test_case; i_case++) {
		cin >> N;
		cin >> M;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> map[i][j];
			}
		}


		Answer = solve();
		cout << "#" << i_case << " ";
		cout << Answer;
		cout << endl;
	}

	return 0;
}
