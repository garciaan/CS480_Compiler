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

	std::queue<Token*> tokens;

	tokenize(source_file, tokens);

	Token *token;
	while (!tokens.empty()){
		token = tokens.front();
		tokens.pop();
		token->print();
		free(token);
	}

	return 0;
}

