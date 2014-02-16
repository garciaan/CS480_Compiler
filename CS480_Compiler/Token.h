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


#define INPUT_END 333

#define LINE_END 999



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


#endif //TOKEN_H