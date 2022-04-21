/* 
COP3402 Spring 2022
Author Names: Brandon Gibbons, Aiden Ahern, Steven Horn
*/


// Headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"


// Constants
#define MAX_CODE_LENGTH 150
#define MAX_SYMBOL_COUNT 20
#define MAX_REG_HEIGHT 10


// Global variables
instruction *code;
int cIndex;
symbol *table;
int cIndex;
int tIndex;
int tokenIndex;
int level;
int error;
int count;


// Default function declarations
void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void mark();
int multipledeclarationcheck(char name[]);
int findsymbol(char name[], int kind);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();


// Added function declarations
void block(lexeme *list);
void constant_decl(lexeme *list);
int  variable_decl(lexeme *list);
void procedure_decl(lexeme *list);
void statement(lexeme *list);
void expression(lexeme *list);
void condition(lexeme *list);
void term(lexeme *list);
void factor(lexeme *list);
void logic(lexeme *list);




// Parser main function ***(includes PROGRAM function)
instruction *parse(lexeme *list, int printTable, int printCode)
{
	// Initialize
	code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
	table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
	tIndex = 0;
	cIndex = 0;
	tokenIndex=0;
	
	// JMP then add main to symbol table
	emit(7, 0, 0);
	addToSymbolTable(3, "main", 0, 0, 0, 0);

	// Set level to -1 since BLOCK increments it
	level = -1;

	// Enter block function to pass lex level and address
	block(list);

	// Ensure token ends with period
	if(list[tokenIndex].type != periodsym) printparseerror(1); 

	// HALT
	emit(9, 0, 3);

	// Fix M vals of CAL instructions
	for(int i=0; i < cIndex; i++){
		if(code[i].opcode == 5)
			code[i].m = table[code[i].m].addr;
	}

	// Fix M val of initial JMP
	code[0].m = table[0].addr;

	// Print table and code if flagged
	// if (printTable)	printsymboltable();
	// if (printCode)	printassemblycode();
	printsymboltable();
	printassemblycode();

	// End of array
	code[cIndex].opcode = -1;

	// Return result
	return code;
}


// ********************************************************* BLOCK
void block(lexeme *list) {

	// Increment level
	level++;
	int procIndex = tIndex - 1;
	int x = 0;


	// Constant declaration
	if(list[tokenIndex].type == constsym) constant_decl(list);
	// Variable declaration
	if(list[tokenIndex].type == varsym)   x = variable_decl(list);
	// Procedure declaration
	procedure_decl(list);
	

	// Store code index in procedure addr field so we can use it for CALs
	table[procIndex].addr = cIndex;

	// INC
	emit(6, 0, x + 3);

	// Statement
	statement(list);

	// Mark symbols
	mark();

	// Decrement level
	level--;

}



// ********************************************************* CONSTANT DECLARATION
void constant_decl(lexeme *list) {

	// Loop while symbol is comma
	do {

		// Increment token index
		tokenIndex++;

		// Check for missing identity symbol
		if(list[tokenIndex].type != identsym)	printparseerror(2);

		// Check for multiple declarations
		int sIndex = multipledeclarationcheck(list[tokenIndex].name);

		// Throw error 19 if multiple delcarations of const
		if(sIndex != -1) printparseerror(19);

		// Grab identifier name for adding symbol name to table
		char iName[12];
		strcpy(iName, list[tokenIndex].name);



		// Increment token index again
		tokenIndex++;

		// Check for missing :=
		if(list[tokenIndex].type != assignsym)	printparseerror(2);



		// Increment token index again
		tokenIndex++;

		// Check for missing number / literal
		if(list[tokenIndex].type != numbersym)	printparseerror(2);



		// Finally, add to symbol table
		addToSymbolTable(1, iName, list[tokenIndex].value, level, 0, 0);
		tokenIndex++;


	} while(list[tokenIndex].type == commasym);


	// Check for semicolon symbol
	if(list[tokenIndex].type != semicolonsym) {
		// Check for extra identity symbol
		if(list[tokenIndex].type == identsym) printparseerror(13);
		else								  printparseerror(14);
	}

	// Increment token index 
	tokenIndex++;
}


