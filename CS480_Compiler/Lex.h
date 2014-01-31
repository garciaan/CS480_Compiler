// Lex.h

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <queue>

#include "Token.h"
#include "SymbolTable.h"

void tokenize(std::istream &source_file, std::queue<Token*> &queue, SymbolTable &table);

#endif //LEX_H