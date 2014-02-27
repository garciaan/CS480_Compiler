// Token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <cstdlib>


//tag values
//Terminal symbols go from 1 to 199
#define AND 1
#define OR 2
#define TRUE 3
#define FALSE 4
#define NOT 5
#define BOOL_T 6
#define INT_T 7
#define REAL_T 8
#define STRING_T 9
#define LET 10
#define IF 11
#define WHILE 12
#define L_BRACKET 13
#define R_BRACKET 14
#define PLUS 15
#define MINUS 16
#define MULTI 17
#define DIV 18
#define MOD 19
#define EXP 20
#define EQ 21
#define NE 22
#define LT 23
#define GT 24
#define LE 25
#define GE 26
#define ASSIGN 27
#define ID 28
#define INT_L 29
#define REAL_L 30
#define STRING_L 31
#define SIN 32
#define COS 33
#define TAN 34
#define STDOUT 35

//Non-Terminal Symbols
//These start at 200 and go through 399

//Start Non-Terminal
#define NON_START 200
//S Non-Terminal
#define NON_S 201
#define NON_S_1 202
#define NON_S_2 203
#define NON_EXPR 204
#define NON_EXPR_1 205
#define NON_OPER 206
#define NON_OPER_1 207
#define NON_STMT 208
#define NON_STMT_1 209
#define NON_BINOP 210
#define NON_UNOP 211
#define NON_CONST 212
#define NON_IFSTMT 213
#define NON_EXPRLIST 214
#define NON_EXPRLIST_1 215
#define NON_VARLIST 216
#define NON_VARLIST_1 217
#define NON_TYPE 218
#define NON_NEGOP 219

#define EMPTY 299

#define LINE_END 300
#define INPUT_END 333

// End non-terminals

static

struct tag_meaning
{
	int tag;
	std::string meaning;
	std::string input;
}

Meanings[] =
{
	{ AND, "AND", "and" },
	{ OR, "OR", "or" },
	{ TRUE, "TRUE", "true" },
	{ FALSE, "FALSE", "false" },
	{ NOT, "NOT", "not" },
	{ BOOL_T, "BOOL_T", "bool" },
	{ INT_T, "INT_T", "int" },
	{ REAL_T, "REAL_T", "real" },
	{ STRING_T, "STRING_T", "string" },
	{ LET, "LET", "let" },
	{ IF, "IF", "if" },
	{ WHILE, "WHILE", "while" },
	{ L_BRACKET, "L_BRACKET", "[" },
	{ R_BRACKET, "R_BRACKET", "]" },
	{ PLUS, "PLUS", "+" },
	{ MINUS, "MINUS", "-" },
	{ MULTI, "MULTI", "*" },
	{ DIV, "DIV", "/" },
	{ MOD, "MOD", "%" },
	{ EXP, "EXP", "^" },
	{ EQ, "EQ", "=" },
	{ NE, "NE", "!=" },
	{ LT, "LT", "<" },
	{ GT, "GT", ">" },
	{ LE, "LE", "<=" },
	{ GE, "GE", ">=" },
	{ ASSIGN, "ASSIGN", ":=" },
	{ ID, "ID", "id" },
	{ INT_L, "INT_L", "int_l" },
	{ REAL_L, "REAL_L", "real_l" },
	{ SIN, "SIN", "sin" },
	{ COS, "COS", "cos" },
	{ TAN, "TAN", "tan" },
	{ STDOUT, "STDOUT", "stdout" },
	{ STRING_L, "STRING_L", "string_l" },
	{ LINE_END, "LINE_END", "\n" },
	{ INPUT_END, "INPUT_END", "$" },
	{ NON_START, "NON_START", "start" },
	{ NON_S, "NON_S", "S" },
	{ NON_S_1, "NON_S_1", "S'" },
	{ NON_S_2, "NON_S_2", "S''" },
	{ NON_EXPR, "NON_EXPR", "EXPR" },
	{ NON_EXPR_1, "NON_EXPR_1", "EXPR'" },
	{ NON_OPER, "NON_OPER", "OPER" },
	{ NON_OPER_1, "NON_OPER_1", "OPER'" },
	{ NON_STMT, "NON_STMT", "STMT" },
	{ NON_STMT_1, "NON_STMT_1", "STMT'" },
	{ NON_BINOP, "NON_BINOP", "BINOP" },
	{ NON_UNOP, "NON_UNOP", "UNOP" },
	{ NON_CONST, "NON_CONST", "CONST" },
	{ NON_IFSTMT, "NON_IFSTMT", "IFSTMT" },
	{ NON_EXPRLIST, "NON_EXPRLIST", "EXPRLIST" },
	{ NON_EXPRLIST_1, "NON_EXPRLIST_1", "EXPRLIST'" },
	{ NON_VARLIST, "NON_VARLIST", "VARLIST" },
	{ NON_VARLIST_1, "NON_VARLIST_1", "VARLIST'" },
	{ NON_TYPE, "NON_TYPE", "TYPE" },
	{ NON_NEGOP, "NON_NEGOP", "NEGOP" },
	{ EMPTY, "EMPTY", "EMPTY" }
};


class Token
{

protected:

	int tag;

public:

	Token(int _tag) : tag(_tag){}
	Token(){}
	virtual void print();

	int get_tag();
	void set_tag(int _tag);

	static std::string tag_to_string(int tag);
	static int string_to_tag(std::string word);
	static std::string tag_to_input(int tag);

};

class StrToken : public Token
{
	std::string value;

public:
	StrToken(std::string _str);
	void print();
};

class IdToken : public Token
{
	std::string value;

public:
	IdToken(std::string _str);
	void print();
};


class IntToken : public Token
{
	int value;

public:
	IntToken(std::string _int);
	void print();
};

class RealToken : public Token
{
	double value;

public:
	RealToken(std::string _real);
	void print();
};


class LineToken : public Token
{
	int line;

public:
	LineToken(int _line);
	void print();
};


bool is_terminal(Token token);
bool is_non_terminal(Token token);
bool is_record(Token token);



#endif //TOKEN_H