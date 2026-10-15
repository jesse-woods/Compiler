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
        if (token_at(stream, pos)) {}
        print_token(token_at(stream, pos));

        pos++;
    }
}
static bool is_type_specifier(const Token * token) {

    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    static const char* type_specifiers[9] = {
        "char", "double", "float", "int", "long", "short", "signed","unsigned", "void"
    };
    for (int i = 0; i < 9; i++) {
        if (compareStrToCString(token->slice, type_specifiers[i])) {
            return true;
        }
    }
    return false;
}

static bool is_storage_class_specifier(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    static const char* storage_class_specifiers[5] = {
        "auto", "register", "static", "extern", "typedef"
    };
    for (int i = 0; i < 5; i++) {
        if (compareStrToCString(token->slice, storage_class_specifiers[i])) {
            return true;
        }
    }
    return false;
}

static bool is_type_qualifier(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    if (compareStrToCString(token->slice, "const") || compareStrToCString(token->slice, "volatile")) {
        return true;
    }
    return false;
}

static bool is_struct_or_union(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    if (compareStrToCString(token->slice, "struct") || compareStrToCString(token->slice, "union")) {
        return true;
    }
    return false;
}

static bool is_declaration_specifier(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (is_storage_class_specifier(token) || is_type_specifier(token) || is_type_qualifier(token)) {
        return true;
    }
    return false;
}
