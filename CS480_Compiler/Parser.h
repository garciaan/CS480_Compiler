// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include "Token.h"
#include "Lex.h"


class Parser
{

private:

	Lexer lex;
	std::ifstream &source;
	Symbol_Table &table;
	std::string source_name;

	std::queue<mesg> errors;

	bool fatal_error = false;

public:
	//var type will not exist in the final implimentation but we use it here as a filler
	enum oper_type { INT, REAL, STRING, BOOL, ERROR, EMP, VAR, CODE };

	typedef  struct _synth_return{
		oper_type type;
		std::string attr;
	}synth_return;


	Parser(std::ifstream &source, std::string source_name, Symbol_Table &table);

	std::string parse();

	std::string start();

	std::string s();

	std::string s_1();

	std::string s_2();

	std::string expr();

	std::string expr_1();

	synth_return oper();

	synth_return oper_1();

	std::string stmt();

	std::string stmt_1();

	int binop();

	int unop();

	synth_return const_0();

	std::string ifstmt();

	std::string exprlist();

	std::string exprlist_1();

	std::string varlist();

	std::string varlist_1();

	std::string type();

	synth_return negop();

	synth_return append_ID();

	synth_return append_CONST();

	void error(std::string msg);

	std::string locate_err(int);

	bool is_CONST(int);

	bool is_BINOP(int);

	bool is_UNOP(int);

	bool is_bool_BINOP(int);

	bool is_real_int_BINOP(int);

	bool is_log_BINOP(int);

};

#endif // !PARSER_H