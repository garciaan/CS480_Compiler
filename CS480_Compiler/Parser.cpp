#include "Parser.h"



Parser::Parser(std::ifstream &_source, std::string _source_name, Symbol_Table &_table) : source(_source), source_name(_source_name), table(_table), lex(_source, _table) {}

std::string Parser::parse(){

	std::stringstream add;

	//setup file stream
	source >> std::noskipws;

	//this is what tells us to abort if things are bad.
	fatal_error = false;

	// process all input
	lex.tokenize(0);

	add << start();



	if (!errors.empty()){
		std::cout << "ERRORS:" << std::endl;
	}
	while (!errors.empty()){

		std::cout << errors.front().msg << std::endl;
		std::cout << locate_err(errors.front().loc);
		errors.pop();
	}


	if (fatal_error){
		return "Fatal error while parsing.";
	}
	else {
		return add.str();;
	}

}

std::string Parser::start(){

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
	}
	else {
		error("Code did not begin with left bracket '['. Inserting left bracket to continue parse.");
	}

	add << s();

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		error("Code did not end with right bracket ']'. Inserting right bracket to continue parse.");
	}

	//add closing line to file
	add << std::endl << "bye" << std::endl;

	if (!lex.source_empty()){
		error("Unprocessed input remaining after parse.");
	}

	return add.str();

}

std::string Parser::s(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		add << s_2();


	}
	else if (lex.peek_tag() == ID){
		synth_return ret = append_ID();
		add << ret.attr;
		add << s_1();
	}
	else if (is_CONST(lex.peek_tag())){
		synth_return ret = append_CONST();
		add << ret.attr;
		add<< s_1();
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();

}

std::string Parser::s_1(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET || 
		lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		add << s();
		add << s_1();
	}
	else if (lex.peek_tag() == R_BRACKET){
		;
	}
	else{
		error("Syntax error.");
		fatal_error = true;
	}
	
	return add.str();

}

std::string Parser::s_2(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET || is_CONST(lex.peek_tag()) || lex.peek_tag() == ID){
		
		add << s();

		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			error("Mismatched brackets. Adding Right bracket to continue parse.");
		}

		add << s_1();
	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
		add <<  s_1();
	}
	else if (is_BINOP(lex.peek_tag()) || is_UNOP(lex.peek_tag()) 
		|| (lex.peek_tag() == MINUS) || (lex.peek_tag() == ASSIGN) || (lex.peek_tag() == IF)
		|| (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET) || (lex.peek_tag() == ID)){

		add << expr_1();
		add << s_1();
	}
	else if (lex.peek_tag() == STDOUT){
		add <<  stmt_1();
		add << s_1();
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();
}

std::string Parser::expr(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		add << expr_1();
	}
	else if (lex.peek_tag() == ID){
		synth_return val = append_ID();
		add << val.attr;
	}
	else if (is_CONST(lex.peek_tag())){
		synth_return val = append_CONST();
		add << val.attr;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();
}

std::string Parser::expr_1(){

	if (fatal_error) return "";

	std::stringstream add;

	if ((lex.peek_tag() == ASSIGN) || is_BINOP(lex.peek_tag()) || lex.peek_tag() == MINUS
		|| is_UNOP(lex.peek_tag())){
		synth_return val;
		val = oper_1();
		add << val.attr;
	}
	else if ((lex.peek_tag() == IF) || (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET)){
		add << stmt_1();
	}
	else if (lex.peek_tag() == STDOUT){
		add << s_1();
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();
}

Parser::synth_return Parser::oper(){

	synth_return synth;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		synth = oper_1();
	}
	else if (lex.peek_tag() == ID){

		synth.type = VAR; 
		synth.attr = ((IdToken*)lex.peek())->get_id();

		lex.pop();
	}
	else if (is_CONST(lex.peek_tag())){
		synth = const_0();
	}
	else {
		synth.type = ERROR;
	}

	return synth;
}

