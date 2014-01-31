// Lex.h

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <queue>

#include "Token.h"

void tokenize(std::istream &source_file, std::queue<Token*> &queue);


#endif //LEX_H