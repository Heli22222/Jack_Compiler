#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "symbols.h"

ParserInfo pas;
Token tk;
Symbol s;
int iserror;
extern Symbol class_table[500];
extern Symbol method_table[100];
extern int class_table_number;
int method_table_number;
extern int var_number;
extern int arg_number;
extern int field_number;
extern int static_number;
extern int parse_time;


// you can declare prototypes of parser functions below
void error(SyntaxErrors syn);
void class();
void memberDeclar();
void classVarDeclar();
void subroutineDeclar();
void type();
void paramList();
void subroutineBody();
void statement();
void varDeclarStatement();
void letStatemnt();
void ifStatement();
void whileStatement();
void doStatement();
void subroutineCall();
void expressionList();
void retrunStatemnt();
void expression();
void relationalExpression();
void ArithmeticExpression();
void term();
void factor();
void operand();

void error(SyntaxErrors syn) {
	if (iserror == 0) {
		pas.er = syn;
		pas.tk = tk;
		iserror = 1;
	}
	if (iserror == 1) {}
	return;
}
void get_token() {
	tk = GetNextToken();
	if (tk.tp == ERR) {
		error(lexerErr);
		return;
	}
}

void peek_token() {
	tk = PeekNextToken();
	if (tk.tp == ERR) {
		error(lexerErr);
		tk = GetNextToken();
		return;
	}
}

void class_start() {
	peek_token();
	if (!(strcmp(tk.lx, "class"))) {}
	else { error(classExpected); return; }
	get_token();//消耗 class
	get_token();
	if (tk.tp == ID) {}
	else { error(idExpected); return; }
	peek_token();
	if (!(strcmp(tk.lx, "{"))) {}
	else {
		error(openBraceExpected);
		return;
	}
	get_token();
	while (tk.tp != EOF && iserror == 0 && (strcmp(tk.lx, "}"))) {
		memberDeclar();
		peek_token();
	}
	if (!(strcmp(tk.lx, "}"))) {}
	else error(closeBraceExpected);
	get_token();
	return;
}

void memberDeclar() {
	peek_token();
	if ((!(strcmp(tk.lx, "static"))) || (!(strcmp(tk.lx, "field")))) {
		//get_token(); //得到 token = static | field
		//peek_token();//看下一个 是否是 type()
		classVarDeclar();
	}
	else if ((!(strcmp(tk.lx, "constructor"))) || (!(strcmp(tk.lx, "function"))) || (!(strcmp(tk.lx, "method")))) {
		//get_token();
		//peek_token();
		subroutineDeclar();
	}
	else { error(memberDeclarErr); return; }
	return;
}

void classVarDeclar() {
	if (parse_time == 1) { //第一遍过这个文件 负责添加 class_table
		//第二遍时几乎只管method_symbol
		clear_global_number(); //开始一个新的文件，所以作用域不同，
		//每个offset 就清零
		peek_token();
		strcpy(s.class_name, tk.fl);
		if ((!(strcmp(tk.lx, "static")))) {
			strcpy(s.symbol_kind, "static");
			s.offset = static_number++;
		}
		else if ((!(strcmp(tk.lx, "field")))) {
			strcpy(s.symbol_kind, "field");
			s.offset = field_number++;
		}
		else {
			error(classVarErr); return;
		}
		get_token();//消耗掉static / field
		type();
		peek_token();
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if ((LookUP_class_table(s, "static")) ||
				(LookUP_class_table(s, "field"))) {
				error(redecIdentifier); return;
			}
			else Add_class_symbol(s);
		}
		else { error(idExpected); return; }
		get_token();
		peek_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_token();//消耗 ， 逗号
			get_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_name, tk.lx);
				if ((LookUP_class_table(s, "static")) ||
					(LookUP_class_table(s, "field"))) {
					error(redecIdentifier); return;
				}//是否存在重复声明
				if ((!(strcmp(s.symbol_kind, "static")))) { s.offset = static_number++; }
				else if ((!(strcmp(s.symbol_kind, "field")))) { s.offset = field_number++; }//与上一个symbol的offset 不同
				Add_class_symbol(s);
			}
			else { error(idExpected); return; }
			peek_token();
		}
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
		get_token();
	}
	else {
		peek_token();
		if ((!(strcmp(tk.lx, "static")))) {}
		else if ((!(strcmp(tk.lx, "field")))) {}
		else { error(classVarErr); return; }
		get_token();//消耗掉static / field
		type();
		peek_token();
		if (tk.tp == ID) {}
		else { error(idExpected); return; }
		get_token();
		peek_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_token();//消耗 ， 逗号
			get_token();
			if (tk.tp == ID) {}
			else { error(idExpected); return; }
			peek_token();
		}
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
		get_token();
	}
}


