#include <iostream>
#include <stdio.h>

// 1 시간 20분 걸림

using namespace std;

#define DEBUG_LEVEL 2
#define N_TEST_CASE 1

#define MAX_MAP_SIZE 100
#define MAX_Q_SIZE (((MAX_MAP_SIZE)+2)*((MAX_MAP_SIZE)+2))

#define C_GROUND 0
#define C_VISIT  -1

#define DIR_RIGHT	0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_UP		3

#define P_I		1   // |
#define P_M		2	// ㅡ
#define P_R		3   // ㄱ
#define P_S		4   // ㄴ
#define P_F		5   // F
#define P_J		6   // J
#define P_H		7   // ㅗ
#define P_N		8   // ㅜ
#define P_P		9   // +

typedef struct Move {
	int r;
	int c;
} Move;
Move dir4[4];

typedef struct MoveInfo {
	int r;
	int c;
	int i_type;
	int i_dir;
	int dist;
} MoveInfo;

MoveInfo Q[MAX_Q_SIZE];
int front;
int rear;
int n_Qelm;

int N;
int M;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int start_r;
int start_c;

int Answer;

void addQ(MoveInfo const & elm)
{
	// check full
	if (n_Qelm > MAX_Q_SIZE) {
		cerr << "Error: full Q" << endl;
		exit(1);
	}

	Q[rear].r = elm.r;
	Q[rear].c = elm.c;
	Q[rear].i_type = elm.i_type;
	Q[rear].i_dir = elm.i_dir;
	Q[rear].dist = elm.dist;

	n_Qelm++;

	rear = (rear + 1) % MAX_Q_SIZE;
}

void add4Q(MoveInfo elm)
{
	for (int i = 0 ; i < 4; i++) {
		elm.i_dir = i;
		addQ(elm);
	}
}

MoveInfo delQ(void)
{
	// check empty
	if (n_Qelm <= 0) {
		cerr << "Error: empty Q" << endl;
		exit(1);
	}

	int tmp = front;
	
	front = (front + 1) % MAX_Q_SIZE;

	n_Qelm--;
	return Q[tmp];
}

bool is_emptyQ(void)
{
	if (n_Qelm <= 0)
		return true;
	else 
		return false;	// Not empty
}

void initQ(void)
{
	front = 0;
	rear = 0;
	n_Qelm = 0;
}

void init_move(void)
{
	dir4[0].r =  0; dir4[0].c =  1;
	dir4[1].r =  1; dir4[1].c =  0;
	dir4[2].r =  0; dir4[2].c = -1;
	dir4[3].r = -1; dir4[3].c =  0;
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

bool is_movable(int cur_r, int cur_c, int cur_type, int i_dir)
{
	int next_r; 
	int next_c;
	int next_type;

	next_r = cur_r + dir4[i_dir].r;
	next_c = cur_c + dir4[i_dir].c;
	next_type = map[next_r][next_c];

	switch (next_type) {
	case C_GROUND:
	case C_VISIT:
		return false;
	}

	// 나갈 수 있는 모양인지?
	switch (i_dir) {
	case DIR_RIGHT:

		switch (cur_type) {
		case P_I:
		case P_R:
		case P_F:
		case P_J:
			return false;
		}
		break;


	case DIR_DOWN:

		switch (cur_type) {
		case P_M: 
		case P_S: 
		case P_J:
		case P_H:
			return false;
		}
		break;


	case DIR_LEFT:

		switch (cur_type) {
		case P_I:
		case P_S:
		case P_F:
			return false;
		}
		break;


	case DIR_UP:

		switch (cur_type) {
		case P_M:
		case P_R:
		case P_F:
		case P_N:
			return false;
		}
		break;
	}



	//------------------------------------------------------
	// 들어갈 수 있는 모양인지?
	switch (i_dir) {
	case DIR_RIGHT:

		switch (next_type) {
		case P_I:
		case P_S:
		case P_F:
			return false;
		}
		break;


	case DIR_DOWN:

		switch (next_type) {
		case P_M:
		case P_R:
		case P_F:
		case P_N:
			return false;
		}
		break;


	case DIR_LEFT:

		switch (next_type) {
		case P_I:
		case P_R:
		case P_J:
			return false;
		}
		break;


	case DIR_UP:

		switch (next_type) {
		case P_M:
		case P_S: 
		case P_J: 
		case P_H:
			return false;
		}
		break;
	}


	return true;
}

int solve(void)
{
	MoveInfo elm;
	int max_dist = 0;
	int cur_r, cur_c;
	int cur_type;
	int i_dir;
	int cur_dist;

	int next_r, next_c;
	int next_type;
	int next_dist;

	init_move();
	initQ();

	elm.r = start_r; elm.c = start_c; 
	elm.dist = 0;
	elm.i_type = map[start_r][start_c];

	add4Q(elm);
	map[start_r][start_c] = C_VISIT;

#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
	while ( !is_emptyQ() ) {
		elm = delQ();

		if (elm.dist > max_dist) {
			max_dist = elm.dist;
		}
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
		cur_r = elm.r; cur_c = elm.c; 
		cur_dist = elm.dist;
		cur_type = elm.i_type;
		i_dir = elm.i_dir;

		if ( is_movable(cur_r, cur_c, cur_type, i_dir) ) {
			// 이동한다

			elm.r = cur_r + dir4[i_dir].r;
			elm.c = cur_c + dir4[i_dir].c;
			elm.i_type = map[elm.r][elm.c];
			elm.dist = cur_dist + 1;

			add4Q(elm);
			map[elm.r][elm.c] = C_VISIT;
#if DEBUG_LEVEL > 1
	print_map();
	cout << endl;
#endif
		}
	} 
	return max_dist;
}


int main(void)
{
	int n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N; cin >> M; cin >> start_r; cin >> start_c;

		for (int i = 1; i <= N; i++) 
			for (int j = 1; j <= M; j++) 
				cin >> map[i][j];

		for (int c = 0; c <= M+1; c++) {
			map[0][c] = C_GROUND;
			map[N+1][c] = C_GROUND;
		}
		for (int r = 0; r <= N+1; r++) {
			map[r][0] = C_GROUND;
			map[r][M+1] = C_GROUND;
		}

		Answer = solve();
		cout << "#" << i_tc << " ";
		cout << Answer;
		cout << endl;
	}

	return 0;
}
