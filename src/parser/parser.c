//
// Created by Jesse Woods on 10/7/26.
//
#include "parser/parser.h"

#include "lexer/token_stream/token_stream.h"

void parse_token_stream(const TokenStream* stream) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return;
    }
    size_t pos = 0;
    while (pos < stream->size) {
        print_token(token_at(stream, pos));
        pos++;
    }
}