void type() {
	if (parse_time == 1) {
		peek_token();
		if ((!(strcmp(tk.lx, "int")))) { strcpy(s.symbol_type, "int"); }
		else if ((!(strcmp(tk.lx, "char")))) { strcpy(s.symbol_type, "char"); }
		else if ((!(strcmp(tk.lx, "boolean")))) { strcpy(s.symbol_type, "boolean"); }
		else if (tk.tp == ID) { strcpy(s.symbol_type, tk.lx); }
		else { error(illegalType); return; }
		get_token();//消耗掉 int 之类的
	}
	else {
		peek_token();
		if ((!(strcmp(tk.lx, "int")))) { strcpy(s.symbol_type, "int"); }
		else if ((!(strcmp(tk.lx, "char")))) { strcpy(s.symbol_type, "char"); }
		else if ((!(strcmp(tk.lx, "boolean")))) { strcpy(s.symbol_type, "boolean"); }
		else if (tk.tp == ID) {
			if (isClassName(tk.lx)) { strcpy(s.symbol_type, tk.lx); }
			else { error(undecIdentifier); return; }
		}
		else {  error(illegalType); return; }
		get_token();//消耗掉 int 之类的
	}
}

void subroutineDeclar() { //第一遍要加入到class_table
	if (parse_time == 1) {
		clear_method_table();
		get_token();
		strcpy(s.class_name, tk.fl);
		if (!(strcmp(tk.lx, "constructor"))) { strcpy(s.symbol_kind, "constructor"); }
		else if ((!(strcmp(tk.lx, "function")))) { strcpy(s.symbol_kind, "function"); }
		else if ((!(strcmp(tk.lx, "method")))) { strcpy(s.symbol_kind, "method"); }//也得把函数加入表中，具体看笔记
		else { error(memberDeclarErr);  return; }
		get_token();//消耗掉 函数返回值 没有用peek
		if (!(strcmp(tk.lx, "int"))) {//以下记录了这个函数的返回值
			strcpy(s.symbol_type, "int");
		}
		else if (!(strcmp(tk.lx, "char"))) { strcpy(s.symbol_type, "char"); }

		else if (!(strcmp(tk.lx, "boolean"))) { strcpy(s.symbol_type, "boolean"); }

		else if (tk.tp == ID) { strcpy(s.symbol_type, tk.lx); }

		else if (!(strcmp(tk.lx, "void"))) { strcpy(s.symbol_type, "void"); }

		else { error(illegalType); return; }
		get_token();//没有用 peek 消耗掉ID
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if ((LookUP_class_table(s, "function")) ||
				(LookUP_class_table(s, "method"))) {
				error(redecIdentifier);
				return;
			}//检查是否存在相同的函数 在同一个作用域里
			else Add_class_symbol(s);
		}
		else { error(idExpected); return; }
		get_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { error(openParenExpected); return; }//消耗了（  paralist需要peek才能执行
		paramList();
		get_token();//消耗 “）”
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error(closeParenExpected); return; }
		//clear_method_table();//开始一个新的子函数的描述，
		//之前的局部变量表可以清空
		subroutineBody();
		return;
	}
	else {
		clear_method_table();
		get_token();
		strcpy(s.class_name, tk.fl);
		if (!(strcmp(tk.lx, "constructor"))) {}
		else if ((!(strcmp(tk.lx, "function")))) {}
		else if ((!(strcmp(tk.lx, "method")))) {}
		else { error(memberDeclarErr);  return; }
		get_token();//消耗掉 函数返回值 没有用peek
		if (!(strcmp(tk.lx, "int"))) {}
		else if (!(strcmp(tk.lx, "char"))) {}
		else if (!(strcmp(tk.lx, "boolean"))) {}
		else if (tk.tp == ID) {}
		else if (!(strcmp(tk.lx, "void"))) {}
		else { error(illegalType); return; }
		get_token();//没有用 peek 消耗掉ID
		if (tk.tp == ID) {}
		else { error(idExpected); return; }
		get_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { error(openParenExpected); return; }//消耗了（  paralist需要peek才能执行
		paramList();
		get_token();//消耗 “）”
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error(closeParenExpected); return; }
		//clear_method_table();//开始一个新的子函数的描述，
		//之前的局部变量表可以清空
		subroutineBody();
		return;
	}
}

