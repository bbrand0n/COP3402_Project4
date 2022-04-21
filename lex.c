/* 
COP3402 Spring 2022
Author Names: Brandon Gibbons, Aiden Ahern, Steven Horn
*/

// Header
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5


// Globals
lexeme *list;
int lex_index;


// Functions
int alphaToken(char * input, int inputIndex);
int numberToken(char * input, int inputIndex);
int symbolToken(char * input, int inputIndex);
int reservedCheck(char *buffer);
void printlexerror(int type);
void printtokens();



// Letter token
int alphaToken(char * input, int inputIndex)
{
	// To store current and next char
	char curChar = input[inputIndex];
	char nextChar = input[inputIndex + 1];
	int curIndex = inputIndex;
	
	// Store curChar in buffer
	char buffer[13] = "\0";
	strncat(buffer, &curChar, 1);
	
	// Loop through buffer
	while (strlen(buffer) < 12)
	{
		// If digit or letter
		if (isdigit(nextChar) || isalpha(nextChar))
		{
			// Get next char and add to buffer
			curChar = nextChar;
			curIndex++;
			strncat(buffer, &curChar, 1);
			nextChar = input[curIndex + 1];
			continue;
		}
		
		// Break when char is no longer letter or digit
		break;
	}
	
	
	/// Identifier Length Error
	if (strlen(buffer) > 11)
		return -1;
	

	/// If buffer is a reserved word, it will be handled within reservedCheck
	/// Otherwise buffer is an identifier
	if (reservedCheck(buffer) == 0)
	{
		list[lex_index].type = identsym;
		list[lex_index].value = identsym;
		strcpy(list[lex_index++].name, buffer);
	}
	
	return ++curIndex;
}


// Check for reserved word
int reservedCheck(char * buffer)
{
	token_type type;
	
	if (strcmp(buffer, "var") == 0)
		type = varsym;
	else if (strcmp(buffer, "procedure") == 0)
		type = procsym;
	else if (strcmp(buffer, "call") == 0)
		type = callsym;
	else if (strcmp(buffer, "begin") == 0)
		type = beginsym;
	else if (strcmp(buffer, "end") == 0)
		type = endsym;
	else if (strcmp(buffer, "if") == 0)
		type = ifsym;
	else if (strcmp(buffer, "then") == 0)
		type = thensym;
	else if (strcmp(buffer, "else") == 0)
		type = elsesym;
	else if (strcmp(buffer, "do") == 0)
		type = dosym;
	else if (strcmp(buffer, "read") == 0)
		type = readsym;
	else if (strcmp(buffer, "write") == 0)
		type = writesym;
	else if (strcmp(buffer, "while") == 0)
		type = whilesym;
	else if (strcmp(buffer, "const") == 0)
		type = constsym;
	else
		return 0;
	
	// Copy over 
	list[lex_index].value = type;
	list[lex_index++].type = type;
	strcpy(list[lex_index].name, buffer);
	return 1;
}

int numberToken(char * input, int inputIndex)
{
	// Get current and next char
	char curChar = input[inputIndex];
	char nextChar = input[inputIndex + 1];
	int curIndex = inputIndex;
	
	// Buffer
	char buffer[7] = "\0";
	strncat(buffer, &curChar, 1);
	
	// Loop over buffer
	while (strlen(buffer) < 6)
	{
		// Invalid identifier
		if (isalpha(nextChar))
			return -2;
		
		// Is digit
		else if (isdigit(nextChar))
		{
			// Add to buffer
			curChar = nextChar;
			curIndex++;
			strncat(buffer, &curChar, 1);
			nextChar = input[curIndex + 1];
			continue;
		}
		
		// Other char
		break;
	}
	
	// Number Length Error
	if (strlen(buffer) > 5)
		return -1;
	

	// Copy over
	list[lex_index].type = numbersym;
	list[lex_index++].value = atoi(buffer);
	strcpy(list[lex_index].name, "numbersym");
	return ++curIndex;
}


