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

struct _lex_mesg{
	int line;
	std::string msg;
} typedef lex_mesg;



class Lexer
{
public:
	Lexer(std::istream &_source, Symbol_Table &_table) : source(_source), table(_table){}
	~Lexer();

	Token peek();
	int peek_tag();
	void pop();
	int tokenize(int num_tokens);

	const std::queue<lex_mesg>& get_errors();
	const std::queue<lex_mesg>& get_warnings();

private:

	std::istream &source;
	Symbol_Table &table;
	std::queue<Token*> queue; 
	std::queue<lex_mesg> errors;
	std::queue<lex_mesg> warnings;

};



#endif //LEX_H