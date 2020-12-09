#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "assembly.h"

int write_token(token *theToken, FILE *writeFile)
{

    switch (theToken->type)
    {
    case DEFUN:
        break;
    case IDENT:
        break;
    case RETURN:
        break;
    case PLUS:
        break;
    case MINUS:
        break;
    case MUL:
        break;
    case DIV:
        break;
    case MOD:
        break;
    case AND:
        break;
    case OR:
        break;
    case NOT:
        break;
    case LT:
        break;
    case LE:
        break;
    case EQ:
        break;
    case GE:
        break;
    case GT:
        break;
    case IF:
        break;
    case ELSE:
        break;
    case ENDIF:
        break;
    case DROP:
        break;
    case DUP:
        break;
    case SWAP:
        break;
    case ROT:
        break;
    case ARG:
        break;
    case LITERAL:
        break;
    case BROKEN_TOKEN:
        break;
    default:
        break;
    }
}