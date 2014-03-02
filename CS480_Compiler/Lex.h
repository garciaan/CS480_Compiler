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
#include "Symbol_Table.h"

struct _mesg{
	int line;
	int loc;
	std::string msg;
} typedef mesg;



class Lexer
{
public:
	Lexer(std::istream &_source, Symbol_Table &_table) : source(_source), table(_table){}
	~Lexer();

	Token *peek();
	int peek_tag();
	void pop();
	bool source_empty();
	int get_loc();

	int tokenize(int num_tokens);

	const std::queue<mesg>& get_errors();
	const std::queue<mesg>& get_warnings();

private:

	std::istream &source;
	Symbol_Table &table;
	std::queue<Token*> queue; 
	std::queue<mesg> errors;
	std::queue<mesg> warnings;

};



#endif //LEX_H