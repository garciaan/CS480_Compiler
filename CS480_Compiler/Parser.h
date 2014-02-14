// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"

typedef struct _map_key{
	int prod_sym;
	int in_sym;
}map_key;

struct map_key_comparer
{
	bool operator()(const map_key & Left, const map_key & Right) const
	{
		if (Left.prod_sym == Right.prod_sym){
			return Left.in_sym < Right.in_sym;
		}
		else {
			return Left.prod_sym < Right.prod_sym;
		}
	}
};

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
