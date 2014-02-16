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

	//setup parse table
	std::map<map_key, std::vector<int>, map_key_comparer> parse_table;
	Symbol_Table symbol_table;


	create_symbol_table(symbol_table);
	create_parse_table(parse_table);

	std::queue<Token*> tokens;
	std::queue<lex_mesg> errors;
	std::queue<lex_mesg> warnings;

	tokenize(source_file, tokens, symbol_table, errors, warnings);

	Parser parser = Parser(parse_table);

	parser.parse(tokens);

	return 0;
}