void paramList() {
	if (parse_time == 2) {
		peek_token();
		if ((!(strcmp(tk.lx, "int")))
			|| (!(strcmp(tk.lx, "char")))
			|| (!(strcmp(tk.lx, "boolean")))
			|| tk.tp == ID) {
			type();//这一步 已经把 symbol 的type给解决了
			get_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_kind, "argument");
				strcpy(s.symbol_name, tk.lx);
				if (LookUP_method_table(s)) {
					error(redecIdentifier);
					return;
				}
				else {
					s.offset = arg_number++;//argument 成功添加
					Add_method_symbol(s);
				}
			}//消耗 ID
			else { error(idExpected); return; }
			peek_token();
			while ((!(strcmp(tk.lx, ",")))) {
				get_token();//消耗 逗号 ，
				type();
				get_token();
				if (tk.tp == ID) {
					strcpy(s.symbol_name, tk.lx);
					if (LookUP_method_table(s)) {
						error(redecIdentifier);
						return;
					}//如果有多个参数，要一个一个判断是否重复声明
					else {
						s.offset = arg_number++;//argument 成功添加
						Add_method_symbol(s);
					}
				}//消耗 ID
				else { error(idExpected); return; }
				peek_token();
			}
		}
		else {} //空字符串
	}
	else {
		peek_token();
		if ((!(strcmp(tk.lx, "int")))
			|| (!(strcmp(tk.lx, "char")))
			|| (!(strcmp(tk.lx, "boolean")))
			|| tk.tp == ID) {
			type();
			get_token();
			if (tk.tp == ID) {}//消耗 ID
			else { error(idExpected); return; }
			peek_token();
			while ((!(strcmp(tk.lx, ",")))) {
				get_token();//消耗 逗号 ，
				type();
				get_token();
				if (tk.tp == ID) {}//消耗 ID
				else { error(idExpected); return; }
				peek_token();
			}
		}
	}
}

void subroutineBody() {
	get_token();
	if ((!(strcmp(tk.lx, "{")))) {}
	else { error(openBraceExpected); return; }
	peek_token();
	while ((!(strcmp(tk.lx, "var"))) || (!(strcmp(tk.lx, "let"))) || (!(strcmp(tk.lx, "if"))) || (!(strcmp(tk.lx, "while"))) || (!(strcmp(tk.lx, "return"))) || (!(strcmp(tk.lx, "do")))) {
		//不能消耗第一个词,交给后面的做
		statement();
		peek_token();
	}

	if ((!(strcmp(tk.lx, "}")))) { get_token(); }
	else { error(closeBraceExpected);  return; }
}

