//
// Created by Jesse Woods on 10/7/26.
//
#include "parser/parser.h"

#include "lexer/token_stream/token_stream.h"
#include "parser/parser_function_names.h"

static bool is_type_specifier(const Token * token) {

    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    static const char* type_specifiers[9] = {
        "char", "double", "float", "int", "long", "short", "signed", "unsigned", "void"
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
static bool is_declarator(const Token * token) {
    //TODO actually implement declarator recognization
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type == IDENTIFIER) {
        return true;
    }
    return false;
}
static bool is_specifier_qualifier(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (is_type_specifier(token) || is_type_qualifier(token)) {
        return true;
    }
    return false;
}
/*---------------------------------------------------------------
 * Parser recognition functions
 * All recognition functions that take a TokenStream* and a size_t*
 * follow the same cursor contract:
 * SUCCESS:
 *   Return true. *position will be advanced to the Token*
 *   immediately following recognized grammatical structure
 * FAILURE:
 *   Return false and leave *position unchanged.
 *
 * Position is updated only upon success.
 *---------------------------------------------------------------*/
static bool is_parameter(const TokenStream *stream , size_t *position) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    if (*position > stream->size) {
        printf("Attempt to parse a function definition beyond end of TokenStream.\n");
        return false;
    }

    size_t temp_pos = *position;
    while (temp_pos < stream->size
        && (is_declaration_specifier(token_at(stream, temp_pos)) || token_at(stream, temp_pos)->type == IDENTIFIER)) {
        temp_pos++;
    }
    if (temp_pos >= stream->size) { return false; }
    if (token_at(stream, temp_pos)->type != COMMA && token_at(stream, temp_pos)->type != R_PARENTHESES) {
        return false;
    }
    if (temp_pos - *position > 4) { return false; }
    if (temp_pos - *position == 3) {
        if (token_at(stream, temp_pos)->type == IDENTIFIER) { temp_pos++; }
        else{ return false; }
        //more stuff
    }
    if (temp_pos - *position == 2) {
        if (token_at(stream, temp_pos)->type == IDENTIFIER && is_type_specifier(token_at(stream, temp_pos - 1))
            && !compareStrToCString(token_at(stream,temp_pos -1)->slice, "void")) {
            *position = temp_pos;
            return true;
            }
    }
    if (temp_pos - *position == 1) {
        if (token_at(stream, temp_pos)->type == IDENTIFIER) {
            *position = temp_pos;
            return true;
        }
    }
}
static bool is_parameter_list(const TokenStream* stream, size_t * position) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    if (*position > stream->size) {
        printf("Attempt to parse a function definition beyond end of TokenStream.\n");
        return false;
    }

    size_t temp_pos = *position;
    if (token_at(stream, temp_pos)->type == R_PARENTHESES) {
        temp_pos++;
        *position = temp_pos;
        return true;
    }
    if (temp_pos < stream->size -1 && compareStrToCString(token_at(stream, temp_pos)->slice, "void") && token_at(stream, temp_pos + 1)->type == R_PARENTHESES) {
        temp_pos+=2;
        *position = temp_pos;
        return true;
    }
    while (temp_pos < stream->size && token_at(stream, temp_pos)->type != R_PARENTHESES) {
        if (token_at(stream, temp_pos)->type != COMMA && !is_type_specifier(token_at(stream, temp_pos))) { return false; }
        if (token_at(stream, temp_pos)->type == COMMA) { temp_pos++; }
        if (is_type_specifier(token_at(stream, temp_pos))) {
            if (compareStrToCString(token_at(stream, temp_pos)->slice, "void")) { temp_pos++; }
            else {
                if (temp_pos < stream->size - 1 && token_at(stream, temp_pos + 1)->type == IDENTIFIER) { temp_pos+=2; } else { return false; }
            }
        }
    }

    if (token_at(stream, temp_pos)->type != COMMA) {
        *position = temp_pos;
        return true;
    }
    return false;
}
static bool is_function_definition(const TokenStream *stream , size_t *position) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    if (*position > stream->size) {
        printf("Attempt to parse a function definition beyond end of TokenStream.\n");
        return false;
    }
    size_t temp_pos = *position;

    if (!is_declaration_specifier(token_at(stream, temp_pos))) {
        return false;
    }
    while (is_declaration_specifier(token_at(stream, temp_pos))) {
        temp_pos++;
    }
    bool contains_type_specifier = false;
    for (size_t i = *position; i < temp_pos; i++) {

        if (is_type_specifier(token_at(stream, *position))) {
            contains_type_specifier = true;
        }
    }
    if (!contains_type_specifier) { return false; }
    if (temp_pos < stream->size && is_declarator(token_at(stream, temp_pos))) {
        temp_pos++;
    }
    else{ return false; }
    if (temp_pos < stream->size && token_at(stream, temp_pos)->type == L_PARENTHESES) {
        temp_pos++;
        //send to is_parameter_list
    }
    else { return false; }

    *position = temp_pos;
    return true;
}
void parse_token_stream(const TokenStream* stream) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return;
    }
    size_t position = 0;
    if (is_function_definition(stream, &position)) {
        printf("Working so far...\n");
    }
    printf("New position: %lu\n", position);
}
void test_parse_function(const TokenStream *stream,const enum PARSER_FUNCTION_NAME function_name) {
    if (function_name == IS_PARAMETER) {
        size_t position = 0;
        const bool function_return_value = is_parameter(stream, &position);
        printf("Test 1:\nFunction returned: %hhd  New position: %lu\n", function_return_value, position);
    }
}

