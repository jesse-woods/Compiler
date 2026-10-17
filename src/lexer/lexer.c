#include "../../include/lexer/lexer.h"
#include <stdlib.h>
#include <stdio.h>

static bool isDelimiter(char);
static bool isOperator(char);
static bool isPunctuation(char);
static bool isDelimiter(const char c) {
    return c == ' '  || c == '\t' || c == '\r' ||
           c == '\n'  || c == '\r'  ||
           isPunctuation(c) || isOperator(c);
}
static bool isOperator(const char op) {

    return (op == '+' || op == '-' || op == '*' || op == '/'
    || op == '%' || op == '>' || op == '<' || op == '=');

}
static bool isPunctuation(const char c) {
    return c == '('  || c == ')'  ||
           c == '['  || c == ']'  ||
           c == '{'  || c == '}'  ||
           c == ';'  || c == ','  ||
           c == '?'  || c == '@'  ||
           c == '!'  || c == '.';
}
TokenStream* lexical_analyzer(const char* input) {

    if (input == nullptr) {
        printf("Error: attempt to lexical analyzer with null input\n");
        return nullptr;
    }

    TokenStream* stream = create_stream();
    size_t row = 1;
    size_t column = 0;
    size_t left = 0;
    size_t right = 0;

    //Window logic for lexer to create tokens
    while (input[right] != '\0') {
        //move index "right" over one by one until a delimiter is found
        while (!isDelimiter(input[right])) {
            right++;
        }
        //Once a delimiter is found, if there's a window, create a token.
        if (left != right) {
            const auto newToken = create_token(input + left, right - left, row, column);
            push_token(stream, newToken);
        }
        //update column to where right is
        column += right - left;

        //regardless if there's a window, handle the delimiter
        if (isPunctuation(input[right])) {
            const auto newToken = create_token(input + right, 1, row, column);
            push_token(stream, newToken);
        }

        //if there are two operators in a row, such as ++ or -=, create token of length 2
        if (isOperator(input[right]) && isOperator(input[right + 1])) {
            const auto newToken = create_token(input + right, 2, row, column);
            push_token(stream, newToken);

            right++;
            column++;
        }
        else if (isOperator(input[right])) {
            const auto newToken = create_token(input + right, 1, row, column);
            push_token(stream, newToken);
        }
        //if delimiter is line feed, reset column and increment row
        if (input[right] == 10) {
            row++;
            column = 0;
        }
        right++;
        column ++;
        left = right;
    }
    return stream;

}