void statement() {
	if ((!(strcmp(tk.lx, "var")))) varDeclarStatement();
	else if ((!(strcmp(tk.lx, "let")))) letStatemnt();
	else if ((!(strcmp(tk.lx, "if")))) ifStatement();
	else if ((!(strcmp(tk.lx, "while")))) whileStatement();
	else if ((!(strcmp(tk.lx, "return")))) retrunStatemnt();
	else if ((!(strcmp(tk.lx, "do")))) doStatement();
	else { error(syntaxError); return; }
}

void varDeclarStatement() {
	if (parse_time == 2) {//第二次parse时要 添加 本地variable
		get_token();//消耗掉var
		type();//解决了 symbol_type的问题
		strcpy(s.symbol_kind, "variable");
		get_token();
		if (tk.tp == ID) {
			strcpy(s.symbol_name, tk.lx);
			if (LookUP_method_table(s)) {
				error(redecIdentifier);
				return;
			}
			else {
				s.offset = var_number++;//variable 成功添加
				Add_method_symbol(s);
			}
		}
		else { error(idExpected); return; }
		peek_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_token();//消耗逗号
			get_token();
			if (tk.tp == ID) {
				strcpy(s.symbol_name, tk.lx);
				if (LookUP_method_table(s)) {
					error(redecIdentifier);
					return;
				}
				else {
					s.offset = var_number++;//variable 成功添加
					Add_method_symbol(s);
				}
			}
			else { error(idExpected); return; }
			peek_token();
		}
		get_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
	}
	else {
		get_token();//消耗掉var
		type();
		get_token();
		if (tk.tp == ID) {}
		else { error(idExpected); return; }
		peek_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_token();//消耗逗号
			get_token();
			if (tk.tp == ID) {}
			else { error(idExpected); return; }
			peek_token();
		}
		get_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
	}
}

void letStatemnt() {
	get_token();//消耗 let
	get_token();//消耗 ID
	if (tk.tp == ID) {}
	else { error(idExpected); return; }
	if (parse_time == 1) {
		peek_token();//看一下是不是 [
		if ((!(strcmp(tk.lx, "[")))) {
			get_token();//消耗 [
			expression();
			get_token();
			if ((!(strcmp(tk.lx, "]")))) {}
			else { error(closeBracketExpected); return; }
		}
		else {}
		get_token();
		if ((!(strcmp(tk.lx, "=")))) {}
		else { error(equalExpected); return; }
		expression();
		get_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
	}
	else {//第二遍过的时候要看看id是不是undeclared
		strcpy(s.symbol_name, tk.lx);
		memset(s.class_name, 0, sizeof(s.class_name));
		strcpy(s.class_name, tk.fl);
		if (LookUP_method_table(s)) {}
		else if (LookUP_class_table(s, "field") || LookUP_class_table(s, "static")) {}
		else { error(undecIdentifier); return; }
		peek_token();//看一下是不是 [
		if ((!(strcmp(tk.lx, "[")))) {
			get_token();//消耗 [
			expression();
			get_token();
			if ((!(strcmp(tk.lx, "]")))) {}
			else { error(closeBracketExpected); return; }
		}
		else {}
		get_token();
		if ((!(strcmp(tk.lx, "=")))) {}
		else { error(equalExpected); return; }
		expression();
		get_token();
		if ((!(strcmp(tk.lx, ";")))) {}
		else { error(semicolonExpected); return; }
	}
}

