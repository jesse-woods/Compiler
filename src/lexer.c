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
TokenStream* lexicalAnalyzer(const char* in, const size_t lineNumber)
{
    size_t len = 0;
    TokenStream* stream = create_stream();
    while (in[len] != 10)
    {
        len++;
    }
    const Str* input = sliceString(in, 0, len);
    if (isValidStr(input))
    {
        size_t left = 0, right = 0;
        while (right < input->length)
        {
            while (!isDelimiter(input->strSlice[right]) && !isOperator(input->strSlice[right]))
            {
                right++;
            }
            if ((isDelimiter(input->strSlice[right]) || isOperator(input->strSlice[right])) && left != right){
                const Str* slice = sliceString(input->strSlice, left, right - left);
                const auto token = (Token*)malloc(sizeof(Token));
                token->token = slice;
                token->column = left;
                token->line = lineNumber;
                if (isValidIdentifier(slice))
                {
                    printf("This is the identifier: ");
                    printStr(slice);

                    token->type = IDENTIFIER;
                }
                else if (isKeyword(slice))
                {
                    printf("This is the keyword: ");
                    printStr(slice);
                    token->type = KEYWORD;
                }
                else if (isInteger(slice))
                {
                    printf("This is the integer: ");
                    printStr(slice);
                    token->type = INTEGER;
                }
                else
                {
                    printf("Uncategorized: ");
                    printStr(slice);
                    token->type = UNDEFINED;
                }
                push_token(stream, token);
            }
            if (isDelimiter(input->strSlice[right]))
            {
                if (input->strSlice[right] != ' ')
                {
                    printf("We're registering this as a delimiter: %c ASCII: %d Pos: %lu \n", input->strSlice[right], input->strSlice[right], right);
                    const Str* slice = sliceString(input->strSlice, left, 1);
                    const auto token = (Token*)malloc(sizeof(Token));
                    token->token = slice;
                    token->column = left;
                    token->line = lineNumber;
                    switch (input->strSlice[right])
                    {
                    case '(':
                        token->type = L_PARENTHESES;
                        break;
                    case ')':
                        token->type = R_PARENTHESES;
                        break;
                    case '{':
                        token->type = L_BRACE;
                        break;
                    case '}':
                        token->type = R_BRACE;
                        break;
                    case '[':
                        token->type = L_BRACKET;
                        break;
                    case ']':
                        token->type = R_BRACKET;
                        break;
                    case ';':
                        token->type = SEMICOLON;
                        break;
                    case ',':
                        token->type = COMMA;
                        break;
                    default:
                        token->type = UNDEFINED;
                        break;
                    }
                    push_token(stream, token);
                }
            }
            else if (isOperator(input->strSlice[right]))
            {
                printf("We're registering this as an operator: %c Pos: %lu \n", input->strSlice[right], right);

                const auto token = (Token*)malloc(sizeof(Token));
                //token->token = slice;
                token->column = left;
                token->line = lineNumber;
                if (right < input->length - 1)
                {
                    if (input->strSlice[right + 1] == '=')
                    {
                        const Str* slice = sliceString(input->strSlice, left, 2);
                        token->token = slice;
                        right++;
                        switch (input->strSlice[right])
                        {
                            case '<':
                                token->type = LESS_THAN_OR_EQUAL;
                                break;
                            case '>':
                                token->type = GREATER_THAN_OR_EQUAL;
                                break;
                            case '+':
                                token->type = PLUS_EQUAL;
                                break;
                            case '-':
                                token->type = MINUS_EQUAL;
                                break;
                            case '*':
                                token->type = TIMES_EQUAL;
                                break;
                            case '/':
                                token->type = DIVIDE_EQUAL;
                                break;
                            case '%':
                                token->type = MODULO_EQUAL;
                                break;
                            default:
                                token->type = UNDEFINED;
                                break;
                        }
                    }
                    if (input->strSlice[right] == '+' && input->strSlice[right + 1] == '+')
                    {
                        const Str* slice = sliceString(input->strSlice, left, 2);
                        token->token = slice;
                        token->type = INCREMENT;
                        right++;
                    }
                    if (input->strSlice[right] == '-' && input->strSlice[right + 1] == '-')
                    {
                        const Str* slice = sliceString(input->strSlice, left, 2);
                        token->token = slice;
                        token->type = DECREMENT;
                        right++;
                    }


                }
                push_token(stream, token);
            }
            right++;
            left = right;
        }
    }
    else {
        printf("Attempt to lexical analyze empty string.\n");
    }
    return stream;
}

static void printStr(const Str* str)
{
    if (str != nullptr)
    {
        for (size_t i = 0; i < str->length; i++)
            printf("%c", str->strSlice[i]);
        printf("\n");
    }
}


/*
 *
 void lexicalAnalyzer(const char* in, size_t lineNumber)
{
    size_t len = 0;
    while (in[len] != 10)
    {
        len++;
    }
    const Str* input = sliceString(in, 0, len);
    if (isValidStr(input))
    {
        size_t left = 0, right = 0;
        while (right < input->length)
        {
            while (!isDelimiter(input->strSlice[right]) && !isOperator(input->strSlice[right]))
            {
                right++;
            }
            if ((isDelimiter(input->strSlice[right]) || isOperator(input->strSlice[right])) && left != right){
                const Str* test = sliceString(input->strSlice, left, right - left);
                printf("This is the window: ");
                printStr(test);
                if (isValidIdentifier(test))
                {
                    printf("This is the identifier: ");
                    printStr(test);
                    printf("\n");
                }
                else if (isKeyword(test))
                {
                    printf("This is the keyword: ");
                    printStr(test);
                    printf("\n");
                }
                else if (isInteger(test))
                {
                    printf("This is the integer: ");
                    printStr(test);
                    printf("\n");
                }
                else
                {
                    printf("Uncategorized: ");
                    printStr(test);
                    printf("\n");
                }
                free((void*)test);
            }
            if (isDelimiter(input->strSlice[right]))
            {
                if (input->strSlice[right] != ' ')
                {
                    printf("We're registering this as a delimiter: %c ASCII: %d Pos: %lu \n", input->strSlice[right], input->strSlice[right], right);
                }
            }
            else if (isOperator(input->strSlice[right]))
            {
                printf("We're registering this as an operator: %c Pos: %lu \n", input->strSlice[right], right);
                if (right < input->length - 1)
                {
                    if (input->strSlice[right + 1] == '=')
                    {
                        const Str* getEm = sliceString(input->strSlice, right, 2);
                        printf("Found a unary: ");
                        printStr(getEm);
                        right++;
                    }
                    if (input->strSlice[right] == '+' && input->strSlice[right + 1] == '+')
                    {
                        printf("increment right here\n");
                        right++;
                    }
                    if (input->strSlice[right] == '-' && input->strSlice[right + 1] == '-')
                    {
                        printf("increment right here\n");
                        right++;
                    }

                }
            }
            right++;
            left = right;
        }

        //remove when full token creation is done
        free((void*)input);
    }
    else {
        printf("Attempt to lexical analyze empty string.\n");
    }
}
*/








