#include "Parser.h"

void parse(std::ifstream &source, std::string source_name, Symbol_Table &table, bool verbose){


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

int start(Lexer &lex){
	if (lex.peek_tag() != L_BRACKET){
		//error
		return -1;
	}

	lex.pop();

	std::string code = "";

	//TODO get return value?
	int result = s(lex, &code);

	if (lex.peek_tag() != R_BRACKET || (result != 0)){
		//error
		return -1;
	}

	//just print out for now
	std::cout << code << std::endl;

	//accept
	return 0;

}

int s(Lexer &lex, std::string *code){


	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		s_2(lex, code);
	}
	else if (lex.peek_tag() == ID){
		lex.pop();
		*code += " " + ((IdToken*)lex.peek())->get_id();
		s_1(lex, code);
	}
	else if (is_CONST(lex.peek_tag())){
		oper_return val;
		const_0(lex, &val);
		s_1(lex, code);

	}
	else {
		//time to error
		return -1;
	}

	return 0;

}

int s_1(Lexer &lex, std::string *code){
	if (lex.peek_tag() == L_BRACKET || 
		lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
			s(lex, code);
			s_1(lex, code);
	}
	else if (lex.peek_tag() == R_BRACKET){
		return 0;
	}
	else{
		//error
		return -1;
	}

	return 0;
}

int s_2(Lexer &lex, std::string *code){
	if (lex.peek_tag() == L_BRACKET || is_CONST(lex.peek_tag()) || lex.peek_tag() == ID){
		lex.pop();
		s(lex, code);
		if (lex.peek_tag() != R_BRACKET){
			//error
			return -1;
		}
		else {
			lex.pop();
			s_1(lex, code);
			return 0;
		}
	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
		s_1(lex, code);
	}
	else if (is_BINOP(lex.peek_tag()) || is_UNOP(lex.peek_tag()) 
		|| (lex.peek_tag() == MINUS) || (lex.peek_tag() == ASSIGN) || (lex.peek_tag() == IF)
		|| (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET) || (lex.peek_tag() == ID)){
		expr_1(lex);
		s_1(lex, code);
	}
	else if (lex.peek_tag() == STDOUT){
		stmt_1(lex);
		s_1(lex, code);
	}
	else {
		//error
		return -1;
	}
}

int expr(Lexer &lex, std::string *code){

	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		expr_1(lex, code);
	}
	else if (lex.peek_tag() == ID){
		lex.pop();
	}
	else if (is_CONST(lex.peek_tag())){
		oper_return val;
		const_0(lex, &val);
	}
	else {
		//error
		return -1;
	}

	return 0;
}

int expr_1(Lexer &lex, std::string *code){
	if ((lex.peek_tag() == ASSIGN) || is_BINOP(lex.peek_tag()) || lex.peek_tag() == MINUS
		|| is_UNOP(lex.peek_tag())){
		oper_return val;
		oper_1(lex, &val);
	}
	else if ((lex.peek_tag() == IF) || (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET)){
		stmt_1(lex, code);
	} else if (lex.peek_tag() == STDOUT){
		s_1(lex, code);
	}

	return 0;
}

int oper(Lexer &lex, oper_return* synth){
	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		oper_1(lex, synth);
	}
	else if (lex.peek_tag() == ID){

		synth->type = CODE;
		synth->code = ((IdToken*)lex.peek())->get_id();

		lex.pop();
	}
	else if (is_CONST(lex.peek_tag())){
		const_0(lex, synth);
	}
	else {
		return -1;
	}

	return 0;
}

int oper_1(Lexer &lex, oper_return* synth){
	if (lex.peek_tag() == ASSIGN){
		lex.pop();
		if (lex.peek_tag() == ID){
			lex.pop();
			oper(lex);
			if (lex.peek_tag() != R_BRACKET){
				return -1;
			}
			else {
				lex.pop();
			}
		}
	}
	else if (is_BINOP(lex.peek_tag())){
		int bin_op;
		oper_return oper1;
		oper_return oper2;
		binop(lex, &bin_op);
		oper(lex, &oper1);
		oper(lex, &oper2);



		if (bin_op == PLUS){
			plus(oper1, oper2, synth);
		}
		


		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return -1;
		}
	}
	else if (is_UNOP(lex.peek_tag())){
		int un_op;
		oper_return oper1;

		unop(lex, &un_op);
		oper(lex, &oper1);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return -1;
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
}

