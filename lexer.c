#include "lexer.h"
#include <string.h>
//
// Created by jwoods on 8/6/26.
//



/*------------------------------------------------------------
Identifier                              [a-zA-Z_]\w*\b
Constant                                [0-9]+\b
int keyword                             int\b
void keyword                            void\b
return keyword                          return\b
Open parenthesis                        \(
Close parenthesis                       \)
Open brace                              {
Close brace                             }
Semicolon                               ;
--------------------------------------------------------------*/
Str sliceString(char* slicedString, const size_t start, const size_t end) {
    Str slice;
    slice.strSlice = slicedString + start;
    slice.length = end - start;

    return slice;
}

static bool isDelimiter(const char c)
{
    return c == '\0'|| c ==' '||c == '('|| c == ')'||c == '['|| c == ']' || c == '{'||
        c == '}' || c =='{'||c == '}'||c == '['||c == ']'||c == ';'||c == ',' ||
        c =='+' || c =='-' || c == '*' || c =='/' || c =='%' || c =='^';
}

static bool isOperator(const char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^'||
        c == '>' || c == '<' || c == '=';
}
/*-------------------------------------------------------------------------------
 *We shall start with 32 keywords, similar to the c89 standard.
 *The current c23 standard has 67 keywords.
 *Current implementation is to search through keywords with a loop, in the future
 *perhaps its better to use a dictionary for purposes of speed.
 *------------------------------------------------------------------------------*/
static bool isKeyword(const char* str)
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
    for (int i = 0;
         i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
         }
    return false;
}

static bool isValidIdentifier(const char* str) {
    return (str[0] != '0' && str[0] != '1' && str[0] != '2'
            && str[0] != '3' && str[0] != '4'
            && str[0] != '5' && str[0] != '6'
            && str[0] != '7' && str[0] != '8'
            && str[0] != '9' && !isDelimiter(str[0]));
}