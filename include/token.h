//
// Created by jwoods on 9/5/26.
//
#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <stddef.h>
#include "string_slice.h"
enum TOKEN_TYPE
{
    IDENTIFIER,
    INTEGER,
    KEYWORD,
    PUNCTUATION,// For ;, ,, etc.
    SEMICOLON,
    COMMA,
    L_PARENTHESES,     // Or give crucial ones their own explicit type
    R_PARENTHESES,
    L_BRACE,
    R_BRACE,
    L_BRACKET,
    R_BRACKET,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    PLUS_EQUAL,
    MINUS_EQUAL,
    TIMES_EQUAL,
    DIVIDE_EQUAL,
    MODULO_EQUAL,
    INCREMENT,
    DECREMENT,
    UNDEFINED,
};
typedef struct
{
    enum TOKEN_TYPE type;
    const Str* token;
    size_t line;
    size_t column;

}Token;
void free_token(Token*);
#endif //COMPILER_TOKEN_H
