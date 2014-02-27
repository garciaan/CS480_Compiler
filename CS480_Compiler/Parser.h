// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"


void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, bool verbose);

int start(Lexer &lex);

int s(Lexer &lex);

int s_1(Lexer &lex);

int s_2(Lexer &lex);

int expr(Lexer &lex);

int expr_1(Lexer &lex);

int oper(Lexer &lex);

int oper_1(Lexer &lex);

int stmt(Lexer &lex);

int stmt_1(Lexer &lex);

int binop(Lexer &lex);

int unop(Lexer &lex);

int const_0(Lexer &lex);

int ifstmt(Lexer &lex);

int exprlist(Lexer &lex);

int exprlist_1(Lexer &lex);

int varlist(Lexer &lex);

int varlist_1(Lexer &lex);

int type(Lexer &lex);

int negop(Lexer &lex);

bool is_CONST(int);

bool is_BINOP(int);

bool is_UNOP(int);


#endif // !PARSER_H