#include "lexer.h"
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

static bool isDelimiter(char c)
{
    return c == '\0'|| c ==' '||c == '('|| c == ')'||c == '['|| c == ']' || c == '{'|| c == '}' ||
        c =='{'||c == '}'||c == '['||c == ']'||c == ';'||c == ',';
}