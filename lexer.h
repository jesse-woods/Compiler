//
// Created by jwoods on 8/6/26.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H
#include <stdio.h>
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
typedef struct
{
    char* strSlice;
    size_t length;
} Str;

enum TOKEN_TYPE
{
    IDENTIFIER,
    CONSTANT,
    KEYWORD,
    OPERATOR,
    SEMICOLON,
    COMMA,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
};
typedef struct
{
    enum TOKEN_TYPE type;
    Str token;
}Token;
static Str sliceString(char*, size_t, size_t);
static bool isDelimiter(char);
#endif //COMPILER_LEXER_H