int stmt(Lexer &lex, std::string *code){
	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		stmt_1(lex, code);
	}
	else {
		return -1;
	}
	return 0;
}

int stmt_1(Lexer &lex, std::string *code){
	if (lex.peek_tag() == IF){
		lex.pop();
		expr(lex, code);
		expr(lex, code);
		ifstmt(lex, code);
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
			varlist(lex);
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
		oper(lex);
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

	if (lex.peek_tag == STRING_L){
		synth->type = STRING;
		synth->str_value = ((StrToken*)lex.peek())->get_str();
		synth->code = ((StrToken*)lex.peek())->get_str();
	}
	else if (lex.peek_tag == INT_L){
		synth->type = INT;
		synth->str_value = ((IntToken*)lex.peek())->get_int();
		synth->code = ((IntToken*)lex.peek())->get_int();
	}
	else if (lex.peek_tag == REAL_L){
		synth->type = REAL;
		synth->str_value = ((RealToken*)lex.peek())->get_real();
		synth->code = ((RealToken*)lex.peek())->get_real();
	}
	else if (lex.peek_tag == TRUE){
		synth->type = BOOL;
		synth->int_value = 1;
		synth->code = "true";
	}
	else if (lex.peek_tag == FALSE){
		synth->type = BOOL;
		synth->int_value = 0;
		synth->code = "false";
	}
	else {
		return -1;
	}
	return 0;
}

int ifstmt(Lexer &lex){

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		expr(lex);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
			return 0;
		}
		else {
			return -1;
		}

	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else {
		return 0;
	}
}

int exprlist(Lexer &lex){
	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		expr(lex);
		exprlist_1(lex);
	}
	else {
		return -1;
	}

	return 0;
}

int exprlist_1(Lexer &lex){
	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		exprlist(lex);
	}
	else if (lex.peek_tag() == R_BRACKET){
		return 0;
	}
	else {
		return -1;
	}

	return 0;

}

int varlist(Lexer &lex){
	if (lex.peek_tag() == L_BRACKET){
		lex.pop();
		if (lex.peek_tag() == ID){
			lex.pop();
			type(lex);
			if (lex.peek_tag() == R_BRACKET){
				lex.pop();
				varlist_1(lex);
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

int varlist_1(Lexer &lex){
	if (lex.peek_tag() == R_BRACKET){
		lex.pop();
	}
	else if (lex.peek_tag() == L_BRACKET){
		varlist(lex);
	}
	else {
		return -1;
	}

	return 0;
}

int type(Lexer &lex){
	if (lex.peek_tag() == BOOL_T || lex.peek_tag() == INT_T ||
		lex.peek_tag() == REAL_T || lex.peek_tag() == STRING_T){
		lex.pop();
	}
	else {
		return -1;
	}

	return 0;
}

int negop(Lexer &lex){
	if (is_CONST(lex.peek_tag()) || lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID){
		oper(lex);
		if (lex.peek_tag() == R_BRACKET){
			lex.pop();
		}
		else {
			return - 1;
		}
	}
	else if (lex.peek_tag() == R_BRACKET){
		lex.pop();
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
		synth->code = oper1.int_value + oper2.int_value;
	}
	//addition and mixed int/float
	else if ((oper1.type == INT && oper2.type == REAL)){
		synth->real_value = (double)oper1.int_value + oper2.real_value;
		synth->type = REAL;
		synth->code = (double)oper1.int_value + oper2.real_value;
	}
	else if ((oper1.type == REAL && oper2.type == INT)){
		synth->real_value = (double)oper2.int_value + oper1.real_value;
		synth->type = REAL;
		synth->code = (double)oper2.int_value + oper1.real_value;
	}
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