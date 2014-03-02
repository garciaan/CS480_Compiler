// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include "Token.h"
#include "Lex.h"

namespace parser
{
	//var type will not exist in the final implimentation but we use it here as a filler
	enum oper_type { INT, REAL, STRING, BOOL, ERROR, EMP, VAR };

	typedef  struct _oper_return{
		oper_type type;
		std::string code;
	}oper_return;

	void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, bool verbose);

	int start(Lexer &lex);

	std::string s(Lexer &lex);

	std::string s_1(Lexer &lex);

	std::string s_2(Lexer &lex);

	std::string expr(Lexer &lex);

	std::string expr_1(Lexer &lex);

	oper_return oper(Lexer &lex);

	oper_return oper_1(Lexer &lex);

	std::string stmt(Lexer &lex);

	std::string stmt_1(Lexer &lex);

	int binop(Lexer &lex);

	int unop(Lexer &lex);

	oper_return const_0(Lexer &lex);

	std::string ifstmt(Lexer &lex);

	std::string exprlist(Lexer &lex);

	std::string exprlist_1(Lexer &lex);

	std::string varlist(Lexer &lex);

	std::string varlist_1(Lexer &lex);

	std::string type(Lexer &lex);

	oper_return negop(Lexer &lex);

	std::string append_ID(Lexer &lex);

	std::string append_CONST(Lexer &lex);

	bool is_CONST(int);

	bool is_BINOP(int);

	bool is_UNOP(int);

	bool is_bool_BINOP(int);

	bool is_real_int_BINOP(int);

	bool is_log_BINOP(int);

}

#endif // !PARSER_H