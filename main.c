#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "include/lexer/lexer.h"
#include "include/lexer/token_stream/token_stream.h"
#include "parser/parser.h"

static bool isCFile(const char *);
static char* stringify(const char*);
static void lex_file(const char*);
static void parse_file(const char*);
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
        lex_file(optarg);
      }
      break;
    case 'p':
      printf("Parser called\n");
      fileFromArgs = optarg;
      if (isCFile(fileFromArgs)) {
        printf("%s\n", fileFromArgs);
      }
      parse_file(fileFromArgs);
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
static void lex_file(const char* file) {
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
static void parse_file(const char* file) {
  char* file_contents = stringify(file);
  const TokenStream* stream = lexical_analyzer(file_contents);
  parse_token_stream(stream);
  free_stream(stream);
  free(file_contents);
}
static void run_tests() {
  lex_file("tests/test.c");
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
char* stringify(const char* filename) {

  //Open file in binary mode for accurate file size
  FILE* c_program = fopen(filename, "rb");
  if (c_program == nullptr) {
    printf("Error: could not open file for reading.\n");
    return nullptr;
  }
  //Find end of file to determine size
  if (fseek(c_program, 0, SEEK_END) != 0) {
    printf("Error: could not seek to end of file.\n");
    fclose(c_program);
    return nullptr;
  }
  //retrieve numerical representation of number of bytes in file
  const long long_size = ftell(c_program);
  if (long_size < 0) {
    printf("Error: could not read file size.\n");
    fclose(c_program);
    return nullptr;
  }
  //ftell explicitly returns a long, so we cast to size_t
  const size_t size = (size_t)long_size;

  //Reset pointer to beginning of file
  if (fseek(c_program, 0, SEEK_SET) != 0) {
    printf("Error: could not seek to start of file.\n");
    fclose(c_program);
    return nullptr;
  }

  //allocate memory +1 byte for '\0'
  const auto file_contents = (char*)malloc(size + 1);
  if (file_contents == nullptr) {
    printf("Error: could not allocate memory for file contents.\n");
    fclose(c_program);
    return nullptr;
  }
  //Read file
  const size_t bytes_read = fread(file_contents, sizeof(char), size, c_program);
  if (bytes_read != size) {
    printf("Error reading file.\n");
    free(file_contents);
    fclose(c_program);
    return nullptr;
  }
  //set null-terminator
  file_contents[bytes_read] = '\0';
  fclose(c_program);
  return file_contents;
}

