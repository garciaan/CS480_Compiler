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
	if (lex.peek_tag() != L_BRACKET){
		//error
		return -1;
	}

	lex.pop();

	//string that will repersent our complete code
	std::string code = s(lex);

	if (lex.peek_tag() != R_BRACKET || (code.at(code.length() - 1) == '$')){
		//error
		return -1;
	}

	//just print out for now
	std::cout << code << std::endl;

	//accept
	return 0;

}

std::string parser::s(Lexer &lex){

	std::string code = "";

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		code  += s_2(lex);
	}
	else if (lex.peek_tag() == ID){
		code += parser::append_ID(lex);
		code += s_1(lex);
	}
	else if (is_CONST(lex.peek_tag())){
		code += parser::append_CONST(lex);
		code += s_1(lex);
	}
	else {
		//time to error
		return "$";
	}

	return code;
}

std::string parser::s_1(Lexer &lex){

	std::string code;

	if (lex.peek_tag() == L_BRACKET || 
		lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		code += s(lex) + s_1(lex);
			
	}
	else if (lex.peek_tag() == R_BRACKET){
		code += "";
	}
	else{
		//error
		return "$";
	}

	return code;
}

std::string parser::s_2(Lexer &lex){

	std::string code;

	if (lex.peek_tag() == L_BRACKET || is_CONST(lex.peek_tag()) || lex.peek_tag() == ID){
		code += s(lex);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
			code += s_1(lex);
		}
		else {
			//error
			return "$";
		}
	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
		code += s_1(lex);
	}
	else if (is_BINOP(lex.peek_tag()) || is_UNOP(lex.peek_tag()) 
		|| (lex.peek_tag() == MINUS) || (lex.peek_tag() == ASSIGN) || (lex.peek_tag() == IF)
		|| (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET) || (lex.peek_tag() == ID)){

		code += expr_1(lex) + s_1(lex);
	}
	else if (lex.peek_tag() == STDOUT){
		code += stmt_1(lex) + s_1(lex);
	}
	else {
		//error
		return "$";
	}

	return code;
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
		binop(lex, &bin_op);
		oper1 = oper(lex);
		oper2 = oper(lex);


		if (oper1.type == CODE || oper2.type == CODE){
			synth->type = CODE;
			synth->code = oper1.code + " " + oper2.code + " " + Token::tag_to_input(bin_op);
		}
		else{

			switch (bin_op)
			{
			case PLUS:
				plus(oper1, oper2, synth);
				break;
			case MULTI:
				multi(oper1, oper2, synth);
				break;
			case DIV:
				div(oper1, oper2, synth);
				break;
			case MOD:
				mod(oper1, oper2, synth);
				break;
			case EXP:
				exp(oper1, oper2, synth);
				break;
			case EQ:
				eq(oper1, oper2, synth);
				break;
			case LE:
				le(oper1, oper2, synth);
				break;
			case LT:
				lt(oper1, oper2, synth);
				break;
			case GE:
				ge(oper1, oper2, synth);
				break;
			case GT:
				gt(oper1, oper2, synth);
				break;
			case NE:
				ne(oper1, oper2, synth);
				break;
			case OR:
				or(oper1, oper2, synth);
				break;
			case AND:
				and(oper1, oper2, synth);
				break;
			default:
				break;
			}
		}
	}
	else if (is_UNOP(lex.peek_tag())){
		int un_op;
		oper_return oper1;

		unop(lex, &un_op);
		oper(lex, &oper1);

		if (oper1.type == CODE){
			synth->type = CODE;
			synth->code = oper1.code + " " +  Token::tag_to_input(un_op);
		}
		else{

			switch (un_op)
			{
			case SIN:
				sin(oper1, synth);
				break;
			case COS:
				cos(oper1, synth);
				break;
			case TAN:
				tan(oper1, synth);
				break;
			case NOT:
				not(oper1, synth);
				break;
			default:
				break;
			}

		}
	}
	else if (lex.peek_tag() == MINUS){
		lex.pop();

		oper_return oper1;
		oper_return oper2;

		oper(lex, &oper1);
		negop(lex, &oper2);
	} else {
		return -1;
	}

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		return -1;
	}
}

int stmt(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		stmt_1(lex, &code_0);
	}
	else {
		return -1;
	}

	*code += code_0;
	return 0;
}

