#include "Parser.h"


//Constructor
Parser::Parser(std::ifstream &_source, std::string _source_name) : source(_source), source_name(_source_name), lex(_source), table() {}


//beginning or our recursive desent 
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

		while (!errors.empty()){

			std::cout << errors.front().msg << std::endl;
			//std::cout << locate_err(errors.front().loc);
			errors.pop();
		}
	}

	if (fatal_error){
		return "Fatal error while parsing.";
	}
	else {
		return add.str();
	}

}

std::string Parser::start(){

	std::stringstream add;

	//we wrap our code in a function "main"
	add << ": main " << std::endl;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
	}
	else {
		error("Code did not begin with left bracket '['. Inserting left bracket to continue parse.");
	}

	//countiune recursive desent 
	add << s();

	if (lex.peek_tag() == R_BRACKET){
		pop_lex(add);
	}
	else {
		error("Code did not end with right bracket ']'. Inserting right bracket to continue parse.");
	}

	add << std::endl << ";" << std::endl << "main ";

	//add closing line to file
	add << std::endl << "bye" << std::endl;

	if (!lex.source_empty() && !fatal_error){
		error("Unprocessed input remaining after parse.");
	}

	return variables + "\n\n" +add.str();

}

std::string Parser::s(){

	if (fatal_error) return "";

	std::stringstream add;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
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
		
		table.increse_scope();

		add << s();

		table.decrese_scope();

		if (lex.peek_tag() == R_BRACKET){
			pop_lex(add);
		}
		else {
			error("Mismatched brackets. Adding Right bracket to continue parse.");
		}

		add << s_1();
	}
	else if (lex.peek_tag() == R_BRACKET){
		pop_lex(add);
		add <<  s_1();
	}
	else if (is_BINOP(lex.peek_tag()) || is_UNOP(lex.peek_tag()) 
		|| (lex.peek_tag() == MINUS) || (lex.peek_tag() == ASSIGN) || (lex.peek_tag() == IF)
		|| (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET) || (lex.peek_tag() == ID)){

		add << expr_1().attr;
		add << s_1();
	}
	else if (lex.peek_tag() == STDOUT){
		add << stmt_1().attr;
		add << s_1();
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	return add.str();
}

