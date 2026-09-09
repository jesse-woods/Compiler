//
// Created by Jesse Woods on 9/5/26.
//
#include "token_stream.h"

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
void push_token(TokenStream* stream, const Token* token)
{
    if (stream == nullptr || token == nullptr)
    {
        printf("Error: Token Stream or Token is null\n");
        return;
    }

    // 1. Manage capacity up front
    if (stream->size >= stream->capacity)
    {
        // If capacity is 0, give it a starting size (e.g., 32 tokens). Otherwise, double it.
        const size_t new_capacity = stream->capacity == 0 ? 32 : stream->capacity * 2;

        const auto new_tokens = (Token*)realloc(stream->tokens, new_capacity * sizeof(Token));
        if (new_tokens == nullptr)
        {
            printf("Error: realloc failed\n");
            return; // Original buffer is still intact, prevents memory leak
        }

        stream->tokens = new_tokens;
        stream->capacity = new_capacity;
    }
    stream->tokens[stream->size] = *token;
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
    return &stream->tokens[index]; // Clearer array syntax
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
    if (stream == nullptr)
    {
        printf("Error: stream is null\n");
        return;
    }
    for (size_t i = 0; i < stream->size; i++)
    {
        if (stream->tokens + i == nullptr)
        {
            printf("Error: token is null\n");
            return;
        }
        printf("Token: ");
        print_str(stream->tokens[i].token);
        printf("Line number: %lu Column number: %lu\n", stream->tokens[i].line, stream->tokens[i].column);
    }
}
void free_stream(const TokenStream* stream) {
    if (stream != NULL) {
        for (size_t i = 0; i < stream->size; i++) {
            if (stream->tokens + i != nullptr) {
                free_token(&stream->tokens[i]);
            }
        }
        free((void*)stream);
    }
}


