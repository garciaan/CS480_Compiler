// Symbol_Table.h

#ifndef SYMTAB_H
#define SYMTAB_H

#include <vector>
#include <string>
#include <map>
#include <sstream>

enum oper_type { INT, FLOAT, STRING, BOOL, ERROR, EMP, USD, EXT };

typedef struct _var_info{
	oper_type type;
	std::string name;
}var_info;


class Symbol_Table{

private:

	std::vector<std::map<std::string, var_info>> scopes;

	var_info _find_symbol(std::string, int* level);

	int var_count;

public:

	Symbol_Table();

	var_info find_symbol(std::string);
	var_info insert_symbol(std::string, oper_type);

	int increse_scope();
	int decrese_scope();

};

#endif // SYMTAB_H