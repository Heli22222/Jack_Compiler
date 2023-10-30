#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "symbols.h"

#define ifTrueLabel "IF_TRUE"
#define ifFalseLabel "IF_FALSE"
#define ifEndLabel "IF_END"
#define whileLabel "WHILE_EXP"
#define whileEndLabel  "WHILE_END"

ParserInfo pas;
Token tk;
Symbol s;
int iserror;
int if_number;
int while_number;
extern Symbol table_of_class[500];
extern Symbol table_of_method[100];
extern int class_table_number;
 int method_table_number;
extern int var_number;
extern int arg_number;
extern int field_number;
extern int static_number;
extern int parse_time;
Token error_token;
char current_class_name[20];
extern char project_name[20];
FILE* file_out_vm = NULL;
SubroutineInformation sub_info;
// you can declare prototypes of parser functions below
void errSort(SyntaxErrors syn);
void class();
void memberDeclar();
void classVarDeclar();
void subroutineDeclar();
void type();
int paramList(char* function_type);
void subroutineBody();
void statement();
void varDeclarStatement();
void letStatemnt();
void ifStatement();
void whileStatement();
void doStatement();
void subroutineCall();
int expressionList();
void retrunStatemnt();
void expression();
void relationalExpression();
void ArithmeticExpression();
void term();
void factor();
void operand();

void errSort(SyntaxErrors syn) {
	if (iserror == 0) {
		pas.er = syn;

		pas.tk = tk;
		iserror = 1;
	}
	else if (iserror == 1) {}
	return;
}
void get_next_token() {
	tk = GetNextToken();
	if (tk.tp == ERR) {
		
		errSort(lexerErr);
		return;
	}
}

void peek_next_token() {
	tk = PeekNextToken();
	if (tk.tp == ERR) {
		
		errSort(lexerErr);
		tk = GetNextToken();
		return;
	}
}

void class_start() {
	get_next_token();
	if (!(strcmp(tk.lx, "class"))) {}
	else { errSort(classExpected); return; }
	get_next_token();
	if (tk.tp == ID) { 
		strcpy(current_class_name, tk.lx); 
		while_number = 0;
		if_number = 0;
	    init_file_out(project_name, current_class_name);
		clear_global_number(); 
	}
	else { errSort(idExpected); return; }
	peek_next_token();
	if (!(strcmp(tk.lx, "{"))) {}
	else {
		error_token = tk;
		errSort(openBraceExpected);
		return;
	}
	get_next_token();
	while (tk.tp != EOF && iserror == 0 && (strcmp(tk.lx, "}"))) {
		memberDeclar();
		peek_next_token();
	}
	if (!(strcmp(tk.lx, "}"))) {}
	else { errSort(closeBraceExpected); }
	get_next_token();
	return;
}

void memberDeclar() {
	peek_next_token();
	if ((!(strcmp(tk.lx, "static"))) || (!(strcmp(tk.lx, "field")))) {
		classVarDeclar();
	}
	else if ((!(strcmp(tk.lx, "constructor"))) || (!(strcmp(tk.lx, "function"))) || (!(strcmp(tk.lx, "method")))) {
		subroutineDeclar();
	}
	else { errSort(memberDeclarErr); return; }
	return;
}

void classVarDeclar() {
	if (parse_time == 1) { 
		peek_next_token();
		strcpy(s.class_name, current_class_name);
		if ((!(strcmp(tk.lx, "static")))) {
			strcpy(s.symbol_kind, "static");
			s.offset = static_number++;
		}
		else if ((!(strcmp(tk.lx, "field")))) {
			strcpy(s.symbol_kind, "field");
			s.offset = field_number++;
		}
		else {
			error_token = tk;
			errSort(classVarErr); return;
		}
		get_next_token();
		type();
		peek_next_token();
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if ((LookUP_class_table(s,"static"))||
				(LookUP_class_table(s, "field"))) {
				error_token = tk; errSort(redecIdentifier); return; }
			else Add_class_symbol(s);
		}
		else { error_token = tk; errSort(idExpected); return; }
		get_next_token();
		peek_next_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_next_token();
			get_next_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_name, tk.lx);
				if ((LookUP_class_table(s, "static")) ||
					(LookUP_class_table(s, "field"))) {
					error_token = tk; errSort(redecIdentifier); return; }
				if ((!(strcmp(s.symbol_kind, "static")))) { s.offset = static_number++; }
				else if ((!(strcmp(s.symbol_kind, "field")))) { s.offset = field_number++; }
				Add_class_symbol(s);
			}
			else { error_token = tk; errSort(idExpected); return; }
			peek_next_token();
		}
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error_token = tk; errSort(semicolonExpected); return; }
		get_next_token();
	}
	else {
		get_next_token();
		type();
		get_next_token();
		peek_next_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_next_token();
			get_next_token();
			peek_next_token();
		}
		get_next_token();
	}
}


