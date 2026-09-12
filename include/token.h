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
    SEMICOLON,
    COMMA,
    L_PARENTHESES,
    R_PARENTHESES,
    L_BRACE,
    R_BRACE,
    L_BRACKET,
    R_BRACKET,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
    PLUS,
    MINUS,
    ASTERISK,
    FORWARD_SLASH,
    MODULO,
    EQUAL,
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
    const string_slice* slice;
    size_t line;
    size_t column;

}Token;

Token* create_token(const char*,size_t, size_t, size_t);
static bool isValidStr(const string_slice*);
void print_token(const Token*);
void free_token(Token*);

#endif //COMPILER_TOKEN_H
