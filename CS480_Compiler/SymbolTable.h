// SymbolTable.h

#ifndef SYMTAB_H
#define SYMTAB_H

// Symbol Table Types
#define  ST_ID -1

#include <string>
#include <map>

struct _symbol{

	int type;

}typedef Symbol;

class SymbolTable{

private:

	std::map<std::string, Symbol> table;

	int _insert_symbol(std::string id, int value);

public:

	SymbolTable(std::string *keys, int *values, int n);

	int insert_symbol(std::string id);

};



#endif // SYMTAB_H