void type() {
	if (parse_time == 1) {
		peek_next_token();
		if ((!(strcmp(tk.lx, "int")))) { strcpy(s.symbol_type, "int"); }
		else if ((!(strcmp(tk.lx, "char")))) { strcpy(s.symbol_type, "char"); }
		else if ((!(strcmp(tk.lx, "boolean")))) { strcpy(s.symbol_type, "boolean"); }
		else if (tk.tp == ID) { strcpy(s.symbol_type, tk.lx); }
		else { error_token = tk; errSort(illegalType); return; }
		get_next_token();
	}
	else {
		peek_next_token();
		if ((!(strcmp(tk.lx, "int")))) { strcpy(s.symbol_type, "int"); }
		else if ((!(strcmp(tk.lx, "char")))) { strcpy(s.symbol_type, "char"); }
		else if ((!(strcmp(tk.lx, "boolean")))) { strcpy(s.symbol_type, "boolean"); }
		else if (tk.tp == ID) { 
			if (isClassName(tk.lx)) { strcpy(s.symbol_type, tk.lx); }
			else { errSort(undecIdentifier); return; }
		}
		else { error_token = tk; errSort(illegalType); return; }
		get_next_token();
	}
}

void subroutineDeclar() { 
	if (parse_time == 1) {
		clear_method_table();
		get_next_token();
		strcpy(s.class_name, current_class_name);
		if (!(strcmp(tk.lx, "constructor"))) { strcpy(s.symbol_kind, "constructor"); }
		else if ((!(strcmp(tk.lx, "function")))) { strcpy(s.symbol_kind, "function"); }
		else if ((!(strcmp(tk.lx, "method")))) { strcpy(s.symbol_kind, "method"); }
		else { error_token = tk; errSort(memberDeclarErr);  return; }
		get_next_token();
		if (!(strcmp(tk.lx, "int"))) {
			strcpy(s.symbol_type, "int");
		}
		else if (!(strcmp(tk.lx, "char"))) { strcpy(s.symbol_type, "char"); }

		else if (!(strcmp(tk.lx, "boolean"))) { strcpy(s.symbol_type, "boolean"); }

		else if (tk.tp == ID) { strcpy(s.symbol_type,tk.lx); }

		else if (!(strcmp(tk.lx, "void"))) { strcpy(s.symbol_type, "void"); }

		else { error_token = tk; errSort(illegalType); return; }
		get_next_token();
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if ((LookUP_class_table(s,"function"))||
				(LookUP_class_table(s, "method"))) {
				error_token = tk;
				errSort(redecIdentifier);
				return;
			}
			else Add_class_symbol(s);
		}
		else { error_token = tk; errSort(idExpected); return; }
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { error_token = tk; errSort(openParenExpected); return; }
		paramList("none");
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error_token = tk; errSort(closeParenExpected); return; }
		subroutineBody();
		return;
	}
	else if(parse_time==2){
		clear_method_table();
		get_next_token(); // "constructor" "function"  "method"
		strcpy(s.class_name, current_class_name); 
		get_next_token();
		get_next_token();
		get_next_token();
		paramList("none");
		get_next_token();
		subroutineBody();
		return;
	}
	else { 
		clear_method_table();
		get_next_token();
		strcpy(s.class_name, current_class_name);
		if (!(strcmp(tk.lx, "constructor"))) { strcpy(sub_info.subroutine_type, "constructor"); }
		else if ((!(strcmp(tk.lx, "function")))) { strcpy(sub_info.subroutine_type, "function"); }
		else if ((!(strcmp(tk.lx, "method")))) { strcpy(sub_info.subroutine_type, "method"); }
		get_next_token();
		if (!(strcmp(tk.lx, "int"))) {}
		else if (!(strcmp(tk.lx, "char"))) {}
		else if (!(strcmp(tk.lx, "boolean"))) {}
		else if (tk.tp == ID) {}
		else if (!(strcmp(tk.lx, "void"))) {}
		get_next_token();
		strcpy(sub_info.subroutine_name, tk.lx); 
		get_next_token();
		sub_info.local_number = paramList(sub_info.subroutine_type);
		sub_info.field_number = find_feild_number(current_class_name);
		get_next_token();
		if (!(strcmp(sub_info.subroutine_type, "constructor"))) { write_function(current_class_name, sub_info.subroutine_name, 0); }
		else { write_function(current_class_name, sub_info.subroutine_name, sub_info.local_number); }
		if (!(strcmp(sub_info.subroutine_type, "constructor"))) write_object_alloc(sub_info.field_number);
		else if (!(strcmp(sub_info.subroutine_type, "method"))) { write_push("argument", 0); write_pop("pointer",0); }
		subroutineBody();
		return;
	}
}

