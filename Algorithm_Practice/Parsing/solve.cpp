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
int get_var_value(char str[]);
int get_num_value(char str[]);







#define MAX_STACK_SIZE 100
Element stack[MAX_STACK_SIZE];
int top = -1;





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

int solve(char in_str[], int in_var[])
{
	Token tok;
	int s_pos = 0;

	while (get_token(&tok, in_str, s_pos) >= 0) {

		cout << tok.tok_id << endl;
		if (tok.tok_id == TID_VAR)
			cout << "D";
		cout << tok.tok_str << endl;
		cout << "-------------" << endl;
	}
	return 0;
}
