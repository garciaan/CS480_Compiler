// Lex.h

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <queue>
#include <sstream>

#include "Token.h"


struct _mesg{
	int line;
	int loc;
	std::string msg;
} typedef mesg;

static const std::string inital_syms[] = { "true", "false", "and", "or", "not", "bool",
"int", "real", "string", "let", "if", "while", "sin", "cos", "tan",
"stdout"
};

static const int inital_values[] = { TRUE, FALSE, AND, OR, NOT, BOOL_T, INT_T, REAL_T,
STRING_T, LET, IF, WHILE, SIN, COS, TAN, STDOUT };


class Lexer
{
public:
	Lexer(std::istream &_source) : source(_source){}
	~Lexer();

	Token *peek();
	int peek_tag();
	void pop();
	bool source_empty();
	int is_keyword(std::string guess);
	int get_loc();

	int tokenize(int num_tokens);

	const std::queue<mesg>& get_errors();
	const std::queue<mesg>& get_warnings();

private:

	std::istream &source;
	std::queue<Token*> queue; 
	std::queue<mesg> errors;
	std::queue<mesg> warnings;



};



#endif //LEX_H