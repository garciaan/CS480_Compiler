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

	return "";

}

std::string Token::tag_to_input(int tag){

	const int numMeanings = sizeof(Meanings) / sizeof(struct tag_meaning);

	for (int i = 0; i < numMeanings; ++i){

		if (tag == Meanings[i].tag)
		{
			return Meanings[i].input;
		}
	}

	return "";

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

int Token::get_loc(){
	return loc;
}

StrToken::StrToken(std::string _str, int loc) : Token(STRING_L, loc), value(_str){}

std::string StrToken::get_str(){
	return value;
}

void StrToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

IdToken::IdToken(std::string _str, int loc) : Token(ID, loc), value(_str){}

std::string IdToken::get_id(){
	return value;
}

void IdToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

IntToken::IntToken(std::string _int, int loc) : Token(INT_L, loc)
{
	value = atoi(_int.c_str());
}

int IntToken::get_int(){
	return value;
}

void IntToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

RealToken::RealToken(std::string _real, int loc) : Token(REAL_L, loc)
{
	value = atof(_real.c_str());
}

double RealToken::get_real(){
	return value;
}

void RealToken::print(){
	std::cout << "<" << tag_to_string(tag) << ", " << value << ">";
}

LineToken::LineToken(int _line, int loc) : Token(LINE_END, loc)
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

bool is_record(Token token){
	if (token.get_tag() >= 400 && token.get_tag() < 600){
		return true;
	}
	return false;
}

