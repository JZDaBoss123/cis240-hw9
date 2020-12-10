#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "assembly.h"
#include "stack.h"

int write_token(token *theToken, FILE *writeFile, int *wasDefun, int *ifCounter, int *compareCounter, stack *theStack)
{
    char *prologue = "\tADD R6 R6 #-3\n\tSTR R7 R6 #1\n\tSTR R5 R6 #0\n\tADD R5 R6 #0\n";
    //ldr from r6 because unsure of how many local variables
    char *epilogue = "\tLDR R7 R6 #0\n\tSTR R7 R5 #2\n\tADD R6 R5 #0\n\tLDR R5 R6 #0\n\tLDR R7 R6 #1\n\tADD R6 R6 #3\n\tRET\n";

    //popping from stack
    char *popTwo = "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n";
    char *popOne = "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n";
    char *popThree = "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n\tLDR R3 R6 #0\n\tADD R6 R6 #1\n";
    //store r1
    char *storeOne = "\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n";
    int literalValue;
    int stackNum;
    int stackDenom;
    int argNum;
    switch (theToken->type)
    {
    case DEFUN:
        *wasDefun = 1;
        break;
    case IDENT:
        if (*wasDefun == 1)
        {
            *wasDefun = 0;
            fprintf(writeFile, "%s", ".CODE\n");
            fprintf(writeFile, "%s", ".FALIGN\n");
            //function label
            fprintf(writeFile, "%s\n", theToken->str);
            fprintf(writeFile, "%s", prologue);
            //then body
        }
        else
        {
            fprintf(writeFile, "%s", "JSR ");
            fprintf(writeFile, "%s\n", theToken->str);
            fprintf(writeFile, "%s", "ADD R6, R6, #-1\n");
        }
        break;
    case RETURN:
        fprintf(writeFile, "%s", epilogue);
        break;
    case PLUS:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "ADD R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case MINUS:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "SUB R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case MUL:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "MUL R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case DIV:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "DIV R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case MOD:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "MOD R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case AND:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "AND R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case OR:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "OR R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case NOT:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "NOT R1 R1 R2\n");
        fprintf(writeFile, "%s", storeOne);
        break;
    case LT:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "CMP R1 R2\n");
        fprintf(writeFile, "%s", "BRn ");
        *compareCounter = *compareCounter + 1;
        //branch to lt_n if true
        fprintf(writeFile, "lt_%d\n", *compareCounter);
        //else
        fprintf(writeFile, "%s", "CONST R0 #0\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "JMP lt_end_%d\n", *compareCounter);
        //true branch (if)
        fprintf(writeFile, "lt_%d\n", *compareCounter);
        fprintf(writeFile, "%s", "CONST R0 #1\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "lt_end_%d\n", *compareCounter);
        break;
    case LE:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "CMP R1 R2\n");
        fprintf(writeFile, "%s", "BRnz ");
        *compareCounter = *compareCounter + 1;
        //branch to lt_n if true
        fprintf(writeFile, "le_%d\n", *compareCounter);
        //else
        fprintf(writeFile, "%s", "CONST R0 #0\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "JMP le_end_%d\n", *compareCounter);
        //true branch (if)
        fprintf(writeFile, "le_%d\n", *compareCounter);
        fprintf(writeFile, "%s", "CONST R0 #1\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "le_end_%d\n", *compareCounter);
        break;
    case EQ:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "CMP R1 R2\n");
        fprintf(writeFile, "%s", "BRz ");
        *compareCounter = *compareCounter + 1;
        //branch to lt_n if true
        fprintf(writeFile, "eq_%d\n", *compareCounter);
        //else
        fprintf(writeFile, "%s", "CONST R0 #0\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "JMP eq_end_%d\n", *compareCounter);
        //true branch (if)
        fprintf(writeFile, "eq_%d\n", *compareCounter);
        fprintf(writeFile, "%s", "CONST R0 #1\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "eq_end_%d\n", *compareCounter);
        break;
    case GE:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "CMP R1 R2\n");
        fprintf(writeFile, "%s", "BRzp ");
        *compareCounter = *compareCounter + 1;
        //branch to lt_n if true
        fprintf(writeFile, "ge_%d\n", *compareCounter);
        //else
        fprintf(writeFile, "%s", "CONST R0 #0\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "JMP ge_end_%d\n", *compareCounter);
        //true branch (if)
        fprintf(writeFile, "ge_%d\n", *compareCounter);
        fprintf(writeFile, "%s", "CONST R0 #1\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "ge_end_%d\n", *compareCounter);
        break;
    case GT:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "CMP R1 R2\n");
        fprintf(writeFile, "%s", "BRp ");
        *compareCounter = *compareCounter + 1;
        //branch to lt_n if true
        fprintf(writeFile, "gt_%d\n", *compareCounter);
        //else
        fprintf(writeFile, "%s", "CONST R0 #0\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "JMP gt_end_%d\n", *compareCounter);
        //true branch (if)
        fprintf(writeFile, "gt_%d\n", *compareCounter);
        fprintf(writeFile, "%s", "CONST R0 #1\n");
        fprintf(writeFile, "%s", "STR R0 R6 #-1\n\tADD R6 R6 #-1\n");
        fprintf(writeFile, "gt_end_%d\n", *compareCounter);
        break;
    case IF:
        *ifCounter = *ifCounter + 1;
        add(theStack, *ifCounter, 1);
        fprintf(writeFile, "%s", popOne);
        //branch to else if 0
        fprintf(writeFile, "CMPI R1 #0\n");
        //above will either give z or p
        fprintf(writeFile, "BRz else_%d\n", *ifCounter);
        //rest of if block follows
        break;
    case ELSE:
        stackNum = theStack->first->numerator;
        pop(theStack);
        //encountered an else
        add(theStack, stackNum, -1);
        //previous if block ended, should jump to endif
        fprintf(writeFile, "JMP endif_%d\n", stackNum);
        //label for else
        fprintf(writeFile, "else_%d\n", stackNum);
        break;
    case ENDIF:
        stackNum = theStack->first->numerator;
        stackDenom = theStack->first->denominator;
        pop(theStack);
        if (stackDenom < 0) {
            //encountered an else
            fprintf(writeFile, "endif_%d\n", stackNum);
        } else {
            //no else
            fprintf(writeFile, "else_%d\n", stackNum);
            fprintf(writeFile, "endif_%d\n", stackNum);
        }
        break;
    case DROP:
        fprintf(writeFile, "%s", "ADD R6 R6 #1\n");
        break;
    case DUP:
        fprintf(writeFile, "%s", "LDR R1 R6 #0\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
    case SWAP:
        fprintf(writeFile, "%s", popTwo);
        fprintf(writeFile, "%s", "STR R1 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R2 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
    case ROT:
        fprintf(writeFile, "%s", popThree);
        fprintf(writeFile, "%s", "STR R2 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
    case ARG:
        argNum = theToken->arg_no; //maybe need to +2
        argNum = argNum + 2;
        fprintf(writeFile, "LDR R3 R5 #%d\n", argNum);
        fprintf(writeFile, "STR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
    case LITERAL:
        literalValue = theToken->literal_value;
        if (literalValue > 256 || literalValue < -256)
        {
            //const hiconst
            int constBits = literalValue & 0xFF;
            int hiConstBits = (literalValue >> 8) & 0xFF;
            fprintf(writeFile, "CONST R3 #%d\n", constBits);
            fprintf(writeFile, "HICONST R3 #%d\n", hiConstBits);
            fprintf(writeFile, "STR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        }
        else
        {
            fprintf(writeFile, "CONST R3 #%d\n", literalValue);
            fprintf(writeFile, "STR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        }
        break;
    case BROKEN_TOKEN:
        return 1;
        break;
    default:
        return 1;
        break;
    }
    return 0;
}