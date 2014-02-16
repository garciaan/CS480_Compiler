//Parser.cpp

#include "Parser.h"


void Parser::parse(std::queue<Token*> tokens){

	//for our parsing purposes
	tokens.push(new Token(INPUT_END));


	stack.push_back(INPUT_END);
	stack.push_back(NON_START);

	while (stack.back() != INPUT_END){
		if (stack.back() == tokens.front()->get_tag()){
			save_stack.push_back(stack.back());
			stack.pop_back();
			tokens.pop();
		}
		else if (is_terminal(stack.back())){
			//error here
			break;
		}
		else if (parse_table.find({ stack.back(), tokens.front()->get_tag() }) != parse_table.end()){
			std::vector<int> prod = parse_table[{stack.back(), tokens.front()->get_tag()}];
			stack.pop_back();
			for (std::vector<int>::reverse_iterator it = prod.rbegin(); it != prod.rend(); ++it){
				if (*it != EMPTY){
					stack.push_back(*it);
				}
			}

			//visualization

			//print stack in reverse order
			for (std::vector<int>::iterator it = save_stack.begin(); it != save_stack.end(); ++it){
				std::cout << ' ' << Token::tag_to_input(*it) << ' ';
			}

			for (std::vector<int>::reverse_iterator it = stack.rbegin(); it != stack.rend(); ++it){
				std::cout << ' ' << Token::tag_to_input(*it) << ' ';
			}
			std::cout << std::endl;
		}
		else {
			//error here too
			break;
		}

	}

	if (stack.back() == INPUT_END && stack.back() == tokens.front()->get_tag()){
		printf("Accepted!");
	}
	else {
		printf("Failed!");
	}

}

void add_to_parse_table(map_key key, int* prod, int prod_size, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table){

	std::vector<int> prod_vec(prod, prod + prod_size);
	parse_table[key] = prod_vec;

}