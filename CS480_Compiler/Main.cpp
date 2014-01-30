#include <iostream>
#include <fstream>
#include <cctype>
#include <list>
#include <string>

#include "Token.h"

using namespace std;

int main(int argc, char** argv)
{

	//Current usage: 1 source file
	if (argc != 2){
		cout << "Error : usage \"compiler filename\"" << endl;
		return -1;
	}

	ifstream source_file(argv[1]);

	//setup file stream
	source_file >> noskipws;

	if (!source_file.is_open()){
		cout << "Error : opening file: " << argv[1] << endl;
		return -2;
	}

	char c;
	int state = 0;

	string value;

	//infinte loop to process each character
	//one pass through this loop should be equivlent to going
	//from a start state to a final state on our FSA
	for (;;){
		
		//value is used to hold itermediate lexme values. Cleared after every run.
		value = "";

		//read a chacter
		source_file >> c;

		//if we hit the end of the file we are done here
		if (source_file.eof()){
			break;
		}

		//ids
		//we will also check the value here versus our keywords
		else if (isalpha(c) || c == '_'){
			
			do{	
				value.append(1, c);
				source_file >> c;

			} while (isalpha(c) || isdigit(c) || c == '_');

			//here we have gone one over, so we will place the character back onto the stream

			source_file.putback(c);

			StrToken token = StrToken("id", value);
			token.print();

		}

		//numbers
		else if (isdigit(c)){

			//while we have just digits get the digits
			do{
				value.append(1, c);
				source_file >> c;

			} while (isdigit(c));

			//if either our first character is a . or we got a . while
			//processing the above, add it and start looking for digits again
			if (c == '.'){

				do{
					value.append(1, c);
					source_file >> c;

				} while (isdigit(c));

			}
			
			if (c == 'e' || c == 'E'){

				do{
					value.append(1, c);
					source_file >> c;

				} while (isdigit(c));

			}

			//here we have gone one over, so we will place the character back onto the stream

			source_file.putback(c);


			//remove e or E if no numbers are found
			//we actually might want to report invaild lexeme here
			if (value[value.length() - 1] == 'e' || value[value.length() - 1] == 'E'){
				value.erase(value.length() - 1, 1);
			}


			if (value.find(".") != string::npos ||
				value.find("e") != string::npos ||
				value.find("E") != string::npos){
				RealToken token = RealToken("Real", value);
				token.print();
			}
			else {
				IntToken token = IntToken("Int", value);
				token.print();
			}

		}
		//string literals
		else if (c == '"'){

			//eat the leading "
			source_file >> c;
			do{
				value.append(1, c);
				source_file >> c;

			} while (c != '"' || (c == '"' && value.back() == '\\' ));


			StrToken token = StrToken("Str", value);
			token.print();

		}
		else {

			//ugly, fix later
			Token token;
			switch (c)
			{

			case '[':
				token.setTag("[");
				token.print();
				break;
			case ']':
				token.setTag("]");
				token.print();
				break;
			case '+':
				token.setTag("+");
				token.print();
				break;
			case '-':
				token.setTag("-");
				token.print();
				break;
			case '*':
				token.setTag("*");
				token.print();
				break;
			case '/':
				token.setTag("/");
				token.print();
				break;
			case '=':
				token.setTag("=");
				token.print();
				break;
			case '<':
				//add <= here
				token.setTag("<");
				token.print();
				break;
			case '>':
				//add >= here
				token.setTag(">");
				token.print();
				break;
			case '%':
				token.setTag("%");
				token.print();
				break;
			case '^':
				token.setTag("^");
				token.print();
				break;
			//non-standard comment 
			case '#':
				//eat the comment line
				do{
					source_file >> c;
				} while (c != '\n');

			default:
				break;
			}
		}

	}
	return 0;
}