int paramList(char* function_type) {
	int parameter_number = 0;
	if (parse_time == 2) {
		peek_next_token();
		if ((!(strcmp(tk.lx, "int")))
			|| (!(strcmp(tk.lx, "char")))
			|| (!(strcmp(tk.lx, "boolean")))
			|| tk.tp == ID) {
			type();
			get_next_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_kind, "argument");
				strcpy(s.symbol_name, tk.lx);
				if (LookUP_method_table(s)) { 
					errSort(redecIdentifier); 
					return; 
				}
				else {
					if ((!(strcmp(function_type, "method")))) {parameter_number++; }
					s.offset = parameter_number++;
					Add_method_symbol(s);
				}
			}
			else { errSort(idExpected); return; }
			peek_next_token();
			while ((!(strcmp(tk.lx, ",")))) {
				get_next_token();
				type();
				get_next_token();
				if (tk.tp == ID) {
					strcpy(s.symbol_name, tk.lx);
					if (LookUP_method_table(s)) {
						errSort(redecIdentifier);
						return;
					}
					else {
						s.offset = parameter_number++;
						Add_method_symbol(s);
					}
				}
				else { errSort(idExpected); return; }
				peek_next_token();
			}
		}
		else {} 
	}
	else {
		peek_next_token();
		if ((!(strcmp(tk.lx, "int")))
			|| (!(strcmp(tk.lx, "char")))
			|| (!(strcmp(tk.lx, "boolean")))
			|| tk.tp == ID) {
			type();
			get_next_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_kind, "argument");
				strcpy(s.symbol_name, tk.lx);
				if (LookUP_method_table(s)) {
					errSort(redecIdentifier);
					return;
				}
				else {
					if ((!(strcmp(function_type, "method")))) { parameter_number++; }
					s.offset = parameter_number++;

					Add_method_symbol(s);
				}
				
			}
			else { errSort(idExpected); return; }
			peek_next_token();
			while ((!(strcmp(tk.lx, ",")))) {
				get_next_token();
				type();
				get_next_token();
				if (tk.tp == ID) {
					strcpy(s.symbol_name, tk.lx);
					if (LookUP_method_table(s)) {
						errSort(redecIdentifier);
						return 0;
					}
					else {
						s.offset = parameter_number++;
						Add_method_symbol(s);
					}
				}
					//parameter_number++; }//消耗 ID
				else { errSort(idExpected); return; }
				peek_next_token();
			}
			return parameter_number;
		}
		else { return parameter_number; }
	}
	return parameter_number;
}

void subroutineBody() {
	get_next_token();
	if ((!(strcmp(tk.lx, "{")))) {}
	else { errSort(openBraceExpected); return; }
	peek_next_token();
	while ((!(strcmp(tk.lx, "var"))) || (!(strcmp(tk.lx, "let"))) || (!(strcmp(tk.lx, "if"))) || (!(strcmp(tk.lx, "while"))) || (!(strcmp(tk.lx, "return"))) || (!(strcmp(tk.lx, "do")))) {
		
		statement();
		peek_next_token();
	}

	if ((!(strcmp(tk.lx, "}")))) { get_next_token(); }
	else { errSort(closeBraceExpected);  return; }
}

void statement() {
	if ((!(strcmp(tk.lx, "var")))) varDeclarStatement();
	else if ((!(strcmp(tk.lx, "let")))) letStatemnt();
	else if ((!(strcmp(tk.lx, "if")))) ifStatement();
	else if ((!(strcmp(tk.lx, "while")))) whileStatement();
	else if ((!(strcmp(tk.lx, "return")))) retrunStatemnt();
	else if ((!(strcmp(tk.lx, "do")))) doStatement();
	else { errSort(syntaxError); return; }
}

void varDeclarStatement() {
	if (parse_time == 2 || parse_time==3) {
		get_next_token();
		type();
		strcpy(s.symbol_kind, "variable");
		get_next_token();
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if (LookUP_method_table(s)) {
				errSort(redecIdentifier);
				return;
			}
			else {
				s.offset = var_number++;
				Add_method_symbol(s);
			}
		}
		else { errSort(idExpected); return; }
		peek_next_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_next_token();
			get_next_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_name, tk.lx);
				if (LookUP_method_table(s)) {
					errSort(redecIdentifier);
					return;
				}
				else {
					s.offset = var_number++;
					Add_method_symbol(s);
				}
			}
			else { errSort(idExpected); return; }
			peek_next_token();
		}
		get_next_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { errSort(semicolonExpected); return; }
	}
	else {
		get_next_token();
		type();
		get_next_token();
		if (tk.tp == ID) {}
		else { errSort(idExpected); return; }
		peek_next_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_next_token();
			get_next_token();
			if (tk.tp == ID) {}
			else { errSort(idExpected); return; }
			peek_next_token();
		}
		get_next_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { errSort(semicolonExpected); return; }
	}
}