void ifStatement() {
	get_token();//消耗 if
	get_token();
	if ((!(strcmp(tk.lx, "(")))) {}
	else { error(openParenExpected); return; }
	expression();
	get_token();
	if ((!(strcmp(tk.lx, ")")))) {}
	else { error(closeParenExpected); return; }
	get_token();
	if ((!(strcmp(tk.lx, "{")))) {}
	else { error(openBraceExpected); return; }
	peek_token();
	while ((!(strcmp(tk.lx, "var")))
		|| (!(strcmp(tk.lx, "let")))
		|| (!(strcmp(tk.lx, "if")))
		|| (!(strcmp(tk.lx, "while")))
		|| (!(strcmp(tk.lx, "return")))
		|| (!(strcmp(tk.lx, "do")))) {
		//不能消耗第一个词,交给后面的做
		statement();
		peek_token();
	}
	if ((!(strcmp(tk.lx, "}")))) { get_token(); }
	else { error(closeBraceExpected); return; }
	peek_token();
	if ((!(strcmp(tk.lx, "else")))) {
		get_token();//消耗else
		get_token();
		if ((!(strcmp(tk.lx, "{")))) {}
		else { error(openBraceExpected); return; }
		peek_token();
		while ((!(strcmp(tk.lx, "var")))
			|| (!(strcmp(tk.lx, "let")))
			|| (!(strcmp(tk.lx, "if")))
			|| (!(strcmp(tk.lx, "while")))
			|| (!(strcmp(tk.lx, "return")))
			|| (!(strcmp(tk.lx, "do")))) {
			//不能消耗第一个词,交给后面的做
			statement();
			peek_token();
		}
		get_token();
		if ((!(strcmp(tk.lx, "}")))) {}
		else { error(closeBraceExpected); return; }
	}
	else {}
}

void whileStatement() {
	get_token();//消耗while
	get_token();
	if ((!(strcmp(tk.lx, "(")))) {}
	else { error(openParenExpected); return; }
	expression();
	get_token();
	if ((!(strcmp(tk.lx, ")")))) {}
	else { error(closeParenExpected); return; }
	get_token();
	if ((!(strcmp(tk.lx, "{")))) {}
	else { error(openBraceExpected); return; }
	peek_token();
	while ((!(strcmp(tk.lx, "var"))) || (!(strcmp(tk.lx, "let"))) || (!(strcmp(tk.lx, "if"))) || (!(strcmp(tk.lx, "while"))) || (!(strcmp(tk.lx, "return"))) || (!(strcmp(tk.lx, "do")))) {
		//不能消耗第一个词,交给后面的做
		statement();
		peek_token();
	}
	get_token();
	if ((!(strcmp(tk.lx, "}")))) {}
	else { error(closeBraceExpected); return; }
}

void doStatement() {
	get_token();//消耗do
	peek_token();
	if (tk.tp == ID) {}
	else { error(idExpected); return; }
	subroutineCall();
	get_token();
	if ((!(strcmp(tk.lx, ";")))) {}
	else { error(semicolonExpected); return; }
}

