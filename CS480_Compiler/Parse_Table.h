#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include <vector>
#include <queue>
#include <string>
#include <map>
#include "Token.h"


typedef struct _map_key{
	int prod_sym;
	int in_sym;
}map_key;

struct map_key_comparer
{
	bool operator()(const map_key & Left, const map_key & Right) const
	{
		if (Left.prod_sym == Right.prod_sym){
			return Left.in_sym < Right.in_sym;
		}
		else {
			return Left.prod_sym < Right.prod_sym;
		}
	}
};

//parse table rules
//each line corresponds to a rule in our parse table
static std::string parse_table_rules[] = {
	//Start Rules
	"NON_START:L_BRACKET:L_BRACKET,NON_S,R_BRACKET,",
	//S Rules
	"NON_S:L_BRACKET:L_BRACKET,NON_S_2,",
	"NON_S:ID:ID,NON_S_1,",
	"NON_S:CONST_ALL:NON_CONST,NON_S_1,",
	//S' Rules
	"NON_S_1:L_BRACKET:NON_S,NON_S_1,",
	"NON_S_1:ID:NON_S,NON_S_1,", 
	"NON_S_1:CONST_ALL:NON_S,NON_S_1,",
	"NON_S_1:R_BRACKET:EMPTY,",
	//S'' Rules 
	"NON_S_2:L_BRACKET:NON_S,R_BRACKET,NON_S_1,",
	"NON_S_2:R_BRACKET:R_BRACKET,NON_S_1,",
	"NON_S_2:BIN_ALL:NON_EXPR_1,NON_S_1,",
	"NON_S_2:UN_ALL:NON_EXPR_1,NON_S_1,",
	"NON_S_2:STDOUT:NON_EXPR_1,NON_S_1,",
	"NON_S_2:MINUS:NON_EXPR_1,NON_S_1,",
	"NON_S_2:ASSIGN:NON_EXPR_1,NON_S_1,",
	"NON_S_2:IF:NON_EXPR_1,NON_S_1,",
	"NON_S_2:WHILE:NON_EXPR_1,NON_S_1,",
	"NON_S_2:LET:NON_EXPR_1,NON_S_1,",
	"NON_S_2:ID:NON_S,R_BRACKET,NON_S_1,",
	"NON_S_2:CONST_ALL:NON_S,R_BRACKET,NON_S_1,",  
	//Expr Rules 
	"NON_EXPR:L_BRACKET:L_BRACKET,NON_EXPR_1,",
	"NON_EXPR:ID:ID,",
	"NON_EXPR:CONST_ALL:NON_CONST,",
	//Expr' Rules 
	"NON_EXPR_1:ASSIGN:NON_OPER_1,",
	"NON_EXPR_1:IF:NON_STMT_1,",
	"NON_EXPR_1:WHILE:NON_STMT_1,",
	"NON_EXPR_1:LET:NON_STMT_1,",
	"NON_EXPR_1:BIN_ALL:NON_OPER_1,",
	"NON_EXPR_1:MINUS:NON_STMT_1,",
	"NON_EXPR_1:UN_ALL:NON_OPER_1,",
	"NON_EXPR_1:STDOUT:NON_S_1,",
	//Oper Rules 
	"NON_OPER:L_BRACKET:L_BRACKET,NON_OPER_1,",
	"NON_OPER:ID:ID,",
	"NON_OPER:CONST_ALL:NON_CONST,",
	//Oper' Rules 
	"NON_OPER_1:ASSIGN:ASSIGN,ID,NON_OPER,R_BRACKET,",
	"NON_OPER_1:BIN_ALL:NON_BINOP,NON_OPER,NON_OPER,R_BRACKET,",
	"NON_OPER_1:UN_ALL:NON_UNOP,NON_OPER,R_BRACKET,",
	"NON_OPER_1:MINUS:MINUS,NON_OPER,NON_NEGOP,",
	//Stmt Rule 
	"NON_STMT:L_BRACKET:L_BRACKET,NON_STMT_1,",
	//Stmt' Rules 
	"NON_STMT_1:IF:IF,NON_EXPR,NON_EXPR,NON_IFSTMT,",
	"NON_STMT_1:WHILE:WHILE,NON_EXPR,NON_EXPRLIST,R_BRACKET,",
	"NON_STMT_1:LET:LET,L_BRACKET,NON_VARLIST,R_BRACKET,",
	"NON_STMT_1:STDOUT:STDOUT,NON_OPER,R_BRACKET,",
	//Binop rules
	"NON_BINOP:PLUS:PLUS,",
	"NON_BINOP:MULTI:MULTI,",
	"NON_BINOP:MOD:MOD,",
	"NON_BINOP:EXP:EXP,",
	"NON_BINOP:EQ:EQ,",
	"NON_BINOP:LT:LT,",
	"NON_BINOP:LE:LE,",
	"NON_BINOP:GT:GT,",
	"NON_BINOP:GE:GE,",
	"NON_BINOP:NE:NE,",
	"NON_BINOP:OR:OR,",
	"NON_BINOP:AND:AND,",
	//Unop rule
	"NON_UNOP:NOT:NOT,",
	"NON_UNOP:SIN:SIN,",
	"NON_UNOP:COS:COS,",
	"NON_UNOP:TAN:TAN,",
	//Const rules
	"NON_CONST:STRING_L:STRING_L,",
	"NON_CONST:INT_L:INT_L,",
	"NON_CONST:REAL_L:REAL_L,",
	"NON_CONST:TRUE:TRUE,",
	"NON_CONST:FALSE:FALSE,",
	//Ifstmt rules
	"NON_IFSTMT:L_BRACKET:NON_EXPR,L_BRACKET,",
	"NON_IFSTMT:ID:NON_EXPR,L_BRACKET,",
	"NON_IFSTMT:CONST_ALL:L_BRACKET,",
	"NON_IFSTMT:R_BRACKET:R_BRACKET,",
	//exprlist rules
	"NON_EXPRLIST:L_BRACKET:NON_EXPR,NON_EXPRLIST_1,",
	"NON_EXPRLIST:ID:NON_EXPR,NON_EXPRLIST_1,",
	"NON_EXPRLIST:CONST_ALL:NON_EXPR,NON_EXPRLIST_1,",
	//exprlist' rules
	"NON_EXPRLIST_1:L_BRACKET:NON_EXPRLIST,",
	"NON_EXPRLIST_1:R_BRACKET:EMPTY,",
	"NON_EXPRLIST_1:ID:NON_EXPRLIST,",
	"NON_EXPRLIST_1:CONST_ALL:NON_EXPRLIST,",
	//varlist rules
	"NON_VARLIST:L_BRACKET:L_BRACKET,ID,NON_TYPE,R_BRACKET,NON_VARLIST_1,",
	"NON_VARLIST:R_BRACKET:EMPTY,",
	"NON_VARLIST_1:L_BRACKET:NON_VARLIST,",
	//type rules
	"NON_TYPE:BOOL_T:BOOL_T,",
	"NON_TYPE:INT_T:INT_T,",
	"NON_TYPE:REAL_T:REAL_T,",
	"NON_TYPE:STRING_T:STRING_T,",
	//negop rules
	"NON_NEGOP:L_BRACKET:NON_OPER,R_BRACKET,",
	"NON_NEGOP:R_BRACKET:R_BRACKET,",
	"NON_NEGOP:CONST_ALL:NON_OPER,R_BRACKET,",
	"NON_NEGOP:ID:NON_OPER,R_BRACKET,",

};


int parse_table_setup(std::string prod, std::map<map_key, std::vector<int>, map_key_comparer> &parse_table);

void create_parse_table(std::map<map_key, std::vector<int>, map_key_comparer> &parse_table);


#endif // !PARSE_TABLE_H
