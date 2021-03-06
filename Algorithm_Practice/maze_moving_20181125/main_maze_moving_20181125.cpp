#ifdef RUN_20181125

// 10:56
// 2시간. 디버깅 필요(오답)->+1시간 해결(공위치 읽을 때 맵의 표시)

// 정답:
// 1 ( 6, 5): 4   (R,D,D,D)
// 2 ( 5, 5): 2   (D,R)
// 3 (12,11): 9   (D,D,D,D,D,D,R,R,R)
// 4 ( 3, 5): -1  
// 5 ( 7,10): 5   (R,R,R,R,U)
// 6 ( 5, 8): 3   (R,D,D)
//#define DEBUG

#include <iostream>
#include <stdio.h>

using namespace std;

#define M_EMPTY 0
#define M_RED 1
#define M_BLUE 2
#define M_WALL 3
#define M_EXIT 4

#define NULL 0
// 빨간색이 파란색보다 먼저 출구에 도착해야함.

#define MAX_ROW 100
#define MAX_COL 100
#define MAX_Q_SIZE ((MAX_ROW)*(MAX_COL)*100)

typedef struct Dir4 {
	int r;
	int c;
}Dir4;
Dir4 dir[4];
void init_dir()
{
	dir[0].r = 0; dir[0].c = 1;		// ->
	dir[1].r = 1; dir[1].c = 0;     // D
	dir[2].r = 0; dir[2].c = -1;    // <- 
	dir[3].r = -1; dir[3].c = 0;    // U
}

class Position
{
public:
	unsigned int r;
	unsigned int c;
};

class MoveInfo
{
public:
	Position red;
	Position blue;
	int n_move;
};


class Queue
{
public:
	MoveInfo qd[MAX_Q_SIZE];
	int n_Qelm;
	int front;
	int rear;

public:
	Queue();

	void init();
	void addQ(MoveInfo & const elm);
	MoveInfo delQ();
	bool is_empty(void);
	void print(void);
};


class Moving
{
public:
	int map[MAX_ROW][MAX_COL];
	unsigned int nr;
	unsigned int nc;

	Position red;
	Position blue;
	int n_move;

public:
	Moving();
	~Moving();

	void init(void);
	void read_map(void);
	void print_map(void);
};


Moving mv;
Queue qq;

bool take_move(Position *pos, int i_dir)
{
	int r;
	int c;
	r = pos->r + dir[i_dir].r;
	c = pos->c + dir[i_dir].c;

	if (mv.map[r][c] != M_WALL) {
		pos->r = r;
		pos->c = c;

		return true;
	}
	return false;
}

bool try_move(Position const & red, Position const & blue, int i_dir)
{
	Position t_red = red;
	Position t_blue = blue;
	bool mv_flag1, mv_flag2;
	int r, c;

	mv_flag1 = take_move(&t_red, i_dir);
	mv_flag2 = take_move(&t_blue, i_dir);

	if (mv_flag1 == false && mv_flag2 == false) return false;	// 둘다 이동 없음.
	if (t_red.r == t_blue.r && t_red.c == t_blue.c) return false;	// 충돌
	if (mv.map[t_blue.r][t_blue.c] == M_EXIT) return false; // 파랑이 출구에

	return true;
}

void print_direction(int i)
{
	if (i == 0) cout << "R" << endl;
	else if (i == 1) cout << "D" << endl;
	else if (i == 2) cout << "L" << endl;
	else cout << "U" << endl;
}

