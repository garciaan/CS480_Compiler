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
	{ MOD, "MOD", "mod" },
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
	std::string get_str();
	void print();
};

class IdToken : public Token
{
	std::string value;

public:
	IdToken(std::string _str);
	std::string get_id();
	void print();
};


class IntToken : public Token
{
	int value;

public:
	IntToken(std::string _int);
	int get_int();
	void print();
};

class RealToken : public Token
{
	double value;

public:
	RealToken(std::string _real);
	double get_real();
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