void letStatemnt() {
	Token id;
	get_next_token();
	get_next_token();
	id = tk;
	if (tk.tp == ID) {}
	else { errSort(idExpected); return; }
	if (parse_time == 1) {
		peek_next_token();
		if ((!(strcmp(tk.lx, "[")))) {
			get_next_token();
			expression();
			get_next_token();
			if ((!(strcmp(tk.lx, "]")))) {}
			else { errSort(closeBracketExpected); return; }
		}
		else {}
		get_next_token();
		if ((!(strcmp(tk.lx, "=")))) {}
		else { errSort(equalExpected); return; }
		expression();
		get_next_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { errSort(semicolonExpected); return; }
	}
	else if(parse_time==3) {
		peek_next_token();
		if ((!(strcmp(tk.lx, "[")))) {
			get_next_token();
			expression();
			get_next_token();
			if ((!(strcmp(tk.lx, "]")))) {}
			else { errSort(closeBracketExpected); return; }
			write_variable(id.lx,0);
			write_arithmetic("add");
			get_next_token();
			if ((!(strcmp(tk.lx, "=")))) {}
			else { errSort(equalExpected); return; }
			expression();
			write_pop("temp",0);
			write_pop("pointer", 1);
			write_push("temp", 0);
			write_pop("that", 0);
			get_next_token();
			if ((!(strcmp(tk.lx, ";")))) {}
			else { errSort(semicolonExpected); return; }
		}
		else if((!(strcmp(tk.lx, "=")))) {
			get_next_token();
			expression();
			write_variable(id.lx,1);
			get_next_token();
			if ((!(strcmp(tk.lx, ";")))) {}
			else { errSort(semicolonExpected); return; }
		}
	}
	else {
		strcpy(s.symbol_name,tk.lx);
		memset(s.class_name,0,sizeof(s.class_name));
		strcpy(s.class_name, current_class_name);
		if (LookUP_method_table(s)) {}
		else if (LookUP_class_table(s, "field") || LookUP_class_table(s, "static")) {}
		else { errSort(undecIdentifier); return; }
		peek_next_token();
		if ((!(strcmp(tk.lx, "[")))) {
			get_next_token();
			expression();
			get_next_token();
			if ((!(strcmp(tk.lx, "]")))) {}
			else { errSort(closeBracketExpected); return; }
		}
		else {}
		get_next_token();
		if ((!(strcmp(tk.lx, "=")))) {}
		else { errSort(equalExpected); return; }
		expression();
		get_next_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { errSort(semicolonExpected); return; }
	}
}

void ifStatement() {
	if (parse_time == 3) {
		int index = if_number++;
		get_next_token();
		get_next_token();
		expression();
		get_next_token();
		write_if(ifTrueLabel,index);
		write_goto(ifFalseLabel,index);
		get_next_token();
		peek_next_token();
		while ((!(strcmp(tk.lx, "var")))
			|| (!(strcmp(tk.lx, "let")))
			//|| (!(strcmp(tk.lx, "if")))
			|| (!(strcmp(tk.lx, "while")))
			|| (!(strcmp(tk.lx, "return")))
			//|| (!(strcmp(tk.lx, "do")))) 
){
		
			write_label(ifTrueLabel, index);
			statement();
			peek_next_token();
		}
		if ((!(strcmp(tk.lx, "}")))) { get_next_token(); }
		else { errSort(closeBraceExpected); return; }
		peek_next_token();
		if ((!(strcmp(tk.lx, "else")))) {
			get_next_token();
			write_goto(ifEndLabel,index);
			write_label(ifFalseLabel,index);
			get_next_token();
			if ((!(strcmp(tk.lx, "{")))) {}
			else { errSort(openBraceExpected); return; }
			peek_next_token();
			while ((!(strcmp(tk.lx, "var")))
				|| (!(strcmp(tk.lx, "let")))
				|| (!(strcmp(tk.lx, "if")))
				|| (!(strcmp(tk.lx, "while")))
				|| (!(strcmp(tk.lx, "return")))
				|| (!(strcmp(tk.lx, "do")))) {
				
				statement();
				write_label(ifEndLabel,index);
				peek_next_token();
			}
			get_next_token();//“}”
		}
		else { write_label(ifFalseLabel,index); }
	}
	else {
		get_next_token();
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { errSort(openParenExpected); return; }
		expression();
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
		get_next_token();
		if ((!(strcmp(tk.lx, "{")))) {}
		else { errSort(openBraceExpected); return; }
		peek_next_token();
		while ((!(strcmp(tk.lx, "var")))
			|| (!(strcmp(tk.lx, "let")))
			|| (!(strcmp(tk.lx, "if")))
			|| (!(strcmp(tk.lx, "while")))
			|| (!(strcmp(tk.lx, "return")))
			|| (!(strcmp(tk.lx, "do")))) {
			
			statement();
			peek_next_token();
		}
		if ((!(strcmp(tk.lx, "}")))) { get_next_token(); }
		else { errSort(closeBraceExpected); return; }
		peek_next_token();
		if ((!(strcmp(tk.lx, "else")))) {
			get_next_token();
			get_next_token();
			if ((!(strcmp(tk.lx, "{")))) {}
			else { errSort(openBraceExpected); return; }
			peek_next_token();
			while ((!(strcmp(tk.lx, "var")))
				|| (!(strcmp(tk.lx, "let")))
				|| (!(strcmp(tk.lx, "if")))
				|| (!(strcmp(tk.lx, "while")))
				|| (!(strcmp(tk.lx, "return")))
				|| (!(strcmp(tk.lx, "do")))) {
				
				statement();
				peek_next_token();
			}
			get_next_token();
			if ((!(strcmp(tk.lx, "}")))) {}
			else { errSort(closeBraceExpected); return; }
		}
		else {}
	}	
}

