// Token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <cstdlib>


//tag values
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


#endif //TOKEN_H