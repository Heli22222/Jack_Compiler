/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Compiler Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:
Student ID:
Email:
Date Work Commenced:
*************************************************************************/

#include "compiler.h"
#include"symbols.h"
#include"parser.h"
#include"stdio.h"
#include <io.h>
#include<string.h>

#pragma warning(disable:4996)
Symbol class_table[500];
Symbol method_table[100];
int class_table_number;
int parse_time;
char project_name[20];

char* TypeOfError(SyntaxErrors s)
{
	switch (s)
	{
	case none: return "none";
	case lexerErr: return "lexerErr";
	case classExpected: return "classExpected";
	case idExpected: return "idExpected";
	case openBraceExpected: return "openBraceExpected";
	case closeBraceExpected: return "closeBraceExpected";
	case memberDeclarErr: return "memberDeclarErr";
	case classVarErr: return "classVarErr";
	case illegalType: return "illegalType";
	case semicolonExpected: return "semicolonExpected";
	case subroutineDeclarErr: return "subroutineDeclarErr";
	case openParenExpected: return "openParenExpected";
	case closeParenExpected: return "closeParenExpected";
	case closeBracketExpected: return "closeBracketExpected";
	case equalExpected: return "equalExpected";
	case syntaxError: return "syntaxError";
	case undecIdentifier: return "undecIdentifier";
	case redecIdentifier: return "redecIdentifier";
	}
}


int InitCompiler()
{
	return 1;
}

ParserInfo compile(char* dir_name)
{
	clear_class_table();
	strcpy(project_name, dir_name);
	parse_time = 1;
	ParserInfo ps;
	//文件存储信息结构体 
	struct _finddata_t fileinfo;
	//保存文件句柄 
	long fHandle;
	//文件数记录器
	int i = 0;
	char file_list[10][260];
	char* full_dir = (char*)malloc(strlen(dir_name) + 40);// 这个储存完整文件夹的路径
	char* full_file = (char*)malloc(strlen(dir_name) + 40); // 这个是储存完整文件名所在路径的
	sprintf(full_dir, "%s%s%s", "../", project_name, "/*.jack");
	Add_library_to_class_table();

	if ((fHandle = _findfirst(full_dir, &fileinfo)) == -1L)
	{
		printf("当前目录下没有txt文件\n");
	}
	else {
		do {
			char* c = &fileinfo.name;
			sprintf(full_file, "%s%s%s%s", "../", dir_name, "/", c);
			strcpy(file_list[i], full_file);
			//file_list[i][260]= strdup(full_file);
			i++;
		} while (_findnext(fHandle, &fileinfo) == 0);
	}
	//关闭文件 
	_findclose(fHandle);

	for (int j = 0; j < i; j++) {
		InitParser(file_list[j]);
		ps = Parse();
		if (ps.er != none) return ps;
	}
	parse_time++;
	for (int j = 0; j < i; j++) {
		InitParser(file_list[j]);
		ps = Parse();
		//ps = Parse(file_list[j], 2);
		if (ps.er != none) return ps;
	}
	return ps;
}

int StopCompiler()
{
	StopParser();
	return 1;
}
//void PrintError(ParserInfo p) {
//	printf("%s", TypeOfError(p.er));
//	printf("< %s, %d, %s,%s > \n", p.tk.fl, p.tk.ln, p.tk.lx, TypeOfToken(p.tk));
//}
#ifndef TEST_COMPILER
int main()
{
	InitCompiler();

	ParserInfo p = compile("Square1");
	PrintError(p);
	StopCompiler();
	return 0;
}
#endif
