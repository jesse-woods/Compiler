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
    const char* strSlice;
    size_t length;
} Str;

enum TOKEN_TYPE
{
    IDENTIFIER,
    INTEGER,
    KEYWORD,
    OPERATOR,
    PUNCTUATION, // For ;, ,, etc.
    L_PAREN,     // Or give crucial ones their own explicit type
    R_PAREN,
    L_BRACE,
    R_BRACE,
    L_BRACK,
    R_BRACK,
    UNDEFINED,
};
typedef struct
{
    enum TOKEN_TYPE type;
    Str token;
    size_t line;
    size_t column;
}Token;
static const Str* sliceString(const char*, size_t, size_t);
static bool isDelimiter(char);
static bool isOperator(char);
static bool strEqualsCStr(const Str*, const char*);
static bool isKeyword(const Str*);
static bool isValidIdentifier(const Str*);
static bool isInteger(const Str*);
static bool compare(const Str*, const Str*);
static bool isValidStr(const Str*);
void lexicalAnalyzer(const char*);
static void printStr(const Str*);

#endif //COMPILER_LEXER_H