// ********************************************************* VARIABLE DECLARATION
int variable_decl(lexeme *list) {

	// Keep track of how many vars being declared
	int numVars = 0;


	// Loop while symbol is comma
	do {

		// Increment token index
		tokenIndex++;

		// Check for identity symbol
		if(list[tokenIndex].type != identsym) printparseerror(3);

		// Check for multiple declaration
		int sIndex = multipledeclarationcheck(list[tokenIndex].name);
		if(sIndex != -1) printparseerror(18);

		// Add symbol to table
		addToSymbolTable(2, list[tokenIndex].name, 0, level, numVars+3, 0);

		// Increment numVars token index
		numVars++;
		tokenIndex++;

	} while (list[tokenIndex].type == commasym);


	// Check for semicolon symbol
	if(list[tokenIndex].type != semicolonsym) {
		// Check for extra identity symbol
		if(list[tokenIndex++].type == identsym) printparseerror(13);
		else								    printparseerror(14);
	}

	// Increment token index
	tokenIndex++;

	// Return number of vars
	return numVars;
}



// ********************************************************* PROCEDURE DECLARATION
void procedure_decl(lexeme *list) {

	// Loop while token is procedure symbol
	while(list[tokenIndex].type == procsym) {

		// Increment token index
		tokenIndex++;

		// Check for identity symbol (for procedures)
		if(list[tokenIndex].type != identsym) printparseerror(4);

		// Check for multiple declaration
		int sIndex = multipledeclarationcheck(list[tokenIndex].name);
		if(sIndex != -1) printparseerror(19);

		// Add to symbol table
		addToSymbolTable(3, list[tokenIndex].name, 0, level, 0, 0);



		// Increment token index
		tokenIndex++;

		// Check for semicolon
		if(list[tokenIndex].type != semicolonsym) printparseerror(4);



		// Increment token index
		tokenIndex++;

		// Call block for this procedure
		block(list);
		
		// Check for semicolon when we return from block
		if(list[tokenIndex].type != semicolonsym) printparseerror(14);

		
		// Increment token index one more time
		tokenIndex++;

		// RET
		emit(2, 0, 0);
	}
}


