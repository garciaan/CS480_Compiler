#include "Parser.h"


int main(int argc, char** argv)
{

	//Current usage: 1 source file
	if (argc < 2){
		std::cout << "Error : usage \"compiler filename1 [filename 2...]\"" << std::endl;
		return -1;
	}

	std::vector<std::ifstream*> sources;
	std::vector<std::string> source_names;

	//boolean settings
	bool verbose = false;

	for (int i = 1; i < argc; ++i){
	

		if (argv[i][0] == '-'){
			switch (argv[i][1]){
			case 'v':
				verbose = true;
				break;
			}
		}
		else {
			std::ifstream *source = new std::ifstream(argv[i]);

			if (!source->is_open()){
				std::cout << "Error : opening file: " << argv[i] << std::endl;
			}
			else {
				sources.push_back(source);
				source_names.push_back(argv[i]);
			}
		}

	}


	for (unsigned i = 0; i < sources.size(); ++i){
		Parser parse_0 = Parser(*sources[i], source_names[i]);
		std::cout << parse_0.parse();
	}


	//release ifstream memory
	for (std::vector<std::ifstream*>::iterator it = sources.begin(); it != sources.end(); ++it){
		delete *it;
	}

	return 0;
}

