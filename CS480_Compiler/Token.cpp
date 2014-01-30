//Token.cpp

#include "Token.h"


void Token::print(){
	std::cout << "<" << tag << ">";
}

std::string Token::getTag(){
	return tag;
}

void Token::setTag(std::string _tag){
	tag = _tag;
}

StrToken::StrToken(std::string _tag, std::string _str) : Token(_tag), value(_str){}

void StrToken::print(){
	std::cout << "<" << this->getTag() << ", " << value << ">";
}

IntToken::IntToken(std::string _tag, std::string _int) : Token(_tag)
{
	value = atoi(_int.c_str());
}

void IntToken::print(){
	std::cout << "<" << this->getTag() << ", " << value << ">";
}

RealToken::RealToken(std::string _tag, std::string _real) : Token(_tag)
{
	value = atof(_real.c_str());
}

void RealToken::print(){
	std::cout << "<" << this->getTag() << ", " << value << ">";
}