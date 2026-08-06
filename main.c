#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>


static bool isCFile(const char *);

int main(const int argc, char *argv[])
{
    int opt = 0;

    while ((opt = getopt(argc, argv, "al:p:")) != -1) {
        const char *fileFromArgs;
        switch (opt)
        {
            case 'a':
            printf("Option a selected\n");
            break;
            case 'l':
            printf("Lexer called\n");
            fileFromArgs = optarg;
            if (isCFile(optarg))
            {
                printf("%s\n", optarg);
            }
            break;
            case 'p':
            printf("Parser called\n");
            fileFromArgs = optarg;
            if (isCFile(fileFromArgs))
            {
                printf("%s\n", fileFromArgs);
            }
            break;
            case '?':
            fprintf(stderr, "Usage: %s [-l] [path]  Run Lexer\n       %s [-p] [path]  Run Parser\n", argv[0], argv[0]);
            return EXIT_FAILURE;
            default:
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

static bool isCFile(const char *fileName)
{
    if (fileName == NULL || strlen(fileName) < 3)
    {
        return false;
    }
    printf("filetype: %s\n", fileName + strlen(fileName) - 2);
    return strcmp(fileName + strlen(fileName) - 2 , ".c") == 0;
}