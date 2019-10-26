#include <stdio.h>
#include <iostream>

using namespace std;

#define DEFAULT_ARR_SIZE	10

typedef int data_type;

struct DynamicArray
{
	data_type * data;
	data_type default_value;
	int len;
	DynamicArray(int n, data_type val)
	{
		data = new data_type[n];
		default_value = val;
		len = n;

		for (int i = 0; i < len; i++) {
			data[i] = default_value;
		}
	}

	~DynamicArray()
	{
		delete[] data;
	}

	int put(int idx, data_type d)
	{
		if (idx < 0) {
			printf("Invalid index:%d\n", idx);
			return -1;
		}

		if (idx < len) {
			data[idx] = d;
			return 0;
		}

		// 범위초과:
		//----------------------------------------
		//1. 확장
		int tmp_n = len;
		while (len < (idx + 1)) len *= 2;

		data_type * tmp_data = data;
		data = new data_type[len];

		//----------------------------------------
		//2. 복사
		for (int i = 0; i < tmp_n; i++) {
			data[i] = tmp_data[i];
		}

		for (int i = tmp_n; i < len; i++) {
			data[i] = default_value;
		}
		data[idx] = d;

		//----------------------------------------
		//3. 메모리 반환
		delete[] tmp_data;
	}

	data_type get(data_type * v, int idx)
	{
		if (idx >= 0 && idx < len) {
			*v = data[idx];
			return 0;
		}
		
		printf("Invalid index:%d\n", idx);
		return -1;
	}

	int print()
	{
		cout << "------------------------------" << endl;
		cout << "Len:" << len << endl;
		for (int i = 0; i < len; i++) {
			cout << data[i] << " ";
		}
		cout << endl;

		return 0;
	}
};

int main(void)
{
	DynamicArray da = DynamicArray(2, 0);
	data_type val;

	da.print();

	da.put(0, 10);
	da.print();
	da.get(&val, 0);
	cout << val << endl;;
	
	da.put(1, 11);
	da.print();
	da.get(&val, 1);
	cout << val << endl;;

	da.put(3, 13);
	da.print();
	da.get(&val, 3);
	cout << val << endl;;

	da.put(20, 30);
	da.print();
	da.get(&val, 20);
	cout << val << endl;

	return 0;
}