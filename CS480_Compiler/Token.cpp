//Token.cpp

#include "Token.h"


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