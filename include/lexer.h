//
// Created by jwoods on 8/6/26.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H
#include "token_stream.h"


static bool isDelimiter(char);
static bool isOperator(char);
static bool isPunctuation(char);
char* stringify(const char*);
TokenStream* lexical_analyzer(const char*);
TokenStream* lexical_analyzer2(const char*, size_t);


#endif //COMPILER_LEXER_H
