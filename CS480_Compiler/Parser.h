// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include "Symbol_Table.h"
#include "Token.h"
#include "Lex.h"


class Parser
{

private:

	Lexer lex;
	std::ifstream &source;
	std::string source_name;

	std::string variables;

	Symbol_Table table;
	std::queue<mesg> errors;

	bool fatal_error;

public:
	//var type will not exist in the final implimentation but we use it here as a filler
	
	typedef  struct _synth_return{
		oper_type type;
		std::string attr;
	}synth_return;


	Parser(std::ifstream &source, std::string source_name);

	std::string parse();

	std::string start();

	std::string s();

	std::string s_1();

	std::string s_2();

	synth_return expr();

	synth_return expr_1();

	synth_return oper();

	synth_return oper_1();

	synth_return stmt();

	synth_return stmt_1();

	int binop();

	int unop();

	synth_return const_0();

	synth_return ifstmt();

	synth_return exprlist();

	synth_return exprlist_1();

	synth_return varlist();

	synth_return varlist_1();

	synth_return type();

	synth_return negop();

	synth_return append_ID_let();

	synth_return append_ID();

	synth_return append_CONST();

	void pop_lex(std::stringstream&);

	void error(std::string msg);

	bool is_CONST(int);

	bool is_BINOP(int);

	bool is_UNOP(int);

	bool is_bool_BINOP(int);

	bool is_real_int_BINOP(int);

	bool is_log_BINOP(int);

	std::string oper_to_string(oper_type);

};

#endif // !PARSER_H