void subroutineCall() {
	if (parse_time == 2) { //第二次parse才会查看是否在子函数的表达式里有未定义的变量/方程
		get_token();//消耗identifier
		Token const id1 = tk;
		Token id2; // do ID1.ID2(); 接下来是分类讨论
		peek_token();
		char* object_name = NULL;
		if ((!(strcmp(tk.lx, ".")))) {//牵扯到了其他文件的调用
			strcpy(s.symbol_name, id1.lx);//要是再method table 里  得返回id1 的symbol_type 来找到PongGame
			if (isLibrary(id1.lx)) {// 如果是自带库的 那么 classname 会有点不一样 不是完整路径
				strcpy(s.class_name, id1.lx);
				get_token();//消耗 “.”
				get_token();//消耗 ID2
				id2 = tk;
				if (tk.tp == ID) {}
				else { error(idExpected); return; }
				strcpy(s.symbol_name, id2.lx);
				if ((LookUP_class_table(s, "function")) ||
					(LookUP_class_table(s, "constructor"))) {
				}
				else { error(undecIdentifier); return; }
			}
			else if (LookUP_method_table(s)) {
				char object_name[20];
				strcpy(object_name, find_object_in_method_table(id1.lx));
				if (!(strcmp(object_name, "none"))) {
					strcpy(s.symbol_name, id1.lx);
					tk = id1;
					error(undecIdentifier); return;
				}
				if (isLibrary(object_name)) {// 如果是自带库的 那么 classname 会有点不一样 不是完整路径
					strcpy(s.class_name, object_name);
					get_token();//消耗 “.”
					get_token();//消耗 ID2
					id2 = tk;
					if (tk.tp == ID) {}
					else { error(idExpected); return; }
					strcpy(s.symbol_name, id2.lx);
					if ((LookUP_class_table(s, "method"))) {
					}
					else { error(undecIdentifier); return; }
				}
				else {
					memset(s.class_name, 0, sizeof(s.class_name));
					sprintf(s.class_name, "%s%s%s%s%s", "../", project_name, "/", object_name, ".jack");//   /.Pong/PongGame.jack
					get_token();//消耗 “.”
					get_token();//消耗 ID2
					id2 = tk;
					if (tk.tp == ID) {}
					else { error(idExpected); return; }
					strcpy(s.symbol_name, id2.lx);
					//strcpy(s.symbol_kind, "method");
					if (LookUP_class_table(s, "method")) {}
					else { error(undecIdentifier); return; }
				}
			}

			else {//说明不是 本地变量的method 是对象名.function（）或者是 varname.methodname()
				get_token();//消耗 “.”
				get_token();//消耗 ID2
				if (tk.tp == ID) {}
				else { error(idExpected); return; }
				id2 = tk;
				if ((!(strcmp(find_symbol_kind_in_class_table(id2.lx), "method")))) {//第二种情况
					char object_name1[20];
					strcpy(object_name1, find_object_in_class_table(id1.lx));
					if ((!(strcmp(object_name1, "none")))) {
						tk = id1;
						strcpy(s.symbol_name, id1.lx);
						tk = id1;
						error(undecIdentifier); return;
					}
					memset(s.class_name, 0, sizeof(s.class_name));
					sprintf(s.class_name, "%s%s%s%s%s", "../", project_name, "/", object_name1, ".jack");//   /.Pong/PongGame.jack
					strcpy(s.symbol_name, id2.lx);
					if (LookUP_class_table(s, "method")) {}
				}
				else if ((!(strcmp(find_symbol_kind_in_class_table(id2.lx), "function"))) ||
					(!(strcmp(find_symbol_kind_in_class_table(id2.lx), "constructor")))) {//第一种情况
					sprintf(s.class_name, "%s%s%s%s%s", "../", project_name, "/", id1.lx, ".jack");//.Pong/PongGame.jack
					strcpy(s.symbol_name, id2.lx);
					if ((LookUP_class_table(s, "function")) ||
						(LookUP_class_table(s, "constructor"))) {
					}
					else { error(undecIdentifier); return; }
				}
				else {
					tk = id2;
					error(undecIdentifier); return;
				}
			}
		}
		else {//没有牵扯到其他文件调用,easy
			//strcpy(id1.fl);
			strcpy(s.symbol_name, id1.lx);
			strcpy(s.class_name, id1.fl);
			strcpy(s.symbol_type, "method");
			if (LookUP_class_table(s, "method")) {}
			else { tk = id1; error(undecIdentifier); return; }
		}
		get_token();//消耗（
		if ((!(strcmp(tk.lx, "(")))) {}
		else { error(openParenExpected); return; }
		expressionList();
		get_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error(closeParenExpected); return; }
	}
	else {//这个是第一遍parse
		get_token();//消耗identifier
		peek_token();
		if ((!(strcmp(tk.lx, ".")))) {
			get_token();
			get_token();
			if (tk.tp == ID) {}
			else { error(idExpected); return; }
		}
		get_token();
		if ((!(strcmp(tk.lx, "(")))) {}
		else { error(openParenExpected); return; }
		expressionList();
		get_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error(closeParenExpected); return; }
	}
}

void expressionList() {
	peek_token();
	if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~"))) || tk.tp == ID || tk.tp == INT || (!(strcmp(tk.lx, "("))) || tk.tp == STRING || (!(strcmp(tk.lx, "true")))
		|| (!(strcmp(tk.lx, "false"))) || (!(strcmp(tk.lx, "null"))) || (!(strcmp(tk.lx, "this")))) {
		expression();
		peek_token();
		while ((!(strcmp(tk.lx, ",")))) {
			get_token();//消耗逗号
			expression();
			peek_token();
		}
	}
	else {}
}