Parser::synth_return Parser::expr(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
		synth = expr_1();
		add << synth.attr;
	}
	else if (lex.peek_tag() == ID){
		synth = append_ID();
		add << synth.attr;
	}
	else if (is_CONST(lex.peek_tag())){
		synth = append_CONST();
		add << synth.attr;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::expr_1(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if ((lex.peek_tag() == ASSIGN) || is_BINOP(lex.peek_tag()) || lex.peek_tag() == MINUS
		|| is_UNOP(lex.peek_tag())){
		synth = oper_1();
		add << synth.attr;
	}
	else if ((lex.peek_tag() == IF) || (lex.peek_tag() == WHILE) || (lex.peek_tag() == LET)){
		synth = stmt_1();
		add << synth.attr;
	}
	else if (lex.peek_tag() == STDOUT){
		synth = stmt_1();
		add << synth.attr;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::oper(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
		synth = oper_1();
		add << synth.attr;
	}
	else if (lex.peek_tag() == ID){

		synth = append_ID();
		add << synth.attr;
	}
	else if (is_CONST(lex.peek_tag())){
		synth = const_0();
		add << synth.attr;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::oper_1(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == ASSIGN){
		pop_lex(add);

		synth_return id;
		synth_return oper_0;

		if (lex.peek_tag() == ID){
			id = append_ID_let();
			oper_0 = oper();

			if ((oper_0.type == INT && id.type == INT) || (oper_0.type == BOOL && id.type == BOOL)){
				add << oper_0.attr << id.attr << "! ";
		}
			else if (oper_0.type == INT && id.type == FLOAT){
				add << oper_0.attr << "s>f " <<  id.attr << "f! ";
	}
			else if (oper_0.type == FLOAT && id.type == FLOAT){
				add << oper_0.attr <<  id.attr << "f! ";
			}
			else if (oper_0.type == STRING && id.type == STRING){
				add << oper_0.attr << id.attr << "2! ";
			}
			else {
				error("Operand of type " + oper_to_string(oper_0.type) + " and variable of type " + oper_to_string(id.type) +
					" have no defintion for assignment.");
				fatal_error = true;
				return synth;
			}
		}
	}

	else if (is_BINOP(lex.peek_tag())){
		//get all the synth. attributes filled in
		int bin_op;
		synth_return oper1;
		synth_return oper2;
		bin_op = binop();
		pop_lex(add);
		oper1 = oper();
		oper2 = oper();

		if (is_bool_BINOP(bin_op)){
			if (oper1.type == BOOL && oper2.type == BOOL){
				synth.type = BOOL;
				add << oper1.attr << oper2.attr << Token::tag_to_string(bin_op) << " ";
			}
			else {
				error("Operands of type " + oper_to_string(oper1.type) + " and " + oper_to_string(oper2.type) +
					" have no defintion for operation " + Token::tag_to_string(bin_op));
				fatal_error = true;
				return synth;
			}
		}
		else{
			if (oper1.type == INT && oper2.type == INT && bin_op == EXP){
				synth.type = INT;
				add << oper1.attr << "s>f " << oper2.attr << "s>f " << "f** f>s ";
			}
			else if (oper1.type == STRING && oper2.type == STRING && bin_op == PLUS){
				synth.type = STRING;
				add << oper1.attr << "pad place " << oper2.attr << "pad +place pad count ";
			}
			else if (oper1.type == FLOAT && oper2.type == FLOAT){
				synth.type = is_log_BINOP(bin_op) ? BOOL : FLOAT;
				add << oper1.attr << oper2.attr << "f" << Token::tag_to_input(bin_op) << " ";
			}
			else if (oper1.type == FLOAT && oper2.type == INT){
				synth.type = is_log_BINOP(bin_op) ? BOOL : FLOAT;
				add << oper1.attr << oper2.attr << "s>f " << "f" << Token::tag_to_input(bin_op) << " ";
			}
			else if (oper1.type == INT && oper2.type == FLOAT){
				synth.type = is_log_BINOP(bin_op) ? BOOL : FLOAT;
				add << oper1.attr << "s>f " << oper2.attr << "f" << Token::tag_to_input(bin_op) << " ";
			}
			else if (oper1.type == INT && oper2.type == INT){
				synth.type = is_log_BINOP(bin_op) ? BOOL : INT;
				add << oper1.attr << oper2.attr << Token::tag_to_input(bin_op) << " ";
			}
			else {
				error("Operands of type " + oper_to_string(oper1.type) + " and " + oper_to_string(oper2.type) +
					" have no defintion for operation " + Token::tag_to_string(bin_op));
				fatal_error = true;
				return synth;
			}
		}
	}
	else if (is_UNOP(lex.peek_tag())){
		int un_op;
		synth_return oper1;

		un_op = unop();
		pop_lex(add);
		oper1 = oper();

		if (oper1.type == BOOL && un_op == NOT){
			synth.type = BOOL;
			add << oper1.attr << "invert ";
		}
		else if (oper1.type == INT){
			synth.type = FLOAT;
			add << oper1.attr << "s>f " << Token::tag_to_input(un_op) << " ";
		}
		else if (oper1.type == FLOAT){
			synth.type = FLOAT;
			add << oper1.attr << Token::tag_to_input(un_op) << " ";
		}

	}
	else if (lex.peek_tag() == MINUS){
		pop_lex(add);

		synth_return oper1 = oper();
		synth_return oper2 = negop();

		if (oper2.type == EMP){
			if (oper1.type == FLOAT){
				synth.type = FLOAT;
				add << oper1.attr << "fnegate ";
			}
			else if (oper1.type == INT){
				synth.type = INT;
				add << oper1.attr << "negate ";
			}
		}
		else {
			if (oper1.type == FLOAT && oper2.type == FLOAT){
				synth.type = FLOAT;
				add << oper1.attr << oper2.attr << "f- ";
			}
			else if (oper1.type == FLOAT && oper2.type == INT){
				synth.type = FLOAT;
				add << oper1.attr << oper2.attr << "s>f f- ";
			}
			else if (oper1.type == INT && oper2.type == FLOAT){
				synth.type = FLOAT;
				add << oper1.attr << "s>f " << oper2.attr << "f- ";
			}
			else if (oper1.type == INT && oper2.type == INT){
				synth.type = INT;
				add << oper1.attr << oper2.attr << "- ";
			}
		}
		
	} else {
		error("Syntax error.");
		fatal_error = true;
	}

	if (lex.peek_tag() == R_BRACKET){
		pop_lex(add);
	}
	else {
		error("Missing right bracket. Inserting to countine parse.");
	}

	synth.attr = add.str();
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

Parser::synth_return Parser::stmt(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
		synth = stmt_1();
		add << synth.attr;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::stmt_1(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == IF){
		pop_lex(add);

		synth = expr();
		if (synth.type != BOOL){
			error("First expr in a if must be of type BOOL.");
			fatal_error = true;
			return synth;
		}
		add << synth.attr;
		add << "if ";
		synth = expr();
		add << synth.attr;

		synth = ifstmt();
		if (synth.type != EMP){
			add << "else ";
			add << synth.attr;
		}
		add << "endif ";

	}
	else if (lex.peek_tag() == WHILE){
		pop_lex(add);

		synth = expr();
		if (synth.type != BOOL){
			error("First expr in a while must be of type BOOL.");
			fatal_error = true;
			return synth;
		}
		add << "begin " << synth.attr << "while ";

		table.increse_scope();
		synth = exprlist();
		table.decrese_scope();

		add << synth.attr << "repeat ";
	}
	else if (lex.peek_tag() == LET){
		pop_lex(add);

		if (lex.peek_tag() == L_BRACKET){
			pop_lex(add);
		}
		else {
			error("Missing left bracket. Inserting left bracket to continue parse.");
		}

		synth = varlist();
		add << synth.attr;

		if (lex.peek_tag() == R_BRACKET){
			pop_lex(add);
		}
		else {
			error("Missing right bracket. Inserting right bracket to continue parse.");
		}

	}
	else if (lex.peek_tag() == STDOUT){
		pop_lex(add);
		synth = oper();

		add << synth.attr;

		switch (synth.type){
		case STRING:
			add << "type ";
			break;
		case FLOAT:
			add << "f. ";
			break;
		case INT:
			add << ". ";
			break;
		case BOOL:
			add << ". ";
			break;
		default:
			break;

		}
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	//pop final right bracket
	if (lex.peek_tag() == R_BRACKET){
		pop_lex(add);
	}
	else {
		error("Missing right bracket. Inserting right bracket to continue parse.");
	}

	synth.attr = add.str();
	return synth;

}

int Parser::binop(){

	if (fatal_error) return -1;

	if (is_BINOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		return synth;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
		return -1;
	}

}

int Parser::unop(){

	std::stringstream add;

	if (is_UNOP(lex.peek_tag())){
		int synth = lex.peek_tag();
		return synth;
	}
	else {
		return -1;
	}
}

Parser::synth_return Parser::const_0(){

	synth_return synth;
	std::stringstream add;


	if (fatal_error) return synth;

	if (lex.peek_tag() == STRING_L){
		synth.type = STRING;
		add << "s\" " << ((StrToken*)lex.peek())->get_str() << "\" ";
	}
	else if (lex.peek_tag() == INT_L){
		synth.type = INT;
		add << ((IntToken*)lex.peek())->get_int() << " ";
	}
	else if (lex.peek_tag() == REAL_L){
		synth.type = FLOAT;
		double val = ((RealToken*)lex.peek())->get_real();

		add << std::fixed
			<< std::scientific;
		add.precision(std::numeric_limits<double>::digits10 + 1);
		add	<< val << " ";
	}
	else if (lex.peek_tag() == TRUE){
		synth.type = BOOL;
		add << "true ";
	}
	else if (lex.peek_tag() == FALSE){
		synth.type = BOOL;
		add << "false ";
	}
	else if (lex.peek_tag() == NEWLINE){
		synth.type = EXT;
		add << "cr ";
	}
	else {
		error("Syntax error."); 
			fatal_error = true;
	}

	pop_lex(add);
	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::ifstmt(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){

		synth = expr();
		add << synth.attr;

		if (lex.peek_tag() == R_BRACKET){
			;
		}
		else {
			error("Missing right bracket. Inserting right bracket to continue parsing.");
		}

	}
	else if (lex.peek_tag() == R_BRACKET){
		synth.type = EMP;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::exprlist(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;


	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		
		synth = expr();
		add << synth.attr;
		synth =  exprlist_1();
		add << synth.attr;

	}
	else {
		error("Syntax Error.");
		fatal_error = true;
	}

	synth.attr = add.str();
		return synth;

}

Parser::synth_return Parser::exprlist_1(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	
	if (lex.peek_tag() == L_BRACKET || lex.peek_tag() == ID || is_CONST(lex.peek_tag())){
		synth = exprlist();
		add << synth.attr;
	}
	else if (lex.peek_tag() == R_BRACKET){
		synth.type = EMP;
	}
	else {
		error("Syntax error.");
		fatal_error = true;
	}

	synth.attr = add.str();
	return synth;

}

Parser::synth_return Parser::varlist(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == L_BRACKET){
		pop_lex(add);
	}
	else {
		error("Missing left bracket. Inserting left bracket to continue parsing.");
	}

	var_info var;
	synth_return type_0;

	//get name spedificly here

	if (lex.peek_tag() == ID){

		std::string id = ((IdToken*)lex.peek())->get_id();
		pop_lex(add);
		type_0 = type();

		var = table.insert_symbol(id, type_0.type);

		if (var.type == USD){
			error("Redefinition of variable " + id + " to different basic type.");
			fatal_error = true;
			return synth;
		}

		switch (type_0.type){
		case BOOL:
		case INT:
			add << "variable " << var.name << " ";
			break;
		case FLOAT:
			add << "fvariable " << var.name << " ";
			break;
		case STRING:
			add << "2variable " << var.name << " ";
			break;
		}

	}
	else {
		error("Syntax Error");
		fatal_error = true;
	}



	if (lex.peek_tag() == R_BRACKET){
		pop_lex(add);
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}


	add << varlist_1().attr;

	variables += add.str();
	synth.attr = "";
	return synth;

}

Parser::synth_return Parser::varlist_1(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;

	if (lex.peek_tag() == R_BRACKET){
		return synth;
	}
	else if (lex.peek_tag() == L_BRACKET){
		return varlist();
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}

	return synth;

}

Parser::synth_return Parser::type(){

	synth_return synth;
	std::stringstream add;

	if (fatal_error) return synth;


	switch (lex.peek_tag()){
	case BOOL_T:
		synth.type = BOOL;
		pop_lex(add);
		break;
	case INT_T:
		synth.type = INT;
		pop_lex(add);
		break;
	case REAL_T:
		synth.type = FLOAT;
		pop_lex(add);
		break;
	case STRING_T:
		synth.type = STRING;
		pop_lex(add);
		break;
	default:
		error("Syntax_error.");
		fatal_error = true;
		break;
	}

	synth.attr = add.str();
	return synth;
}

Parser::synth_return Parser::append_ID_let(){

	std::stringstream add;
	synth_return synth;
	var_info result;

	if (lex.peek_tag() == ID){

		std::string id = ((IdToken*)lex.peek())->get_id();

		result = table.find_symbol(id);

		if (result.type == EMP){
			error("Undeclared Variable " + id + ".");
			fatal_error = true;
			return synth;
		}

		add << result.name + " ";
		pop_lex(add);
		synth.attr = add.str();
		synth.type = result.type;

		return synth;
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
		return synth;
	}
}

Parser::synth_return Parser::append_ID(){

	std::stringstream add;
	synth_return synth;

	if (lex.peek_tag() == ID){

		synth = append_ID_let();
		add << synth.attr;

		switch (synth.type){
		case BOOL:
		case INT:
			add << "@ ";
			break;
		case STRING:
			add << "2@ ";
			break;
		case FLOAT:
			add << "f@ ";
			break;
		}

		synth.attr = add.str();
		return synth;
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
		return synth;
	}
}

Parser::synth_return Parser::append_CONST(){

	synth_return synth;

	if (is_CONST(lex.peek_tag())){
		return const_0();
	}
	else {
		error("Syntax_error.");
		fatal_error = true;
	}
	return synth;
	
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


void Parser::pop_lex(std::stringstream &str){
	lex.pop();

	if (lex.peek_tag() == LINE_END){
		str << std::endl;
	}

	while (lex.peek_tag() == LINE_END){
		lex.pop();
	}
}


bool Parser::is_CONST(int val){
	return (val == INT_L) || (val == REAL_L) || (val == STRING_L)
		|| (val == TRUE) || (val == FALSE) || (val == NEWLINE);
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

std::string Parser::oper_to_string(oper_type op){

	switch (op){
	case INT:
		return "int";
	case FLOAT:
		return "real";
	case STRING:
		return "string";
	case BOOL:
		return "bool";
	case EMP:
		return "empty";
	case ERROR:
		return "error";
	default:
		return "unkown";

	}

}