// ********************************************************* STATEMENT
void statement(lexeme *list) {

	// Check for identity symbol
	if(list[tokenIndex].type == identsym) {

		// Find index of token in the symbol table
		int sIndex = findsymbol(list[tokenIndex].name, 2);

		// If not found
		if(sIndex == -1) {
			// Missing idetifier OR not a variable
			if(findsymbol(list[tokenIndex].name, 1) != findsymbol(list[tokenIndex].name, 3)) printparseerror(6);

			// Undeclared identifier
			else printparseerror(19);
		}


		// Increment token index
		tokenIndex++;
		// Check for assignment symbol
		if(list[tokenIndex].type != assignsym) printparseerror(5);


		// Increment token index
		tokenIndex++;
		// Expression
		expression(list);


		// STO
		emit(4, level-table[sIndex].level, table[sIndex].addr);
		count--;
		return;
	}


	// Check for begin symbol
	if(list[tokenIndex].type == beginsym) {

		// Loop while symbol is semicolon
		do {
			tokenIndex++;
			statement(list);
		} while(list[tokenIndex].type == semicolonsym);

		// Expecting end symbol
		if(list[tokenIndex].type != endsym) {

			// Check for other symbols
			if(list[tokenIndex].type == callsym || list[tokenIndex].type == identsym || list[tokenIndex].type == ifsym 
				|| list[tokenIndex].type == whilesym || list[tokenIndex].type == beginsym || list[tokenIndex].type == readsym 
					|| list[tokenIndex].type == writesym )   {
						 printparseerror(15);
						 }
			else printparseerror(16);

	
		}
		
		// Increment token index
		tokenIndex++;
		return;
	}


	// Check for if symbol
	if(list[tokenIndex].type == ifsym) {

		// Increment token index
		tokenIndex++;

		// This is a condition statement
		//condition(list);

		// Call LOGIC instead of condition (HW4)
		logic(list);

		// Check for then symbol
		if(list[tokenIndex].type != thensym) printparseerror(8);

		// Store current code index in JPC
		int jIndex = cIndex;
		// JPC (don't know where yet)
		emit(8, 0, 0);
		count--;



		// Increment token index
		tokenIndex++;

		// Statement
		statement(list);


		// Check for else symbol
		if(list[tokenIndex].type == elsesym) {
			int jmpIndex = cIndex;
			// JMP, dont know where yet
			emit(7, 0, 0);
			code[jIndex].m = cIndex;
			tokenIndex++;
			statement(list);
			code[jmpIndex].m = cIndex;
		}
		else code[jIndex].m = cIndex;

		// Return
		return;
	}

	// Check for while symbol
	if(list[tokenIndex].type == whilesym) {

		// Increment token index
		tokenIndex++;
		// Need to know where to jump back to
		int loopIndex = cIndex;

		// Condition
		//condition(list);

		// Instead of condition we call logic (HW4)
		logic(list);


		// Check for do while symbol
		if(list[tokenIndex].type != dosym) printparseerror(9);

		// Increment token index
		tokenIndex++;

		// JMC
		int jpcIndex = cIndex;
		emit(8, 0, 0);
		count--;

		// Statement
		statement(list);

		// JMP M
		emit(7, 0, loopIndex );
		code[jpcIndex].m = cIndex ;

		// Return
		return;
	}


	// Check for read statements
	if(list[tokenIndex].type == readsym) {

		// Increment token index
		tokenIndex++;

		// Ensure identity symbol
		if(list[tokenIndex].type != identsym) printparseerror(6);

		// Find index of symbol from symbol table
		int sIndex = findsymbol(list[tokenIndex].name, 2);

		// Can't find symbol
		if(sIndex == -1) {
			if(findsymbol(list[tokenIndex].name, 1) != findsymbol(list[tokenIndex].name, 3)) printparseerror(18);
			else printparseerror(19);
		}

		// Increment token index
		tokenIndex++;


		// READ
		emit(9, 0, 2);
		// STO
		emit(4, level - table[sIndex].level, table[sIndex].addr);
		count--;

		// Return
		return;
	}


	// Check for write symbol
	if(list[tokenIndex].type == writesym) {

		// Increment token index
		tokenIndex++;
		// Expression
		expression(list);
		// Write
		emit(9, 0, 1);
		// Return
		return;
	}


	// Check for call statements
	if(list[tokenIndex].type == callsym) {

		// Increment token index
		tokenIndex++;

		// Find symbol index from symbol table
		int sIndex       = findsymbol(list[tokenIndex].name, 3);
		int sIndex_var   = findsymbol(list[tokenIndex].name, 2);
		int sIndex_const = findsymbol(list[tokenIndex].name, 1);

		// If not found OR not procedure
		if(list[tokenIndex].type != identsym || sIndex_const != -1 || sIndex_var != -1) 
			printparseerror(7);		// Must be followed by procedure identifier 



		// Cant find identifier in table
		else if(sIndex == -1)	printparseerror(19);

		// Increment token index
		tokenIndex++;

		// CALL
		emit(5, level - table[sIndex].level, sIndex);

	}
}


