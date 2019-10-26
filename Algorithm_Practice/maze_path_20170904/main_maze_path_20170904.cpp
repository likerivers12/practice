#include <iostream>
#include <stdio.h>

using namespace std;

#define MAX_STACK 100
#define MAX_MAP_SIZE 100

#define M_EMPTY	0
#define M_WALL 1
#define M_VISIT 2
#define M_NOPATH 3

int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int R;
int C;

typedef struct Dir{
	int r;
	int c;
} Dir;
Dir dir8[8];


typedef struct MoveInfo {
	int r;   // cur r
	int c;   // cur c
	int i_dir; // next move
};

MoveInfo stack[MAX_STACK];
int top;

void init_stack()
{
	top = -1;
}

void push(MoveInfo &elm)
{
	top++;

	if (top == MAX_STACK) {
		printf("ERROR! STACK_FULL\n");
		exit(1);
	}

	stack[top].r = elm.r;
	stack[top].c = elm.c;
	stack[top].i_dir = elm.i_dir;
}

MoveInfo pop()
{
	if (top < 0) {
		printf("ERROR! STACK_EMPTY\n");
		exit(1);
	}

	int tmp = top;
	top--;
	return stack[tmp];
}

bool is_empty_stack()
{
	if (top < 0) return true;
	return false;
}

void init_dir();
void print_map(int map[MAX_MAP_SIZE][MAX_MAP_SIZE], int m, int n);



void solve()
{
	print_map(map, R, C);
	printf("\n");

	init_stack();

	bool loop_flag = true;
	MoveInfo elm;
	int i_dir;
	int cur_r, cur_c;
	int next_r, next_c;

	elm.r = 1;	elm.c = 1; elm.i_dir = 0;
	push(elm);
	map[elm.r][elm.c] = M_VISIT;

	while (!is_empty_stack() && loop_flag) {
		elm = pop();

		cur_r = elm.r;
		cur_c = elm.c;
		i_dir = elm.i_dir;

		while (i_dir < 8) {
			next_r = cur_r + dir8[i_dir].r;
			next_c = cur_c + dir8[i_dir].c;
			if (map[next_r][next_c] == M_EMPTY) {
				// 이동 가능하면 이동

				// 현재 위치, 다음 이동 방향을 저장
				elm.r = cur_r; 
				elm.c = cur_c; 
				elm.i_dir = i_dir + 1;
				push(elm);

				cur_r = next_r;
				cur_c = next_c;
				i_dir = 0;

				map[cur_r][cur_c] = M_VISIT;
				print_map(map, R, C);
				printf("\n");

				// 목적지에 왔는지 확인
				if (cur_r == R && cur_c == C) {
					// 목적지 도착
					elm.r = cur_r;
					elm.c = cur_c;
					push(elm);

					loop_flag = false;
					break;
				}
			}
			else {
				// 이동불가하면 다음 방향.
				i_dir++;
			}

		}

		map[cur_r][cur_c] = M_NOPATH;
		print_map(map, R, C);
		printf("\n");

	}
	return;
}
int main(void)
{
	int n;
	cin >> n;

	R = n;
	C = n;
	for (int i = 1; i <= R; i++) 
		for (int j = 1; j <= C; j++) 
			cin >> map[i][j];

	for (int i = 0; i <= R + 1; i++) {
		map[i][0] = M_WALL;
		map[i][C + 1] = M_WALL;
	}

	for (int i = 0; i <= C + 1; i++) {
		map[0][i] = M_WALL;
		map[R+1][i] = M_WALL;
	}

	init_dir();
	solve();

	return 0;
}

void print_map(int map[MAX_MAP_SIZE][MAX_MAP_SIZE], int m, int n)
{
	for (int i = 0; i <= m+1; i++) {
		for (int j = 0; j <= n+1; j++) {
			printf("%2d ", map[i][j]);
		}
		cout << endl;
	}
}


void init_dir()
{
	dir8[0].r = 0;	dir8[0].c = 1;  // ->
	dir8[1].r = 1;	dir8[1].c = 1;  //
	dir8[2].r = 1;	dir8[2].c = 0;  // |v
	dir8[3].r = 1;	dir8[3].c = -1;  //  
	dir8[4].r = 0;	dir8[4].c = -1;  // <-
	dir8[5].r = -1;	dir8[5].c = -1;  // 
	dir8[6].r = -1;	dir8[6].c = 0;  // |^
	dir8[7].r = -1;	dir8[7].c = 1;
}