int stmt_1(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == IF){
		lex.pop();
		expr(lex, &code_0);
		expr(lex, &code_0);
		ifstmt(lex, &code_0);
	}
	else if (lex.peek_tag() == WHILE){
		lex.pop();
		expr(lex, code);
		exprlist(lex, code);
		if (lex.peek_tag() != R_BRACKET){
			return -1;
		}
		else {
			lex.pop();
		}
	}
	else if (lex.peek_tag() == LET){
		lex.pop();
		if (lex.peek_tag() == L_BRACKET){
			lex.pop();
			varlist(lex, &code_0);
			if (lex.peek_tag() == R_BRACKET){
				lex.pop();
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	else if (lex.peek_tag() == STDOUT){
		lex.pop();
		oper_return val;
		oper(lex, &val);
		if (lex.peek_tag() != R_BRACKET){
			return -1;
		}
		else {
			lex.pop();
		}
		return 0;
	}
	else {
		return -1;
	}

}

int binop(Lexer &lex, int *operation){
	if (is_BINOP(lex.peek_tag())){
		*operation = lex.peek_tag();
		lex.pop();
	}
	else {
		return -1;
	}

	return 0;
}

int unop(Lexer &lex, int *operation){
	if (is_UNOP(lex.peek_tag())){
		*operation = lex.peek_tag();
		lex.pop();
	}
	else {
		return -1;
	}

	return 0;
}

int const_0(Lexer &lex, oper_return* synth){

	if (lex.peek_tag() == STRING_L){
		synth->type = STRING;
		synth->str_value = ((StrToken*)lex.peek())->get_str();
		synth->code = "\"" + ((StrToken*)lex.peek())->get_str() + "\" ";
	}
	else if (lex.peek_tag() == INT_L){
		synth->type = INT;
		synth->int_value = ((IntToken*)lex.peek())->get_int();
		synth->code =  std::to_string(((IntToken*)lex.peek())->get_int()) + " ";
	}
	else if (lex.peek_tag() == REAL_L){
		synth->type = REAL;
		synth->real_value = ((RealToken*)lex.peek())->get_real();
		std::stringstream temp;
		temp << std::scientific  << ((RealToken*)lex.peek())->get_real() << " ";
		synth->code = temp.str();
	}
	else if (lex.peek_tag() == TRUE){
		synth->type = BOOL;
		synth->int_value = 1;
		synth->code = "true ";
	}
	else if (lex.peek_tag() == FALSE){
		synth->type = BOOL;
		synth->int_value = 0;
		synth->code = "false ";
	}
	else {
		return -1;
	}

	lex.pop();
	return 0;
}

std::string ifstmt(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		expr(lex, &code_0);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
			return code_0;
		}
		else {
			return NULL;
		}

	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		return 0;
	}
}

int exprlist(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		expr(lex,&code_0);
		exprlist_1(lex, &code_0);
	}
	else {
		return -1;
	}

	return 0;
}

int exprlist_1(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		exprlist(lex, &code_0);
	}
	else if (lex.peek_tag() == R_BRACKET){
		return 0;
	}
	else {
		return -1;
	}

	return 0;

}

int varlist(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		if (lex.peek_tag() == ID){
			lex.pop();
			type(lex, &code_0);
			if (lex.peek_tag() == R_BRACKET){
				lex.pop();
				varlist_1(lex, &code_0);
			}
			else {
				return -1;

			}
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}

int varlist_1(Lexer &lex, std::string *code){

	std::string code_0;

	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else if (lex.peek_tag() == L_BRACKET){
		varlist(lex, &code_0);
	}
	else {
		return -1;
	}

	return 0;
}

int type(Lexer &lex, std::string *code){
	if (lex.peek_tag() == BOOL_T || lex.peek_tag() == INT_T ||
		lex.peek_tag() == REAL_T || lex.peek_tag() == STRING_T){
		lex.pop();
	}
	else {
		return -1;
	}

	return 0;
}

int negop(Lexer &lex, oper_return *code){
	if (is_CONST(lex.peek_tag()) || lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID){

		oper_return val;
		oper(lex, &val);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return - 1;
		}
	}
	else if (lex.peek_tag() == R_BRACKET){

	}
	else{
		return -1;
	}
	return 0;

}

void plus(oper_return oper1, oper_return oper2, oper_return* synth){
	//huge bulk of logic here for evaluation
	//addition and integers
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value + oper2.int_value;
		synth->type = INT;
		synth->code = std::to_string(oper1.int_value + oper2.int_value);
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value + oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (double)oper1.int_value + oper2.real_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = oper1.real_value + (double)oper2.int_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value + (double)oper2.int_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = oper1.real_value + oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value + oper2.real_value << " ";
		synth->code = temp.str();
	}
	//other cases?
}

