//
// Created by jessewoods on 10/7/26.

#ifndef COMPILER_FUNCTION_BODY_H
#define COMPILER_FUNCTION_BODY_H
#include "parser/abstract_syntax_tree/return.h"

typedef struct{
/*------------------------------------------------------------------------
 * {<declaration>}* (zero or more declarations)
 * <declaration> ::= {<declaration-spacifier>}+ {<init-declarator>}*
 *----------------------------------------------------------------------*/
    Return* expression;
}FunctionBody;
#endif //COMPILER_FUNCTION_BODY_H
