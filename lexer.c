#include "lexer.h"

#include <stdlib.h>
#include <string.h>
//
// Created by jwoods on 8/6/26.
//



bool isDelimiter(const char c) {
    return c ==' '||c == '('|| c == ')'||c == '['|| c == ']' || c == '{'||
        c == '}' || c =='{'||c == '}'||c == '['||c == ']'||c == ';'||c == ',' ||
        c =='+' || c =='-' || c == '*' || c =='/' || c =='%' || c =='^';
}


bool isOperator(const Str* op) {
    if (op == nullptr || op->strSlice[0] == '\0' || isDelimiter(op->strSlice[0]) || op->length == 0)
    {
        return false;
    }
    else
    {
        const char c = op->strSlice[0];
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^'||
            c == '>' || c == '<' || c == '=';
    }
}
/*-------------------------------------------------------------------------------
 *We shall start with 32 keywords, similar to the c89 standard.
 *The current c23 standard has 67 keywords.
 *Current implementation is to search through keywords with a loop, in the future
 *perhaps its better to use a dictionary for purposes of speed.
 *------------------------------------------------------------------------------*/
static bool isKeyword(const Str* str)
{


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
    return false;
}

static bool isValidIdentifier(const Str* str) {
    return (!isInteger(str) && !isDelimiter(str->strSlice[0]));
}
// check for an integer value
static bool isInteger(const Str* str)
{
    if (str == NULL || str->strSlice[0] == '\0') {
        return false;
    }
    int i = 0;
    while (str->strSlice[i] == '0' || str->strSlice[i] == '1' || str->strSlice[i] == '2' || str->strSlice[i] == '3'
        || str->strSlice[i] == '4' || str->strSlice[i] == '5' || str->strSlice[i] == '6' || str->strSlice[i] == '7'
        || str->strSlice[i] == '8' || str->strSlice[i] == '9') {
        i++;
    }
    return str->strSlice[i] == '\0';
}

// trims a substring from a given string's start and end
// position
const Str* sliceString(const char* slicedString, const size_t start, const size_t length){
    if (slicedString == nullptr || *slicedString == '\0')
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
    if (str1 == nullptr || str2 == nullptr)
    {
        return false;
    }

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
static void freeStr(const Str* str)
{
    if (str == nullptr)
        return;
    free(str->strSlice);
    free((void *)str);
}