void retrunStatemnt() {
	get_token();
	if ((!(strcmp(tk.lx, "return")))) {}
	else { error(syntaxError); return; }//，没有return
	peek_token();
	if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~"))) || tk.tp == ID || tk.tp == INT || (!(strcmp(tk.lx, "("))) || tk.tp == STRING || (!(strcmp(tk.lx, "true")))
		|| (!(strcmp(tk.lx, "false"))) || (!(strcmp(tk.lx, "null"))) || (!(strcmp(tk.lx, "this")))) {
		expression();
	}
	else {}
	get_token();//消耗 ；分号
	if ((!(strcmp(tk.lx, ";")))) {}
	else { error(semicolonExpected); return; }
}

void expression() {
	relationalExpression();
	peek_token();
	while ((!(strcmp(tk.lx, "&"))) || (!(strcmp(tk.lx, "|")))) {
		get_token();
		relationalExpression();
		peek_token();
	}
}

void relationalExpression() {
	ArithmeticExpression();
	peek_token();
	while ((!(strcmp(tk.lx, "="))) || (!(strcmp(tk.lx, "<"))) || (!(strcmp(tk.lx, ">")))) {
		get_token();
		ArithmeticExpression();
		peek_token();
	}
}

void ArithmeticExpression() {
	term();
	peek_token();
	while ((!(strcmp(tk.lx, "+"))) || (!(strcmp(tk.lx, "-")))) {
		get_token();
		term();
		peek_token();
	}
}

void term() {
	factor();
	peek_token();
	while ((!(strcmp(tk.lx, "*"))) || (!(strcmp(tk.lx, "/")))) {
		get_token();
		factor();
		peek_token();
	}
}

void factor() {
	peek_token();
	if ((!(strcmp(tk.lx, "-"))) || (!(strcmp(tk.lx, "~")))) {
		get_token();/*消耗 - ~*/
	}
	else {}
	operand();
}

