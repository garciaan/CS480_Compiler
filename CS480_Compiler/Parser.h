// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"



enum oper_type { INT, REAL, STRING, BOOL, CODE};

typedef  struct _oper_return{
	oper_type type;
	std::string str_value;
	int int_value;
	double real_value;
	std::string code;
}oper_return;


void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, bool verbose);

int start(Lexer &lex);

int s(Lexer &lex, std::string *code);

int s_1(Lexer &lex, std::string *code);

int s_2(Lexer &lex, std::string *code);

int expr(Lexer &lex, std::string *code);

int expr_1(Lexer &lex, std::string *code);

int oper(Lexer &lex, oper_return *val);

int oper_1(Lexer &lex, oper_return *val);

int stmt(Lexer &lex, std::string *code);

int stmt_1(Lexer &lex, std::string *code);

int binop(Lexer &lex, oper_return *val);

int unop(Lexer &lex, oper_return *val);

int const_0(Lexer &lex, oper_return *val);

int ifstmt(Lexer &lex, std::string *code);

int exprlist(Lexer &lex, std::string *code);

int exprlist_1(Lexer &lex, std::string *code);

int varlist(Lexer &lex, std::string *code);

int varlist_1(Lexer &lex, std::string *code);

int type(Lexer &lex, std::string *code);

int negop(Lexer &lex, oper_return *val);

//oper evaluation helper functions

void plus(oper_return, oper_return, oper_return*);



bool is_CONST(int);

bool is_BINOP(int);

bool is_UNOP(int);


#endif // !PARSER_H