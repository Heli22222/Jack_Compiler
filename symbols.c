
/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:
Student ID:
Email:
Date Work Commenced:
*************************************************************************/

#include "symbols.h"
#include "string.h"

extern Symbol table_of_class[500];
extern Symbol table_of_method[100];
Symbol library;
extern int class_table_number;
extern int method_table_number;
int var_number;
int arg_number;
int field_number;
int static_number;

extern FILE* file_out;

void InitLibrary() {
	strcpy(library.class_name, "Array");
	strcpy(library.symbol_name, "new");
	strcpy(library.symbol_type, "Array");
	strcpy(library.symbol_kind, "function");
}

int LookUP_class_table(Symbol s,char* type) {
	for (int i = 0; i < class_table_number; i++) {
		if ((!(strcmp(table_of_class[i].symbol_name, s.symbol_name)))
			&& (!(strcmp(table_of_class[i].class_name, s.class_name)))
			&& (!(strcmp(table_of_class[i].symbol_kind,type)))) return 1;
	}
	return 0;
}
int LookUP_method_table(Symbol s) {
	for (int i = 0; i < method_table_number; i++) {
		if (!(strcmp(table_of_method[i].symbol_name, s.symbol_name))) return 1;
	}
	return 0;
}
int isLibrary(char* c) {
	if ((!(strcmp(c, "Array"))) || (!(strcmp(c, "Screen")))
		|| (!(strcmp(c, "Keyboard"))) || (!(strcmp(c, "Memory")))
		|| (!(strcmp(c, "String"))) || (!(strcmp(c, "Math")))
		|| (!(strcmp(c, "Output"))) || (!(strcmp(c, "Sys")))) return 1;
	else return 0;
}
void Add_class_symbol(Symbol s) {
	table_of_class[class_table_number] = s;
	class_table_number++;
}
void Add_method_symbol(Symbol s) {
	table_of_method[method_table_number] = s;
	method_table_number++;
}
void clear_class_table() {
	memset(table_of_class, 0, sizeof table_of_class);
	class_table_number = 0;
}
char* find_type_of(char* name) {
	char* type = find_object_in_method_table(name);
	if (!(strcmp(type, "none"))) {
		strcpy(type, find_object_in_class_table(name));
		return type;
	}
	return type;
}
int find_index_in_class_table(char* c) {
	for (int i = 0; i < class_table_number; i++) {
		if (!(strcmp(table_of_class[i].symbol_name, c))) {
			return table_of_class[i].offset;
		}
	}
	return -1;
}
int find_index_in_method_table(char* c) {
	for (int i = 0; i < method_table_number; i++) {
		if (!(strcmp(table_of_method[i].symbol_name, c))) {
			return table_of_method[i].offset;
		}
	}
	return -1;
}
void clear_global_number() {
	method_table_number = 0;
	field_number = 0;
	static_number = 0;
}
char* find_kind_in_method_table(char* c) {
	for (int i = 0; i < method_table_number; i++) {
		if (!(strcmp(table_of_method[i].symbol_name, c))) {
			return table_of_method[i].symbol_kind;
		}
	}
	return "none";
}
char* find_kind_in_class_table(char* a) {
	for (int i = 0; i < class_table_number; i++) {
		if (!(strcmp(table_of_class[i].symbol_name, a))) {
			return table_of_class[i].symbol_kind;
		}
	}
	return "none";
}
void clear_method_table() {
	memset(table_of_method, 0, sizeof table_of_method);
	method_table_number = 0;
	var_number = 0;
	arg_number = 0;
}
char* find_object_in_method_table(char* a) {
	for (int i = 0; i < method_table_number; i++) {
		if (!(strcmp(table_of_method[i].symbol_name, a))) {
			return table_of_method[i].symbol_type;
		}
	}
	return "none";
}
int find_feild_number(char* c) {
	int j = 0;
	for (int i = 0; i < class_table_number; i++) {
		if (!(strcmp(table_of_class[i].class_name, c)) && !(strcmp(table_of_class[i].symbol_kind, "field"))) {
			j++;
		}
	}
	return j;
}
char* find_object_in_class_table(char* a) {
	for (int i = 0; i < class_table_number; i++) {
		if (!(strcmp(table_of_class[i].symbol_name, a))) {
			return table_of_class[i].symbol_type;
		}
	}
	return "none";
}

