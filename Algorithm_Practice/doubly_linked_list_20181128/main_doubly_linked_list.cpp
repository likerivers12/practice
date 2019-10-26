#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#define NULL	0

typedef struct Node * Node_pointer;

typedef struct Node
{
//public:
	int data = 0;
	Node_pointer prev = 0;
	Node_pointer next = 0;
};


int make_new_node(Node_pointer *p_nd, int d)
{
	//*p_nd = new Node[1];
	//*p_nd = new Node();
	*p_nd = new Node;
	//*p_nd = (Node_pointer)malloc(sizeof(Node) * 1);

	if (*p_nd == NULL) {
		printf("Out of memory!\n");
		return -1;
	}

	(*p_nd)->data = d;
	(*p_nd)->prev = NULL;
	(*p_nd)->next = NULL;

	return 0;
}

int delete_list(Node_pointer * p_head, Node_pointer * p_tail)
{
	if (*p_head == NULL && *p_tail == NULL) {
		printf("NO del\n");
		return 0;
	}
	else if (*p_head == *p_tail) {
		delete *p_head;
		printf("del:%d\n", (*p_head)->data);
		*p_head = NULL;
		*p_tail = NULL;
		return 0;
	}

	Node_pointer s;
	Node_pointer t;
	for (s = *p_head; s != *p_tail; ) {
		t = s;
		s = s->next;
		printf("del:%d\n", t->data);
		delete t;
	}
	printf("del:%d\n", (*p_tail)->data);
	delete *p_tail;

	*p_head = NULL;
	*p_tail = NULL;

	return 0;
}

int get_data(int * p_val, Node_pointer head, int index)
{
	if (head == NULL) return -1;

	int i;
	Node_pointer t = head;

	for (i = 0; i <= index; i++) {
		if (i == index) {
			*p_val = t->data;
			return 0;
		}
		else if (t->next != NULL) {
			t = t->next;
		}
		else {
			return -1;
		}
	}

	return -1;
}

int putQ(Node_pointer * p_head, Node_pointer * p_tail, int d)
{
	Node_pointer nd;
	Node_pointer t;
	int ret = make_new_node(&nd, d);
	if (ret != 0) {
		return -1;
	}

	if (*p_head == NULL && *p_tail == NULL) {
		(*p_head) = nd;
		(*p_tail) = nd;
	}
	else if (*p_head == *p_tail) {
		nd->prev = (*p_head);
		nd->prev->next = nd;

		(*p_tail) = nd;
	}
	else {
		//t = (*p_tail)
		nd->prev = (*p_tail);
		nd->prev->next = nd;

		(*p_tail) = nd;
	}

	return 0;
}

int getQ(int * p_val, Node_pointer * p_head, Node_pointer * p_tail)
{
	if (*p_head == NULL && *p_tail == NULL) {
		printf("Q is empty!\n");
		return -1;
	}
	else if (*p_head == *p_tail) {
		*p_val = (*p_head)->data;
		delete *p_head;
		*p_head = NULL;
		*p_tail = NULL;
	}
	else {
		Node_pointer t = *p_head;
		*p_val = t->data;
		*p_head = t->next;
		delete t;
	}

	return 0;
}

int print_list(Node_pointer head, Node_pointer tail)
{
	Node_pointer t = head;

	
	while (t != NULL) {
		printf("%d ", t->data);
		t = t->next;
	}
	printf("\n");
	return 0;
}

int init_list(Node_pointer * p_head, Node_pointer * p_tail)
{

	*p_head = NULL;
	*p_tail = NULL;

	return 0;
}

int main(void)
{
	Node_pointer head = NULL;
	Node_pointer tail = NULL;
	int val;
	int ret;

	//----------------------
	delete_list(&head, &tail);
	init_list(&head, &tail);


	//----------------------
	Node_pointer p_node;
	ret = make_new_node(&p_node, 1);
	printf("ret:%d, val:%d\n", ret, p_node->data);
	printf("[%d] [%d] [%d]\n", p_node->data, p_node->prev, p_node->next);

	//----------------------
	putQ(&head, &tail, 1);
	print_list(head, tail);

	putQ(&head, &tail, 2);
	print_list(head, tail);

	putQ(&head, &tail, 3);
	print_list(head, tail);

	//----------------------
	ret = get_data(&val, head, 2);
	printf("ret:%d, val:%d\n", ret, val);

	ret = get_data(&val, head, 1);
	printf("ret:%d, val:%d\n", ret, val);

	ret = get_data(&val, head, 0);
	printf("ret:%d, val:%d\n", ret, val);

	//----------------------
	ret = getQ(&val, &head, &tail);
	printf("ret:%d, val:%d\n", ret, val);
	print_list(head, tail);

	ret = getQ(&val, &head, &tail);
	printf("ret:%d, val:%d\n", ret, val);
	print_list(head, tail);

	ret = getQ(&val, &head, &tail);
	printf("ret:%d, val:%d\n", ret, val);
	print_list(head, tail);

	//----------------------
	putQ(&head, &tail, 4);
	print_list(head, tail);

	putQ(&head, &tail, 5);
	print_list(head, tail);

	putQ(&head, &tail, 6);
	print_list(head, tail);

	//----------------------
	delete_list(&head, &tail);
	print_list(head, tail);


	ret = get_data(&val, p_node, 0);
	printf("ret:%d, val:%d\n", ret, val);


	delete_list(&head, &tail);
	return 0;
}