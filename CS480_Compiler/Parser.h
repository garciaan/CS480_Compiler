// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>
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

int binop(Lexer &lex, int *val);

int unop(Lexer &lex, int *val);

int const_0(Lexer &lex, oper_return *val);

int ifstmt(Lexer &lex, std::string *code);

int exprlist(Lexer &lex, std::string *code);

int exprlist_1(Lexer &lex, std::string *code);

int varlist(Lexer &lex, std::string *code);

int varlist_1(Lexer &lex, std::string *code);

int type(Lexer &lex, std::string *code);

int negop(Lexer &lex, oper_return *val);

//oper evaluation helper functions

//binop helpers
void plus(oper_return, oper_return, oper_return*);
void multi(oper_return, oper_return, oper_return*);
void div(oper_return, oper_return, oper_return*);
void mod(oper_return, oper_return, oper_return*);
void exp(oper_return, oper_return, oper_return*);
void eq(oper_return, oper_return, oper_return*);
void le(oper_return, oper_return, oper_return*);
void lt(oper_return, oper_return, oper_return*);
void ge(oper_return, oper_return, oper_return*);
void gt(oper_return, oper_return, oper_return*);
void ne(oper_return, oper_return, oper_return*);
void or(oper_return, oper_return, oper_return*);
void and(oper_return, oper_return, oper_return*);

//unop helpers
void not(oper_return, oper_return*);
void sin(oper_return, oper_return*);
void cos(oper_return, oper_return*);
void tan(oper_return, oper_return*);


void append_ID(Lexer &lex, std::string*);

void append_CONST(Lexer &lex, std::string*);


bool is_CONST(int);

bool is_BINOP(int);

bool is_UNOP(int);


#endif // !PARSER_H