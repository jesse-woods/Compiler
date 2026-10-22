//
// Created by jessewoods on 10/7/26.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "lexer/token_stream/token_stream.h"
#include "parser/parser_function_names.h"

void parse_token_stream(const TokenStream*);
void test_parse_function(const TokenStream*, enum PARSER_FUNCTION_NAME);
#endif //COMPILER_PARSER_H
