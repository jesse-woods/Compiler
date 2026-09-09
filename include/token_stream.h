//
// Created by jwoods on 9/5/26.
//

#ifndef COMPILER_TOKEN_STREAM_H
#define COMPILER_TOKEN_STREAM_H
#include "token.h"

typedef struct {

    Token* tokens;
    size_t size;
    size_t capacity;

}TokenStream;

TokenStream* create_stream();
void push_token(TokenStream*,const Token*);
void pop_token(TokenStream*);
Token* token_at(const TokenStream*, size_t);
size_t token_count(const TokenStream*);
bool is_empty(const TokenStream*);
void print_stream(const TokenStream*);
void free_stream(const TokenStream*);

#endif //COMPILER_TOKEN_STREAM_H
