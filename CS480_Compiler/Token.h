// Token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <cstdlib>

class Token
{
	std::string tag;

public:
	Token(std::string _tag) : tag(_tag){}
	Token(){}
	void print();
	std::string getTag();
	void setTag(std::string _tag);

};

class StrToken : public Token
{
	std::string value;

public:
	StrToken(std::string _tag, std::string _str);
	void print();
};

class IntToken : public Token
{
	int value;

public:
	IntToken(std::string _tag, std::string _int);
	void print();
};

class RealToken : public Token
{
	double value;

public:
	RealToken(std::string _tag, std::string _real);
	void print();
};


#endif //TOKEN_H