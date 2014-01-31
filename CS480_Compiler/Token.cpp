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
	{ LINE_END, "LINE_END" }
};

std::string Token::tag_to_string(int tag){

	const int numMeanings = sizeof(Meanings) / sizeof(struct tag_meaning);

	for (int i = 0; i < numMeanings; ++i){

		if (tag == Meanings[i].tag)
		{
			return Meanings[i].meaning;
		}
	}

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