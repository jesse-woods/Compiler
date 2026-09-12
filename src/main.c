#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "lexer.h"
#include "token_stream.h"

static bool isCFile(const char *);
static void processFile(const char*);



int main(const int argc, char *argv[]) {
  int opt = 0;
  while ((opt = getopt(argc, argv, "al:p:")) != -1) {
    const char *fileFromArgs;
    switch (opt) {
    case 'a':
      printf("Option a selected\n");
      break;
    case 'l':
      printf("Lexer called\n");
      fileFromArgs = optarg;
      if (isCFile(optarg)) {
        printf("%s\n", optarg);
        processFile(optarg);
      }
      break;
    case 'p':
      printf("Parser called\n");
      fileFromArgs = optarg;
      if (isCFile(fileFromArgs)) {
        printf("%s\n", fileFromArgs);
      }
      break;
    case '?':
      fprintf(stderr,
              "Usage: %s [-l] [path]  Run Lexer\n       %s [-p] [path]  Run "
              "Parser\n",
              argv[0], argv[0]);
      return EXIT_FAILURE;
    default:
      return EXIT_FAILURE;
    }
  }


  return EXIT_SUCCESS;
}

static bool isCFile(const char *fileName) {
  if (fileName == nullptr || strlen(fileName) < 3) {
    return false;
  }
  printf("filetype: %s\n", fileName + strlen(fileName) - 2);
  return strcmp(fileName + strlen(fileName) - 2, ".c") == 0;
}
static void processFile(const char* file) {
  char* file_contents = stringify(file);
  printf("Contents: %s\n", file_contents);
  const TokenStream* stream = lexical_analyzer(file_contents);

  free_stream(stream);
  free(file_contents);

}

