#include "lexer.h"
#include <stdlib.h>


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

// trims a substring from a given string's start and end
// position
const Str* sliceString(const char* slicedString, const size_t start, const size_t length){
    if (slicedString == nullptr || slicedString[0] == '\0')
    {
        printf("Attempt to slice empty string.\n");

        return nullptr;
    }
    Str* returnStr = malloc(sizeof(Str));
    if (returnStr == nullptr) {
        return nullptr; // Always protect against failed mallocs
    }
    returnStr->strSlice = slicedString + start;
    returnStr->length =  length;
    return returnStr;
}
static bool isValidStr(const Str * str){

    return str != nullptr || str->strSlice != nullptr || str->strSlice[0] != '\0' || str->length != 0;

}
void lexicalAnalyzer(const char* in)
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
            bool foundUnary = false;
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
                        foundUnary = true;
                    }
                    if (input->strSlice[right] == '+' && input->strSlice[right + 1] == '+')
                    {
                        printf("increment right here\n");
                        foundUnary = true;
                    }
                    if (input->strSlice[right] == '-' && input->strSlice[right + 1] == '-')
                    {
                        printf("increment right here\n");
                        foundUnary = true;
                    }

                }
            }
            if (foundUnary)
            {
                right+=2;
                left = right;
            }
            else
            {
                right++;
                left = right;
            }
        }
    }
    else {
        printf("Attempt to lexical analyze empty string.\n");
    }
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