int isClassName(char* c) {
	char isFileName[40];
	memset(isFileName, 0, sizeof(isFileName));
	sprintf(isFileName, "%s%s%s%s", project_name, "/", c, ".jack");
	for (int i = 0; i < class_table_number; i++) {
		if (!(strcmp(table_of_class[i].class_name, isFileName)) || !(strcmp(table_of_class[i].class_name, c))) {
			return 1;
		}
	}
	return 0;
}
void Add_library_to_class_table() {
	InitLibrary();
	Add_Array();
	Add_Screen();
	Add_Keyboard();
	Add_String();
	Add_Math();
	Add_Memory();
	Add_Sys();
	Add_Output();
	return;
}
void Add_Array() {
	strcpy(library.class_name, "Array");
	strcpy(library.symbol_name, "new");
	strcpy(library.symbol_type, "Array");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "dispose");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
}
void Add_Memory() {
	strcpy(library.class_name, "Memory");
	strcpy(library.symbol_name, "peek");
	strcpy(library.symbol_type, "int");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "poke");
	strcpy(library.symbol_type, "void");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "alloc");
	strcpy(library.symbol_type, "Array");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "deAlloc");
	strcpy(library.symbol_type, "void");
	Add_class_symbol(library);
}
void Add_Screen() {
	strcpy(library.class_name, "Screen");
	strcpy(library.symbol_name, "clearScreen");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "setColor");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "drawPixel");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "drawLine");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "drawRectangle");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "drawCircle");
	Add_class_symbol(library);
}
void Add_Keyboard() {
	strcpy(library.class_name, "Keyboard");
	strcpy(library.symbol_name, "keyPressed");
	strcpy(library.symbol_type, "char");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "readChar");
	strcpy(library.symbol_type, "char");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "readLine");
	strcpy(library.symbol_type, "String");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "readInt");
	strcpy(library.symbol_type, "int");
	Add_class_symbol(library);
}
void Add_String() {
	strcpy(library.class_name, "String");
	strcpy(library.symbol_name, "new");
	strcpy(library.symbol_type, "String");
	strcpy(library.symbol_kind, "constructor");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "dispose");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "length");
	strcpy(library.symbol_type, "int");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "charAt");
	strcpy(library.symbol_type, "char");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "setCharAt");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "appendChar");
	strcpy(library.symbol_type, "String");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "eraseLastChar");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "intValue");
	strcpy(library.symbol_type, "int");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "setInt");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_kind, "method");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "newLine");
	strcpy(library.symbol_type, "char");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "backSpace");
	strcpy(library.symbol_type, "char");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "doubleQuote");
	strcpy(library.symbol_type, "char");
	strcpy(library.symbol_kind, "function");
	Add_class_symbol(library);
}
void Add_Math() {
	strcpy(library.class_name, "Math");
 	strcpy(library.symbol_kind, "function");
	strcpy(library.symbol_type, "int");
	strcpy(library.symbol_name, "abs");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "multiply");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "divide");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "min");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "max");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "sqrt");
	Add_class_symbol(library);
}
void Add_Sys() {
	strcpy(library.class_name, "Sys");
	strcpy(library.symbol_kind, "function");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_name, "halt");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "error");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "wait");
	Add_class_symbol(library);

}
void Add_Output() {
	strcpy(library.class_name, "Output");
	strcpy(library.symbol_kind, "function");
	strcpy(library.symbol_type, "void");
	strcpy(library.symbol_name, "init");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "moveCursor");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "printChar");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "printString");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "printInt");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "println");
	Add_class_symbol(library);
	strcpy(library.symbol_name, "backSpace");
	Add_class_symbol(library);
}


