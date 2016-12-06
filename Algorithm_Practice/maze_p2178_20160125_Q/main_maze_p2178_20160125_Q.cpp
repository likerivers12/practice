#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// https://www.acmicpc.net/problem/2178

using namespace std;

// 성공, add4Q, add1Q

//#define SAQ	
#define ACMICPC 

#define DEBUG_LEVEL 2
#define N_TEST_CASE 1

#define MAX_MAP_SIZE	102
#define MAX_Q_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE))

#define D_EMPTY 1
#define D_WALL 0
#define D_VISIT 2
#define D_NOPATH 3
#define D_CUR	8
typedef struct Move {
	int r;
	int c;
} Move;

typedef struct PositionInfo {
	int r;
	int c;
	int n_move;
	int i_dir;
} PositionInfo;


Move dira[4];
PositionInfo Q[MAX_Q_SIZE];
int front;
int rear;
int n_Qelm;

int N, M;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];

int Answer;

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

	Q[rear].r = elm.r;
	Q[rear].c = elm.c;
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

void print_map(void)
{
	for (int i = 0; i <= N+1; i++) {
		for (int j = 0; j <= M+1; j++) {
			printf("%2d", map[i][j]);
		}
		cout << endl;
	}
}

void print_map(int cur_r, int cur_c)
{
	for (int i = 0; i <= N+1; i++) {
		for (int j = 0; j <= M+1; j++) {
			if (i == cur_r && j == cur_c) 
				printf("%2d", D_CUR);
			else
				printf("%2d", map[i][j]);
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

	int i_dir;
	int n_move = 0;
	int cur_r;
	int cur_c;
	int next_r;
	int next_c;

	init_move();
	initQ();

	elm.r = 1; elm.c = 1;
	elm.n_move = 1;
	//add4Q(elm);
	addQ(elm);
	map[1][1] = D_VISIT;

	while ( !is_emptyQ() ) {
		elm = delQ();

		for (int i_dir = 0; i_dir < 4; i_dir++) {
			cur_r = elm.r;
			cur_c = elm.c;
			//i_dir = elm.i_dir;
			n_move = elm.n_move;

#if DEBUG_LEVEL > 1
			print_map(cur_r, cur_c);
			cout << get_dir_char(i_dir);
			cout << endl;
#endif
			next_r = cur_r + dira[i_dir].r;
			next_c = cur_c + dira[i_dir].c;

			if ( map[next_r][next_c] == D_EMPTY ) {
				// 이동
				cur_r = next_r;
				cur_c = next_c;
				n_move++;
				map[cur_r][cur_c] = D_VISIT;

#if DEBUG_LEVEL > 1
			print_map(cur_r, cur_c);
			cout << endl;
#endif

				next_elm.r = cur_r;
				next_elm.c = cur_c;
				next_elm.n_move = n_move;

				//add4Q(elm);
				addQ(next_elm);

				// 도착 확인
				if (cur_r == N && cur_c == M) {
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

	int i_dir;
	int n_move = 0;
	int cur_r;
	int cur_c;
	int next_r;
	int next_c;

	init_move();
	initQ();

	elm.r = 1; elm.c = 1;
	elm.n_move = 1;
	add4Q(elm);
	map[1][1] = D_VISIT;

	while ( !is_emptyQ() ) {
		elm = delQ();

		cur_r = elm.r;
		cur_c = elm.c;
		i_dir = elm.i_dir;
		n_move = elm.n_move;

#if DEBUG_LEVEL > 1
			print_map(cur_r, cur_c);
			cout << get_dir_char(i_dir);
			cout << endl;
#endif
		next_r = cur_r + dira[i_dir].r;
		next_c = cur_c + dira[i_dir].c;

		if ( map[next_r][next_c] == D_EMPTY ) {
			// 이동
			cur_r = next_r;
			cur_c = next_c;
			n_move++;
			map[cur_r][cur_c] = D_VISIT;

#if DEBUG_LEVEL > 1
			print_map(cur_r, cur_c);
			cout << endl;
#endif

			elm.r = cur_r;
			elm.c = cur_c;
			elm.n_move = n_move;

			add4Q(elm);

			// 도착 확인
			if (cur_r == N && cur_c == M) {
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


int main(void)
{
	int n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> N;
		cin >> M;

#ifdef SAQ
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++) {
				cin >> map[i][j];
			}
		}
#else
		read_input_acmicpc();
#endif
		// 둘레벽
		for (int r = 0 ; r <= N+1; r++) {
			map[r][0] = D_WALL;
			map[r][M+1] = D_WALL;
		}
		for (int c = 0 ; c <= M+1; c++) {
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