void operand() {
	peek_token();
	if (tk.tp == INT) {
		get_token();//消耗INT
	}
	else if (tk.tp == STRING) {
		get_token();//消耗string
	}
	else if ((!(strcmp(tk.lx, "true")))) {
		get_token();//消耗true
	}
	else if ((!(strcmp(tk.lx, "false")))) {
		get_token();//消耗false
	}
	else if ((!(strcmp(tk.lx, "this")))) {
		get_token();//消耗this
	}
	else if ((!(strcmp(tk.lx, "null")))) {
		get_token();//消耗this
	}
	else if ((!(strcmp(tk.lx, "(")))) {
		get_token();//消耗 （
		expression();
		get_token();
		if ((!(strcmp(tk.lx, ")")))) {}
		else { error(closeParenExpected); return; }
	}
	else if (tk.tp == ID) {
		if (parse_time == 2) {
			get_token();//消耗identifier
			Token const id1 = tk;
			Token id2;
			char object_name[20];
			char symbol_kind_name[20];
			peek_token();
			if ((!(strcmp(tk.lx, ".")))) {
				get_token();//消耗 “.”
				get_token();//消耗 ID2
				id2 = tk;
				if (tk.tp == ID) {}
				else { error(idExpected); return; }
				strcpy(symbol_kind_name, find_symbol_kind_in_class_table(id2.lx));
				if ((!(strcmp(symbol_kind_name, "none")))) { tk = id2; error(undecIdentifier); return; }
				if ((!(strcmp(symbol_kind_name, "method"))) || (!(strcmp(symbol_kind_name, "field")))) {//如果id2是一个method 或者 field 得返回id1 的类名
					strcpy(object_name, find_object_in_class_table(id1.lx));
					if ((!(strcmp(object_name, "none")))) {
						strcpy(object_name, find_object_in_method_table(id1.lx));
						if ((!(strcmp(object_name, "none")))) { tk= id1; error(undecIdentifier); return; }
					}

					if (isLibrary(object_name)) { //id1来自标准库
						strcpy(s.class_name, object_name);
						strcpy(s.symbol_name, id2.lx);
						if (LookUP_class_table(s, "method")) {}
						else { tk = id2; error(undecIdentifier); return; }
					}
					else {
						memset(s.class_name, 0, sizeof(s.class_name));
						sprintf(s.class_name, "%s%s%s%s%s", "../", project_name, "/", object_name, ".jack");//.Pong/PongGame.jack
						strcpy(s.symbol_name, id2.lx);
						if (LookUP_class_table(s, "method") || LookUP_class_table(s, "field")) {}
						else { tk = id2; error(undecIdentifier); return; }
					}
				}
				else if ((!(strcmp(symbol_kind_name, "function"))) || (!(strcmp(symbol_kind_name, "constuctor")))
					|| (!(strcmp(symbol_kind_name, "static")))) {
					if (isLibrary(id1.lx)) { //id1来自标准库
						strcpy(s.class_name, id1.lx);
						strcpy(s.symbol_name, id2.lx);
						if (LookUP_class_table(s, "method") || LookUP_class_table(s, "function") || LookUP_class_table(s, "constructor")) {}
						else { tk = id2; error(undecIdentifier); return; }
					}
					else {
						memset(s.class_name, 0, sizeof(s.class_name));
						sprintf(s.class_name, "%s%s%s%s%s", "../", project_name, "/", id1.lx, ".jack");//.Pong/PongGame.jack
						strcpy(s.symbol_name, id2.lx);
						if (LookUP_class_table(s, "function") || LookUP_class_table(s, "static") || LookUP_class_table(s, "constructor")) {}
						else { tk = id2; error(undecIdentifier); return; }
					}
				}
			}
			else { // 不牵扯类的调用
				strcpy(s.symbol_name, id1.lx);
				strcpy(s.class_name, id1.fl);
				if (LookUP_method_table(s)) {}
				else if (LookUP_class_table(s, "static") || LookUP_class_table(s, "field") || LookUP_class_table(s, "method")) {}
				else { tk = id1; error(undecIdentifier); return; }
			}
			peek_token();
			if ((!(strcmp(tk.lx, "[")))) {
				get_token(); //消耗[
				expression();
				get_token();
				if ((!(strcmp(tk.lx, "]")))) {}
				else { error(closeBracketExpected); return; }
			}
			else if ((!(strcmp(tk.lx, "(")))) {
				get_token();
				expressionList();
				get_token();
				if ((!(strcmp(tk.lx, ")")))) {}
				else { error(closeBracketExpected); return; }
			}
		}
		else {//第一遍parse
			get_token();//消耗identifier
			peek_token();
			if ((!(strcmp(tk.lx, ".")))) {
				get_token();//消耗 点号
				get_token();
				if (tk.tp == ID) {}
				else { error(idExpected); return; }
			}
			peek_token();
			if ((!(strcmp(tk.lx, "[")))) {
				get_token(); //消耗[
				expression();
				get_token();
				if ((!(strcmp(tk.lx, "]")))) {}
				else { error(closeBracketExpected); return; }
			}
			else if ((!(strcmp(tk.lx, "(")))) {
				get_token();
				expressionList();
				get_token();
				if ((!(strcmp(tk.lx, ")")))) {}
				else { error(closeBracketExpected); return; }
			}
		}
	}
	else { error(syntaxError); return; }//啥也没有的expression
}

int InitParser(char* file_name)
{
	InitLexer(file_name);
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
int main()
{
	ParserInfo p;
	p = Parse();
	printf("< %s, %d, %s,%s >\n", p.tk.fl, p.tk.ln, p.tk.lx, TypeOfToken(p.tk));
	printf("%d", p.er);
	return 1;
}
#endif