Parser::synth_return Parser::oper_1(){

	synth_return synth;

	if (fatal_error) return synth;

	//we are going to ignore this step for now
	if (lex.peek_tag() == ASSIGN){
		lex.pop();
		if (lex.peek_tag() == ID){
			lex.pop();
			synth = oper();
		}
	}

	else if (is_BINOP(lex.peek_tag())){
		//get all the synth. attributes filled in
		int bin_op;
		synth_return oper1;
		synth_return oper2;
		bin_op = binop();
		oper1 = oper();
		oper2 = oper();

		if (is_bool_BINOP(bin_op)){
			if (oper1.type == BOOL && oper2.type == BOOL){
				synth.type = BOOL;
				synth.attr = oper1.attr + oper2.attr;
			}
			else {
				//error
			}
		}
		else if (is_real_int_BINOP(bin_op)){
			if (oper1.type == INT && oper2.type == INT && bin_op == EXP){
				synth.type = INT;
				synth.attr = oper1.attr + "s>f " + oper2.attr + "s>f " + "f** f>s ";
			}
			else if (oper1.type == REAL && oper2.type == REAL){
				synth.type = REAL;
				synth.attr = oper1.attr + oper2.attr + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == REAL && oper2.type == INT){
				synth.type = REAL;
				synth.attr = oper1.attr + oper2.attr + "s>f " + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == INT && oper2.type == REAL){
				synth.type = REAL;
				synth.attr = oper1.attr + "s>f " + oper2.attr + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == INT && oper2.type == INT){
				synth.type = INT;
				synth.attr = oper1.attr + oper2.attr + Token::tag_to_input(bin_op) + " ";
			}
		}
		else if (is_log_BINOP(bin_op)){
			if (oper1.type == REAL && oper2.type == REAL){
				synth.type = BOOL;
				synth.attr = oper1.attr + oper2.attr + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == REAL && oper2.type == INT){
				synth.type = BOOL;
				synth.attr = oper1.attr + oper2.attr + "s>f " + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == INT && oper2.type == REAL){
				synth.type = BOOL;
				synth.attr = oper1.attr + "s>f " + oper2.attr + "f" + Token::tag_to_input(bin_op) + " ";
			}
			else if (oper1.type == INT && oper2.type == INT){
				synth.type = BOOL;
				synth.attr = oper1.attr + oper2.attr + Token::tag_to_input(bin_op) + " ";
			}
		}

	}
	else if (is_UNOP(lex.peek_tag())){
		int un_op;
		synth_return oper1;

		un_op = unop();
		oper1 = oper();

		if (oper1.type == BOOL && un_op == NOT){
			synth.type = BOOL;
			synth.attr = oper1.attr + "invert ";
		}
		else if (oper1.type == INT){
			synth.type = REAL;
			synth.attr = oper1.attr + "s>f " + Token::tag_to_input(un_op) + " ";
		}
		else if (oper1.type == REAL){
			synth.type = REAL;
			synth.attr = oper1.attr + Token::tag_to_input(un_op) + " ";
		}

	}
	else if (lex.peek_tag() == MINUS){
		lex.pop();

		synth_return oper1 = oper();
		synth_return oper2 = negop();

		if (oper2.type == EMP){
			if (oper1.type == REAL){
				synth.type == REAL;
				synth.attr = oper1.attr + "fnegate ";
			}
			else if (oper1.type == INT){
				synth.type = INT;
				synth.attr = oper1.attr + "negate ";
			}
		}
		else {
			if (oper1.type == REAL && oper2.type == REAL){
				synth.type = REAL;
				synth.attr = oper1.attr + oper2.attr + "f- ";
			}
			else if (oper1.type == REAL && oper2.type == INT){
				synth.type = REAL;
				synth.attr = oper1.attr + oper2.attr + "s>f f- ";
			}
			else if (oper1.type == INT && oper2.type == REAL){
				synth.type = REAL;
				synth.attr = oper1.attr + "s>f " + oper2.attr + "f- ";
			}
			else if (oper1.type == INT && oper2.type == INT){
				synth.type = REAL;
				synth.attr = oper1.attr + oper2.attr + "- ";
			}
		}
		
	} else {
		error("Syntax error.");
		fatal_error = true;
	}

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		error("Missing right bracket. Inserting to countine parse.");
	}

	return synth;
}

Parser::synth_return Parser::negop(){

	synth_return synth;

	if (fatal_error) return synth;

	if (is_CONST(lex.peek_tag()) || lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID){
		synth = oper();
	}
	else if (lex.peek_tag() == R_BRACKET){
		synth.type = EMP;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}
	
	return synth;

}

std::string Parser::stmt(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		add << stmt_1();
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();
}

std::string Parser::stmt_1(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == IF){
		lex.pop();

		add << expr();
		add << expr();
		add << ifstmt();

	}
	else if (lex.peek_tag() == WHILE){
		lex.pop();

		add << expr();
		add << exprlist();
	}
	else if (lex.peek_tag() == LET){
		lex.pop();

		if (lex.peek_tag() == L_BRACKET){
			lex.pop();
		}
		else {
			error("Missing left bracket. Inserting left bracket to continue parse.");
		}

		add << varlist();

		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			error("Missing right bracket. Inserting right bracket to continue parse.");
		}

	}
	else if (lex.peek_tag() == STDOUT){
		lex.pop();
		add << oper().attr;
		
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	//pop final right bracket
	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		error("Missing right bracket. Inserting right bracket to continue parse.");
	}

	return add.str();

}

int Parser::binop(){

	if (fatal_error) return -1;

	if (is_BINOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		lex.pop();
		return synth;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
		return -1;
	}

}

int Parser::unop(){

	if (is_UNOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		lex.pop();
		return synth;
	}
	else {
		return -1;
	}
}

