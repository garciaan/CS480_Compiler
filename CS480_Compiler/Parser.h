// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"
#include "Parse_Table.h"




void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table, bool verbose);


class Stack_Type
{
public:
	Stack_Type(int _sym) : tag(_sym) {}
	
	inline int get_tag(){
		return tag;
	}

	virtual void func(std::vector<int> &stack){
		return;
	}

private:
	int tag;
};




#endif // !PARSER_H
