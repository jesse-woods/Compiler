#include "token.h"
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
//
// Created by Jesse Woods on 9/7/26.
//

static bool compareStrToCString(const string_slice* str, const char* cstr) {
    size_t i = 0;
    while (i < str->length && cstr[i] != '\0') {
        if (str->slice[i] != cstr[i]) {
            return false;
        }
        i++;
    }
    // Both must reach the end simultaneously
    return (i == str->length && cstr[i] == '\0');
}

static bool isKeyword(const string_slice* str) {
    if (!isValidStr(str)) return false;

    //We make this a static const so when called, the array of keywords is built only once
    static const char* keywords[32] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    for (int i = 0; i < 32; i++) {
        if (compareStrToCString(str, keywords[i])) {
            return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------------------------------------------
* Identifiers can contain uppercase and lowercase alphabets (A–Z, a–z), digits (0–9), and the underscore (_).
* The first character of an identifier must be a letter or an underscore.
* Identifiers are case-sensitive.
* Identifiers cannot be keywords in C (such as int, return, if, while etc.)
* ------------------------------------------------------------------------------------------------------------------*/

static bool isValidIdentifier(const string_slice* str) {
    if (!isValidStr(str) || str->length == 0) {
        printf("Attempt to check if invalid string is identifier.\n");
        return false;
    }

    // Identifiers cannot start with a digit
    if (str->slice[0] >= '0' && str->slice[0] <= '9') {
        return false;
    }

    // Check all characters
    for (size_t i = 0; i < str->length; i++) {
        const char c = str->slice[i];
        const bool isAlpha = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        const bool isDigit = (c >= '0' && c <= '9');
        const bool isUnderscore = (c == '_');

        if (!isAlpha && !isDigit && !isUnderscore) {
            return false; // Found an invalid symbol (e.g., @, !, space)
        }
    }

    // Keywords are not identifiers
    if (isKeyword(str)) {
        return false;
    }

    return true;
}
// check for an integer value
static bool isInteger(const string_slice* str) {
    if (isValidStr(str))
    {
        int i = 0;
        while (str->slice[i] == '0' || str->slice[i] == '1' || str->slice[i] == '2' || str->slice[i] == '3'
            || str->slice[i] == '4' || str->slice[i] == '5' || str->slice[i] == '6' || str->slice[i] == '7'
            || str->slice[i] == '8' || str->slice[i] == '9') {
            i++;
            }
        return i == str->length;
    }
    printf("Attempt to compare invalid string to an integer.\n");
    return false;
}
static bool isValidStr(const string_slice * str){

    return str != nullptr || str->slice != nullptr || str->slice[0] != '\0' || str->length != 0;

}
Token* create_token(const char* input, const size_t length, const size_t row, const size_t column) {

    if (input == nullptr || length == 0) {
        printf("Error creating token: string input null or empty.\n");
        return nullptr;
    }
    const auto token = (Token*)malloc(sizeof(Token));
    if (token == nullptr) {
        printf("Error creating token: malloc failed.\n");
        return nullptr;
    }

    //Set new token params except type
    const string_slice* slice = slice_string(input, 0, length);
    token->slice = slice;
    token->column = column;
    token->line = row;

    if (isKeyword(token->slice)) { token->type = KEYWORD; }
    else if (isValidIdentifier(token->slice)) {token->type = IDENTIFIER;}
    else if (isInteger(token->slice)){ token->type = INTEGER; }

    //If length is 1, test for specific types of punctuation or operators
    else if (length == 1) {

        switch (input[0])
        {
            case '(': token->type = L_PARENTHESES; break;
            case ')': token->type = R_PARENTHESES; break;
            case '{': token->type = L_BRACE;       break;
            case '}': token->type = R_BRACE;       break;
            case '[': token->type = L_BRACKET;     break;
            case ']': token->type = R_BRACKET;     break;
            case ';': token->type = SEMICOLON;     break;
            case ',': token->type = COMMA;         break;
            case '+': token->type = PLUS;          break;
            case '-': token->type = MINUS;         break;
            case '*': token->type = ASTERISK;      break;
            case '/': token->type = FORWARD_SLASH; break;
            case '%': token->type = MODULO;        break;
            case '>': token->type = GREATER_THAN;  break;
            case '<': token->type = LESS_THAN;     break;
            case '=': token->type = EQUAL;         break;
            default:  token->type = UNDEFINED;     break;
        }
    }

    //Test for 2 character operators, such as increment or less than or equal to
    else if (length == 2) {
        const char first = input[0];
        const char second = input[1];

        if (second == '=')
        {
            switch (first)
            {
                case '<':
                    token->type = LESS_THAN_OR_EQUAL;
                    break;
                case '>': token->type = GREATER_THAN_OR_EQUAL; break;
                case '+': token->type = PLUS_EQUAL;            break;
                case '-': token->type = MINUS_EQUAL;           break;
                case '*': token->type = TIMES_EQUAL;           break;
                case '/': token->type = DIVIDE_EQUAL;          break;
                case '%': token->type = MODULO_EQUAL;          break;
                default:  token->type = UNDEFINED;             break;
            }
        }
        else if (first == '+' && second == '+') { token->type = INCREMENT; }
        else if (first == '-' && second == '-') { token->type = DECREMENT; }
    }

    //fallback
    else { token->type = UNDEFINED; }

    return token;
}
void print_token(const Token* token) {
    if (token == nullptr) {
        printf("Error printing token: null token.\n");
        return;
    }
    printf("Token: ");
    print_str(token->slice);
    printf("Line: %lu, Column: %lu\n", token->line, token->column);
    printf("Type: %d\n", token->type);
}
void free_token(Token* token) {
    if (token == nullptr) return;
    if (token->slice != nullptr) {
        free_slice(token->slice);
        token->slice = nullptr;
    }
    free(token);
}
