//
// Created by Jesse Woods on 9/5/26.
//
#include "token_stream.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

TokenStream* create_stream()
{
    const auto stream = (TokenStream*)malloc(sizeof(TokenStream));
    if (stream == nullptr)
    {
        printf("Error: malloc failed\n");
        return nullptr;
    }
    stream->tokens = nullptr;
    stream->size = 0;
    stream->capacity = 0;

    return stream;

}
void push_token(TokenStream* stream, Token* token)
{
    if (stream == nullptr || token == nullptr)
    {
        printf("Error: Token Stream or Token is null\n");
        return;
    }

    if (stream->size >= stream->capacity)
    {
        const size_t new_capacity = stream->capacity == 0 ? 32 : stream->capacity * 2;

        //Allocate space for pointers (Token*), not structures (Token)
        const auto new_tokens = (Token**)realloc(stream->tokens, new_capacity * sizeof(Token*));
        if (new_tokens == nullptr)
        {
            printf("Error: realloc failed\n");
            return;
        }

        stream->tokens = new_tokens;
        stream->capacity = new_capacity;
    }

    //Stores the actual pointer from the lexer, as opposed to a shallow copy
    stream->tokens[stream->size] = token;
    stream->size++;
}
void pop_token(TokenStream* stream)
{
    if (stream != nullptr)
    {
        if (stream->size > 0){
            stream->size--;
        }
    }
    else
    {
        printf("Error popping token: Token Stream is null\n");
    }
}
Token* token_at(const TokenStream* stream, const size_t index)
{
    if (stream == nullptr || index >= stream->size)
    {
        printf("Size: %lu\n", stream->size);
        printf("Error: index out of bounds or stream is null\n");
        return nullptr;
    }
    return stream->tokens[index];
}
size_t token_count(const TokenStream* stream)
{
    return stream->size;
}
bool is_empty(const TokenStream* stream)
{
    return stream->size == 0;
}
void print_stream(const TokenStream* stream)
{
    if (stream == nullptr) return;

    for (size_t i = 0; i < stream->size; i++)
    {
        if (stream->tokens[i] == nullptr) continue;

        printf("Token: ");
        print_str(stream->tokens[i]->slice);
        printf("Line number: %lu Column number: %lu\n", stream->tokens[i]->line, stream->tokens[i]->column);
        printf("Type: %d\n", stream->tokens[i]->type);
    }
}
void free_stream(const TokenStream* stream) {
    if (stream == nullptr) return;

    for (size_t i = 0; i < stream->size; i++) {
        if (stream->tokens[i] != nullptr) {
            free_token(stream->tokens[i]); // Passes the original pointer
        }
    }
    free(stream->tokens);
    free((void*)stream);
}


