#include "Parser.h"

void parser::parse(std::ifstream &source, std::string source_name, Symbol_Table &table, bool verbose){


	//setup file stream
	source >> std::noskipws;

	Lexer lex = Lexer(source, table);

	// process all input
	lex.tokenize(0);

	int result = start(lex);

	if (result == 0){
		std::cout << "Accepted!" << std::endl;
	}
	else {
		std::cout << "Rejected!" << std::endl;
	}
}

int parser::start(Lexer &lex){

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
	}
	else {
		return -1;
	}

	//string that will repersent our complete code
	std::string code = s(lex);

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		return -1;
	}

	//add closing line to file
	code += " bye";

	if (!lex.source_empty()){

		//error
		return -1;
	}

	//just print out for now
	std::cout << code << std::endl;

	//accept
	return 0;

}

std::string parser::s(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		synth += s_2(lex);
	}
	else if (lex.peek_tag() == ID){
		synth += parser::append_ID(lex); 
		synth += s_1(lex);
	}
	else if (is_CONST(lex.peek_tag())){
		synth += parser::append_CONST(lex);
		synth += s_1(lex);
	}
	else {
		//time to error
		return "$";
	}

	return synth;

}

std::string parser::s_1(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == L_BRACKET || 
		lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		synth += s(lex);
		synth += s_1(lex);
	}
	else if (lex.peek_tag() == R_BRACKET){
		synth += "";
	}
	else{
		//error
		return "$";
	}
	
	return synth;

}

std::string parser::s_2(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == L_BRACKET || is_CONST(lex.peek_tag()) || lex.peek_tag() == ID){
		
		synth += s(lex);

		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return "$";
		}

		synth += s_1(lex);
	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
		synth += s_1(lex);
	}
	else if (is_BINOP(lex.peek_tag()) || is_UNOP(lex.peek_tag()) 
		|| (lex.peek_tag() == MINUS) || (lex.peek_tag() == ASSIGN) || (lex.peek_tag() == IF)
		|| (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET) || (lex.peek_tag() == ID)){

		synth += expr_1(lex);
		synth += s_1(lex);
	}
	else if (lex.peek_tag() == STDOUT){
		synth += stmt_1(lex);
		synth += s_1(lex);
	}
	else {
		//error
		return "$";
	}

	return synth;
}

std::string parser::expr(Lexer &lex){

	std::string code;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		code += expr_1(lex);
	}
	else if (lex.peek_tag() == ID){
		code += append_ID(lex);
	}
	else if (is_CONST(lex.peek_tag())){
		code += append_CONST(lex);
	}
	else {
		//error
		return "$";
	}

	return code;
}

std::string parser::expr_1(Lexer &lex){

	std::string code;

	if ((lex.peek_tag() == ASSIGN) || is_BINOP(lex.peek_tag()) || lex.peek_tag() == MINUS
		|| is_UNOP(lex.peek_tag())){
		oper_return val;
		val = oper_1(lex);
		code += val.code;
	}
	else if ((lex.peek_tag() == IF) || (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET)){
		code += stmt_1(lex);
	}
	else if (lex.peek_tag() == STDOUT){
		code += s_1(lex);
	}
	else {
		//error
		return "$";
	}

	return code;
}

parser::oper_return parser::oper(Lexer &lex){

	oper_return val;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		val = oper_1(lex);
	}
	else if (lex.peek_tag() == ID){

		val.type = CODE;
		val.code = ((IdToken*)lex.peek())->get_id();

		lex.pop();
	}
	else if (is_CONST(lex.peek_tag())){
		val = const_0(lex);
	}
	else {
		val.code = '$';
	}

	return val;
}

parser::oper_return parser::oper_1(Lexer &lex){

	oper_return synth;

	//we are going to ignore this step for now
	if (lex.peek_tag() == ASSIGN){
		lex.pop();
		if (lex.peek_tag() == ID){
			lex.pop();
			synth = oper(lex);
		}
	}

	else if (is_BINOP(lex.peek_tag())){
		//get all the synth. attributes filled in
		int bin_op;
		oper_return oper1;
		oper_return oper2;
		bin_op = binop(lex);
		oper1 = oper(lex);
		oper2 = oper(lex);


		if (oper1.type == CODE || oper2.type == CODE){
			synth.type = CODE;
			synth.code = oper1.code + " " + oper2.code + " " + Token::tag_to_input(bin_op);
		}
		
	}
	else if (is_UNOP(lex.peek_tag())){
		int un_op;
		oper_return oper1;

		un_op = unop(lex);
		oper1 = oper(lex);

		if (oper1.type == CODE){
			synth.type = CODE;
			synth.code = oper1.code + " " +  Token::tag_to_input(un_op);
		}
		else{



		}
	}
	else if (lex.peek_tag() == MINUS){
		lex.pop();

		oper_return oper1;
		oper_return oper2;

		oper(lex);
		negop(lex);
	} else {
		synth.type = ERROR;
	}

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		synth.type = ERROR;
	}

	return synth;
}

