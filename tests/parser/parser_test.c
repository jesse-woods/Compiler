//
// Created by jessewoods on 10/21/26.
//

#include <string.h>
#include <stdlib.h>
#include "tests/parser_test.h"
#include "file_processor.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/parser_function_names.h"
#include "lexer/token_stream/token_stream.h"

static void test_runner(char*, enum PARSER_FUNCTION_NAME);
void run_parser_tests() {

    //Run tests for is_parameter functionality
    char* file_contents = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_test_file.c");
    test_runner(file_contents, IS_PARAMETER);
    free(file_contents);
    //Run tests for is_parameter_list functionality
    char* file_contents_2 = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_list_test_file.c");
    test_runner(file_contents_2, IS_PARAMETER_LIST);
    free(file_contents_2);
}
void test_runner(char* file_contents, const enum PARSER_FUNCTION_NAME function_name) {

    char *line = strtok(file_contents, "\r\n");
    bool passed_all_tests = true;
    while (line != nullptr) {
        bool expected_result = true;
        if (line[0] == '0'){expected_result = false;}
        const size_t expected_position = line[2] - '0';
        line = line + 4;

        const TokenStream* stream = lexical_analyzer(line);
        if (!test_parse_function(stream, function_name, expected_result, expected_position)) {
            printf("Input that failed: %s\n\n", line);
            passed_all_tests = false;
        }
        free_stream(stream);
        line = strtok(nullptr, "\r\n");
    }
    if (passed_all_tests) {
        printf("Parser function number: %d All tests passed!\n\n", function_name);
    }
}
