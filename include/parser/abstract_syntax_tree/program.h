//
// Created by jessewoods on 10/16/26.
//

#ifndef COMPILER_PROGRAM_H
#define COMPILER_PROGRAM_H
#include "parser/abstract_syntax_tree/function.h"

typedef struct {
    Function* function_definition;
}Program;
#endif //COMPILER_PROGRAM_H
