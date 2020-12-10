#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "assembly.h"

int write_token(token *theToken, FILE *writeFile, int *wasDefun, int *ifCounter, int *compareCounter)
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
            fprintf(writeFile, "%s", "JSR\n");
            fprintf(writeFile, "%s\n", theToken->str);
            //fprintf(writeFile, "%s". "ADD R6, R6, #-1");
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
        *compareCounter++;
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
        *compareCounter++;
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
        *compareCounter++;
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
        *compareCounter++;
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
        *compareCounter++;
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
        break;
    case ELSE:
        break;
    case ENDIF:
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
        break;
    case LITERAL:
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