// Non alphanumeric character
int symbolToken(char * input, int inputIndex)
{
	// Get current and next char
	char curChar = input[inputIndex];
	char nextChar = input[inputIndex + 1];
	int curIndex = inputIndex;
	char curString[] = {curChar, '\0'};
	
	// Determine what the cahracter is
	switch (curChar)
	{
		case '.':
			list[lex_index++].type = periodsym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case ',':
			list[lex_index++].type = commasym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case ';':
			list[lex_index++].type = semicolonsym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case '(':
			list[lex_index++].type = lparensym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case ')':
			list[lex_index++].type = rparensym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case '*':
			list[lex_index++].type = multsym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case '-':
			list[lex_index++].type = minussym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case '+':
			list[lex_index++].type = plussym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case '<':
			// LEQ
			if (nextChar == '=') {
				char tempString[] = {'<', '=', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = leqsym;
				return inputIndex + 2;
			}
			list[lex_index++].type = lsssym;
			strcpy(list[lex_index].name, curString);
			return ++inputIndex;

		case ':':
			// Assignment
			if (nextChar == '=') {
				char tempString[] = {':', '=', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = assignsym;
				strcpy(list[lex_index].name, curString);
				return inputIndex + 2;
			}
			// Not valid
			printf("Next char: %c\n", nextChar);
			return -1;

		case '=':
			if (nextChar == '=')
			{
				char tempString[] = {'=', '=', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = eqlsym;
				return inputIndex + 2;
			}
			// Not valid
			printf("Next char= %c\n", nextChar);
			return -1;

		case '!': 
			// NEQ
			if (nextChar == '=')
			{
				char tempString[] = {'!', '=', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = neqsym;
				return inputIndex + 2;
			}

		case '>':
			// GEQ
			if (nextChar == '=')
			{
				char tempString[] = {'>', '=', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = geqsym;
				return inputIndex + 2;
			}

			strcpy(list[lex_index].name, curString);
			list[lex_index++].type = gtrsym;
			return ++inputIndex;

		case '/':
      
			// Comments
			if (nextChar == '/') {
				curChar = nextChar;
				curIndex++;
				while (curChar != '\n' && curChar != '\0')
				{
					curChar = input[++curIndex];
				}
				
				return curIndex;
			}
			// Block comment
			else if(nextChar == '*') {
				curChar = nextChar;
				curIndex++;
				while (1)
				{
					// Reached max capacity, never ending comment
					if(curIndex == MAX_NUMBER_TOKENS) {
						printlexerror(5);
						exit(0);
					}

					// End of comment
					if(curChar == '*' && input[curIndex+1] == '/') return curIndex+2;
					curChar = input[++curIndex];
				}
				
				return curIndex;
			}
			
			strcpy(list[lex_index].name, curString);
			list[lex_index++].type = divsym;
			return ++inputIndex;

		case '&':
			// AND
			if (nextChar == '&')
			{
				char tempString[] = {'&', '&', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = andsym;
				return inputIndex + 2;
			}

    
		case '|':
			// ORR
			if (nextChar == '|')
			{
				char tempString[] = {'|', '|', '\0'};
        		strcpy(list[lex_index].name, tempString);
				list[lex_index++].type = orsym;
				return inputIndex + 2;
			}

		// Invalid symbol
		default:
			return -1;
	}
}

lexeme *lexanalyzer(char *input, int printFlag)
{
	// Initialize
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	lex_index = 0;
	int inputIndex = 0;
	char curChar = input[inputIndex];
	
	// Loop while char is not termination
	while (curChar != '\0')
	{
		// Space
		if (iscntrl(curChar) || isspace(curChar))
		{
			curChar = input[++inputIndex];
			continue;
		}
		
		// Digit
		else if (isdigit(curChar))
		{
			inputIndex = numberToken(input, inputIndex);
			
			// number length error
			if (inputIndex == -1)
			{
				if (printFlag) 
					printlexerror(2);
				return NULL;
				
			}
			
			// invalid identifier error
			else if (inputIndex == -2)
			{
				if (printFlag)
					printlexerror(1);	
				return NULL;
			}
			
			/// No errors
			else
			{
				curChar = input[inputIndex];
				continue;
			}
		}
		
		// Letter
		else if (isalpha(curChar))
		{
			inputIndex = alphaToken(input, inputIndex);
			
			/// Identifier Length Error
			if (inputIndex == -1)
			{
				// Print error and return
				if (printFlag) 
					printlexerror(3);
				return NULL;
			}
			
			/// No error
			else
			{
				curChar = input[inputIndex];
				continue;
			}
		}
		
		// Other
		else
		{
			inputIndex = symbolToken(input, inputIndex);
			
			/// Invalid Symbol Error
			if (inputIndex == -1)
			{
				if (printFlag)
					printlexerror(4);
				return NULL;
			}
			
			/// No error
			else
			{
				curChar = input[inputIndex];
				continue;
			}
		}
	}
	
	if (1)
		printtokens();
	list[lex_index].type = -1;
	return list;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case plussym:
				printf("%11s\t%d", "+", plussym);
				break;
			case minussym:
				printf("%11s\t%d", "-", minussym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
  printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
}


void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 5)
		printf("Lexical Analyzer Error: Never-ending comment\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");

	free(list);
	return;
}
