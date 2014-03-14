#include "Symbol_Table.h"

Symbol_Table::Symbol_Table(){
	var_count = 0;
	scopes.push_back(std::map<std::string, var_info>());
}

var_info Symbol_Table::_find_symbol(std::string s, int *level){
	std::map<std::string, var_info>::iterator it;

	int l;

	for (int i = scopes.size() - 1; i >= 0; --i){
		it = scopes[i].find(s);
		if (it != scopes[i].end()){
			l = i;
			break;
		}
	}

	if (it != scopes.front().end()){
		*level = l;
		return it->second;
	}
	else {
		return{ EMP, "" };
	}
}

var_info Symbol_Table::find_symbol(std::string s){
	int level;
	return _find_symbol(s, &level);

}

var_info Symbol_Table::insert_symbol(std::string s, oper_type o){
	//find variable 
	var_info result = find_symbol(s);


	scopes.back().insert(std::pair<std::string, var_info>(s, { o, "blah" }));
	return{ o, "blah" };
}

int Symbol_Table::increse_scope(){
	scopes.push_back(std::map<std::string, var_info>());
	return 0;
}
int Symbol_Table::decrese_scope(){
	scopes.pop_back();
	return 0;
}