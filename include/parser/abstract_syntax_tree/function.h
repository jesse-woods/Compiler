//
// Created by jessewoods on 10/7/26.
//

#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H
#include "lexer/token_stream/token.h"
#include "parser/abstract_syntax_tree/function_body.h"

typedef struct {
    Token* identifier;
    FunctionBody* body;
}Function;
#endif //COMPILER_FUNCTION_H
