#include "Lex.h"
#include "Parser.h"


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
		if (loc + 1 > prod.length()){
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

		int values_length = 13;

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
			"NOT"};

		int values_length = 1;

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
			"FALSE"};

		int values_length = 5;

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



int main(int argc, char** argv)
{

	//Current usage: 1 source file
	if (argc != 2){
		std::cout << "Error : usage \"compiler filename\"" << std::endl;
		return -1;
	}

	std::ifstream source_file(argv[1]);

	//setup file stream
	source_file >> std::noskipws;

	if (!source_file.is_open()){
		std::cout << "Error : opening file: " << argv[1] << std::endl;
		return -2;
	}

	//setup symbol table
	std::string inital_syms[] = {"true", "false", "and", "or", "not", "bool",
		"int", "real", "string", "let", "if", "while", "sin", "cos", "tan",
		"stdout"
	};

	//setup parse table
	std::map<map_key, std::vector<int>, map_key_comparer> parse_table;

	std::string parse_preload[] = {
		//Start Rules 1
		"NON_START:L_BRACKET:L_BRACKET,NON_S,R_BRACKET,",
		//S Rules 2 - 4
		"NON_S:L_BRACKET:L_BRACKET,NON_S_2,",
		"NON_S:ID:ID,NON_S_1,",
		"NON_S:CONST_ALL:NON_CONST,NON_S_1,",
		//S' Rules 5 - 6
		"NON_S_1:L_BRACKET:NON_S,NON_S_1,",
		"NON_S_1:ID:NON_S,NON_S_1,", //76
		"NON_S_1:CONST_ALL:NON_S,NON_S_1,", //77
		"NON_S_1:R_BRACKET:EMPTY,",
		//S'' Rules 7 - 16
		"NON_S_2:L_BRACKET:NON_S,R_BRACKET,NON_S_1,",
		"NON_S_2:R_BRACKET:R_BRACKET,NON_S_1,",
		"NON_S_2:BIN_ALL:NON_EXPR_1,NON_S_1,",
		"NON_S_2:UN_ALL:NON_EXPR_1,NON_S_1,",
		"NON_S_2:MINUS:NON_EXPR_1,NON_S_1,",
		"NON_S_2:ASSIGN:NON_EXPR_1,NON_S_1,",
		"NON_S_2:IF:NON_EXPR_1,NON_S_1,",
		"NON_S_2:WHILE:NON_EXPR_1,NON_S_1,",
		"NON_S_2:LET:NON_EXPR_1,NON_S_1,",
		"NON_S_2:ID:NON_S,R_BRACKET,NON_S_1,",         
		"NON_S_2:CONST_ALL:NON_S,R_BRACKET,NON_S_1,",  //78
		//Expr Rules 17 - 19
		"NON_EXPR:L_BRACKET:L_BRACKET,NON_EXPR_1,",
		"NON_EXPR:ID:ID,",
		"NON_EXPR:CONST_ALL:NON_CONST,",
		//Expr' Rules 20 - 26
		"NON_EXPR_1:ASSIGN:NON_OPER_1,",
		"NON_EXPR_1:IF:NON_STMT_1,",
		"NON_EXPR_1:WHILE:NON_STMT_1,",
		"NON_EXPR_1:LET:NON_STMT_1,",
		"NON_EXPR_1:BIN_ALL:NON_OPER_1,",
		"NON_EXPR_1:MINUS:NON_STMT_1,",
		"NON_EXPR_1:UN_ALL:NON_STMT_1,",
		//Oper Rules 27 - 29
		"NON_OPER:L_BRACKET:L_BRACKET,NON_OPER_1,",
		"NON_OPER:ID:ID,",
		"NON_OPER:CONST_ALL:NON_CONST,",
		//Oper' Rules 30 - 33
		"NON_OPER_1:ASSIGN:ASSIGN,ID,NON_OPER,R_BRACKET,",
		"NON_OPER_1:BIN_ALL:NON_BINOP,NON_OPER,NON_OPER,R_BRACKET,",
		"NON_OPER_1:UN_ALL:NON_UNOP,NON_OPER,R_BRACKET,",
		"NON_OPER_1:MINUS:MINUS,NON_OPER,NON_NEGOP,",
		//Stmt Rule 34
		"NON_STMT:L_BRACKET:L_BRACKET,NON_STMT_1,",
		//Stmt' Rules 35 - 37
		"NON_STMT_1:IF:IF,NON_EXPR,NON_EXPR,NON_IFSTMT,",
		"NON_STMT_1:WHILE:WHILE,NON_EXPR,NON_EXPRLIST,R_BRACKET,",
		"NON_STMT_1:LET:LET,L_BRACKET,NON_VARLIST,R_BRACKET,",
		//Binop rules 38 - 49
		"NON_BINOP:PLUS:PLUS,",
		"NON_BINOP:MULTI:MULTI,",
		"NON_BINOP:MOD:MOD,",
		"NON_BINOP:EXP:EXP,",
		"NON_BINOP:EQ:EQ,",
		"NON_BINOP:LT:LT,",
		"NON_BINOP:LE:LE,",
		"NON_BINOP:GT:GT,",
		"NON_BINOP:GE:GE,",
		"NON_BINOP:NE:NE,",
		"NON_BINOP:OR:OR,",
		"NON_BINOP:AND:AND,",
		//Unop rule 50 
		"NON_UNOP:NOT:NOT,",
		//Const rules 51 - 55
		"NON_CONST:STRING_L:STRING_L,",
		"NON_CONST:INT_L:INT_L,",
		"NON_CONST:REAL_L:REAL_L,",
		"NON_CONST:TRUE:TRUE,",
		"NON_CONST:FALSE:FALSE,",
		//Ifstmt rules 56 - 59
		"NON_IFSTMT:L_BRACKET:NON_EXPR,L_BRACKET,",
		"NON_IFSTMT:ID:NON_EXPR,L_BRACKET,",
		"NON_IFSTMT:CONST_ALL:L_BRACKET,",
		"NON_IFSTMT:R_BRACKET:R_BRACKET,",
		//exprlist rules 60 - 62
		"NON_EXPRLIST:L_BRACKET:NON_EXPR,NON_EXPRLIST_1,",
		"NON_EXPRLIST:ID:NON_EXPR,NON_EXPRLIST_1,",
		"NON_EXPRLIST:CONST_ALL:NON_EXPR,NON_EXPRLIST_1,",
		//exprlist' rules 63 - 66
		"NON_EXPRLIST_1:L_BRACKET:NON_EXPRLIST,",
		"NON_EXPRLIST_1:R_BRACKET:EMPTY,",
		"NON_EXPRLIST_1:ID:NON_EXPRLIST,",
		"NON_EXPRLIST_1:CONST_ALL:NON_EXPRLIST,",
		//varlist rules 67 - 69
		"NON_VARLIST:L_BRACKET:L_BRACKET,ID,NON_TYPE,R_BRACKET,NON_VARLIST_1,",
		"NON_VARLIST:R_BRACKET:EMPTY,",
		"NON_VARLIST_1:L_BRACKET:NON_VARLIST,",
		//type rules 70 - 73
		"NON_TYPE:BOOL_T:BOOL_T,",
		"NON_TYPE:INT_T:INT_T,",
		"NON_TYPE:REAL_T:REAL_T,",
		"NON_TYPE:STRING_T:STRING_T,",
		//negop rules 74 - 75
		"NON_NEGOP:L_BRACKET:NON_OPER,R_BRACKET,",
		"NON_NEGOP:R_BRACKET:R_BRACKET,",
		"NON_NEGOP:CONST_ALL:NON_OPER,R_BRACKET,",
		"NON_NEGOP:ID:NON_OPER,R_BRACKET,",

	};

	//CHECK THIS IF YOU ARE HAVING PROBLEMS!
	int parse_preload_size = 80;

	for (int i = 0; i < parse_preload_size; ++i){
		parse_table_setup(parse_preload[i], parse_table);
	}

	int inital_values[] = { TRUE, FALSE, AND, OR, NOT, BOOL_T, INT_T, REAL_T,
		STRING_T, LET, IF, WHILE, ST_ID, ST_ID, ST_ID, ST_ID };

	int n = sizeof(inital_values) / sizeof(int);

	SymbolTable table = SymbolTable(inital_syms, inital_values, n);

	std::queue<Token*> tokens;
	std::queue<lex_mesg> errors;
	std::queue<lex_mesg> warnings;

	tokenize(source_file, tokens, table, errors, warnings);

	Parser parser = Parser(parse_table);

	parser.parse(tokens);

	return 0;
}