void init_file_out(char* project_n, char* class_n) {
	char full_file_out[30];
	sprintf(full_file_out,"%s%s%s%s",project_n,"/",class_n,".vm");
	file_out_vm = fopen(full_file_out, "wt+");
	if (file_out_vm == NULL) {
		printf("error : %s",full_file_out);
		exit(1);
	}
}
void close_file_out() {
	fclose(file_out_vm);
}
void write_return() {
	fprintf(file_out_vm,"return\n");
}
void write_function(char* class_name, char* function_name, int variable_number) {
	fprintf(file_out_vm,"function %s.%s %d\n",class_name, function_name,variable_number);
}
void write_call(char* class_name, char* function_name, int argument_number) {
	fprintf(file_out_vm, "call %s.%s %d\n", class_name, function_name, argument_number);
}
void write_if(char* label, int index) {
	fprintf(file_out_vm,"if-goto %s%d\n",label,index);
}
void write_goto(char* label, int index) {
	fprintf(file_out_vm, "goto %s%d\n", label, index);
}
void write_label(char* label, int index) {
	fprintf(file_out_vm, "label %s%d\n", label, index);
}
void write_arithmetic(char* command) {
	fprintf(file_out_vm, "%s\n",command);
}
void write_pop(char* segment, int index) {
if (!strcmp(segment, "variable")) { strcpy(segment, "local"); }
	fprintf(file_out_vm, "pop %s %d\n", segment,index);
}
void write_push(char* segment, int index) {
if (!strcmp(segment, "variable")) { strcpy(segment, "local"); }
	fprintf(file_out_vm, "push %s %d\n", segment, index);
}
void write_push_constant(char* c) {
	fprintf(file_out_vm, "push %s %s\n", "constant", c);
}
void write_operator_keyword_constant(char* op) { 
	if (!(strcmp(op, "+"))) fprintf(file_out_vm,"add\n");
	else if (!(strcmp(op, "-"))) fprintf(file_out_vm, "sub\n");
	else if (!(strcmp(op, "*"))) fprintf(file_out_vm, "Math.multiply\n");
	else if (!(strcmp(op, "/"))) fprintf(file_out_vm, "Math.divide\n");
	else if (!(strcmp(op, "&"))) fprintf(file_out_vm, "and\n");
	else if (!(strcmp(op, "|"))) fprintf(file_out_vm, "or\n");
	else if (!(strcmp(op, "<"))) fprintf(file_out_vm, "lt\n");
	else if (!(strcmp(op, ">"))) fprintf(file_out_vm, "gt\n");
	else if (!(strcmp(op, "="))) fprintf(file_out_vm, "eq\n");
	else if (!(strcmp(op, "~"))) fprintf(file_out_vm, "not\n");
	else if (!(strcmp(op, "true"))) { write_push("constant",0); fprintf(file_out_vm, "not\n"); }
	else if (!(strcmp(op, "false"))) write_push("constant", 0);
	else if (!(strcmp(op, "null"))) write_push("constant", 0);
	else if (!(strcmp(op, "this"))) write_push("pointer", 0);
	
}
void write_string_constant(char* string) {
	int length = sizeof(string) / sizeof(char);
		write_push("constant", length);
		write_call("String","new",1);
		for (int i = 0; i < length; i++) {
			write_push("constant",string[i]);
			write_call("String", "appendChar", 2);
		}
}
void write_array_alloc(int size) {
	write_push("constant",size);
	write_call("Memory","alloc",1);
}
void write_object_alloc(int size) {
	write_push("constant", size);
	write_call("Memory", "alloc", 1);
	write_pop("pointer",0);
}
void write_variable(char* symbol_name, int flag) { 
	char segment[30];
		strcpy(segment, find_kind_in_method_table(symbol_name));
	if (!(strcmp(segment, "none"))) {
		strcpy(segment, find_kind_in_class_table(symbol_name));
		if (!(strcmp(segment, "none"))) {
			printf("not found kind %s in ", symbol_name); 
		    exit(1); 
		}
			if(!(strcmp(segment, "field"))) strcpy(segment,"this");
			if (flag) write_pop(segment, find_index_in_class_table(symbol_name));
			else write_push(segment, find_index_in_class_table(symbol_name));
		}
	else {
		if (flag) write_pop(segment, find_index_in_method_table(symbol_name));
		else write_push(segment, find_index_in_method_table(symbol_name));
	}
}
void write_unary_op(char* op) {
	if (!(strcmp(op, "-"))) fprintf(file_out_vm, "neg\n");
	else if (!(strcmp(op, "~"))) fprintf(file_out_vm, "not\n");
	else { printf("unary err"); }
}
