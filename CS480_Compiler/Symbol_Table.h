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

//setup symbol table
static std::string inital_syms[] = { "true", "false", "and", "or", "not", "bool",
"int", "real", "string", "let", "if", "while", "sin", "cos", "tan",
"stdout"
};

static int inital_values[] = { TRUE, FALSE, AND, OR, NOT, BOOL_T, INT_T, REAL_T,
STRING_T, LET, IF, WHILE, SIN, COS, TAN, STDOUT };


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
