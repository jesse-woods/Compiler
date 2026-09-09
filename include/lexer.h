//
// Created by jwoods on 8/6/26.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <stdio.h>

#include "string_slice.h"
#include "token_stream.h"
/*-------------------------------------------------------------
while input isn't empty:
if input starts with whitespace:
trim whitespace from start of input
else:
find longest match at start of input for any regex
if no match is found, raise an error
convert matching substring into a token
remove matching substring from start of input
-------------------------------------------------------------*/



static bool isDelimiter(char);
static bool isOperator(char);
static bool compareStrToCString(const Str*, const char*);
static bool isKeyword(const Str*);
static bool isValidIdentifier(const Str*);
static bool isInteger(const Str*);
static bool isValidStr(const Str*);
char* stringify(const char*, size_t);
TokenStream* lexical_analyzer(const char*, size_t);

#endif //COMPILER_LEXER_H
