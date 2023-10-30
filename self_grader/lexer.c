/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design the and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:
Student ID:
Email:
Date Work Commenced:
*************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE
#define MaxKeyWord  21  //the number of keyword
#define MaxSymNum 19    //number of symbols


char character = ' ';
int lineNumber = 1;
FILE* f_in = NULL;
FILE* f_out = NULL;
char fileName[32];
Token t_spare;
extern char project_name[20];

const char* const KeyWord[MaxKeyWord] =
{
	"class","constructor","method","function","int","boolean","char",
	"void","var","static","field","let","do","if","else","while","return",
	"true","false","null","this"
};
const char Symbol[MaxSymNum] =
{
	'(', ')' ,'[',']','{','}',',',';','=','.','+','-','*','/','&','|','~','<','>'
};
char lexeme[128];

int isNumber() {
	if (character >= '0' && character <= '9')
		return 1;
	return 0;
}
int isLetter() {
	if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
		return 1;
	return 0;
}
int is_() {
	if (character == '_')
		return 1;
	return 0;
}
void getNextChar(FILE* f_in) {
	character = getc(f_in);
	if (character=='\r') lineNumber++;
};
void getNextNotBlankChar(FILE* f_in) {
	getNextChar(f_in);
	while (character == ' ' || character == '\n' || character == '\r' || character == '\0' || character == '\t') {
		getNextNotBlankChar(f_in);
	}
};
void retract(FILE* f) {
	fseek(f, -1L, 1);
	character = '\0';
}
void connect() {
	char* c = &character;
	strcat(lexeme, c);
}
int isKeyWord() {
	int i = 0;
	for (i = 0; i < MaxKeyWord; i++) {
		if (strcmp(lexeme, KeyWord[i]) == 0)
			return 1;
	}
	return 0;
}
Token initToken(Token* t) {
	t->tp = ERR;
	strcpy(t->lx, "\0");
	t->ln = 0;
	strcpy(t->fl, " ");
	t->ec = -1;
	t_spare.tp = ERR;
	strcpy(t_spare.lx, "\0");
	t_spare.ln = 0;
	strcpy(t_spare.fl, " ");
	t_spare.ec = -1;
	strcpy(lexeme, "\0");
	return *t;
}
char* TypeOfToken(Token t)
{
	switch (t.tp)
	{
	case RESWORD: return "RESWORD";
	case ID: return "ID";
	case INT: return "INT";
	case SYMBOL: return "SYMBOL";
	case  STRING: return " STRING";
	case EOFile: return "EOFile";
	case ERR: return "ERR";
	default: return "there is an error";
	}
}

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer(char* file_name)
{
	lineNumber = 1;
	f_in = fopen(file_name, "rb");//"Pong/Ball.jack"
	strcpy(fileName, file_name);
	if (f_in == NULL)
		return 0;
	else return 1;
}


// Get the next token from the source file
Token GetNextToken()
{
	Token t;
	t = initToken(&t);
	//char c;
	getNextNotBlankChar(f_in);
BEGIN:
	switch (character) {
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
		while (isLetter() || isNumber() || is_()) {
			connect();
			getNextChar(f_in);
		}
		if (character != EOF) {
			if (character == '\n' || character == '\r') lineNumber--;
			retract(f_in);
		}
		if (isKeyWord())
			t.tp = RESWORD;
		else t.tp = ID;
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '0':
		while (isNumber()) {
			connect();
			getNextChar(f_in);
		}
		retract(f_in);
		t.tp = INT;
		break;
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}':
	case ',':
	case ';':
	case '=':
	case '.':
	case '+':
	case '-':
	case '&':
	case '|':
	case '~':
	case '<':
	case '>':
		connect();
		t.tp = SYMBOL;
		break;
	case '"':
		while (1) {
			getNextChar(f_in);
			if (character == EOF) { t.tp = ERR; t.ec = 2; goto END; }
			if (character == '\r' || character == '\n') { t.tp = ERR; t.ec = 1; t.ln--; goto END; }
			if (character == '"') { t.tp = STRING; goto END; }
			connect();
		}
		break;
	case '/':
		getNextChar(f_in);
		if (character != '/' && character != '*') {
			strcpy(lexeme, "/");
			fseek(f_in, -1L, 1);
			t.tp = SYMBOL;
		}
		else if (character == '/') {
			getNextChar(f_in);
			while (character != '\n' && character != EOF) {
				getNextChar(f_in);
			}
			if (character == '\n') {
				getNextNotBlankChar(f_in);
			}
			goto BEGIN;
		}
		else if (character == '*') {
		state0:
			getNextChar(f_in);
			if (character == EOF) {
			state1:
				t.tp = ERR;
				t.ec = 0;
				goto END;
			}
			if (character == '*') {
			state2:
				getNextChar(f_in);
				if (character == EOF) goto state1;
				if (character == '/') { getNextNotBlankChar(f_in); goto BEGIN; }
				else goto state0;
			}
			else {
			state3:
				getNextChar(f_in);
				if (character == EOF) goto state1;
				if (character == '*') goto state2;
				else goto state3;
			}
		}
		break;
	case '*':
		connect();
		t.tp = SYMBOL;
		break;
	case EOF:
		t.tp = EOFile;
		break;
	default:
		t.tp = ERR;
		t.ec = 3;
		break;
	}

END:
	strcpy(t.lx, lexeme);
	strcpy(t.fl, fileName);
	t.ln = lineNumber;
	if (t.tp == EOFile) strcpy(t.lx, "End of File");
	if (t.tp == ERR) {
		switch (t.ec) {
		case 0: strcpy(t.lx, "Error: unexpected eof in comment"); break;
		case 1: strcpy(t.lx, "Error: new line in string constant"); break;
		case 2: strcpy(t.lx, "Error: unexpected eof in string constant"); break;
		case 3: strcpy(t.lx, "Error: illegal symbol in source file"); break;
		default: break;
		}
	}
	t_spare.tp = t.tp;
	t_spare.ec = t.ec;
	t_spare.ln = t.ln;
	strcpy(t_spare.lx, t.lx);
	strcpy(t_spare.fl, t.fl);
	return t;

};

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken() {
	GetNextToken();
	int l = (strlen(t_spare.lx));

	if (t_spare.tp == STRING)
		fseek(f_in, -l - 2, 1);
	else if (t_spare.tp == EOFile) fseek(f_in, 0, SEEK_END);
	else
		fseek(f_in, -l, 1);
	return t_spare;
};

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer()
{
	/*fclose(f_in);
	fclose(f_out);*/
	return 0;
};

// do not remove the next line
#ifndef TEST
int main()
{
	// implement your main function here
  // NOTE: the autograder will not use your main function
	char* c = "Main.jack";
	f_out = fopen("Main.txt", "w");
	InitLexer(c);
	while (1) {
		t_spare = GetNextToken(f_in);
		fprintf(f_out, "< %s, %d, %s,%s > \n", t_spare.fl, t_spare.ln, t_spare.lx, TypeOfToken(t_spare));
		if (t_spare.tp == EOFile) break;
		//fputs("Ball.jack \n", f_out);
	}
STOP:
	StopLexer();
	return 0;
}
// do not remove the next line
#endif
