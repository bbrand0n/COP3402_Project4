/* 
COP3402 Spring 2022
Author Names: Brandon Gibbons, Aiden Ahern, Steven Horn
*/


// Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define MAX_REG_LENGTH 10
#define MAX_DATA_LENGTH 50
#define MAX_PROGRAM_LENGTH 150


// Print output
void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int RP, int *data_stack, int *register_stack)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.l, IR.m, PC, BP, SP, RP);
	
	// print register stack
	for (i = MAX_REG_LENGTH - 1; i >= RP; i--)
		printf("%d ", register_stack[i]);
	printf("\n");
	
	// print data stack
	printf("\tdata stack : ");
	for (i = 0; i <= SP; i++)
		printf("%d ", data_stack[i]);
	printf("\n");
}


// Base level
int base(int L, int BP, int *data_stack)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = data_stack[rtn];
		ctr--;
	}
	return rtn;
}


// Execute
void execute_program(instruction *code, int printFlag)
{
	// Initialize values
	int halt = 1;
	int BP = 0;
	int SP = BP - 1;
	int PC = 0;
	int RP = MAX_REG_LENGTH;
	struct instruction IR;
	int ds[MAX_DATA_LENGTH] = {0};
	int rs[MAX_REG_LENGTH] = {0};
	char *opname;
	int line = PC;

	if (1)
	{
		printf("\t\t\t\tPC\tBP\tSP\tRP\n");
		printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, RP);
	}

	// Loop until exit code reached
	while(halt == 1) {


		// ---------- FETCH CYCLE ----------
		IR = code[PC];
		line = PC;
		PC += 1;


		// --------- EXECUTE CYCLE ----------
		switch(IR.opcode) {

			// LIT
			case 1:  RP -= 1;
					 rs[RP] = IR.m;
					 opname = "LIT";
					 break;
			
			// OPR
			case 2:
					// RET 
					if(IR.m == 0){
						ds[SP+1] = base(IR.l, BP, ds);
						ds[SP+2] = BP;
						ds[SP+3] = PC;
						SP = BP-1;
						BP = ds[SP+2];
						PC = ds[SP+3];
						opname = "RET";
					}
					// NEG
					if(IR.m == 1) {
						rs[RP] = (-1) * rs[RP];
						opname = "NEG";
					}
					// ADD
					if(IR.m == 2) {
						rs[RP+1] = rs[RP] + rs[RP+1];
						RP++;
						opname = "ADD";
					}
					// SUB
					if(IR.m == 3) {
						rs[RP+1] = rs[RP+1] - rs[RP];
						RP++;
						opname = "SUB";
					}
					// MUL
					if(IR.m == 4) {
						rs[RP+1] = rs[RP] * rs[RP+1];
						RP++;
						opname = "MUL";
					}
					// DIV
					if(IR.m == 5) {
						rs[RP+1] = rs[RP+1] / rs[RP];
						RP++;
						opname = "DIV";
					}
					// EQL
					if(IR.m == 6) {
						if(rs[RP] == rs[RP+1]) rs[RP+1] = 1;
						else				   rs[RP+1] = 0;
						RP++;
						opname = "EQL";
					}
					// NEQ
					if(IR.m == 7) {
						if(rs[RP] == rs[RP+1]) rs[RP+1] = 0;
						else				   rs[RP+1] = 1;
						RP++;
						opname = "NEQ";
					}
					// LSS
					if(IR.m == 8) {
						if(rs[RP+1] < rs[RP]) rs[RP+1] = 1;
						else 				  rs[RP+1] = 0;
						RP++;
						opname = "LSS";
					}
					// LEQ
					if(IR.m == 9) {
						if(rs[RP+1] <= rs[RP]) rs[RP+1] = 1;
						else 				   rs[RP+1] = 0;
						RP++;
						opname = "LEQ";
					}
					// GTR
					if(IR.m == 10) {
						if(rs[RP+1] > rs[RP])  rs[RP+1] = 1;
						else 				   rs[RP+1] = 0;
						RP++;
						opname = "GTR";
					}	
					// GEQ
					if(IR.m == 11) {
						if(rs[RP+1] >= rs[RP]) rs[RP+1] = 1;
						else 				   rs[RP+1] = 0;
						RP++;
						opname = "GEQ";
					}	
					// AND (HW4)
					if(IR.m == 12) {
						if(rs[RP+1] && rs[RP]) rs[RP+1] = 1;
						else 				   rs[RP+1] = 0;
						RP++;
						opname = "AND";
					}	
					// ORR (HW4)
					if(IR.m == 13) {
						if(rs[RP+1] || rs[RP]) rs[RP+1] = 1;
						else 				   rs[RP+1] = 0;
						RP++;
						opname = "ORR";
					}
					// NOT (HW4)
					if(IR.m == 14) {
						//rs[RP+1] = 1 - rs[RP+1];  maybe try this if it doesnt work?
						rs[RP+1] = !rs[RP+1];
						RP++;
						opname = "NOT";
					}	
					 break;
			// LOD
			case 3:  RP -= 1;
					 rs[RP] = ds[base(IR.l, BP, ds) + IR.m];
					 opname = "LOD";
					 break;
			// STO
			case 4: ds[base(IR.l, BP, ds) + IR.m] = rs[RP];
					 opname = "STO";
					 RP += 1;
			    	 break;
			// CAL
			case 5:  ds[SP+1] = base(IR.l, BP, ds);
					 ds[SP+2] = BP;
					 ds[SP+3] = PC;
					 BP = SP+1;
					 PC = IR.m;
					 opname = "CAL";
					 break;
			// INC
			case 6: SP = SP + IR.m;
					 opname = "INC";
					 break;
			// JMP
			case 7: PC = IR.m;
					 opname = "JMP";
					 break;
			// JPC
			case 8: if(rs[RP] == 0) PC = IR.m;
					RP++;
					opname = "JPC";
					break;
			case 9: 
					// WRT
					if(IR.m == 1) {
						printf("Top of Stack Value: %d\n", rs[RP]);
						RP += 1;
						opname = "WRT";
					} 
					// RED
					if(IR.m == 2){
						printf("Please Enter an Integer: ");
						RP -= 1;
						scanf("%d", &rs[RP]);
						printf("\n");
						opname = "RED";
					}
					// HAL
					if(IR.m == 3){
						halt = 0;
						opname = "HAL";
					}
					break;
		}

		// Print values and stack at end of loop
		print_execution(line, opname, IR, PC, BP, SP, RP, ds, rs);
	}	
}
	