void multi(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value *  oper2.int_value;
		synth->type = INT;
		synth->code = std::to_string(oper1.int_value * oper2.int_value);
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value * oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (double)oper1.int_value * oper2.real_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = oper1.real_value * (double)oper2.int_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value * (double)oper2.int_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = oper1.real_value * oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value * oper2.real_value << " ";
		synth->code = temp.str();
	}
}
void div(oper_return oper1, oper_return oper2, oper_return *synth){

	if (oper2.type == INT && oper2.int_value == 0 ||
		oper2.type == REAL && oper2.real_value == 0){
		//handle divide by zero errors
	}


	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value / oper2.int_value;
		synth->type = INT;
		synth->code = std::to_string(oper1.int_value / oper2.int_value);
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value / oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (double)oper1.int_value / oper2.real_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = oper1.real_value / (double)oper2.int_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value / (double)oper2.int_value << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = oper1.real_value / oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << oper1.real_value / oper2.real_value << " ";
		synth->code = temp.str();
	}
}
void mod(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value % oper2.int_value;
		synth->type = INT;
		synth->code = std::to_string(oper1.int_value % oper2.int_value);
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = fmod((double)oper1.int_value,oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << fmod((double)oper1.int_value, oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = fmod(oper1.real_value, (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << fmod(oper1.real_value, (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = fmod(oper1.real_value, oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << fmod(oper1.real_value, oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void exp(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = pow(oper1.int_value, oper2.int_value);
		synth->type = INT;
		synth->code = std::to_string(pow(oper1.int_value, oper2.int_value));
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = pow((double)oper1.int_value, oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << pow((double)oper1.int_value, oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = pow(oper1.real_value, (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << pow(oper1.real_value, (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = pow(oper1.real_value, oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << pow(oper1.real_value, oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void eq(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value == oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value == oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value == oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value == (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value == (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value == oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value == oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void le(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value <= oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value <= oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value <= oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value <= (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value <= (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value <= oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value <= oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void lt(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value < oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value < oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value < oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value < (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value < (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value < oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value < oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void ge(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value >= oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value >= oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value >= oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value >= (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value >= (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value >= oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value >= oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void gt(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value > oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value > oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value > oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value > (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value > (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value > oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value > oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void ne(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == INT && oper2.type == INT){
		synth->int_value = oper1.int_value == oper2.int_value;
		synth->type = BOOL;
		synth->code = (oper1.int_value != oper2.int_value) == 1 ? "true " : "false ";
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value != oper2.real_value;
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << ((double)oper1.int_value != oper2.real_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (oper1.real_value != (double)oper2.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value != (double)oper2.int_value) << " ";
		synth->code = temp.str();
	}
	else if ((oper1.type == REAL && oper2.type == REAL)){
		synth->real_value = (oper1.real_value != oper2.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (oper1.real_value != oper2.real_value) << " ";
		synth->code = temp.str();
	}
}
void or(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == BOOL && oper2.type == BOOL){
		synth->int_value = (oper1.int_value || oper2.int_value);
		synth->type = BOOL;
		synth->code = (oper1.int_value || oper2.int_value) == 1 ? "true " : "false ";
	}
	else {
		//error
	}
}
void and(oper_return oper1, oper_return oper2, oper_return *synth){
	if (oper1.type == BOOL && oper2.type == BOOL){
		synth->int_value = (oper1.int_value && oper2.int_value);
		synth->type = BOOL;
		synth->code = (oper1.int_value && oper2.int_value) == 1 ? "true " : "false ";
	}
	else {
		//error
	}
}


void not(oper_return oper, oper_return *synth){
	if (oper.type == BOOL){
		synth->int_value = (!oper.int_value);
		synth->type = BOOL;
		synth->code = (synth->int_value) > 0 ? "true " : "false ";
	}
	else {
		//error
	}
}
void sin(oper_return oper, oper_return *synth){
	if (oper.type == REAL){
		synth->real_value = sin(oper.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else if (oper.type == INT){
		synth->real_value = sin(oper.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else {
		//error
	}
}
void cos(oper_return oper, oper_return *synth){
	if (oper.type == REAL){
		synth->real_value = cos(oper.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else if (oper.type == INT){
		synth->real_value = cos(oper.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else {
		//error
	}
}
void tan(oper_return oper, oper_return *synth){
	if (oper.type == REAL){
		synth->real_value = tan(oper.real_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else if (oper.type == INT){
		synth->real_value = tan(oper.int_value);
		synth->type = REAL;
		std::stringstream temp;
		temp << std::scientific << (synth->real_value) << " ";
		synth->code = temp.str();
	}
	else {
		//error
	}
}

std::string parser::append_ID(Lexer &lex){
	std::string code = ((IdToken*)lex.peek())->get_id() + " ";
	lex.pop();
	return code;
}

std::string parser::append_CONST(Lexer &lex){
	oper_return val;
	const_0(lex, &val);
	return val.code;
}

bool is_CONST(int val){
	return (val == INT_L) || (val == REAL_L) || (val == STRING_L)
		|| (val == TRUE) || (val == FALSE);
}

bool is_BINOP(int val){
	return (val == AND) || (val == OR) || (val == PLUS) || (val == MULTI) || (val == DIV) ||
		(val == MOD) || (val == EXP) || (val == EQ) || (val == NE) || (val == LT) ||
		(val == GT) || (val == LE) || (val == GE);
}

bool is_UNOP(int val){
	return (val == NOT) || (val == SIN) || (val == COS) || (val == TAN);
}