int solve()
{
	//Position c_red;
	//Position c_blue;

	bool loop_flag = true;

	MoveInfo cur_elm;
	MoveInfo next_elm;
	//Position cur_red;
	//Position cur_blue;
	//int cur_n_move;
	Position next_red;
	Position next_blue;

	cur_elm.red = mv.red;
	cur_elm.blue = mv.blue;
	cur_elm.n_move = 0;

	qq.addQ(cur_elm);
	while (!qq.is_empty() && loop_flag) {
		cur_elm = qq.delQ();

		//if (mv.map[cur_elm.red.r][cur_elm.red.c] == M_EXIT || cur_elm.n_move > 10) {
		if (cur_elm.n_move > 10) {
			continue;
		}

		for (int i = 0; i < 4; i++) {
			mv.red = cur_elm.red;
			mv.blue = cur_elm.blue;
			mv.n_move = cur_elm.n_move;
#ifdef DEBUG
			mv.print_map();
			print_direction(i);
#endif

			// 1. red와 blue 이동
			bool flag = try_move(mv.red, mv.blue, i);

			// 이동 있으면.
			if (flag == true) {
				take_move(&mv.red, i);
				take_move(&mv.blue, i);
#ifdef DEBUG
				mv.print_map();
#endif

				if (mv.map[mv.red.r][mv.red.c] == M_EXIT) {
					return mv.n_move + 1;
				}

				next_elm.red = mv.red;
				next_elm.blue = mv.blue;
				next_elm.n_move = mv.n_move + 1;
				qq.addQ(next_elm);
			}
			
		}
	}

	return -1;
}



int main(void)
{
	int n_tc;
	scanf("%d", &n_tc);
	//printf("%d\n", n_tc);

	int i;
	int answer;

	init_dir();
	for (i = 0; i < n_tc; i++) {
		qq.init();
		mv.init();

		scanf("%d %d", &(mv.nr), &(mv.nc));


		mv.read_map();
		//mv.print_map();
		//qq.print();
#ifdef DEBUG
		mv.print_map();
#endif
		answer = solve();

		//mv.print_map();
		cout << "#" << i+1 << ": " << answer << endl;
	}


	return 0;
}





//========================================================
//
Queue::Queue()
{
	n_Qelm = 0;
	front = 0;
	rear = 0;
}

void Queue::init()
{
	n_Qelm = 0;
	front = 0;
	rear = 0;
}

void Queue::addQ(MoveInfo & const elm)
{
	if (n_Qelm >= MAX_Q_SIZE) {
		cout << "Error! Full Q\n";
		return;
	}

	n_Qelm++;
	qd[rear] = elm;
	rear = (rear + 1) % MAX_Q_SIZE;
}

MoveInfo Queue::delQ(void)
{
	if (n_Qelm <= 0) {
		cout << "Error! Empty Q\n";
	}

	n_Qelm--;
	int tmp = front;
	front = (front + 1) % MAX_Q_SIZE;
	return qd[tmp];
}

bool Queue::is_empty(void)
{
	if (n_Qelm <= 0) return true;
	return false;
}

void Queue::print(void)
{
	cout << n_Qelm << endl;
	cout << front << endl;
	cout << rear << endl;
}
//========================================================
//
Moving::Moving()
{
	nr = 0;
	nc = 0;
	n_move = 0;
}

void Moving::init()
{
	nr = 0;
	nc = 0;
	n_move = 0;
}

Moving::~Moving()
{

}

void Moving::read_map(void)
{
	int r, c;
	int d;
	for (r = 0; r < nr; r++) {
		for (c = 0; c < nc; c++) {
			scanf("%d", &d);

			if (d == M_RED) {
				red.r = r;
				red.c = c;
				map[r][c] = M_EMPTY;
			}
			else if (d == M_BLUE) {
				blue.r = r;
				blue.c = c;
				map[r][c] = M_EMPTY;
			}
			else {
				map[r][c] = d;
			}
		}
	}
}

void Moving::print_map(void)
{
	int r, c;
	int d;
	cout << "============================================" << endl;
	for (r = 0; r < nr; r++) {
		for (c = 0; c < nc; c++) {
			if (r == red.r && c == red.c) {
				d = M_RED;
			}
			else if (r == blue.r && c == blue.c) {
				d = M_BLUE;
			}
			else {
				d = map[r][c];
			}
			printf("%2d ", d);
		}
		cout << endl;
	}
}

#endif
