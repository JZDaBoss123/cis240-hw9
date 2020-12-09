#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"

int read_token(token *theToken, char *tokenString)
{
    //check for ident in the end since it can be arbitrary characters.
    int i;
    if (tokenString == NULL)
    {
        printf("null string");
        return 1;
    }
    if (strcmp(tokenString, "defun") == 0)
    {
        theToken->type = DEFUN;
        return 0;
    }

    else if (strcmp(tokenString, "return") == 0)
    {
        theToken->type = RETURN;
        return 0;
    }

    else if (strcmp(tokenString, "+") == 0)
    {
        theToken->type = PLUS;
        return 0;
    }

    else if (strcmp(tokenString, "-") == 0)
    {
        theToken->type = MINUS;
        return 0;
    }

    else if (strcmp(tokenString, "*") == 0)
    {
        theToken->type = MUL;
        return 0;
    }

    else if (strcmp(tokenString, "/") == 0)
    {
        theToken->type = DIV;
        return 0;
    }

    else if (strcmp(tokenString, "%") == 0)
    {
        theToken->type = MOD;
        return 0;
    }

    else if (strcmp(tokenString, "and") == 0)
    {
        theToken->type = AND;
        return 0;
    }

    else if (strcmp(tokenString, "or") == 0)
    {
        theToken->type = OR;
        return 0;
    }

    else if (strcmp(tokenString, "not") == 0)
    {
        theToken->type = NOT;
        return 0;
    }

    else if (strcmp(tokenString, "lt") == 0)
    {
        theToken->type = LT;
        return 0;
    }

    else if (strcmp(tokenString, "le") == 0)
    {
        theToken->type = LE;
        return 0;
    }

    else if (strcmp(tokenString, "eq") == 0)
    {
        theToken->type = EQ;
        return 0;
    }

    else if (strcmp(tokenString, "ge") == 0)
    {
        theToken->type = GE;
        return 0;
    }

    else if (strcmp(tokenString, "gt") == 0)
    {
        theToken->type = GT;
        return 0;
    }

    else if (strcmp(tokenString, "if") == 0)
    {
        theToken->type = IF;
        return 0;
    }

    else if (strcmp(tokenString, "else") == 0)
    {
        theToken->type = ELSE;
        return 0;
    }

    else if (strcmp(tokenString, "endif") == 0)
    {
        theToken->type = ENDIF;
        return 0;
    }

    else if (strcmp(tokenString, "drop") == 0)
    {
        theToken->type = DROP;
        return 0;
    }

    else if (strcmp(tokenString, "dup") == 0)
    {
        theToken->type = DUP;
        return 0;
    }

    else if (strcmp(tokenString, "swap") == 0)
    {
        theToken->type = SWAP;
        return 0;
    }

    else if (strcmp(tokenString, "rot") == 0)
    {
        theToken->type = ROT;
        return 0;
    }

    //missing arg, literal, ident, broken token
    else if (tokenString[0] == 'a' && tokenString[1] == 'r' && tokenString[2] == 'g')
    {
        //arg
        if (strlen(tokenString) == 4)
        {
            if (isdigit(tokenString[3]))
            {
                theToken->type = ARG;
                theToken->arg_no = tokenString[3] - '0';
                return 0;
            }
        }
        else if (strlen(tokenString) == 5)
        {
            if (isdigit(tokenString[3]) && isdigit(tokenString[4]))
            {
                theToken->type = ARG;
                char number[2] = {tokenString[3], tokenString[4]};
                int argNumber = strtol(number, NULL, 10);
                if (argNumber > 20)
                {
                    printf("argnumber greater than 20");
                    return 1;
                }
                theToken->arg_no = argNumber;
                return 0;
            }
        }
    }

    //literal
    //segfault here
    else if (isdigit(tokenString[0]) || tokenString[0] == '-')
    {
        int decimalNumber;
        if (sscanf(tokenString, "%d", &decimalNumber) == 1)
        {
            theToken->type = LITERAL;
            theToken->literal_value = decimalNumber;
            return 0;
        }
        else
        {
            printf("invalid number");
            return 1;
        }
    }
    //hex
    else if (tokenString[0] == '0' && tokenString[1] == 'x')
    {
        int hexNumber;
        char *hexDigits = malloc(sizeof(char) * strlen(tokenString));
        strncpy(hexDigits, tokenString + 2, strlen(tokenString) - 1);
        if (sscanf(tokenString, "%X", &hexNumber) == 1)
        {
            theToken->type = LITERAL;
            theToken->literal_value = hexNumber;
            free(hexDigits);
            return 0;
        }
        else
        {
            printf("invalid number");
            free(hexDigits);
            return 1;
        }
    }
    //ident or broken
    else
    {
        if (!isalpha(tokenString[0]))
        {
            printf("does not start with alphabetical char, not a valid ident");
            return 1;
        }
        for (i = 1; i < strlen(tokenString); i++)
        {
            if (tokenString[i] == '_') {
                continue;
            }
            
            if (!isalnum(tokenString[i]))
            {
                printf("ident has invalid character");
                return 1;
            }
        }
        theToken->type = IDENT;
    }
}

//R5 and R6 are the top two elements of the stack, overwrite R5.
//model stack with registers
//not with registers but with memory stack space?
//R6 is frame pointer