void whileStatement() {
	if (parse_time == 3) {
		int index = while_number++;
		write_label(whileLabel,index);
		get_next_token();
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { errSort(openParenExpected); return; }
		expression();
		write_arithmetic("not");
		write_label(whileEndLabel,index);
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
		get_next_token();
		if ((!(strcmp(tk.lx, "{")))) {}
		else { errSort(openBraceExpected); return; }
		peek_next_token();
		while ((!(strcmp(tk.lx, "var"))) || (!(strcmp(tk.lx, "let"))) || (!(strcmp(tk.lx, "if"))) || (!(strcmp(tk.lx, "while"))) || (!(strcmp(tk.lx, "return"))) || (!(strcmp(tk.lx, "do")))) {
			
			statement();
			peek_next_token();
		}
		write_goto(whileLabel,index);
		write_label(whileEndLabel,index);
		get_next_token();
		if ((!(strcmp(tk.lx, "}")))) {}
		else { errSort(closeBraceExpected); return; }
	}
	else {
		get_next_token();
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { errSort(openParenExpected); return; }
		expression();
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
		get_next_token();
		if ((!(strcmp(tk.lx, "{")))) {}
		else { errSort(openBraceExpected); return; }
		peek_next_token();
		while ((!(strcmp(tk.lx, "var"))) || (!(strcmp(tk.lx, "let"))) || (!(strcmp(tk.lx, "if"))) || (!(strcmp(tk.lx, "while"))) || (!(strcmp(tk.lx, "return"))) || (!(strcmp(tk.lx, "do")))) {
			
			statement();
			peek_next_token();
		}
		get_next_token();
		if ((!(strcmp(tk.lx, "}")))) {}
		else { errSort(closeBraceExpected); return; }
	}
}

void doStatement() {
	get_next_token();
	peek_next_token();
	if (tk.tp == ID) {}
	else { errSort(idExpected); return; }
	subroutineCall();
	get_next_token();
	if ((!(strcmp(tk.lx, ";")))) {}
	else { errSort(semicolonExpected); return; }
	if (parse_time == 3) {
		write_pop("temp",0);
	}
}

