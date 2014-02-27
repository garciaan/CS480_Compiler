#include "Lex.h"


using namespace std;




Lexer::~Lexer(){

	//free memory in our queue
	while (!queue.empty()){
		delete queue.front();
		queue.pop();
	}
}


Token *Lexer::peek(){

	if (queue.empty()){
		 int result = tokenize(1);
		 if (result == -1){
			 return NULL;
		}
	} 
	
	return queue.front();
}

int Lexer::peek_tag(){

	if (queue.empty()){
		int result = tokenize(1);
		if (result == -1){
			return INPUT_END;
		}
	}

	return queue.front()->get_tag();
}

void Lexer::pop(){

	queue.pop();
}

// returns the number of tokens created, or -1 if non were
int Lexer::tokenize(int num_tokens){

	int line_num = 1;
	int cur_tokens = 0;
	num_tokens = num_tokens > 0 ? num_tokens : -1;

	string value;

	//infinte loop to process each character
	//one pass through this loop should be equivlent to going
	//from a start state to a final state on our FSA
	while (cur_tokens != num_tokens){

		//value is used to hold itermediate lexme values. Cleared after every run.
		value = "";

		//read a chacter, but eat any white space since going through with these characters
		//will just be a waste of time
		while (source.peek() == ' ' || source.peek() == '\t'){
			source.ignore();
		}
		

		//if we hit the end of the file we are done here
		if (source.eof()){
			break;
		}

		//ids
		//we will also check the value here versus our keywords
		else if (isalpha(source.peek()) || source.peek() == '_'){

			do{
				value.append(1, source.get());

			} while (isalpha(source.peek()) || isdigit(source.peek()) || source.peek() == '_');

			//insert id into symbol table unless we have 
			//a keyword which we then put into the correct token
			int attempt = table.insert_symbol(value);

			if (attempt < 0){
				queue.push(new IdToken(value));
				++cur_tokens;
			}
			else {
				queue.push(new Token(attempt));
				++cur_tokens;
			}

		}

		//numbers
		else if (isdigit(source.peek()) || source.peek() == '.'){


			//while we have just digits, get the digits
			if (isdigit(source.peek())){
				do{
					value.append(1, source.get());
				} while (isdigit(source.peek()));
			}

			//if either our first character is a . or we got a . while
			//processing the above, add it and start looking for digits again
			if (source.peek() == '.'){

				do{
					value.append(1, source.get());
				} while (isdigit(source.peek()));

			}

			//do the same as above
			if (source.peek() == 'e' || source.peek() == 'E'){

				//normally we take in '-' as tokens and let the semantics sort it
				//out. This is the one exception, since 3.45e-3 will not be handled 
				//correctly if we do not
				value.append(1, source.get());

				//we will take one and exactly one minus symbol
				if (source.peek() == '-'){
					value.append(1, source.get());
				}

				while (isdigit(source.peek())){
					value.append(1, source.get());
				}


			}

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
				++cur_tokens;
			}
			else {
				queue.push(new IntToken(value));
				++cur_tokens;
			}

		}
		//string literals
		else if (source.peek() == '"'){

			//eat the leading "
			source.ignore();
			while (source.peek() != '"'){
				value.append(1, source.get());
			}
			source.ignore();

			queue.push(new StrToken(value));
			++cur_tokens;

		}
		else {

			//Match all the little weird symbols. This part is nasty and large

			switch (source.peek())
			{

			case '[':
				queue.push(new Token(L_BRACKET));
				++cur_tokens;
				source.ignore();
				break;
			case ']':
				queue.push(new Token(R_BRACKET));
				++cur_tokens;
				source.ignore();
				break;
			case '+':
				queue.push(new Token(PLUS));
				++cur_tokens;
				source.ignore();
				break;
			case '-':
				queue.push(new Token(MINUS));
				++cur_tokens;
				source.ignore();
				break;
			case '*':
				queue.push(new Token(MULTI));
				++cur_tokens;
				source.ignore();
				break;
			case '/':
				queue.push(new Token(DIV));
				++cur_tokens;
				source.ignore();
				break;
			case '=':
				queue.push(new Token(EQ));
				++cur_tokens;
				source.ignore();
				break;
			case '<':
				source.ignore();
				if (source.peek() == '='){
					queue.push(new Token(LE));
					++cur_tokens;
					source.ignore();
				}
				else{
					queue.push(new Token(LT));
					++cur_tokens;
				}
				
				break;
			case '>':
				source.ignore();
				if (source.peek() == '='){
					queue.push(new Token(GE));
					++cur_tokens;
					source.ignore();
				}
				else{
					queue.push(new Token(GT));
					++cur_tokens;
				}
				
				break;
			case '%':
				queue.push(new Token(MOD));
				++cur_tokens;
				source.ignore();
				break;
			case '^':
				queue.push(new Token(EXP));
				++cur_tokens;
				source.ignore();
				break;
			case ':':
				source.ignore();
				if (source.peek() == '='){
					queue.push(new Token(ASSIGN));
					++cur_tokens;
					source.ignore();
				}
				break;
			case '!':
				source.ignore();
				if (source.peek() == '='){
					queue.push(new Token(NE));
					++cur_tokens;
					source.ignore();
				}
				break;
				//non-standard comment 
			case '#':
				//eat the comment line
				do{
					source.ignore();
				} while (source.peek() != '\n');

				line_num++;
				break;
			case '\n':
				//queue.push(new LineToken(line_num++));
				source.ignore();
				break;
			case ' ':
				break;

			default:
				lex_mesg error;
				error.line = line_num;
				std::ostringstream str;
				str << "Unparasable symbol '" << source.get() << "' on line " 
					<< line_num << ".";
				error.msg = str.str();

				errors.push(error);
					
				break;
			}
		}

	}

	if (cur_tokens > 0){
		return cur_tokens;
	}
	else {
		return -1;
	}
	
}