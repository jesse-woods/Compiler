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
        if (compare_string_slice_to_c_string(token->slice, type_specifiers[i])) {
            return true;
        }
    }
    return false;
}
/*----------------------------------------------------------------
 * Currently returns true only for keywords "static".
 * TODO: Support "auto", "register", "extern" and "typedef" as well,
 *  when they become valid prefixes.
 * -------------------------------------------------------------*/
static bool is_storage_class_specifier(const Token * token) {
    if (token == nullptr || token->slice == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type == KEYWORD && compare_string_slice_to_c_string(token->slice, "static")) {
        return true;
    }
    //static const char* storage_class_specifiers[5] = {
    //    "auto", "register", "static", "extern", "typedef"
    //};
    //for (int i = 0; i < 5; i++) {
    //    if (compare_string_slice_to_c_string(token->slice, storage_class_specifiers[i])) {
    //        return true;
    //    }
    //}
    return false;
}
static bool is_signed_or_unsigned(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (compare_string_slice_to_c_string(token->slice, "signed") || compare_string_slice_to_c_string(token->slice, "unsigned")) {
        return true;
    }
    return false;
}
/*----------------------------------------------------------------
 * Currently returns true only for keyword "volatile".
 * TODO: Support "const" as well, when it becomes a valid prefix
 * -------------------------------------------------------------*/