void subroutineCall() {
	if (parse_time == 2) { 
		get_next_token();
		Token id1 = tk;
		Token id2; 
		peek_next_token();
		char* object_name=NULL;
		if ((!(strcmp(tk.lx, ".")))) {
			strcpy(s.symbol_name, id1.lx);
		    if (isLibrary(id1.lx)) {
			strcpy(s.class_name, id1.lx);
			get_next_token();
			get_next_token();
			id2 = tk;
			if (tk.tp == ID) {}
			else { errSort(idExpected); return; }
			strcpy(s.symbol_name, id2.lx);
			if ((LookUP_class_table(s, "function")) ||
				(LookUP_class_table(s, "constructor"))) {
			}
			else { errSort(undecIdentifier); return; }
		}
			else  if (LookUP_method_table(s)) {
				char object_name[20];
				strcpy(object_name,find_object_in_method_table(id1.lx));
				if (!(strcmp(object_name, "none"))) { 
					strcpy(s.symbol_name, id1.lx);
					error_token = id1;
					errSort(undecIdentifier); return; }
				if (isLibrary(object_name)) {
					strcpy(s.class_name, object_name);
					get_next_token();
					get_next_token();
					id2 = tk;
					if (tk.tp == ID) {}
					else { errSort(idExpected); return; }
					strcpy(s.symbol_name, id2.lx);
					if ((LookUP_class_table(s, "method"))) {
					}
					else { errSort(undecIdentifier); return; }
				}
				else {
					memset(s.class_name, 0, sizeof(s.class_name));
					//sprintf(s.class_name, "%s%s%s%s%s", "./", project_name, "/", object_name, ".jack");//   /.Pong/PongGame.jack
					sprintf(s.class_name, "%s", object_name);
					get_next_token();
					get_next_token();
					id2 = tk;
					if (tk.tp == ID) {}
					else { errSort(idExpected); return; }
					strcpy(s.symbol_name, id2.lx);
					
					if (LookUP_class_table(s, "method")) {}
					else { errSort(undecIdentifier); return; }
				}
			}
			
			else {
				get_next_token();//消耗 “.”
				get_next_token();//消耗 ID2
				if (tk.tp == ID) {}
				else { errSort(idExpected); return; }
				id2 = tk;
				if ((!(strcmp(find_kind_in_class_table(id2.lx), "method")))) {//第二种情况
					char object_name1[20];
					strcpy(object_name1, find_object_in_class_table(id1.lx));
					if ((!(strcmp(object_name1, "none")))) {
						tk = id1;
						strcpy(s.symbol_name, id1.lx);
						error_token = id1;
						errSort(undecIdentifier); return;
					}
					memset(s.class_name, 0, sizeof(s.class_name));
					//sprintf(s.class_name, "%s%s%s%s%s", "./", project_name, "/", object_name1, ".jack");//   /.Pong/PongGame.jack
					sprintf(s.class_name, "%s", object_name1);
					strcpy(s.symbol_name, id2.lx);
					if (LookUP_class_table(s, "method")) {}
				}
				else if ((!(strcmp(find_kind_in_class_table(id2.lx), "function"))) ||
					(!(strcmp(find_kind_in_class_table(id2.lx), "constructor")))) {
					//sprintf(s.class_name, "%s%s%s%s%s", "./", project_name, "/", id1.lx, ".jack");//.Pong/PongGame.jack
					sprintf(s.class_name, "%s", id1.lx);
					strcpy(s.symbol_name, id2.lx);
					if ((LookUP_class_table(s, "function")) ||
						(LookUP_class_table(s, "constructor"))) {
					}
					else { errSort(undecIdentifier); return; }
				}
				else {
					tk = id2;
					error_token = id2;
					errSort(undecIdentifier); return;
				}
			}
		}
		else {
			strcpy(s.symbol_name, id1.lx);
			strcpy(s.class_name, current_class_name);
			strcpy(s.symbol_type, "method");
			if (LookUP_class_table(s,"method")) {}
			else { tk = id1; errSort(undecIdentifier); return; }
		}
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { errSort(openParenExpected); return; }
		expressionList();
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
	}
	else if (parse_time == 3) {
	get_next_token();//ID1
	Token id1 = tk;
	Token id2 = tk;
	int arg_num = 0;
	peek_next_token();
	if ((!(strcmp(tk.lx, ".")))) {
		get_next_token();
		get_next_token();
		id2 = tk;
		get_next_token();
		if (isClassName(id1.lx)) {
			arg_num = expressionList();
			write_call(id1.lx,id2.lx,arg_num);
		}
		else {
			write_variable(id1.lx,0);
			arg_num = expressionList();
			arg_num++;
			write_call(find_object_in_class_table(id1.lx),id2.lx,arg_num);
		}
		get_next_token();//")"
		return;
	}
	else {
		write_push("pointer", 0);
		get_next_token();//"("
		arg_num = expressionList();
		arg_num++;
		write_call(current_class_name,id1.lx,arg_num);
		get_next_token();//")"
		return;
	}
	}
	else {
		get_next_token();
		peek_next_token();
		if ((!(strcmp(tk.lx, ".")))) {
			get_next_token();
			get_next_token();
			if (tk.tp == ID) {}
			else { errSort(idExpected); return; }
		}
		get_next_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { errSort(openParenExpected); return; }
		expressionList();
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
	}
}

int expressionList() {
	int number = 0;
	peek_next_token();
	if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~"))) || tk.tp == ID || tk.tp == INT || (!(strcmp(tk.lx, "("))) || tk.tp == STRING || (!(strcmp(tk.lx, "true")))
		|| (!(strcmp(tk.lx, "false"))) || (!(strcmp(tk.lx, "null"))) || (!(strcmp(tk.lx, "this")))) {
		expression();
		peek_next_token();
		number++;
		while ((!(strcmp(tk.lx, ",")))) {
			get_next_token();
			expression();
			peek_next_token();
			number++;
		}
	}
	else {}
	return number;
}

void retrunStatemnt() {
	if (parse_time == 3) {
		get_next_token();//"return"
		peek_next_token(); 
		if ((!(strcmp(tk.lx, ";")))) {
			get_next_token();
			write_push("constant",0);
			write_return();
		}
		else if ((!(strcmp(tk.lx, "this")))) {
			expression();
			get_next_token();
			write_return();
		}
		else {
			expression();
			get_next_token();
			write_return(); 
		}
	}
	else {
		get_next_token();
		if ((!(strcmp(tk.lx, "return")))) {}
		else { errSort(syntaxError); return; }
		peek_next_token();
		if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~"))) || tk.tp == ID || tk.tp == INT || (!(strcmp(tk.lx, "("))) || tk.tp == STRING || (!(strcmp(tk.lx, "true")))
			|| (!(strcmp(tk.lx, "false"))) || (!(strcmp(tk.lx, "null"))) || (!(strcmp(tk.lx, "this")))) {
			expression();
		}
		else {}
		get_next_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { errSort(semicolonExpected); return; }
	}
}

