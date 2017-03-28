#include <iostream>
#include <stdio.h>

// 10:30 ~

using namespace std;

#define MAX_STR_SIZE 1000
#define MAX_VAR_NUM 10

char in_str[MAX_STR_SIZE];
int in_var[MAX_VAR_NUM];
int N = MAX_VAR_NUM;



#include "solve.h"

int main(void)
{
	for (int i = 0; i < MAX_VAR_NUM; i++) {
		cin >> in_var[i];
	}

	scanf("%s", in_str);


	int val = solve(in_str, in_var);
	cout << val << endl;

	return 0;
}