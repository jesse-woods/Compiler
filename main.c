#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "lexer.h"
#include "token_stream.h"

static bool isCFile(const char *);
static void process_file(const char*);
static void run_tests();



int main(const int argc, char *argv[]) {
  int opt = 0;
  while ((opt = getopt(argc, argv, "atl:p:")) != -1) {
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
        process_file(optarg);
      }
      break;
    case 'p':
      printf("Parser called\n");
      fileFromArgs = optarg;
      if (isCFile(fileFromArgs)) {
        printf("%s\n", fileFromArgs);
      }
      break;
    case 't':
        printf("Tests called\n");
        run_tests();
        break;
    case '?':
      fprintf(stderr,
              "Usage: %s [-l] [path]  Run Lexer\n       %s [-p] [path]  Run "
              "Parser\n       %s [-t]         Run Tests\n",
              argv[0], argv[0], argv[0]);
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
static void process_file(const char* file) {
  char* file_contents = stringify(file);
  const TokenStream* stream = lexical_analyzer(file_contents);
  FILE* fp = fopen("out.l", "w");

  if (fp == nullptr) {
    printf("ERROR: Could not create or open .l file\n");
    return;
  }

  print_stream_to_file(fp, stream);
  fclose(fp);
  free_stream(stream);
  free(file_contents);

}
static void run_tests() {
  process_file("tests/test.c");
  char*  golden_file_contents = stringify("tests/test.golden");
  char* compared_test = stringify("out.l");

  if (strcmp(compared_test, golden_file_contents) == 0) {
    printf("Files match! Test passed.\n");
  }
  else {
    printf("Files do not match! Test failed.\n");
  }

  free(golden_file_contents);
  free(compared_test);
}