void expression() {
	char c[5];
	int flag = 0;
	relationalExpression();
	peek_next_token();
	while ((!(strcmp(tk.lx, "&"))) || (!(strcmp(tk.lx, "|")))) {
		get_next_token();
		strcpy(c, tk.lx); flag = 1;
		relationalExpression();
		peek_next_token();
	}
	//get_token()
	if (flag && parse_time == 3) write_operator_keyword_constant(c);
}

void relationalExpression() {
	char c[5];
	int flag = 0;
	ArithmeticExpression();
	peek_next_token();
	while ((!(strcmp(tk.lx, "="))) || (!(strcmp(tk.lx, "<"))) || (!(strcmp(tk.lx, ">")))) {
		get_next_token();
		strcpy(c, tk.lx); flag = 1;
		ArithmeticExpression();
		peek_next_token(); 
	}
	if (flag && parse_time == 3) write_operator_keyword_constant(c);
}

void ArithmeticExpression() {
	char c[5];
	int flag = 0;
	term();
	peek_next_token();
	while ((!(strcmp(tk.lx, "+"))) || (!(strcmp(tk.lx, "-")))) {
		get_next_token();
		strcpy(c, tk.lx);
		term();
		peek_next_token(); flag = 1;
	}
	if (flag && parse_time == 3) write_operator_keyword_constant(c);
}

void term() {
	char c[5];
	int flag = 0;
	factor();
	peek_next_token();
	while ((!(strcmp(tk.lx, "*"))) || (!(strcmp(tk.lx, "/")))) {
		get_next_token();
		strcpy(c, tk.lx);
		factor();
		peek_next_token();
		flag = 1;
	}
	if (flag && parse_time == 3) write_operator_keyword_constant(c);
}

void factor() {
	char c[5];
	int flag = 0;
	peek_next_token();
	if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~")))) {
		get_next_token();
		strcpy(c, tk.lx);
		flag = 1;
	}
	operand();
	if (flag && parse_time ==3) { write_unary_op(c); }
}