// ********************************************************* EXPRESSION
void expression(lexeme *list) {

	// Check for sub symbol
	if(list[tokenIndex].type == minussym) {

		// Increment token index
		tokenIndex++;
		// Term
		term(list);
		// NEG
		emit(2, 0 ,1);

		// Loop while tooken is addition or subtraction
		while(list[tokenIndex].type == plussym || list[tokenIndex].type == minussym){

			// Add
			if(list[tokenIndex].type == plussym) {
				tokenIndex++;
				term(list);
				emit(2, 0, 2);
				count--;
			}
			// Subtract
			else{
				tokenIndex++;
				term(list);
				emit(2, 0, 3);
				count--;
			}
		}
	}

	// Not a minus symbol
	else {

		// Check for addition
		if(list[tokenIndex].type == plussym) tokenIndex++;
		// Term
		term(list);

		// Loop while tooken is addition or subtraction
		while(list[tokenIndex].type == plussym || list[tokenIndex].type == minussym){

			// Add
			if(list[tokenIndex].type == plussym) {
				tokenIndex++;
				term(list);
				emit(2, 0, 2);
				count--;
			}
			// Subtract
			else{
				tokenIndex++;
				term(list);
				emit(2, 0, 3);
				count--;
			}
		}
	}


	// Check for bad arithmetic
	if(list[tokenIndex].type == plussym || list[tokenIndex].type == minussym || list[tokenIndex].type == multsym || list[tokenIndex].type == divsym || list[tokenIndex].type == identsym) printparseerror(17);
}


// ********************************************************* CONDITION
void condition(lexeme *list){

	// Check for left parenthesis   (HW4)
	if(list[tokenIndex].type == lparensym) {

		// Increment token index
		tokenIndex++;

		// Logic (HW4)
		logic(list);

		// Must end with right parentheses
		if(list[tokenIndex].type != rparensym) printparseerror(12);
	}
	
	// Symbol is not a (
	else {

		// Expression
		expression(list);

		// Equal symbol
		if(list[tokenIndex].type == eqlsym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 6);
			count--;
		}
		// Not equal symbol
		else if(list[tokenIndex].type == neqsym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 7);
			count--;
		}
		// Less than symbol
		else if(list[tokenIndex].type == lsssym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 8);
			count--;
		}
		// Less than or equal to symbol
		else if(list[tokenIndex].type == leqsym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 9);
			count--;
		}
		// Greater than symbol
		else if(list[tokenIndex].type == gtrsym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 10);
			count--;
		}
		// Greater than or equal to symbol
		else if(list[tokenIndex].type == geqsym) {
			tokenIndex++;
			expression(list);
			emit(2, 0, 11);
			count--;
		}
		// Missing relational op
		else printparseerror(10);

	}

}


// ********************************************************* TERM
void term(lexeme *list) {

	// Factor
	factor(list);

	// Loop while symbol is multiply or divide
	while(list[tokenIndex].type == multsym || list[tokenIndex].type == divsym) {
		// Multiply
		if(list[tokenIndex].type == multsym){
			tokenIndex++;
			factor(list);
			emit(2, 0, 4);
			count--;
		}
		// Divide
		else if(list[tokenIndex].type == divsym){
			tokenIndex++;
			factor(list);
			emit(2, 0, 5);
			count--;
		}
		// Divide ?
		else {
			tokenIndex++;
			factor(list);
			emit(2, 0 , 7);
			count--;
		}
	}
}



