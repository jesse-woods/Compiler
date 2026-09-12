#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>


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
            printf("%c", input[right]);
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
char* stringify(const char* filename) {

    //Open file in binary mode for accurate file size
    FILE* c_program = fopen(filename, "rb");
    if (c_program == nullptr) {
        printf("Error: could not open file for reading.\n");
        return nullptr;
    }
    //Find end of file to determine size
    if (fseek(c_program, 0, SEEK_END) != 0) {
        printf("Error: could not seek to end of file.\n");
        fclose(c_program);
        return nullptr;
    }
    //retrieve numerical representation of number of bytes in file
    const long long_size = ftell(c_program);
    if (long_size < 0) {
        printf("Error: could not read file size.\n");
        fclose(c_program);
        return nullptr;
    }
    //ftell explicitly returns a long, so we cast to size_t
    const size_t size = (size_t)long_size;

    //Reset pointer to beginning of file
    if (fseek(c_program, 0, SEEK_SET) != 0) {
        printf("Error: could not seek to start of file.\n");
        fclose(c_program);
        return nullptr;
    }

    //allocate memory +1 byte for '\0'
    const auto file_contents = (char*)malloc(size + 1);
    if (file_contents == nullptr) {
        printf("Error: could not allocate memory for file contents.\n");
        fclose(c_program);
        return nullptr;
    }
    //Read file
    const size_t bytes_read = fread(file_contents, sizeof(char), size, c_program);
    if (bytes_read != size) {
        printf("Error reading file.\n");
        free(file_contents);
        fclose(c_program);
        return nullptr;
    }
    //set null-terminator
    file_contents[bytes_read] = '\0';
    fclose(c_program);
    return file_contents;
}




