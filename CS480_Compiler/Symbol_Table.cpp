#include "Symbol_Table.h"


Symbol_Table::Symbol_Table(){
	var_count = 0;
	scopes.push_back(std::map<std::string, var_info>());
}

var_info Symbol_Table::_find_symbol(std::string s, int *level){
	std::map<std::string, var_info>::iterator it;

	int l;
	bool found = false;

	for (int i = scopes.size() - 1; i >= 0; --i){
		it = scopes[i].find(s);
		if (it != scopes[i].end()){
			l = scopes.size() - 1 - i;
			found = true;
			break;
		}
	}

	if (found){
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
	int level;
	var_info result = _find_symbol(s, &level);

	if (result.type != EMP && level == 0){
		return{ USD, result.name };
	}
	else{
		std::stringstream var_name;
		var_name << "var" << var_count++;
		scopes.back().insert(std::pair<std::string, var_info>(s, { o, var_name.str() }));
		return{ o, var_name.str() };
	}

}

int Symbol_Table::increse_scope(){
	scopes.push_back(std::map<std::string, var_info>());
	return 0;
}
int Symbol_Table::decrese_scope(){
	scopes.pop_back();
	return 0;
}