// ********************************************************* FACTOR
void factor(lexeme *list) {

	// Check for identity symbol
	if(list[tokenIndex].type == identsym) {

		// Find constants and variables that match in symbol table
		int sIndex_const = findsymbol(list[tokenIndex].name, 1);
		int sIndex_var   = findsymbol(list[tokenIndex].name, 2);
		int sIndex_proc  = findsymbol(list[tokenIndex].name, 3);

		// If no const or var found
		if(sIndex_const == -1 && sIndex_var == -1) {
			// Identifier is a procedure
			if(sIndex_proc != -1) printparseerror(11);
			else printparseerror(19);
		}

		// Ensure register is not full
		if(count != MAX_REG_HEIGHT) {

			// Variable not found
			if(sIndex_var == -1) {
				// LIT M
				emit(1, 0, table[sIndex_const].val);		
				count++;
			}
			else if(sIndex_const == -1 || table[sIndex_var].level > table[sIndex_const].level) { 
				// LOD
				emit(3, level - table[sIndex_var].level, table[sIndex_var].addr);				
				count++;																		
			}
			else {
				// LIT
				emit(1, 0, table[sIndex_const].val);											
				count++;
			}
		}

		// Register is full
		else printparseerror(20);


		// Increment token index
		tokenIndex++;
	}

	// Check for number symbol
	else if(list[tokenIndex].type == numbersym) {

		// Ensure register not full
		if(count != MAX_REG_HEIGHT) {
			// LIT
			emit(1, 0, list[tokenIndex].value);
			count++;
			tokenIndex++;
		}
		// Register is full
		else printparseerror(20);

	}

	// Check for left parenthesis
	else if(list[tokenIndex].type == lparensym) {
		// Increment token index
		tokenIndex++;
		// Expression
		expression(list);
		// Must end with right parenthesis after (expression)
		if(list[tokenIndex].type != rparensym) printparseerror(12);
		// Increment token index
		tokenIndex++;
	}

	// Bad arithmetic
	else printparseerror(11);
}


// ********************************************************* LOGIC
void logic(lexeme *list) {

	// Check for NOT symbol
	if(list[tokenIndex].type == notsym) {

		// Increase token index maybe ???
		tokenIndex++;
		// Condition
		condition(list);
		// Emit NOT
		emit(2, 0, 14);
	}

	else {
		
		// Condition
		condition(list);

		// Loop while token is AND or OR
		while(list[tokenIndex].type == andsym || list[tokenIndex].type == orsym) {

			// And symbol
			if(list[tokenIndex].type == andsym) {
				// Increase index
				tokenIndex++;
				// Condition
				condition(list);
				// Emit AND
				emit(2, 0, 12);
				count--;
			}

			// Or symbol
			else {
				// Increase index
				tokenIndex++;
				// Condition
				condition(list);
				// Emit ORR
				emit(2, 0, 13);
				count--;
			}
		}
	}
}










// ********************************************************* FUNCTIONS PROVIDED BY PROFESSOR
// ********************************************************* (only modification is added exit(0) in printparseerror() )
// adds a line of code to the program
void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

// add a symbol to the symbol table
void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}

// mark the symbols belonging to the current procedure, should be called at the end of block
void mark()
{
	int i;
	for (i = tIndex - 1; i >= 0; i--)
	{
		if (table[i].mark == 1)
			continue;
		if (table[i].level < level)
			return;
		table[i].mark = 1;
	}
}

// checks if a new symbol has a valid name, by checking if there's an existing symbol
// with the same name in the procedure
int multipledeclarationcheck(char name[])
{
	int i;
	for (i = 0; i < tIndex; i++)
		if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
			return i;
	return -1;
}

// returns the index of a symbol with a given name and kind in the symbol table
// returns -1 if not found
// prioritizes lower lex levels
int findsymbol(char name[], int kind)
{
	int i;
	int max_idx = -1;
	int max_lvl = -1;
	for (i = 0; i < tIndex; i++)
	{
		if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
		{
			if (max_idx == -1 || table[i].level > max_lvl)
			{
				max_idx = i;
				max_lvl = table[i].level;
			}
		}
	}
	return max_idx;
}

// Print parser error
void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		case 20:
			printf("Parser Error: Register Overflow Error\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
	exit(0);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RET\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("EQL\t");
						break;
					case 7:
						printf("NEQ\t");
						break;
					case 8:
						printf("LSS\t");
						break;
					case 9:
						printf("LEQ\t");
						break;
					case 10:
						printf("GTR\t");
						break;
					case 11:
						printf("GEQ\t");
						break;
					case 12:
						printf("AND\t");
						break;
					case 13:
						printf("ORR\t");
						break;
					case 14:	
						printf("NOT\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}