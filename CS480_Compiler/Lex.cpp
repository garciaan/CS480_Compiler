#include "Lex.h"


using namespace std;

void tokenize(istream &source, std::queue<Token*> &queue, Symbol_Table &table, std::queue<lex_mesg> &errors, std::queue<lex_mesg> &warnings){

	int line_num = 1;

	char c;
	string value;

	//infinte loop to process each character
	//one pass through this loop should be equivlent to going
	//from a start state to a final state on our FSA
	for (;;){

		//value is used to hold itermediate lexme values. Cleared after every run.
		value = "";

		//read a chacter, but eat any white space since going through with these characters
		//will just be a waste of time
		do{
			source >> c;
		} while (c == ' ' || c == '\t');
		

		//if we hit the end of the file we are done here
		if (source.eof()){
			break;
		}

		//ids
		//we will also check the value here versus our keywords
		else if (isalpha(c) || c == '_'){

			do{
				value.append(1, c);
				source >> c;

			} while (isalpha(c) || isdigit(c) || c == '_');

			//here we have gone one over, so we will place the character back onto the stream

			source.putback(c);

			int attempt = table.insert_symbol(value);

			if (attempt < 0){
				queue.push(new IdToken(value));
			}
			else {
				queue.push(new Token(attempt));
			}

		}

		//numbers
		else if (isdigit(c) || c == '.'){


			//while we have just digits, get the digits
			if (isdigit(c)){
				do{
					value.append(1, c);
					source >> c;

				} while (isdigit(c));
			}

			//if either our first character is a . or we got a . while
			//processing the above, add it and start looking for digits again
			if (c == '.'){

				do{
					value.append(1, c);
					source >> c;

				} while (isdigit(c));

			}

			//do the same as above
			if (c == 'e' || c == 'E'){

				//normally we take in '-' as tokens and let the semantics sort it
				//out. This is the one exception, since 3.45e-3 will not be handled 
				//correctly if we do not
				value.append(1, c);
				source >> c;

				//we will take one and exactly one minus symbol
				if (c == '-'){
					value.append(1, c);
					source >> c;
				}

				while (isdigit(c)){
					value.append(1, c);
					source >> c;
				}


			}

			//here we have gone one over, so we will place the character back onto the stream

			source.putback(c);

			//remove e or E if no numbers are found
			//we actually might want to report invaild lexeme here
			if (value[value.length() - 1] == 'e' || value[value.length() - 1] == 'E'){
				
				//create warning message and fix syntax
				lex_mesg warning;
				warning.line = line_num;
				std::ostringstream str;
				str << "'" << value << "' on line " << line_num << ". Digit must follow e. Appending 0.";
				warning.msg = str.str();
				warnings.push(warning);

				value.append(1, '0');

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
			source >> c;
			do{
				value.append(1, c);
				source >> c;

			} while (c != '"' || (c == '"' && value[value.length() - 1] == '\\'));

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
				source >> c;
				if (c == '='){
					queue.push(new Token(LE));
				}
				else{
					queue.push(new Token(LT));
					source.putback(c);
				}
				
				break;
			case '>':
				source >> c;
				if (c == '='){
					queue.push(new Token(GE));
				}
				else{
					queue.push(new Token(GT));
					source.putback(c);
				}
				
				break;
			case '%':
				queue.push(new Token(MOD));
				
				break;
			case '^':
				queue.push(new Token(EXP));
				
				break;
			case ':':
				source >> c;
				if (c == '='){
					queue.push(new Token(ASSIGN));
					
				}
				else {
					source.putback(c);
				}
				break;
			case '!':
				source >> c;
				if (c == '='){
					queue.push(new Token(NE));
					
				}
				else {
					source.putback(c);
				}
				break;
				//non-standard comment 
			case '#':
				//eat the comment line
				do{
					source >> c;
				} while (c != '\n');

				line_num++;
				break;
			case '\n':
				//queue.push(new LineToken(line_num++));
				
				break;
			case ' ':
				break;

			default:
				lex_mesg error;
				error.line = line_num;
				std::ostringstream str;
				str << "Unparasable symbol '" << c << "' on line " 
					<< line_num << ".";
				error.msg = str.str();

				errors.push(error);
					
				break;
			}
		}

	}
	return;
}