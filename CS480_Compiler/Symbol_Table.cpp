#include "Symbol_Table.h"

Symbol_Table::Symbol_Table(){
	scopes.push_back(std::map<std::string, oper_type>());
}

oper_type Symbol_Table::find_symbol(std::string s){

	std::map<std::string, oper_type>::iterator it;

	for (int i = scopes.size() - 1; i >= 0; --i){
		it = scopes[i].find(s);
		if (it != scopes[i].end()){
			break;
		}
	}

	if (it != scopes.front().end()){
		return it->second;
	}
	else {
		return EMP;
	}

}

int Symbol_Table::insert_symbol(std::string s, oper_type o){
	scopes.back().insert(std::pair<std::string, oper_type>(s, o));
	return 0;
}

int Symbol_Table::increse_scope(){
	scopes.push_back(std::map<std::string, oper_type>());
	return 0;
}
int Symbol_Table::decrese_scope(){
	scopes.pop_back();
	return 0;
}