parser::oper_return parser::negop(Lexer &lex){

	oper_return synth;


	if (is_CONST(lex.peek_tag()) || lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID){

		oper_return val;
		oper(lex);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			synth.type = ERROR;
		}
	}
	else if (lex.peek_tag() == R_BRACKET){

	}
	else{
		synth.type = ERROR;
	}


	return synth;

}

std::string parser::stmt(Lexer &lex){

	std::string code;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		code += stmt_1(lex);
	}
	else {
		code = "$";
	}

	return code;
}

std::string parser::stmt_1(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == IF){
		lex.pop();

		synth += expr(lex) + expr(lex) + ifstmt(lex);
	}
	else if (lex.peek_tag() == WHILE){
		lex.pop();

		synth += expr(lex) + exprlist(lex);
	}
	else if (lex.peek_tag() == LET){
		lex.pop();

		if (lex.peek_tag() == L_BRACKET){
			lex.pop();
		}
		else {
			return "$";
		}

		synth += varlist(lex);

		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return "$";
		}

	}
	else if (lex.peek_tag() == STDOUT){
		lex.pop();
		synth += oper(lex).code;
		
	}
	else {
		return "$";
	}

	//pop final right bracket
	if (lex.peek_tag() != R_BRACKET){
		return "$";
	}
	else {
		lex.pop();
	}

	return synth;

}

int parser::binop(Lexer &lex){

	if (is_BINOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		lex.pop();
		return synth;
	}
	else {
		return -1;
	}
}

int parser::unop(Lexer &lex){

	if (is_UNOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		lex.pop();
		return synth;
	}
	else {
		return -1;
	}
}

parser::oper_return parser::const_0(Lexer &lex){

	oper_return synth;

	if (lex.peek_tag() == STRING_L){
		synth.type = STRING;
		synth.code = "\"" + ((StrToken*)lex.peek())->get_str() + "\" ";
	}
	else if (lex.peek_tag() == INT_L){
		synth.type = INT;
		synth.code =  std::to_string(((IntToken*)lex.peek())->get_int()) + " ";
	}
	else if (lex.peek_tag() == REAL_L){
		synth.type = REAL;
		std::stringstream temp;
		temp << std::scientific  << ((RealToken*)lex.peek())->get_real() << " ";
		synth.code = temp.str();
	}
	else if (lex.peek_tag() == TRUE){
		synth.type = BOOL;
		synth.code = "true ";
	}
	else if (lex.peek_tag() == FALSE){
		synth.type = BOOL;
		synth.code = "false ";
	}
	else {
		synth.type = ERROR;
	}

	lex.pop();
	return synth;
}

std::string parser::ifstmt(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){

		synth += expr(lex);

		if (lex.peek_tag() == R_BRACKET){
			return synth;
		}
		else {
			return "$";
		}

	}
	else if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else {
		return "$";
	}
}

std::string parser::exprlist(Lexer &lex){

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		
		return (expr(lex) + exprlist_1(lex));
	}
	else {
		return "$";
	}

}

std::string parser::exprlist_1(Lexer &lex){

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		return exprlist(lex);
	}
	else if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else {
		return "$";
	}

}

std::string parser::varlist(Lexer &lex){

	std::string synth;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
	}
	else {
		return "$";
	}
	
	synth += append_ID(lex) + type(lex);

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		return "$";
	}

	synth += varlist_1(lex);

	return synth;

}

std::string parser::varlist_1(Lexer &lex){

	if (lex.peek_tag() == R_BRACKET){
		return "";
	}
	else if (lex.peek_tag() == L_BRACKET){
		return varlist(lex);
	}
	else {
		return "$";
	}

}

std::string parser::type(Lexer &lex){

	if (lex.peek_tag() == BOOL_T || lex.peek_tag() == INT_T ||
		lex.peek_tag() == REAL_T || lex.peek_tag() == STRING_T){
		lex.pop();
		return "";
	}
	else {
		return "$";
	}
}


std::string parser::append_ID(Lexer &lex){

	if (lex.peek_tag() == ID){

		std::string synth = ((IdToken*)lex.peek())->get_id() + " ";
		lex.pop();
		return synth;
	}
	else {
		return "$";
	}
}

std::string parser::append_CONST(Lexer &lex){
	if (is_CONST(lex.peek_tag())){
		oper_return val = const_0(lex);
		return val.code;
	}
	else {
		return "$";
	}
	
}

bool parser::is_CONST(int val){
	return (val == INT_L) || (val == REAL_L) || (val == STRING_L)
		|| (val == TRUE) || (val == FALSE);
}

bool parser::is_BINOP(int val){
	return (val == AND) || (val == OR) || (val == PLUS) || (val == MULTI) || (val == DIV) ||
		(val == MOD) || (val == EXP) || (val == EQ) || (val == NE) || (val == LT) ||
		(val == GT) || (val == LE) || (val == GE);
}

bool parser::is_UNOP(int val){
	return (val == NOT) || (val == SIN) || (val == COS) || (val == TAN);
}