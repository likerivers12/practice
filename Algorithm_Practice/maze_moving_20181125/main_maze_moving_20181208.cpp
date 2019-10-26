#define RUN_20181208

#ifdef RUN_20181208

#include <stdio.h>
#include <iostream>


using namespace std;

#define MAX_MAP_SIZE	100
#define MAX_Q_SIZE		65536

#define M_EMPTY	0
#define M_RED	1
#define M_BLUE	2
#define M_WALL	3
#define M_EXIT	4


struct MoveInfo
{
	int r;
	int c;
	int i_dir;
};

struct Position
{
	int r;
	int c;
};

struct Dir4
{
	int r;
	int c;
};


struct Maze
{
	int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
	int NR, NC;
	Position red, blue;

	Dir4	dir[4];
	
	MoveInfo Q[MAX_Q_SIZE];
	int	front;
	int rear;
	int n_Qelm;

	void init_dir4()
	{
		dir[0].r = 0; dir[0].c = 1;			// 오른쪽
		dir[1].r = 1; dir[1].c = 0;			// 아래
		dir[2].r = 0; dir[2].c = -1;		// 왼쪽
		dir[3].r = -1; dir[3].c = 0;		// 위
	}

	void enq(MoveInfo const elm)
	{
		if (n_Qelm >= MAX_Q_SIZE) {
			cout << "ERROR! Q is FULL!" << endl;
			return;
		}

		rear = (rear + 1) % MAX_Q_SIZE;
		Q[rear] = elm;
	}

	MoveInfo deq()
	{
		if (n_Qelm <= 0) {
			cout << "ERROR! Q is EMPTY!" << endl;
		}

		int tmp = front;
		front = (front + 1) % MAX_Q_SIZE;
		return Q[tmp];
	}
	
	void initQ()
	{
		front = rear = -1;
		n_Qelm = 0;
	}

	void init()
	{
		initQ();
	}

	void read_map(void)
	{
		int c;

		scanf("%d %d", &(NR), &(NC));

		for (int i = 0; i < NR; i++) {
			for (int j = 0; j < NC; j++) {
				scanf("%d", &c);

				if (c == M_RED) {
					map[i][j] = M_EMPTY;
					red.r = i;
					red.c = j;
				}
				else if (c == M_BLUE) {
					map[i][j] = M_EMPTY;
					blue.r = i;
					blue.c = j;
				}
				else {
					map[i][j] = c;
				}

			}
		}
	}

	void print_map()
	{
		int i, j;
		cout << "===================================" << endl;
		for (i = 0; i < NR; i++) {
			for (j = 0; j < NC; j++) {
				if (i == red.r && j == red.c) {
					printf("%d ", M_RED);
				}
				else if (i == blue.r && j == blue.c) {
					printf("%d ", M_BLUE);
				}
				else {
					printf("%d ", map[i][j]);
				}
			}
			cout << endl;
		}

		cout << endl;
	}

	void solve()
	{

	}
};

Maze maze;


int main(void)
{
	maze.init_dir4();

	int N_TC;
	scanf("%d", &N_TC);


	for (int n = 0; n < N_TC; n++) {
		maze.init();

		maze.read_map();
		maze.print_map();

		maze.solve();
	}

	return 0;
}

#endif
