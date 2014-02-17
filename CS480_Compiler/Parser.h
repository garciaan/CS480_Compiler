// Parser.h

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <queue>
#include <vector>
#include "Token.h"
#include "Lex.h"
#include "Parse_Table.h"


void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table);

void add_to_parse_table(map_key key, int *data, int size_prod, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table);

#endif // !PARSER_H