Parser::synth_return Parser::const_0(){

	synth_return synth;

	if (fatal_error) return synth;

	if (lex.peek_tag() == STRING_L){
		synth.type = STRING;
		synth.attr = "\"" + ((StrToken*)lex.peek())->get_str() + "\" ";
	}
	else if (lex.peek_tag() == INT_L){
		synth.type = INT;
		synth.attr =  std::to_string(((IntToken*)lex.peek())->get_int()) + " ";
	}
	else if (lex.peek_tag() == REAL_L){
		synth.type = REAL;
		std::stringstream temp;
		temp << std::scientific  << ((RealToken*)lex.peek())->get_real() << " ";
		synth.attr = temp.str();
	}
	else if (lex.peek_tag() == TRUE){
		synth.type = BOOL;
		synth.attr = "true ";
	}
	else if (lex.peek_tag() == FALSE){
		synth.type = BOOL;
		synth.attr = "false ";
	}
	else {
		error("Syntax error."); 
			fatal_error = true;
	}

	lex.pop();
	return synth;
}

std::string Parser::ifstmt(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){

		add << expr();

		if (lex.peek_tag() == R_BRACKET){
			;
		}
		else {
			error("Missing right bracket. Inserting right bracket to continue parsing.");
		}

	}
	else if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}
}

std::string Parser::exprlist(){

	if (fatal_error) return "";

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		
		return (expr() + exprlist_1());
	}
	else {
		error("Syntax Error.");
		fatal_error = true;
	}

}

std::string Parser::exprlist_1(){

	if (fatal_error) return "";

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		return exprlist();
	}
	else if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

}

std::string Parser::varlist(){

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
	}
	else {
		error("Missing left bracket. Inserting left bracket to continue parsing.");
	}
	
	add << append_ID().attr;
	add << type();

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}

	add << varlist_1();

	return add.str();

}

std::string Parser::varlist_1(){

	if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else if (lex.peek_tag() == L_BRACKET){
		return varlist();
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}

}

std::string Parser::type(){

	if (lex.peek_tag() == BOOL_T || lex.peek_tag() == INT_T ||
		lex.peek_tag() == REAL_T || lex.peek_tag() == STRING_T){
		lex.pop();
		return "";
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}
}


Parser::synth_return Parser::append_ID(){

	if (lex.peek_tag() == ID){

		synth_return synth;

		synth.type = VAR;
		synth.attr = ((IdToken*)lex.peek())->get_id() + " ";
		lex.pop();
		return synth;
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}
}

Parser::synth_return Parser::append_CONST(){
	if (is_CONST(lex.peek_tag())){
		return const_0();
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}
	
}

void Parser::error(std::string msg){

	mesg error_mesg;
	if (lex.source_empty()){
		error_mesg.loc = lex.get_loc();
	}
	else {
		error_mesg.loc = lex.peek()->get_loc();
	}
	error_mesg.msg = msg;

	errors.push(error_mesg);
}

std::string Parser::locate_err(int loc){

	std::stringstream ret;

	std::ifstream *loc_stream = new std::ifstream(source_name);
	//loc_stream.seekg(loc);
	int start = loc - 35 > 0 ? loc - 35 : 0;
	//while (loc_stream.peek() != '\n'){
	//	source.seekg(source.tellg() - (std::streampos)start);
	//	start -= 1;
	//}
	//start = loc_stream.tellg();

	//loc_stream.seekg(loc);
	loc_stream->seekg(0, std::ios_base::end);
	int stop = loc + 35 < loc_stream->tellg() ? loc + 35 : loc_stream->tellg();
	//while (loc_stream.peek() != '\n'){
	//	source.seekg(source.tellg() + (std::streampos)stop);
	//	stop += 1;
	//}
	//stop = loc_stream.tellg();
	for (int i = start; i < stop; ++i){
		loc_stream->seekg(i);
		ret << (char)loc_stream->peek();
	}

	ret << std::endl;

	for (int i = start; i < loc; ++i){
		ret << " ";
	}
	ret << "^" << std::endl;;

	return ret.str();

}




bool Parser::is_CONST(int val){
	return (val == INT_L) || (val == REAL_L) || (val == STRING_L)
		|| (val == TRUE) || (val == FALSE);
}

bool Parser::is_BINOP(int val){
	return is_bool_BINOP(val) || is_real_int_BINOP(val)
		|| is_log_BINOP(val);
}

bool Parser::is_UNOP(int val){
	return (val == NOT) || (val == SIN) || (val == COS) || (val == TAN);
}

bool Parser::is_real_int_BINOP(int val){
	return (val == PLUS) || (val == MULTI) || (val == DIV) ||
		(val == MOD) || (val == EXP);
}

bool Parser::is_bool_BINOP(int val){
	return (val == AND) || (val == OR);
}

bool Parser::is_log_BINOP(int val){
	return (val == EQ) || (val == NE) || (val == LT) ||
		(val == GT) || (val == LE) || (val == GE);

}