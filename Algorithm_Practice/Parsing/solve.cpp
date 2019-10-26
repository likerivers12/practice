#include "solve.h"
#include <iostream>

using namespace std;

extern int N;

#define TID_VAR 0
#define TID_NUM 2
#define TID_EQ 3
#define TID_OP_ADD  10
#define TID_OP_SUB  11
#define TID_OP_MUL 12


#define MAX_BUF_SIZE 256
#define NULL	0

typedef struct {

} Element;


typedef struct {
	int tok_id;
	char tok_str[MAX_BUF_SIZE];
	int tok_value;
} Token;


int get_token(Token *tok, char str[], int &s_pos);
int get_num_value(char * str);


#define MAX_STACK_SIZE 100
Token stack[MAX_STACK_SIZE];
int top;

void init_stack()
{
	top = -1;
}

void push(Token const & tok)
{
	top++;
	stack[top].tok_id = tok.tok_id;
	// for 
	//stack[i].tok_str = tok.tok_str[i];
	stack[top].tok_value = tok.tok_value;
}

Token pop(void)
{
	int tmp = top;
	top--;
	return stack[tmp];
}


//#define MAX_STACK_SIZE 100
//Element stack[MAX_STACK_SIZE];
//int top = -1;


int get_var_token(Token *tok, char str[], int &s_pos)
{
	char ch;
	int n = 0;

	while (true) {
		ch = str[s_pos];

		if (ch - '0' >= 0 && ch - '0' <= 9) {
			tok->tok_str[n++] = ch;
			s_pos++;
		}
		else {
			break;
		}
	}
	tok->tok_str[n] = NULL;

	return 0;
}

int get_num_token(Token *tok, char str[], int &s_pos)
{
	char ch;
	int n = 1;

	while (true) {
		ch = str[s_pos];

		if (ch - '0' >= 0 && ch - '0' <= 9) {
			tok->tok_str[n++] = ch;
			s_pos++;
		}
		else {
			break;
		}
	}
	tok->tok_str[n] = NULL;

	return 0;
}

int get_token(Token *tok, char str[], int &s_pos)
{
	char buf[MAX_BUF_SIZE];

	int n = 0;
	char ch;
	while (true) {
		ch = str[s_pos++];
		switch (ch) {
		case '=':
			tok->tok_str[n] = ch;
			tok->tok_str[n + 1] = NULL;
			tok->tok_id = TID_EQ;
			return 0;

		case '+':
			tok->tok_str[n] = ch;
			tok->tok_str[n + 1] = NULL;
			tok->tok_id = TID_OP_ADD;
			return 0;

		case '-':
			tok->tok_str[n] = ch;
			tok->tok_str[n + 1] = NULL;
			tok->tok_id = TID_OP_SUB;
			return 0;

		case '*':
			tok->tok_str[n] = ch;
			tok->tok_str[n + 1] = NULL;
			tok->tok_id = TID_OP_MUL;
			return 0;

		case 'D':
			tok->tok_id = TID_VAR;
			return get_var_token(tok, str, s_pos);

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			tok->tok_id = TID_NUM;
			tok->tok_str[0] = ch;
			return get_num_token(tok, str, s_pos);
		case 0:
			return -1;
		}
	}
	return 0;
}

int get_num_value(char *str)
{
	int val = 0;
	int num;
	int digit = 10;

	for (int i = 0; str[i] != NULL; i++) {
		num = str[i] - '0';
		val = digit*val + num;
	}

	return val;
}

int solve(char in_str[], int in_var[])
{
	init_stack();

	Token tok;
	Token tok2;
	Token tok1;

	int s_pos = 0;
	int num;

	while (get_token(&tok, in_str, s_pos) >= 0) {

		cout << tok.tok_id << endl;
		if (tok.tok_id == TID_VAR)
			cout << "D";
		cout << tok.tok_str << endl;



		if (tok.tok_id == TID_VAR) {
			int var_idx = get_num_value(tok.tok_str);
			tok.tok_value = in_var[var_idx];

			cout << var_idx << endl;
			cout << "value: (" << in_var[var_idx] << ")" << endl;
			
			// 스택에 넣는다.
			tok.tok_id = TID_NUM;
			push(tok);
		}
		else if (tok.tok_id == TID_NUM) {
			num = get_num_value(tok.tok_str);
			tok.tok_value = num;

			cout << "value: (" << num << ")" << endl;
			// 스택에 넣는다.
			push(tok);
		}
		else if (tok.tok_id == TID_OP_ADD || tok.tok_id == TID_OP_SUB) {
			// POP, PUSH NEW, PUSH BACK
			tok1 = pop();
			push(tok);
			push(tok1);

		}
		else if (tok.tok_id == TID_OP_MUL) {
			tok2;
			get_token(&tok2, in_str, s_pos);

			if (tok2.tok_id == TID_NUM) {
				int num = get_num_value(tok2.tok_str);
				tok2.tok_value = num;
			}
			else if (tok2.tok_id == TID_VAR) {
				int var_idx = get_num_value(tok2.tok_str);
				tok2.tok_value = in_var[var_idx];

				cout << var_idx << endl;
				cout << "value: (" << in_var[var_idx] << ")" << endl;
			}
			else {
				// error!
				cout << "Error! Not a Number!" << endl;
			}

			// POP tok2
			tok1 = pop();
			int num1 = tok1.tok_value;
			int num2 = tok2.tok_value;

			tok1.tok_value = num1 * num2;
			cout << "Mult: (" << tok1.tok_value << ") = (" << num1 << "*" << num2 << ")" << endl;
			push(tok1);

		}

		cout << "-------------" << endl;
	}

	// 후위식으로 스택에 넣고 ex) 1+2 -> BTM| 1 2 + | TOP
	// index 0부터 처리
	Token tok_op;
	int res_val = 0;
	int i = 0;
	tok_op = stack[i++];
	tok1 = stack[i++];
	tok2 = stack[i++];
	if (tok_op.tok_id == TID_OP_ADD) {
		res_val = tok1.tok_value + tok2.tok_value;
	}
	else if (tok_op.tok_id == TID_OP_SUB) {
		res_val = tok1.tok_value - tok2.tok_value;
	}
	else {
		cout << "ERROR! NOT ADD nor SUB" << endl;
	}
	while (i <= top) {
		if (tok_op.tok_id == TID_OP_ADD) {
			res_val = tok1.tok_value + tok2.tok_value;
		}
		else if (tok_op.tok_id == TID_OP_SUB) {
			res_val = tok1.tok_value - tok2.tok_value;
		}
		else {
			cout << "ERROR! NOT ADD nor SUB" << endl;
		}
	}
	//while (top > 2) {
	//	// POP, PUSH NEW, PUSH BACK
	//	tok2 = pop();
	//	tok1 = pop();
	//	tok_op = pop();

	//	if (tok_op.tok_id == TID_OP_ADD) {
	//		tok1.tok_value = tok1.tok_value + tok2.tok_value;
	//		push(tok1);
	//	}
	//	else if (tok_op.tok_id == TID_OP_SUB) {
	//		tok1.tok_value = tok1.tok_value - tok2.tok_value;
	//		push(tok1);
	//	}
	//	else {
	//		cout << "ERROR! NOT ADD nor SUB" << endl;
	//	}
	//}

	return stack[top].tok_value;
}
