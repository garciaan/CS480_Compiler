//Token.cpp

#include "Token.h"

struct tag_meaning
{
	int tag;
	std::string meaning;
	std::string input;
}

Meanings[] = 
{
	{ AND, "AND", "and" },
	{ OR, "OR" , "or"},
	{ TRUE, "TRUE", "true" },
	{ FALSE, "FALSE", "false" },
	{ NOT, "NOT", "not" },
	{ BOOL_T, "BOOL_T", "bool" },
	{ INT_T, "INT_T", "int" },
	{ REAL_T, "REAL_T", "real" },
	{ STRING_T, "STRING_T", "string" },
	{ LET, "LET", "let" },
	{ IF, "IF", "if" },
	{ WHILE, "WHILE", "while" },
	{ L_BRACKET, "L_BRACKET", "[" },
	{ R_BRACKET, "R_BRACKET", "]" },
	{ PLUS, "PLUS", "+" },
	{ MINUS, "MINUS", "-" },
	{ MULTI, "MULTI", "*" },
	{ DIV, "DIV", "/" },
	{ MOD, "MOD", "%" },
	{ EXP, "EXP", "^" },
	{ EQ, "EQ", "=" },
	{ NE, "NE", "!=" },
	{ LT, "LT", "<" },
	{ GT, "GT", ">" },
	{ LE, "LE", "<=" },
	{ GE, "GE", ">=" },
	{ ASSIGN, "ASSIGN", ":=" },
	{ ID, "ID", "id" },
	{ INT_L, "INT_L", "int_l" },
	{ REAL_L, "REAL_L", "real_l" },
	{ SIN, "SIN", "sin" },
	{ COS, "COS", "cos" },
	{ TAN, "TAN", "tan" },
	{ STDOUT, "STDOUT", "stdout" },
	{ STRING_L, "STRING_L", "string_l" },
	{ LINE_END, "LINE_END", "\n" },
	{ INPUT_END, "INPUT_END", "$" },
	{ NON_START, "NON_START", "start" },
	{ NON_S, "NON_S", "S" },
	{ NON_S_1, "NON_S_1", "S'" },
	{ NON_S_2, "NON_S_2", "S''" },
	{ NON_EXPR, "NON_EXPR", "EXPR" },
	{ NON_EXPR_1, "NON_EXPR_1", "EXPR'" },
	{ NON_OPER, "NON_OPER", "OPER" },
	{ NON_OPER_1, "NON_OPER_1", "OPER'" },
	{ NON_STMT, "NON_STMT", "STMT" },
	{ NON_STMT_1, "NON_STMT_1", "STMT'" },
	{ NON_BINOP, "NON_BINOP", "BINOP" },
	{ NON_UNOP, "NON_UNOP", "UNOP" },
	{ NON_CONST, "NON_CONST", "CONST" },
	{ NON_IFSTMT, "NON_IFSTMT", "IFSTMT" },
	{ NON_EXPRLIST, "NON_EXPRLIST", "EXPRLIST" },
	{ NON_EXPRLIST_1, "NON_EXPRLIST_1", "EXPRLIST'" },
	{ NON_VARLIST, "NON_VARLIST", "VARLIST" },
	{ NON_VARLIST_1, "NON_VARLIST_1", "VARLIST'" },
	{ NON_TYPE, "NON_TYPE", "TYPE" },
	{ NON_NEGOP, "NON_NEGOP", "NEGOP" },
	{ EMPTY, "EMPTY", "EMPTY" }
};

std::string Token::tag_to_string(int tag){

	const int numMeanings = sizeof(Meanings) / sizeof(struct tag_meaning);

	for (int i = 0; i < numMeanings; ++i){

		if (tag == Meanings[i].tag)
		{
			return Meanings[i].meaning;
		}
	}

	return "BAD_INPUT";

}

std::string Token::tag_to_input(int tag){

	const int numMeanings = sizeof(Meanings) / sizeof(struct tag_meaning);

	for (int i = 0; i < numMeanings; ++i){

		if (tag == Meanings[i].tag)
		{
			return Meanings[i].input;
		}
	}

	return "BAD_INPUT";

}

int Token::string_to_tag(std::string word){

	const int numMeanings = sizeof(Meanings) / sizeof(struct tag_meaning);

	for (int i = 0; i < numMeanings; ++i){

		if (word == Meanings[i].meaning)
		{
			return Meanings[i].tag;
		}
	}

	return -1;

}


void Token::print(){
	std::cout << "<" << tag_to_string(tag) << ">";
}

int Token::get_tag(){
	return tag;
}

void Token::set_tag(int _tag){
	tag = _tag;
}

StrToken::StrToken(std::string _str) : Token(STRING_L), value(_str){}

void StrToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

IdToken::IdToken(std::string _str) : Token(ID), value(_str){}

void IdToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

IntToken::IntToken(std::string _int) : Token(INT_L)
{
	value = atoi(_int.c_str());
}

void IntToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

RealToken::RealToken(std::string _real) : Token(REAL_L)
{
	value = atof(_real.c_str());
}

void RealToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

LineToken::LineToken(int _line) : Token(LINE_END)
{
	line = _line;
}

void LineToken::print(){
	std::cout << std::endl;
}

bool is_non_terminal(Token token){
	if (token.get_tag() >= 200 && token.get_tag() < 400){
		return true;
	}
	return false;
}

bool is_terminal(Token token){
	if (token.get_tag() > 0 && token.get_tag() < 200){
		return true;
	}
	return false;
}