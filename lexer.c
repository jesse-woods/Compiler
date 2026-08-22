#include "lexer.h"
#include <stdlib.h>


bool isDelimiter(const char c) {
    return c ==' '||c == '('|| c == ')'||c == '['|| c == ']' || c == '{'||
        c == '}' || c =='{'||c == '['||c == ']'||c == ';'||c == ',';
}


bool isOperator(const Str* op) {
    printf("Slice : %s  Size: %lu\n", op->strSlice, op->length);
    if (op == nullptr || op->strSlice == nullptr || op->length == 0)
    {
        return false;
    }
    return (op->strSlice[0] == '+' || op ->strSlice[0] == '-' || op ->strSlice[0] == '*' || op ->strSlice[0] == '/'
    || op ->strSlice[0] == '%' || op ->strSlice[0] == '>' || op ->strSlice[0] == '<' || op ->strSlice[0] == '=');

}
/*-------------------------------------------------------------------------------
 *We shall start with 32 keywords, similar to the c89 standard.
 *The current c23 standard has 67 keywords.
 *Current implementation is to search through keywords with a loop, in the future
 *perhaps its better to use a dictionary for purposes of speed.
 *------------------------------------------------------------------------------*/
static bool isKeyword(const Str* str)
{
    if (isValidStr(str)) {
        constexpr size_t KEYWORD_COUNT = 32;
        for (int i = 0; i < KEYWORD_COUNT; i++)
        {
            const char* keywords[]
                = { "auto",     "break",    "case",     "char",
                    "const",    "continue", "default",  "do",
                    "double",   "else",     "enum",     "extern",
                    "float",    "for",      "goto",     "if",
                    "int",      "long",     "register", "return",
                    "short",    "signed",   "sizeof",   "static",
                    "struct",   "switch",   "typedef",  "union",
                    "unsigned", "void",     "volatile", "while" };
            size_t len = 0;
            while (keywords[i][len] != '\0')
                len++;
            if (compare(str, sliceString(keywords[i], 0, len))) {
                return true;
            }
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

    if (isValidStr(str))
    {
        bool returnStatement = false;
        const char compared = str->strSlice[0];
        for (char i = 0; i < 26; i++){
            if (compared == (i + 'A') || compared == (i + 'a'))
            {

                returnStatement = true;
            }
        }
        if (compared == '_')
        {
            returnStatement = true;
        }
        if (isKeyword(str))
        {
            returnStatement = false;
        }
        return returnStatement;
    }
    printf("Attempt to check if invalid string is identifier.\n");
    return false;
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
        const Str returnStr = { .strSlice = nullptr, .length = 0, };
        return &returnStr;
    }
    //const size_t subLength = length - start + 1;
    const auto subStr = (char*)malloc((length + 1) * sizeof(char));
    for (size_t i = 0; i < length; i++)
    {
        subStr[i] = slicedString[start + i];
    }
    subStr[length] = '\0';
    const auto returnStr = (Str*)malloc((sizeof(subStr) + sizeof(size_t)));
    returnStr->strSlice = subStr;
    returnStr->length = length;

    return returnStr;
}
static bool compare(const Str* str1, const Str* str2)
{
    if (isValidStr(str1) && isValidStr(str2))
    {
        if (str1->length != str2->length)
        {
            return false;
        }
        for (size_t i = 0; i < str1->length; i++)
        {

            if (str1->strSlice[i] != str2->strSlice[i])
            {
                return false;
            }
        }
        return true;
    }
    printf("Attempt to compare one or more invalid strings.\n");
    return false;
}
static bool isValidStr(const Str * str){

    return str != nullptr || str->strSlice != nullptr || str->strSlice[0] != '\0' || str->length != 0;

}
static void freeStr(const Str* str)
{
    if (isValidStr(str))
    {
        free(str->strSlice);
        free((void *)str);
    }
    else
    {
        printf("Attempt to free null string.\n");
    }
}

static void lexicalAnalyzer(const Str* input) {
    if (isValidStr(input)){
        size_t left = 0, right = 0;

        while (right <= input->length && left <= right) {
            if (!isDelimiter(input->strSlice[right])){
                right++;
            }
            if (isDelimiter(input->strSlice[right]) && left == right) {
                right++;
                left = right;

            }
            else if (isDelimiter(input->strSlice[right]) && left != right || (right == input->length - 1 && left != right)) {
                const Str* subStr = sliceString(input->strSlice, left, right - left);

                if (isKeyword(subStr)) {
                    printf("Token: Keyword, Value: %s\n", subStr->strSlice);
                }
                else if (isInteger(subStr)) {
                    printf("Token: Integer, Value: %s\n", subStr->strSlice);
                }
                else if (isValidIdentifier(subStr)) {
                    printf("Token: Identifier, Value: %s\n", subStr->strSlice);
                }
                else if (isOperator(subStr)) {
                    printf("Token: Operator, Value: %s\n", subStr->strSlice);
                }

                else {
                    printf("Token: Unidentified, Value: %s\n", subStr->strSlice);
                }

                freeStr(subStr);
                left = right;
            }
        }
    }
    else {
        printf("Attempt to lexical analyzer empty string.\n");
    }
}
