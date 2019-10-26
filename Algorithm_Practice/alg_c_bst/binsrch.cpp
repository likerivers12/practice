#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "binsrch.h"

using namespace std;


#define BASE(i)		((char *) base + (i)*width)

//typedef int(*FCMP)(const void*, const void*);

int bv_search(void *key, void *base, size_t *num, size_t width, FCMP fcmp)
{
	int mid;
	int left = 0;
	int right = *num - 1;
	while (right >= left) {
		mid = (left + right) / 2;
		if (fcmp(key, BASE(mid)) == 0)
			return mid;
		else if (fcmp(key, BASE(mid)) < 0)
			right = mid - 1;
		else
			left = mid + 1;
	}

	return -1;
}

int bv_insert(void *key, void *base, size_t *num, size_t width, FCMP fcmp)
{
	int p = 0;
	int i;
	while (fcmp(key, BASE(p)) > 0 && p < *num)
		p++;										// key���� ���� ���� ��������.
	for (i = *num; i > p; i--)
		memcpy(BASE(i), BASE(i - 1), width);		// �ڷ� �� ĭ�� �о��.
	memcpy(BASE(p), key, width);					// key ����
	(*num)++;
	return p;
}

int bv_delete(void *key, void *base, size_t *num, size_t width, FCMP fcmp)
{
	int p;
	int i;
	if (*num > 0) {
		if ((p = bv_search(key, base, num, width, fcmp)) < 0)
			return -1;
		for (i = p + 1; i < *num; i++)
			memcpy(BASE(i - 1), BASE(i), width);		// ������ �� ĭ�� ����.
		(*num)--;
		return p;
	}

	return -1;
}

