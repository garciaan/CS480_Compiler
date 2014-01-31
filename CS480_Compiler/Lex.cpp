#include "Lex.h"


using namespace std;

void tokenize(istream &source_file, std::queue<Token*> &queue){

	int line_num = 0;

	char c;
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

			queue.push(new StrToken(value));

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
				queue.push(new RealToken(value));
			}
			else {
				queue.push(new IntToken(value));
			}

		}
		//string literals
		else if (c == '"'){

			//eat the leading "
			source_file >> c;
			do{
				value.append(1, c);
				source_file >> c;

			} while (c != '"' || (c == '"' && value.back() == '\\'));

			queue.push(new StrToken(value));

		}
		else {

			//Match all the little weird symbols. This part is nasty and large

			switch (c)
			{

			case '[':
				queue.push(new Token(L_BRACKET));
				break;
			case ']':
				queue.push(new Token(R_BRACKET));
				break;
			case '+':
				queue.push(new Token(PLUS));
				break;
			case '-':
				queue.push(new Token(MINUS));
				break;
			case '*':
				queue.push(new Token(MULTI));
				break;
			case '/':
				queue.push(new Token(DIV));
				break;
			case '=':
				queue.push(new Token(EQ));
				break;
			case '<':
				if (c == '='){
					queue.push(new Token(LE));
				}
				else{
					queue.push(new Token(LT));
					source_file.putback(c);
				}
				
				break;
			case '>':
				source_file >> c;
				if (c == '='){
					queue.push(new Token(GE));
				}
				else{
					queue.push(new Token(GT));
					source_file.putback(c);
				}
				
				break;
			case '%':
				queue.push(new Token(MOD));
				
				break;
			case '^':
				queue.push(new Token(EXP));
				
				break;
			case ':':
				source_file >> c;
				if (c == '='){
					queue.push(new Token(ASSIGN));
					
				}
				else {
					source_file.putback(c);
				}
				break;
			case '!':
				source_file >> c;
				if (c == '='){
					queue.push(new Token(ASSIGN));
					
				}
				else {
					source_file.putback(c);
				}
				break;
				//non-standard comment 
			case '#':
				//eat the comment line
				do{
					source_file >> c;
				} while (c != '\n');
				break;
			case '\n':
				queue.push(new LineToken(++line_num));
				
				break;
			default:
				break;
			}
		}

	}
	return;
}