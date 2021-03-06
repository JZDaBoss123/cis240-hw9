#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "assembly.h"
#include "stack.h"

int main(int argc, char **argv)
{
    char *filename;
    char *writename;
    char *extension;
    FILE *writeFile;
    FILE *readFile;

    //argv[1] is to write
    //read files
    extension = ".asm";
    filename = argv[1];
    writename = malloc((strlen(filename) + 10) * sizeof(char));
    writename = strncpy(writename, filename, strlen(filename) - 2);
    writename = strcat(writename, extension);

    readFile = fopen(filename, "r");
    writeFile = fopen(writename, "w");
    char *line = malloc(250 * sizeof(char)); //just read one token if needed
    char *tokenString;
    token theToken;
    stack theStack;
    init_stack(&theStack);
    int wasDefun = 0;
    int ifCounter = 0;
    int compareCounter = 0;
    while (fgets(line, 250, readFile) != NULL)
    {
        tokenString = strtok(line, "\n\t\f\v\r ");
        while (tokenString != NULL)
        {
            if (tokenString[0] == ';')
            {
                break;
            }
            if (read_token(&theToken, tokenString) == 1)
            {
                printf("bad token read");
                fclose(readFile);
                fclose(writeFile);
                free(writename);
                free(line);
            }
            // printf(tokenString);
            // printf("\n");
            write_token(&theToken, writeFile, &wasDefun, &ifCounter, &compareCounter, &theStack);
            tokenString = strtok(NULL, "\n\t\f\v\r ");
        }
    }

    fclose(readFile);
    fclose(writeFile);
    free(writename);
    free(line);
}
