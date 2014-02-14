#include "Lex.h"
#include "Parser.h"



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

	//one block for each production
	int prod_1[] = {L_BRACKET, NON_S, R_BRACKET};
	add_to_parse_table({ NON_START, L_BRACKET }, prod_1, sizeof(prod_1)/sizeof(int), parse_table);

	int prod_2[] = { L_BRACKET, NON_S_2 };
	add_to_parse_table({ NON_S, L_BRACKET }, prod_2, sizeof(prod_2) / sizeof(int), parse_table);

	int prod_3[] = { NON_S, NON_S_1 };
	add_to_parse_table({ NON_S_1, L_BRACKET }, prod_3, sizeof(prod_3) / sizeof(int), parse_table);

	int prod_4[] = { EMPTY };
	add_to_parse_table({ NON_S_1, R_BRACKET }, prod_4, sizeof(prod_4) / sizeof(int), parse_table);

	int prod_5[] = { NON_S, R_BRACKET, NON_S_1 };
	add_to_parse_table({ NON_S_2, L_BRACKET }, prod_5, sizeof(prod_5) / sizeof(int), parse_table);

	int prod_6[] = { R_BRACKET, NON_S_1 };
	add_to_parse_table({ NON_S_2, R_BRACKET }, prod_6, sizeof(prod_6) / sizeof(int), parse_table);

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

