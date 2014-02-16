// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"
#include "Parse_Table.h"

class Parser
{
public:
	Parser(std::map<map_key, std::vector<int>, map_key_comparer> &_parse_table) : parse_table(_parse_table) {}

	void parse(std::queue<Token*> tokens);

private:

	std::vector<int> stack;
	std::vector<int> save_stack;
	std::map<map_key, std::vector<int>, map_key_comparer> &parse_table;
};


void add_to_parse_table(map_key key, int *data, int size_prod, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table);

#endif // !PARSER_H
