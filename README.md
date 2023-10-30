# Jack_Compiler
C-language based compiler sperifically designed for the Jack language
Sorts of errors:
        none,					// no errors 0 
	lexerErr,			// lexer error (if the lexer returns an error) 1
	classExpected,			// keyword class expected 2
	idExpected,			// identifier expected 3
	openBraceExpected,		// { expected 4
	closeBraceExpected,		// } expected 5
	memberDeclarErr,		// class member declaration must begin with static, field, constructor , function , or method 6
	classVarErr,			// class variables must begin with field or static 7
	illegalType,			// a type must be int, char, boolean, or identifier 8
	semicolonExpected,		// ; expected 9
	subroutineDeclarErr,	// subrouting declaration must begin with constructor, function, or method 10
	openParenExpected,		// ( expected 11
	closeParenExpected,		// ) expected 12
        openBracketExpected,	// [ expected 13
	closeBracketExpected,	// ] expected 14
	equalExpected,			// = expected 15
	syntaxError				// any other kind of syntax error 16
  syntaxNoError     // any other none syntax errors 17
