//
// Created by jessewoods on 10/21/26.
//
#include "tests/parser_test.h"

#include <stdlib.h>

#include "file_processor.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "lexer/token_stream/token_stream.h"


void run_parser_tests() {
    char* file_contents = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_test_file.c");
    printf("String to be tested: %s\n", file_contents);
    const TokenStream* stream = lexical_analyzer(file_contents);
    printf("TokenStream: ");
    print_stream(stream);
    test_parse_function(stream, IS_PARAMETER);

    free(file_contents);
    free_stream(stream);
}
