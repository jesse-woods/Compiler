#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>


bool isDelimiter(const char c) {
    return c == ' '  || c == '\t' || c == '\r' ||
           c == '('  || c == ')'  ||
           c == '['  || c == ']'  ||
           c == '{'  || c == '}'  ||
           c == ';'  || c == ',';
}
bool isOperator(const char op) {

    return (op == '+' || op == '-' || op == '*' || op == '/'
    || op == '%' || op == '>' || op == '<' || op == '=');

}
/*-------------------------------------------------------------------------------
 *We shall start with 32 keywords, similar to the c89 standard.
 *The current c23 standard has 67 keywords.
 *Current implementation is to search through keywords with a loop, in the future
 *perhaps its better to use a dictionary for purposes of speed.
 *------------------------------------------------------------------------------*/
// Helper to compare a Str (non-null-terminated) with a standard C-string
static bool compareStrToCString(const Str* str, const char* cstr) {
    size_t i = 0;
    while (i < str->length && cstr[i] != '\0') {
        if (str->strSlice[i] != cstr[i]) {
            return false;
        }
        i++;
    }
    // Both must reach the end simultaneously
    return (i == str->length && cstr[i] == '\0');
}

static bool isKeyword(const Str* str) {
    if (!isValidStr(str)) return false;

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

static bool isValidIdentifier(const Str* str) {
    if (!isValidStr(str) || str->length == 0) {
        printf("Attempt to check if invalid string is identifier.\n");
        return false;
    }

    // Identifiers cannot start with a digit
    if (str->strSlice[0] >= '0' && str->strSlice[0] <= '9') {
        return false;
    }

    // Check all characters
    for (size_t i = 0; i < str->length; i++) {
        const char c = str->strSlice[i];
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
static bool isInteger(const Str* str) {
    if (isValidStr(str))
    {
        int i = 0;
        while (str->strSlice[i] == '0' || str->strSlice[i] == '1' || str->strSlice[i] == '2' || str->strSlice[i] == '3'
            || str->strSlice[i] == '4' || str->strSlice[i] == '5' || str->strSlice[i] == '6' || str->strSlice[i] == '7'
            || str->strSlice[i] == '8' || str->strSlice[i] == '9') {
            i++;
            }
        return i == str->length;
    }
    printf("Attempt to compare invalid string to an integer.\n");
    return false;
}


static bool isValidStr(const Str * str){

    return str != nullptr || str->strSlice != nullptr || str->strSlice[0] != '\0' || str->length != 0;

}
char* stringify(const char* filename, size_t size) {

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
    long long_size = ftell(c_program);
    if (long_size < 0) {
        printf("Error: could not read file size.\n");
        fclose(c_program);
        return nullptr;
    }
    //ftell explicitly returns a long, so we cast to size_t
    size = (size_t)long_size;

    //Reset pointer to beginning of file
    if (fseek(c_program, 0, SEEK_SET) != 0) {
        printf("Error: could not seek to start of file.\n");
        fclose(c_program);
        return nullptr;
    }

    //allocate memory +1 byte for '\0'
    char* file_contents = (char*)malloc(size + 1);
    if (file_contents == nullptr) {
        printf("Error: could not allocate memory for file contents.\n");
        fclose(c_program);
        return nullptr;
    }
    //Read file
    size_t bytes_read = fread(file_contents, sizeof(char), size, c_program);
    if (bytes_read != size) {
        printf("Error reading file.\n");
        free(file_contents);
        fclose(c_program);
        return nullptr;
    }
    //set null-terminator
    file_contents[bytes_read] = '\0';
    fclose(c_program);
    for (size_t i = 0; i < bytes_read; i++) {
        printf("character: %c ASCII: %d\n", file_contents[i], file_contents[i]);
    }
    return file_contents;
}
TokenStream* lexical_analyzer(const char* input, size_t size)
{

    if (input != nullptr)
    {
        size_t left = 0, right = 0, line_number = 0, position = 0;
        //TokenStream* stream = malloc(sizeof(TokenStream));
        TokenStream* stream = create_stream();

        while (right < size)
        {
            while (!isDelimiter(input[right]) && !isOperator(input[right]))
            {
                right++;
            }
            if ((isDelimiter(input[right]) || isOperator(input[right])) && left != right){
                const Str* slice = slice_string(input, left, right - left);
                const auto token = (Token*)malloc(sizeof(Token));
                token->token = slice;
                token->column = left;
                token->line = line_number;
                if (isValidIdentifier(slice))
                {token->type = IDENTIFIER;}
                else if (isKeyword(slice))
                {
                    token->type = KEYWORD;
                }
                else if (isInteger(slice))
                {
                    token->type = INTEGER;
                }
                else
                {
                    token->type = UNDEFINED;
                }
                print_str(token->token);
                push_token(stream, token);
            }
            if (isDelimiter(input[right]))
            {
                if (input[right] == 10) {
                    line_number++;
                    printf("Line number: %lu\n", line_number);
                }
                if (input[right] != ' ' && input[right] != '\n')
                {
                    const Str* slice = slice_string(input, right, 1);
                    const auto token = (Token*)malloc(sizeof(Token));
                    token->token = slice;
                    token->column = left;
                    token->line = line_number;
                    switch (input[right])
                    {
                    case '(': token->type = L_PARENTHESES; break;
                    case ')': token->type = R_PARENTHESES; break;
                    case '{': token->type = L_BRACE;       break;
                    case '}': token->type = R_BRACE;       break;
                    case '[': token->type = L_BRACKET;     break;
                    case ']': token->type = R_BRACKET;     break;
                    case ';': token->type = SEMICOLON;     break;
                    case ',': token->type = COMMA;         break;
                    default:  token->type = UNDEFINED;     break;
                    }
                    print_str(token->token);
                    push_token(stream, token);
                }
            }
            else if (isOperator(input[right]))
{
    const auto token = (Token*)malloc(sizeof(Token));
    if (token == nullptr) {
        printf("Malloc failed for Token\n");
        return nullptr;
    }
    token->column = right;
    token->line = line_number;
    token->type = UNDEFINED; // Default fallback to avoid uninitialized memory
    token->token = nullptr;

    // Check for 2-character operators if we aren't at the end of the string
    if (right < size - 1)
    {
        char current = input[right];
        char next = input[right + 1];

        if (next == '=')
        {
            token->token = slice_string(input, right, 2);
            switch (current)
            {
                case '<': token->type = LESS_THAN_OR_EQUAL;    break;
                case '>': token->type = GREATER_THAN_OR_EQUAL; break;
                case '+': token->type = PLUS_EQUAL;            break;
                case '-': token->type = MINUS_EQUAL;           break;
                case '*': token->type = TIMES_EQUAL;           break;
                case '/': token->type = DIVIDE_EQUAL;          break;
                case '%': token->type = MODULO_EQUAL;          break;
                default:  token->type = UNDEFINED;             break;
            }
            right++; // Consume the '='
        }
        else if (current == '+' && next == '+')
        {
            token->token = slice_string(input, right, 2);
            token->type = INCREMENT;
            right++; // Consume the second '+'
        }
        else if (current == '-' && next == '-')
        {
            token->token = slice_string(input, right, 2);
            token->type = DECREMENT;
            right++; // Consume the second '-'
        }
    }

    // Fallback: If it wasn't a 2-character operator, handle it as a 1-character operator
    if (token->token == nullptr)
    {
        token->token = slice_string(input, right, 1);
        // Optional: Add a switch statement here if you have
        // explicit enums for single operators like PLUS, MINUS, etc.
        token->type = UNDEFINED;
    }

    print_str(token->token);
    push_token(stream, token);
}
            right++;
            left = right;
        }
        return stream;
    }
    printf("Attempt to lexical analyze empty string.\n");
    return nullptr;
}






