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
#include "SymbolTable.h"

struct _lex_mesg{
	int line;
	std::string msg;
} typedef lex_mesg;


void tokenize(std::istream &source, std::queue<Token*> &queue, SymbolTable &table, std::queue<lex_mesg> &errors, std::queue<lex_mesg> &warnings);

#endif //LEX_H