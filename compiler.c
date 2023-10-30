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

#include <sys/io.h>

#include<string.h>

 #include <sys/types.h>

 #include <dirent.h>

 #include <sys/stat.h>

 #include<stdlib.h>

Symbol table_of_class[500];

Symbol table_of_method[100];

int class_table_number;

int parse_time;

char file_list[32][500];
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

int readFileList(char *basePath) //传入的其实是 project_name
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
	int i = 0;
	char full_file_name[500];

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8) {   ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
			sprintf(full_file_name,"%s%s%s",basePath,"/",ptr->d_name);
			strcpy(file_list[i++], full_file_name);
			}
        else if(ptr->d_type == 10) {   ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
			sprintf(full_file_name,"%s%s%s",basePath,"/",ptr->d_name);
			strcpy(file_list[i++], full_file_name);
			}
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base);
        }
    }
    closedir(dir);
	
	
    return i; //返回这个文件夹下有多少个文件
}



ParserInfo compile(char* dir_name)

{

	clear_class_table();
	strcpy(project_name, dir_name);
	parse_time = 1;
	ParserInfo ps;
	Add_library_to_class_table();

	int i = readFileList(project_name) ;

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
    parse_time++;
	for (int j = 0; j < i; j++) {
		InitParser(file_list[j]);
		ps = Parse();
		//ps = Parse(file_list[j], 2);
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


