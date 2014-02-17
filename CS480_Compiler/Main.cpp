#include "Parser.h"


int main(int argc, char** argv)
{

	//Current usage: 1 source file
	if (argc > 2){
		std::cout << "Error : usage \"compiler filename1 [filename 2...]\"" << std::endl;
		return -1;
	}

	std::vector<std::ifstream*> sources;

	for (int i = 1; i < argc; ++i){
	

		std::ifstream *source = new std::ifstream(argv[i]);

		if (!source->is_open()){
			std::cout << "Error : opening file: " << argv[i] << std::endl;
		}
		else {
			sources.push_back(source);
		}

	}


	//setup parse table
	std::map<map_key, std::vector<int>, map_key_comparer> parse_table;
	Symbol_Table symbol_table;


	create_symbol_table(symbol_table);
	create_parse_table(parse_table);

	for (unsigned i = 0; i < sources.size(); ++i){
		parse(*sources[i], argv[i], symbol_table, parse_table);
	}


	return 0;
}

