#include "Lex.h"


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

	int inital_values[] = { TRUE, FALSE, AND, OR, NOT, BOOL_T, INT_T, REAL_T,
		STRING_T, LET, IF, WHILE, ST_ID, ST_ID, ST_ID, ST_ID };

	int n = sizeof(inital_values) / sizeof(int);

	SymbolTable table = SymbolTable(inital_syms, inital_values, n);

	std::queue<Token*> tokens;

	tokenize(source_file, tokens, table);

	Token *token;
	while (!tokens.empty()){
		token = tokens.front();
		tokens.pop();
		token->print();
		free(token);
	}

	return 0;
}

