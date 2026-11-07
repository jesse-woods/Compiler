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

static int test_runner(char*, enum PARSER_FUNCTION_NAME);

int run_parser_tests(const char * test_name) {

    int return_status = 1;

    printf("\n----------------------------Parser helper function tests---------------------------------\n\n");

    if(strcmp(test_name, "is-parameter") == 0){
        //Run tests for is_parameter functionality
        char* file_contents = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_test_file.c");
        return_status = test_runner(file_contents, IS_PARAMETER);
        free(file_contents);
    }
    else if(strcmp(test_name, "is-parameter-list") == 0){
        //Run tests for is_parameter_list functionality
        char* file_contents_2 = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_list_test_file.c");
        return_status = test_runner(file_contents_2, IS_PARAMETER_LIST);
        free(file_contents_2);
    }
    else if(strcmp(test_name, "is-function-definition") == 0){
        //Run tests for is_function_definition functionality
        char* file_contents_3 = read_file_to_memory("tests/parser/parser_function_tests/is_function_definition_test_file.c");
        return_status = test_runner(file_contents_3, IS_FUNCTION_DEFINITION);
        free(file_contents_3);
    }
    else{
        printf("ERROR: invalid argument for test function entered.\n");
    }
    return return_status;
}
int test_runner(char* file_contents, const enum PARSER_FUNCTION_NAME function_name) {

    char *line = strtok(file_contents, "\r\n");
    bool passed_all_tests = true;
    while (line != nullptr) {
        bool expected_result = true;
        if (line[0] == '0'){expected_result = false;}
        const size_t expected_position = ((line[2] - '0') * 10) + (line[3] - '0');
        line = line + 5;

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
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
