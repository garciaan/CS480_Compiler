//Parser.cpp

#include "Parser.h"


void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table, bool verbose){

	//setup file stream
	source >> std::noskipws;

	Lexer lex = Lexer(source, table);

	// process all input
	lex.tokenize(0);

	//check for errors, abort if we have any
	/*if (!errors.empty()){
		std::cout << "Errors during tokenization, in " << source_name << " printing error messages and aborting" << std::endl;
		while (!errors.empty()){
			lex_mesg error = errors.front();
			std::cout << error.msg << std::endl;
			errors.pop();
		}
		return;
	}*/


	//std::queue<Token*> tokens;

	//create the stacks we will use
	std::vector<Stack_Type> stack;
	std::vector<Stack_Type> save_stack;


	//intialize stack
	stack.push_back(Stack_Type(INPUT_END));
	stack.push_back(Stack_Type(NON_START));

	//while our stack has not been cleared
	while (stack.back().get_tag() != INPUT_END){

		//if both the stack and queue have the same terminal on top
		if (stack.back().get_tag() == lex.peek().get_tag()){
			save_stack.push_back(stack.back());
			stack.pop_back();
			lex.pop();
		}
		//if we have an unmatched terminal on the stack error out
		else if (is_terminal(stack.back().get_tag())){
			std::cout << "Error during parsing in " << source_name << " exiting now." << std::endl;
			break;
		}
		// if we have one of the records we use to generate code
		else if (is_record(stack.back().get_tag())){

			//run func in the stack record;

		}
		//if we have a non-terminal on top of the stack find the correct rule from the parse table and use it
		else if (parse_table.find({ stack.back().get_tag(), lex.peek_tag() }) != parse_table.end()){
			std::vector<int> prod = parse_table[{stack.back().get_tag(), lex.peek_tag()}];
			stack.pop_back();
			for (std::vector<int>::reverse_iterator it = prod.rbegin(); it != prod.rend(); ++it){
				if (*it != EMPTY){
					stack.push_back(*it);
				}
			}

			//visualization of derivation

			if (verbose){

				//print stack in reverse order
				for (std::vector<Stack_Type>::iterator it = save_stack.begin(); it != save_stack.end(); ++it){
					std::cout << ' ' << Token::tag_to_input(it->get_tag()) << ' ';
				}

				for (std::vector<Stack_Type>::reverse_iterator it = stack.rbegin(); it != stack.rend(); ++it){
					std::cout << ' ' << Token::tag_to_input(it->get_tag()) << ' ';
				}
				std::cout << std::endl;

			}
		}

		//if there was no rule for the non-terminal and input symbol we have a problem
		else {
			std::cout << "Error during parsing in " << source_name << " exiting now." << std::endl;
			break;
		}

	}

	//if we emptied both the stack and the queue with no errors
	if (stack.back().get_tag() == INPUT_END && stack.back().get_tag() == lex.peek_tag()){
		std::cout << source_name << " accepted!" << std::endl;
	/*	if (verbose){
			while (!tokens_copy.empty()){
				std::cout << '<' << Token::tag_to_string(tokens_copy.front()->get_tag()) << '>';
				delete tokens_copy.front();
				tokens_copy.pop();
			}
		}
		else {
			while (!tokens_copy.empty()){
				delete tokens_copy.front();
				tokens_copy.pop();
			}
		}*/
	}
	else {
		std::cout << source_name << " failed!" << std::endl;
		//while (!tokens_copy.empty()){
		//	delete tokens_copy.front();
		//	tokens_copy.pop();
		//}
	}

}
