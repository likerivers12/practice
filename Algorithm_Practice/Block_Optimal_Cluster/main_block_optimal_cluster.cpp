#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define DEBUG_LEVEL 2
#define N_TEST_CASE	1


#define MAX_NUM_BLOCKS 200001
#define MAX_NUM_CLUSTER	200001

#define MAX_STACK_SIZE	MAX_NUM_CLUSTER


long W[MAX_NUM_BLOCKS];
long NB;

long S[MAX_NUM_CLUSTER];
long NK;

long A[MAX_NUM_BLOCKS];
long max_W_cluster[MAX_NUM_CLUSTER];

typedef struct ClusterInfo {
	int cluster_id;
	int idx_start;
	int num_of_blocks;
	int idx_end;

	int idx_next_start;
} ClusterInfo;

ClusterInfo Stack[MAX_STACK_SIZE];
int top;

void init_stack(void)
{
	top = -1;
}

void push(ClusterInfo const & elm)
{
	// check full
	if (top >= MAX_STACK_SIZE) {
		cerr << "Error! stack full!" << endl;
		exit(1);
	}

	top++;
	Stack[top].cluster_id = elm.cluster_id;
	Stack[top].idx_start = elm.idx_start;
	Stack[top].num_of_blocks = elm.num_of_blocks;
	Stack[top].idx_end = elm.idx_end;

	Stack[top].idx_next_start = elm.idx_next_start;
}

ClusterInfo pop(void)
{
	// check empty
	if (top < 0) {
		cerr << "Error! stack empty!" << endl;
		exit(1);
	}

	return Stack[top--];
}

bool is_empty_stack(void)
{
	if (top < 0)	return true;
	else			return false;
}

//bool is_movable(int next_r, int next_c)
//{
//	if (map[next_r][next_c] == D_EMPTY) return true;
//	else								 return false;
//}


void print_A()
{
	for (int bi = 1; bi <= NB; bi++) {
		printf("%2d ", A[bi]);
	}
	cout << endl;
}

void init_A()
{
	for (int bi = 1; bi <= NB; bi++) {
		A[bi] = 0;
	}
}

void assign_cluster()
{
	long cluster_id;
	long idx_start;
	long idx_end;
	long max_W;

	for (int i = 0; i <= top; i++) {
		cluster_id = Stack[i].cluster_id;
		idx_start = Stack[i].idx_start;
		idx_end = Stack[i].idx_end;

		max_W = 0;
		for (int bi = idx_start; bi <= idx_end; bi++) {
			A[bi] = cluster_id;

			if (W[bi] > max_W)
				max_W = W[bi];				
		}
		max_W_cluster[cluster_id] = max_W;
	}
}

void print_max_Ws()
{
	for (int ki = 1; ki <= NK; ki++) {
		printf("%2d ", max_W_cluster[ki]);
	}
	cout << endl;
}
long find_max_W()
{
	long max_W_all = 0;

	for (int ki = 1; ki <= NK; ki++) {
		if (max_W_cluster[ki] > max_W_all)
			max_W_all = max_W_cluster[ki];
	}

	return max_W_all;
}


void print_data(void)
{
	for (int bi = 1; bi <= NB; bi++) {
		printf("%2d ", W[bi]);
	}
	cout << endl;

	for (int ki = 1; ki <= NK; ki++) {
		printf("%2d ", S[ki]);
	}
	cout << endl;
}

int solve(void)
{
	print_data();

	bool loop_flag = true;

	ClusterInfo elm;
	int cur_cluster_id, cur_idx_start, cur_num_of_blocks, cur_idx_end, cur_idx_next_start;
	int next_cluster_id, next_idx_start, next_num_of_blocks, next_idx_end, next_idx_next_start;
	long iblk;


	long max_W;
	long total_min_W = 200000;

	init_stack();

	elm.cluster_id = 1;
	elm.idx_start = 1;
	elm.num_of_blocks = S[1];
	elm.idx_end = elm.idx_start + elm.num_of_blocks - 1;
	elm.idx_next_start = 1;
	push(elm);

	while (!is_empty_stack() && loop_flag) {
		elm = pop();

		cur_cluster_id = elm.cluster_id;
		cur_idx_start = elm.idx_start;			cur_idx_end = elm.idx_end;
		cur_num_of_blocks = elm.num_of_blocks;	cur_idx_next_start = elm.idx_next_start;

		iblk = cur_idx_next_start;

		while (iblk <= NB) {

			next_cluster_id = cur_cluster_id + 1;
			next_idx_start = iblk + cur_num_of_blocks;			
			next_idx_end = next_idx_start + S[next_cluster_id] - 1;
			next_num_of_blocks = S[next_cluster_id];


			if (next_cluster_id <= NK && next_idx_end <= NB) {

				// 현재 위치와 현재 위치에서 다음 번에 볼 방향 푸시
				// (클러스터 정보, 현재 시작 위치)  (다음 번 시작 위치)
				elm.cluster_id = cur_cluster_id;
				elm.idx_start = cur_idx_start;
				elm.num_of_blocks = cur_num_of_blocks;
				elm.idx_end = cur_idx_end;
				elm.idx_next_start = iblk + 1;	// cur_idx_start+1;
				push(elm);

				// move
				cur_cluster_id = next_cluster_id;
				cur_idx_start = next_idx_start;			cur_idx_end = next_idx_end;
				cur_num_of_blocks = next_num_of_blocks;	cur_idx_next_start = next_idx_start +1;
				iblk = next_idx_start;

				// 다 왔는지?
				if (cur_cluster_id == NK) {
					// 
					elm.cluster_id = cur_cluster_id;
					elm.idx_start = cur_idx_start;
					elm.num_of_blocks = cur_num_of_blocks;
					elm.idx_end = cur_idx_end;
					elm.idx_next_start = iblk + 1;	// cur_idx_start+1;
					push(elm);

					assign_cluster();
#if DEBUG_LEVEL >= 1
					print_data();
					print_A();
					print_max_Ws();
#endif
					max_W = find_max_W();
					if (max_W < total_min_W)
						total_min_W = max_W;

					pop();
					init_A();
				}
			}
		}
	}

	return total_min_W;
}

int main(void)
{
	int const n_test_case = N_TEST_CASE;

	for (int i_tc = 1; i_tc <= n_test_case; i_tc++) {
		cin >> NB >> NK;

		for (int bi = 1; bi <= NB; bi++)
			cin >> W[bi];

		for (int ki = 1; ki <= NK; ki++)
			cin >> S[ki];


		int answer = solve();
		cout << "#" << i_tc << " " << answer << endl;
	}


	return 0;
}
