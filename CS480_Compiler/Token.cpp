//Token.cpp

#include "Token.h"

struct tag_meaning
{
	int tag;
	std::string meaning;
}

Meanings[] = 
{
	{ AND, "AND" },
	{ OR, "OR" },
	{ TRUE, "TRUE" },
	{ FALSE, "FALSE" },
	{ NOT, "NOT" },
	{ BOOL_T, "BOOL_T" },
	{ INT_T, "INT_T" },
	{ REAL_T, "REAL_T" },
	{ STRING_T, "STRING_T" },
	{ LET, "LET" },
	{ IF, "IF" },
	{ WHILE, "WHILE" },
	{ L_BRACKET, "L_BRACKET" },
	{ R_BRACKET, "R_BRACKET" },
	{ PLUS, "PLUS" },
	{ MINUS, "MINUS" },
	{ MULTI, "MULTI" },
	{ DIV, "DIV" },
	{ MOD, "MOD" },
	{ EXP, "EXP" },
	{ EQ, "EQ" },
	{ NE, "NE" },
	{ LT, "LT" },
	{ GT, "GT" },
	{ LE, "LE" },
	{ GE, "GE" },
	{ ASSIGN, "ASSIGN" },
	{ ID, "ID" },
	{ INT_L, "INT_L" },
	{ REAL_L, "REAL_L" },
	{ STRING_L, "STRING_L" },
	{ LINE_END, "LINE_END" },
	{ INPUT_END, "INPUT_END" },
	{ NON_START, "NON_START" },
	{ NON_S, "NON_S" },
	{ NON_S_1, "NON_S_1" },
	{ NON_S_2, "NON_S_2" },
	{ NON_EXPR, "NON_EXPR" },
	{ NON_EXPR_1, "NON_EXPR_1" },
	{ NON_OPER, "NON_OPER" },
	{ NON_OPER_1, "NON_OPER_1" },
	{ NON_STMT, "NON_STMT" },
	{ NON_STMT_1, "NON_STMT_1" },
	{ NON_BINOP, "NON_BINOP" },
	{ NON_UNOP, "NON_UNOP" },
	{ NON_CONST, "NON_CONST" },
	{ NON_IFSTMT, "NON_IFSTMT" },
	{ NON_EXPRLIST, "NON_EXPRLIST" },
	{ NON_EXPRLIST_1, "NON_EXPRLIST_1" },
	{ NON_VARLIST, "NON_VARLIST" },
	{ NON_VARLIST_1, "NON_VARLIST_1" },
	{ NON_TYPE, "NON_TYPE" },
	{ NON_NEGOP, "NON_NEGOP" },
	{ EMPTY, "EMPTY" }
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