#include <iostream>

using namespace std;

#define MAX_NODE 10
#define MAX_CHILDREN 10

typedef struct Node {
	int val;
	int n_children;
	int children[MAX_CHILDREN];
} Node;

Node * tree[MAX_NODE];


void free_tree()
{
	for (int i = 0; i < MAX_NODE; i++) {
		delete tree[i];
	}
}

int main(void)
{
	int root_idx = 0;

	tree[0] = new Node();
	tree[0]->val = 10;
	tree[0]->children[0] = 1;
	tree[0]->children[1] = 2;
	tree[0]->children[2] = 3;
	tree[0]->n_children = 3;
		
	free_tree();

	return 0;
}