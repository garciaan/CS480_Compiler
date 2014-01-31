#include "SymbolTable.h"

SymbolTable::SymbolTable(std::string *keys, int *values, int n)
{

	for (int i = 0; i < n; ++i){

		this->_insert_symbol(keys[i], values[i]);
	}

}

int SymbolTable::_insert_symbol(std::string id, int value)
{
	Symbol _temp_symbol;

	std::map<std::string, Symbol>::iterator it;
	it = table.find(id);

	if (it == table.end()){
		_temp_symbol.type = value;

		this->table.insert(std::pair<std::string, Symbol>(id, _temp_symbol));

		return value;
	} 
	else {
		return it->second.type;
	}

}

int SymbolTable::insert_symbol(std::string id)
{
	return _insert_symbol(id, ST_ID);
}
