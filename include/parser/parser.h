//
// Created by jessewoods on 10/7/26.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "lexer/token_stream/token_stream.h"


void parse_token_stream(const TokenStream*);
static bool is_type_specifier(const Token*);
static bool is_storage_class_specifier(const Token*);
static bool is_type_qualifier(const Token*);
static bool is_struct_or_union(const Token*);
#endif //COMPILER_PARSER_H
