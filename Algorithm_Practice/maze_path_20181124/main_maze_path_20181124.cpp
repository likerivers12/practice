#ifdef MAZE_20181124

#include <stdio.h>
#include <iostream>

// 11:44 시작
using namespace std;

void init(void);
void print_map(void);

#define MAX_N	10000000
#define MAX_ROW	(100+2)
#define MAX_COL (100+2)

#define M_EMPTY	0
#define M_WALL  1
#define M_VISIT 2
#define M_NOPATH 3

#define MAX_STACK_SIZE 100000000
typedef struct Dir {
	int r;
	int c;
} Dir;

typedef struct MoveInfo {
	int r;
	int c;
	int i_dir;
} MoveInfo;

Dir dir[8];
int NR;
int NC;
int map[MAX_ROW][MAX_COL];


MoveInfo stack[MAX_STACK_SIZE];
unsigned long top = -1;

int push(MoveInfo elm);
int push(MoveInfo elm)
{
	top++;
	if (top >= MAX_STACK_SIZE) {
		printf("ERROR! STACK FULL\n");
		return -1;
	}

	stack[top] = elm;
	return 0;
}

MoveInfo pop(void);
MoveInfo pop(void)
{
	unsigned long tmp = top;
	if (top < 0) {
		printf("Error! STACK EMPTY\n");
	}

	top--;
	return stack[tmp];
}

bool is_stack_empty()
{
	if (top < 0) {
		return true;
	}

	return false;	// 
}

void init(void)
{
	dir[0].r = 0; dir[0].c = 1;		// 0: ->
	dir[1].r = 1; dir[1].c = 1;		// 1: RD
	dir[2].r = 1; dir[2].c = 0;		// 2: v
	dir[3].r = 1; dir[3].c = -1;	// 3: LD
	dir[4].r = 0; dir[4].c = -1;	// 4: <
	dir[5].r = -1; dir[5].c = -1;	// 5: LU
	dir[6].r = -1; dir[6].c = 0;	// 6: ^
	dir[7].r = -1; dir[7].c = 1;	// 7: RU
}
void print_map(void)
{
	cout << "======================================\n";
	for (int i = 0; i < NR; i++) {
		for (int j = 0; j < NC; j++) {
			printf("%2d ", map[i][j]);
		}
		cout << endl;
	}
}

int solve(void)
{
	init();

	MoveInfo elm;
	int cur_r, cur_c;
	int next_r, next_c;
	int i_dir;

	elm.r = 1, elm.c = 1; elm.i_dir = 0;
	cur_r = elm.r; cur_c = elm.c;
	map[cur_r][cur_c] = M_VISIT;
	push(elm);

	bool loop_flag = true;

	while (!is_stack_empty() && loop_flag) {
		elm = pop();

		cur_r = elm.r;
		cur_c = elm.c;
		i_dir = elm.i_dir;

		print_map();

		while (i_dir < 8) {
			next_r = cur_r + dir[i_dir].r;
			next_c = cur_c + dir[i_dir].c;
			if (map[next_r][next_c] == M_EMPTY) {
				// (현재위치, 다음 이동방향)을 push
				elm.r = cur_r;
				elm.c = cur_c;
				elm.i_dir = i_dir + 1;
				push(elm);

				// 이동
				cur_r = next_r;
				cur_c = next_c;
				i_dir = 0;

				map[cur_r][cur_c] = M_VISIT;
				print_map();

				if (cur_r == NR - 2 && cur_c == NC - 2) {
					// 도착
					elm.r = cur_r;
					elm.c = cur_c;
					elm.i_dir = i_dir + 1;
					push(elm);

					loop_flag = false;
					print_map();
					break;
				}

			}
			else {
				i_dir++;
			}

		} // end while2

		map[cur_r][cur_c] = M_NOPATH;
		print_map();
	} // end while1
	
	return 0;
}

void run_20181124()
{
	int N;

	scanf("%d", &N);
	NR = N + 2;
	NC = N + 2;

	for (int i = 1; i < NR-1; i++) {
		for (int j = 1; j < NC-1; j++) {
			cin >> map[i][j];
		}
	}

	// 경계벽 설정
	for (int i = 0; i < NR; i++) {
		// 왼쪽, 오른쪽
		map[i][0] = M_WALL;
		map[i][NC - 1] = M_WALL;
	}

	for (int j = 0; j < NC; j++) {
		// 위, 아래
		map[0][j] = M_WALL;
		map[NR - 1][j] = M_WALL;
	}

	solve();
}

//int main(void)
//{
//
//	run_20181124();
//
//	return 0;
//}


#endif
