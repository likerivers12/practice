#include <iostream>
#include <stdio.h>



int main(void)
{
	/*******************************/
	const int a = 10;
	// a = 11; // error!
	int const b = 20;
	// b = 30; // error!



	/*******************************/
	const int * p_a = &a;
	//*p_a = 20;	// error!
	p_a = &b;		// ok!

	int const * p_b = &b;
	//*p_b = 10;		// error!
	p_b = &a;		// ok


	/*******************************/
	//int * const p_aa = &a;	// error!
	int aa = 10;
	int bb = 20;
	int * const p_aa = &aa;		// ok
	*p_aa = 20;					// ok
	//p_aa = &bb;					// error!
	

	return 0;
}