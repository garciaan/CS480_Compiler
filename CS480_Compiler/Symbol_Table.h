// Symbol_Table.h

#ifndef SYMTAB_H
#define SYMTAB_H

#include <vector>
#include <string>
#include <map>

enum oper_type { INT, REAL, STRING, BOOL, ERROR, EMP, EXT };

class Symbol_Table{

private:

	std::vector<std::map<std::string, oper_type>> scopes;

public:

	Symbol_Table();

	oper_type find_symbol(std::string);
	int insert_symbol(std::string, oper_type);

	int increse_scope();
	int decrese_scope();

};


#endif // SYMTAB_H
