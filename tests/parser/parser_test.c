//
// Created by jessewoods on 10/21/26.
//

#include <string.h>
#include <stdlib.h>
#include "tests/parser_test.h"
#include "file_processor.h"
#include "lexer/lexer.h"
#include "parser/parser.h"



#include "lexer/token_stream/token_stream.h"


void run_parser_tests() {
    char* file_contents = read_file_to_memory("tests/parser/parser_function_tests/is_parameter_test_file.c");

    char *line = strtok(file_contents, "\r\n");


    while (line != nullptr) {
        bool expected_result = true;
        if (line[0] == '0'){expected_result = false;}
        const size_t expected_position = line[2] - '0';
        line = line + 4;
        printf("String to be tested: %s\n", line);

        const TokenStream* stream = lexical_analyzer(line);
        test_parse_function(stream, IS_PARAMETER, expected_result, expected_position);
        free_stream(stream);
        line = strtok(nullptr, "\r\n");
    }

    free(file_contents);

}