void operand() {
	peek_next_token();
	if (tk.tp == INT) {
		get_next_token();
		if (parse_time == 3) {
			write_push_constant(tk.lx);
		}
	}
	else if (tk.tp == STRING) {
		get_next_token();//c string
		if (parse_time == 3) {
			write_string_constant(tk.lx);
		}
	}
	else if ((!(strcmp(tk.lx, "true")))) {
		get_next_token();//c true
		if (parse_time == 3) {
			write_operator_keyword_constant(tk.lx);
		}
	}
	else if ((!(strcmp(tk.lx, "false")))) {
		get_next_token();//c false
		if (parse_time == 3) {
			write_operator_keyword_constant(tk.lx);
		}
	}
	else if ((!(strcmp(tk.lx, "this")))) {
		get_next_token();//c this
		if (parse_time == 3) {
			write_operator_keyword_constant(tk.lx);
		}
	}
	else if ((!(strcmp(tk.lx, "null")))) {
		get_next_token();//c this
		if (parse_time == 3) {
			write_operator_keyword_constant(tk.lx);
		}
	}
	else if ((!(strcmp(tk.lx, "(")))) {
		get_next_token();//c  （
		expression();
		get_next_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { errSort(closeParenExpected); return; }
	}
	else if (tk.tp == ID) {
		if (parse_time == 2) {
			get_next_token();//c identifier
			Token id1 = tk;
			Token id2;
			char object_name[20];
			char symbol_kind_name[20];
			peek_next_token();
			if ((!(strcmp(tk.lx, ".")))) {
				get_next_token();//c “.”
				get_next_token();//c ID2
				id2 = tk;
				if (tk.tp == ID) {}
				else { errSort(idExpected); return; }
				strcpy(symbol_kind_name, find_kind_in_class_table(id2.lx));
				if ((!(strcmp(symbol_kind_name, "none")))) { tk = id2; errSort(undecIdentifier); return; }
				if ((!(strcmp(symbol_kind_name, "method"))) || (!(strcmp(symbol_kind_name, "field")))) {
					// var.method / var.field
					strcpy(object_name, find_object_in_class_table(id1.lx));
					if ((!(strcmp(object_name, "none")))) {
						strcpy(object_name, find_object_in_method_table(id1.lx));
						if ((!(strcmp(object_name, "none")))) { error_token = id1; errSort(undecIdentifier); return; }
					}
					else {
						memset(s.class_name, 0, sizeof(s.class_name));
						//sprintf(s.class_name, "%s%s%s%s%s", "./", project_name, "/", object_name, ".jack");//.Pong/PongGame.jack
						sprintf(s.class_name, "%s", object_name);
						strcpy(s.symbol_name, id2.lx);
						if (LookUP_class_table(s, "method") || LookUP_class_table(s, "field")) {}
						else { error_token = id2; errSort(undecIdentifier); return; }
					}
				}
				else if ((!(strcmp(symbol_kind_name, "function"))) || (!(strcmp(symbol_kind_name, "constuctor")))
					|| (!(strcmp(symbol_kind_name, "static")))) {
					memset(s.class_name, 0, sizeof(s.class_name));
					//sprintf(s.class_name, "%s%s%s%s%s", "./", project_name, "/", id1.lx, ".jack");//.Pong/PongGame.jack
					sprintf(s.class_name, "%s", id1.lx);
					strcpy(s.symbol_name, id2.lx);
					if (LookUP_class_table(s, "function") || LookUP_class_table(s, "static") || LookUP_class_table(s, "constructor")) {}
					else { error_token = id2; errSort(undecIdentifier); return; }

				}
			}
			else { 
				strcpy(s.symbol_name, id1.lx);
				strcpy(s.class_name, current_class_name);
				if (LookUP_method_table(s)) {}
				else if (LookUP_class_table(s, "static") || LookUP_class_table(s, "field") || LookUP_class_table(s, "method")) {}
				else { error_token = id1; errSort(undecIdentifier); return; }
			}
			peek_next_token();
			if ((!(strcmp(tk.lx, "[")))) {
				get_next_token(); 
				expression();
				get_next_token();
				if ((!(strcmp(tk.lx, "]")))) {}
				else { errSort(closeBracketExpected); return; }
			}
			else if ((!(strcmp(tk.lx, "(")))) {
				get_next_token();
				expressionList();
				get_next_token();
				if ((!(strcmp(tk.lx, ")")))) {}
				else { errSort(closeBracketExpected); return; }
			}
		}
		else if (parse_time == 3) {
			get_next_token();
			Token id1 = tk;
			Token id2;
			char object_name[20];
			char symbol_kind_name[20];
			int arg_num = 0;
			peek_next_token();
			if ((!(strcmp(tk.lx, ".")))) {
				get_next_token();//"."
				get_next_token();//"ID2"
				id2 = tk;
				if (isClassName(id1.lx)) {
					get_next_token();//"("
					if (((strcmp(tk.lx, "(")))) { printf("后面跟的不是函数（）"); exit(1); }
					arg_num = expressionList();
					get_next_token();//")"
					write_call(id1.lx,id2.lx,arg_num);
				}
				else {
					write_variable(id1.lx,0);
					get_next_token();//"("
					if (((strcmp(tk.lx, "(")))) { printf("后面跟的不是函数（）"); exit(1); }
					arg_num = expressionList();
					arg_num++;
					get_next_token();//")"
					write_call(find_object_in_class_table(id1.lx), id2.lx, arg_num);
				}
			}
			else if ((!(strcmp(tk.lx, "(")))) {
				get_next_token();
				arg_num = expressionList();
				write_push("pointer",0);
				arg_num++;
				write_call(current_class_name,id1.lx,arg_num);
				get_next_token();//)
			}
			else if ((!(strcmp(tk.lx, "[")))) {
				get_next_token();//"["
				expression();
				get_next_token();//"]"
				write_variable(id1.lx,0);
				write_arithmetic("add");
				write_pop("pointer",1);
				write_push("that",0);
			}
			else { 
				write_variable(id1.lx,0);
			}
		}
		else {
			get_next_token();
			peek_next_token();
			if ((!(strcmp(tk.lx, ".")))) {
				get_next_token();
				get_next_token();
				if (tk.tp == ID) {}
				else { errSort(idExpected); return; }
			}
			peek_next_token();
			if ((!(strcmp(tk.lx, "[")))) {

				get_next_token(); 
				expression();
				get_next_token();
				if ((!(strcmp(tk.lx, "]")))) {}
				else { errSort(closeBracketExpected); return; }
			}
			else if ((!(strcmp(tk.lx, "(")))) {
				get_next_token();
				expressionList();
				get_next_token();
				if ((!(strcmp(tk.lx, ")")))) {}
				else { errSort(closeBracketExpected); return; }
			}
		}
	}
	else { errSort(syntaxError); return; }
}

int InitParser(char* file_name)
{
	InitLexer( file_name);
	pas.er = none;
	iserror = 0;
	return 1;
}

ParserInfo Parse()
{
	ParserInfo pi;
	// implement the function
	//pi.er = none;
	//InitParser(file_name);
	class_start();
	StopParser();
	pi = pas;
	return pi;
}

int StopParser()
{
	StopLexer();
	return 1;
}

#ifndef TEST_PARSER
//int main()
//{
//	ParserInfo p;
//	p = Parse();
//	printf("< %s, %d, %s,%s >\n", p.tk.fl, p.tk.ln, p.tk.lx, TypeOfToken(p.tk));
//	printf("%d", p.er);
//	return 1;
//}
#endif

