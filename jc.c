#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "assembly.h"

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
    char *token;

    while (fgets(line, 250, readFile) != NULL)
    {
        token = strtok(line, "\n\t\f\v\r ");
        while (token != NULL)
        {
            if (token[0] == ';')
            {
                break;
            }

            printf(token);
            printf("\n");
            token = strtok(NULL, "\n\t\f\v\r ");
        }
    }

    fclose(readFile);
    fclose(writeFile);
    free(writename);
    free(line);
}
