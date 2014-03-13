// Symbol_Table.h

#ifndef SYMTAB_H
#define SYMTAB_H

// Symbol Table Types
#define  ST_ID -1

#include <string>
#include <map>
#include "Token.h"

struct _symbol{

	int type;

}typedef Symbol;




class Symbol_Table{

private:

	std::map<std::string, Symbol> table;

	int _insert_symbol(std::string id, int value);

public:

	Symbol_Table(std::string *keys, int *values, int n);
	Symbol_Table(){}

	int insert_symbol(std::string id);

};


void create_symbol_table(Symbol_Table &table);


#endif // SYMTAB_H
