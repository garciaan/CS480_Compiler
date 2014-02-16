#include "Parse_Table.h"

int parse_table_setup(std::string prod, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table){

	char* safe = new char[prod.length() + 1];
	strcpy_s(safe, prod.length() + 1, prod.c_str());


	//get the stack value
	int loc = prod.find(':');
	if (loc == std::string::npos){
		//errors
	}
	std::string stack_value = prod.substr(0, loc);
	prod = prod.substr(loc + 1);

	//get the input value
	loc = prod.find(':');
	if (loc == std::string::npos){
		//errors
	}
	std::string input_value = prod.substr(0, loc);
	prod = prod.substr(loc + 1);

	std::queue<std::string> rhs;

	while ((loc = prod.find(',')) != std::string::npos){

		std::string prods = prod.substr(0, loc);
		rhs.push(prods);
		if (unsigned(loc + 1) > prod.length()){
			break;
		}

		prod = prod.substr(loc + 1);

	}

	int array_size = rhs.size();
	int *rhs_array = new int[array_size];

	for (int i = 0; i < array_size; ++i){
		rhs_array[i] = Token::string_to_tag(rhs.front());
		if (rhs_array[i] == -1){
			printf("Error in string: %s\n While Parsing %s\n", safe, rhs.front());
			return -1;
		}

		rhs.pop();
	}

	if (input_value == "BIN_ALL"){

		std::string values[] = {
			"AND",
			"OR",
			"PLUS",
			"MULTI",
			"DIV",
			"MOD",
			"EXP",
			"EQ",
			"NE",
			"LT",
			"GT",
			"LE",
			"GE"
		};

		int values_length = sizeof(values)/sizeof(std::string);

		for (int i = 0; i < values_length; ++i){
			map_key key = {
				Token::string_to_tag(stack_value),
				Token::string_to_tag(values[i])
			};

			if (key.in_sym == -1 || key.prod_sym == -1){
				//error
				printf("Error in string: %s\n", safe);
				return -1;
			}

			std::vector<int> prod_vec(rhs_array, rhs_array + array_size);
			parse_table[key] = prod_vec;
		}
	}
	else if (input_value == "UN_ALL"){

		std::string values[] = {
			"NOT",
			"SIN",
			"COS",
			"TAN"};

		int values_length = sizeof(values)/sizeof(std::string);

		for (int i = 0; i < values_length; ++i){
			map_key key = {
				Token::string_to_tag(stack_value),
				Token::string_to_tag(values[i])
			};

			if (key.in_sym == -1 || key.prod_sym == -1){
				//error
				printf("Error in string: %s\n", safe);
				return -1;
			}

			std::vector<int> prod_vec(rhs_array, rhs_array + array_size);
			parse_table[key] = prod_vec;
		}
	}
	else if (input_value == "CONST_ALL"){

		std::string values[] = {
			"INT_L",
			"REAL_L",
			"STRING_L",
			"TRUE",
			"FALSE" };

		int values_length = sizeof(values) / sizeof(std::string);

		for (int i = 0; i < values_length; ++i){
			map_key key = {
				Token::string_to_tag(stack_value),
				Token::string_to_tag(values[i])
			};

			if (key.in_sym == -1 || key.prod_sym == -1){
				//error
				printf("Error in string: %s\n", safe);
				return -1;
			}

			std::vector<int> prod_vec(rhs_array, rhs_array + array_size);
			parse_table[key] = prod_vec;
		}
	}
	else {

		map_key key = {
			Token::string_to_tag(stack_value),
			Token::string_to_tag(input_value)
		};

		if (key.in_sym == -1 || key.prod_sym == -1){
			//error
			printf("Error in string: %s\n", safe);
			return -1;
		}


		std::vector<int> prod_vec(rhs_array, rhs_array + array_size);
		parse_table[key] = prod_vec;
	}


	delete[] rhs_array;

	return 0;

}

void create_parse_table(std::map<map_key, std::vector<int>, map_key_comparer> &parse_table){


	int rules_size = sizeof(parse_table_rules) / sizeof(std::string);

	for (int i = 0; i < rules_size; ++i){
		parse_table_setup(parse_table_rules[i], parse_table);
	}
}