static bool is_type_qualifier(const Token * token) {
    if (token == nullptr || token->slice == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type == KEYWORD && compare_string_slice_to_c_string(token->slice, "volatile")) {
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
    if (compare_string_slice_to_c_string(token->slice, "struct") || compare_string_slice_to_c_string(token->slice, "union")) {
        return true;
    }
    return false;
}
static bool is_short_or_long(const Token * token) {
    if (token == nullptr) {
        printf("Error: attempt to parse a NULL token.\n");
        return false;
    }
    if (token->type != KEYWORD) {
        return false;
    }
    if (compare_string_slice_to_c_string(token->slice, "short") || compare_string_slice_to_c_string(token->slice, "long")) {
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
/*----------------------------------------------------------------
 * Current implementation only returns true if token is
 * of type IDENTIFIER
 * TODO implement <declarator> ::= {<pointer>}? <direct-declarator>
 * -------------------------------------------------------------*/
static bool is_declarator(const Token * token) {
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
static bool is_valid_one_specifier_declaration(const TokenStream *stream, size_t *position) {
    //Memory guards
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    if (*position > stream->size - 2) {
        printf("Attempt to parse a function definition beyond end of TokenStream.\n");
        return false;
    }

    //Function logic
    const size_t temporary_position = *position;
    if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice, "void")) { return false; }
    if (is_type_specifier(token_at(stream, temporary_position)) && token_at(stream, temporary_position + 1)->type == IDENTIFIER){
        *position+=2;
        return true;
    }
    return false;
}
static bool is_valid_two_specifier_declaration(const TokenStream *stream, size_t *position) {
    //Memory guards
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    if (*position > stream->size || stream->size - *position < 3) {
        printf("Attempt to parse beyond end of TokenStream.\n");
        return false;
    }

    //Function logic
    const size_t temporary_position = *position;

    if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice, "void") || compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "void")) { return false; }
    if (token_at(stream, temporary_position)->type == KEYWORD && token_at(stream, temporary_position + 1)->type == KEYWORD && token_at(stream, temporary_position + 2)->type == IDENTIFIER) {
        //reject signed signed, unsigned unsigned, signed, unsigned, and unsigned signed
        if (is_signed_or_unsigned(token_at(stream, temporary_position)) && is_signed_or_unsigned(token_at(stream, temporary_position + 1))) { return false; }
        // reject short float, short double, long float, and short long
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"short") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "float")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"short") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "double")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"long") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "float")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"short") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "long")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"long") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "short")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"float") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "short")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"float") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "long")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"double") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "short")) { return false;}


        //reject signed double, unsigned double, signed float, unsigned float
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"signed") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "float")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"signed") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "double")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"unsigned") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "float")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"unsigned") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "double")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"float") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "signed")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"float") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "unsigned")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"double") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "signed")) { return false;}
        if (compare_string_slice_to_c_string(token_at(stream, temporary_position)->slice,"double") && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "unsigned")) { return false;}


        //Handle general case
        if ((is_type_specifier(token_at(stream, temporary_position)) && is_type_qualifier(token_at(stream, temporary_position + 1)))||
            (is_type_qualifier(token_at(stream, temporary_position)) && is_type_specifier(token_at(stream, temporary_position + 1)))||
            (is_signed_or_unsigned(token_at(stream, temporary_position)) && is_type_specifier(token_at(stream, temporary_position + 1)))||
            (is_type_specifier(token_at(stream, temporary_position)) && is_signed_or_unsigned(token_at(stream, temporary_position + 1)))||
            (is_short_or_long(token_at(stream, temporary_position)) && is_type_specifier(token_at(stream, temporary_position + 1)))||
            (is_type_specifier(token_at(stream, temporary_position)) && is_short_or_long(token_at(stream, temporary_position + 1)))) {
            *position+=3;
            return true;
            }
    }
    return false;
}
static bool is_valid_declaration_specifier_sequence(const TokenStream *stream, size_t *position) {
    if (stream == nullptr || stream->tokens == nullptr) {
        printf("Error: attempt to parse a NULL token stream.\n");
        return false;
    }
    if (position == nullptr) {
        printf("Error: attempt to parse a NULL position.\n");
        return false;
    }
    //Function logic
    size_t temporary_position = *position;
    //Function logic
    if (*position + 3 <= stream->size) {
        if (is_valid_two_specifier_declaration(stream, &temporary_position)) {
            *position = temporary_position;
            return true;
        }
        if (is_valid_one_specifier_declaration(stream, &temporary_position)) {
            *position = temporary_position;
            return true;
        }
    }
    if (*position + 2 <= stream->size) {
        if (is_valid_one_specifier_declaration(stream, &temporary_position)) {
            *position = temporary_position;
            return true;
        }
    }
    return false;
}
static bool is_parameter(const TokenStream *stream , size_t *position) {

    //Memory guards
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
    size_t temporary_position = *position;
    //Function logic
    if (is_valid_declaration_specifier_sequence(stream, &temporary_position)) {
        if (temporary_position < stream->size) {
            if (temporary_position == 1 && token_at(stream, temporary_position)->type != R_PARENTHESES) { return false; }
            if (token_at(stream, temporary_position)->type == R_PARENTHESES || token_at(stream, temporary_position)->type == COMMA) {
                *position = temporary_position;
                return true;
            }
        }
    }
    return false;
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
    size_t temporary_position = *position;
    if (token_at(stream, temporary_position)->type != L_PARENTHESES) { return false; }
    if (temporary_position + 1 >= stream->size) { return false; }
    if (token_at(stream, temporary_position + 1)->type == R_PARENTHESES) {
        *position+=2;
        return true;
    }
    if (temporary_position + 2 <= stream->size && compare_string_slice_to_c_string(token_at(stream, temporary_position + 1)->slice, "void") && token_at(stream, temporary_position + 2)->type == R_PARENTHESES) {
        *position += 3;
        return true;
    }
    if (temporary_position + 1 < stream->size) {
        temporary_position++;
    }
    while (is_parameter(stream, &temporary_position)){
        temporary_position++;
        if (token_at(stream, temporary_position - 1)->type == R_PARENTHESES){
            *position = temporary_position;
            return true;
        }
    }
    return false;
}
/*------------------------------------------------------------------------------
*
* <function-definition> ::= {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
*
* ---------------------------------------------------------------------------*/
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
    size_t temporary_position = *position;

    printf("Current temp_pos: %zu\n", temporary_position);
    while(is_declaration_specifier(token_at(stream, temporary_position))){
        temporary_position++;
    };
    printf("Hi, New temp_pos: %zu\n", temporary_position);
    if(is_declarator(token_at(stream, temporary_position))){
        *position = temporary_position;
        return true;
    }
    return false;
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
bool test_parse_function(const TokenStream *stream, const enum PARSER_FUNCTION_NAME function_name, const bool expected_result, size_t expected_position) {
    if (function_name == IS_PARAMETER) {
        size_t position = 0;
        const bool function_return_value = is_parameter(stream, &position);
        if (function_return_value != expected_result || position != expected_position) {

            printf("Test failed.\n");
            print_stream(stream);
            printf("\nExpected result: %d Actual result: %d Expected position: %lu Actual position: %lu\n", expected_result, function_return_value, expected_position, position);
            return false;
        }
    }
    if (function_name == IS_PARAMETER_LIST) {
        size_t position = 0;
        const bool function_return_value = is_parameter_list(stream, &position);
        if (function_return_value != expected_result || position != expected_position) {
            printf("Test failed.\n");
            printf("Expected result: %d Actual result: %d Expected position: %lu Actual position: %lu\n", expected_result, function_return_value, expected_position, position);
            return false;
        }
    }
    if (function_name == IS_FUNCTION_DEFINITION) {
        size_t position = 0;
        printf("We're calling this.\n");
        const bool function_return_value = is_function_definition(stream, &position);
        if (function_return_value != expected_result || position != expected_position) {
            printf("Test failed.\n");
            printf("Expected result: %d Actual result: %d Expected position: %lu Actual position: %lu\n", expected_result, function_return_value, expected_position, position);
            return false;
        }
    }
    return true;
}

