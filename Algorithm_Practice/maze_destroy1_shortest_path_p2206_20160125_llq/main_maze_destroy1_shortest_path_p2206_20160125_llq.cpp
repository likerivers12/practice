#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// 메모리 초과/런타임에러

// https://www.acmicpc.net/problem/2206

using namespace std;

//#define SAQ	
#define ACMICPC 

#define DEBUG_LEVEL 0
#define N_TEST_CASE 1

#define MAX_MAP_SIZE	1002
#define MAX_Q_SIZE ((MAX_MAP_SIZE)*(MAX_MAP_SIZE)+MAX_MAP_SIZE)

#define D_EMPTY 0
#define D_WALL	1
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
	bool destroyable;	// defalut: false, true: possible to destory 1
	int n_move;
	int i_dir;
	PositionInfo * link;
} PositionInfo;


Move dira[4];
PositionInfo Q[MAX_Q_SIZE];
int front;
int rear;
int n_Qelm;
PositionInfo * head;
PositionInfo * tail;


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

	head = 0;
	tail = 0;
}

bool is_movable(int cur_r, int cur_c, int i_dir, bool destroyable)
{
	int next_r = cur_r + dira[i_dir].r;
	int next_c = cur_c + dira[i_dir].c;

	if (next_r <= 0 || next_r >= N+1) return false;
	if (next_c <= 0 || next_c >= M+1) return false;
	if (map[next_r][next_c] == D_EMPTY) {
		return true;
	}
	else if (map[next_r][next_c] == D_WALL && destroyable ) {
		return true;
	} else {
		return false;
	}
}

void addQp(PositionInfo const & elm)
{
	// check full
	//if (n_Qelm > MAX_Q_SIZE) {
	//	cerr << "Error! full Q!" << endl;
	//}
	PositionInfo * new_elm = new PositionInfo();
	new_elm->r = elm.r;
	new_elm->c = elm.c;
	new_elm->destroyable = elm.destroyable;
	new_elm->n_move = elm.n_move;
	new_elm->i_dir = elm.i_dir;
	new_elm->link = 0;

	if (n_Qelm == 0) {
		head = new_elm;
	} else { 		
		tail->link = new_elm;
	}
	tail = new_elm;

	n_Qelm++;
}

void delQp(PositionInfo * elm)
{
	PositionInfo *tmp = head;
	elm->r				= head->r;
	elm->c				= head->c;
	elm->destroyable	= head->destroyable;
	elm->n_move			= head->n_move;
	elm->i_dir			= head->i_dir;

	n_Qelm--;
	head = head->link;
	delete tmp;

	if (n_Qelm == 0) {
		tail = 0;
	}
}

bool is_emptyQp(void)
{
	if (n_Qelm <= 0) return true;	// empty
	else			 return false;  // not empty
}

void add4Qp(PositionInfo elm)
{
	for (int i = 0; i < 4; i++) {
		if ( is_movable(elm.r, elm.c, i, elm.destroyable) ) { 
			elm.i_dir = i;
			addQp(elm);
		}
	}
}
//
//void addQ(PositionInfo const & elm)
//{
//	// check full
//	if (n_Qelm > MAX_Q_SIZE) {
//		cerr << "Error! full Q!" << endl;
//	}
//
//	Q[rear].r = elm.r;
//	Q[rear].c = elm.c;
//	Q[rear].destroyable = elm.destroyable;
//	Q[rear].n_move = elm.n_move;
//	Q[rear].i_dir = elm.i_dir;
//	n_Qelm++;
//
//	rear = (rear + 1) % MAX_Q_SIZE;
//}
//
//PositionInfo delQ(void)
//{
//	int tmp = front;
//
//	front = (front + 1) % MAX_Q_SIZE;
//	n_Qelm--;
//
//	return Q[tmp];
//}
//
//bool is_emptyQ(void)
//{
//	if (n_Qelm <= 0) return true;	// empty
//	else			 return false;  // not empty
//}
//
//void add4Q(PositionInfo elm)
//{
//	for (int i = 0; i < 4; i++) {
//		if ( is_movable(elm.r, elm.c, i, elm.destroyable) ) { 
//			elm.i_dir = i;
//			addQ(elm);
//		}
//	}
//}

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

bool is_movable(int next_r, int next_c, bool destroyable)
{
	if (next_r <= 0 || next_r >= N+1) return false;
	if (next_c <= 0 || next_c >= M+1) return false;
	if (map[next_r][next_c] == D_EMPTY) {
		return true;
	} else if (map[next_r][next_c] == D_WALL && destroyable ) {
		return true;
	} else {
		return false;
	}
}

int solve(void)
{
	PositionInfo elm;

	int i_dir;
	int n_move = 0;
	int cur_r, cur_c;
	int next_r, next_c;
	bool destroyable = true;

	init_move();
	initQ();

	elm.r = 1;	elm.c = 1;	elm.destroyable = true;
	elm.n_move = 1;
	add4Qp(elm);
	map[1][1] = D_VISIT;

	while ( !is_emptyQp() ) {
		delQp(&elm);

		cur_r = elm.r; cur_c = elm.c;
		destroyable = elm.destroyable;
		n_move = elm.n_move;
		i_dir = elm.i_dir;

#if DEBUG_LEVEL > 1
		print_map(cur_r, cur_c);
		cout << get_dir_char(i_dir);
		cout << endl;
#endif
		next_r = cur_r + dira[i_dir].r;
		next_c = cur_c + dira[i_dir].c;

		if ( is_movable(next_r, next_c, destroyable) ) {
			// 이동
			cur_r = next_r;
			cur_c = next_c;
			n_move++;
			if (map[cur_r][cur_c] == D_WALL) {
				destroyable = false;
			} else {
				//map[cur_r][cur_c] = D_VISIT;
			}
			//map[cur_r][cur_c] = D_VISIT;

#if DEBUG_LEVEL > 1
			print_map(cur_r, cur_c);
			cout << endl;
#endif

			elm.r = cur_r;
			elm.c = cur_c;
			elm.destroyable = destroyable;
			elm.n_move = n_move;

			add4Qp(elm);

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
	return -1;
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
