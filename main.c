#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "include/lexer/lexer.h"
#include "include/lexer/token_stream/token_stream.h"
#include "parser/parser.h"
#include "include/file_processor.h"
#include "tests/parser_test.h"

static bool isCFile(const char *);
static void lex_file(const char*);
static void parse_file(const char*);
static int run_tests(const char* test);



int main(const int argc, char *argv[]) {

  if(argc < 2) {
    fprintf(stderr,
           "Usage: %s [-l] [path]  Run Lexer\n       %s [-p] [path]  Run "
           "Parser\n       %s [-t]         Run Tests\n",
           argv[0], argv[0], argv[0]);
  }
  int opt = 0;
  while ((opt = getopt(argc, argv, "at:l:p:")) != -1) {
    const char *argument;
    switch (opt) {
    case 'a':
      printf("Option a selected\n");
      break;
    case 'l':
      printf("Lexer called\n");
      argument = optarg;
      if (isCFile(optarg)) {
        printf("%s\n", optarg);
        lex_file(optarg);
      }
      break;
    case 'p':
      printf("Parser called\n");
      argument = optarg;
      if (isCFile(argument)) {
        printf("%s\n", argument);
        parse_file(argument);
      }

      break;
    case 't':
        printf("Tests called\n");
        argument = optarg;
        printf("%s\n", argument);
        return run_tests(argument);
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
static void lex_file(const char* file) {
  char* file_contents = read_file_to_memory(file);
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
static void parse_file(const char* file) {
  char* file_contents = read_file_to_memory(file);
  const TokenStream* stream = lexical_analyzer(file_contents);
  parse_token_stream(stream);
  free_stream(stream);
  free(file_contents);
}
static int run_tests(const char * test) {

  int return_value = 1;

  if(strcmp(test, "lexer-test") == 0){
    lex_file("tests/lexer/test.c");
    char*  golden_file_contents = read_file_to_memory("tests/lexer/test.golden");
    char* compared_test = read_file_to_memory("out.l");


    if (strcmp(compared_test, golden_file_contents) == 0) {
      printf("Files match! Test passed.\n");
      return_value = EXIT_SUCCESS;
    }
    else {
      printf("Files do not match! Test failed.\n");
      return_value = EXIT_FAILURE;
    }
    free(golden_file_contents);
    free(compared_test);
  }
  else{
    return_value = run_parser_tests(test);
  }
  return return_value;
}



