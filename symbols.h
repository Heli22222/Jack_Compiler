#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string.h"
#pragma warning(disable:4996)
#pragma warning(disable:4715)
#pragma warning(disable:4024)
// define your own types and function prototypes for the symbol table(s) module below

typedef struct
{
	char class_name[32];
	char symbol_name[128];
	char symbol_type[32];
	char symbol_kind[32];
	int offset;
	// other info for semantic and code generation purposes
} Symbol;

typedef struct
{
	char subroutine_type[32];
	char subroutine_name[128];
	int local_number;
	int field_number;
} SubroutineInformation;

extern Symbol table_of_class[500];
extern Symbol table_of_method[100];
extern Symbol library;
extern int class_table_number;
extern int method_table_number;
extern int var_number;
extern int arg_number;
extern int field_number;
extern int static_number;
extern char project_name[20];
extern FILE* file_out_vm ;

int LookUP_class_table(Symbol s, char* type);
int LookUP_method_table(Symbol s);
void Add_class_symbol(Symbol s);
void Add_method_symbol(Symbol s);
void clear_class_table();
void clear_method_table();
void Add_library_to_class_table();
void Add_Array();
void Add_Screen();
void Add_Keyboard();
void Add_String();
void Add_Math();
void Add_Memory();
void Add_Sys();
void Add_Output();
int isLibrary( char* c);
int isClassName(char* c);
void clear_global_number();
void Initlibrary();
char* find_object_in_method_table(char* a);
char* find_object_in_class_table(char* a);
char* find_type_of(char* name);
int find_index_in_class_table(char* c);
int find_index_in_method_table(char* c);
char* find_kind_in_class_table(char* c);
char* find_kind_in_method_table(char* c);
int find_feild_number(char* c);

void init_file_out(char * project,char* class);
void close_file_out();
void write_return();
void write_function(char* class_name, char* function_name, int variable_number);
void write_call(char* class_name, char* function_name, int argument_number);
void write_if(char* label, int index);
void write_goto(char* label, int index);
void write_label(char* label, int index);
void write_arithmetic(char* command);
void write_pop(char* segment, int index);
void write_push(char* segment, int index);
void write_operator_keyword_constant(char* op);
void write_push_constant(char* c);
void write_string_constant(char* string);
void write_array_alloc(int size);
void write_object_alloc(int size);
void write_variable(char* symbol_name,int flag);
void write_unary_op(char* op);
#endif

