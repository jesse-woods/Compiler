//
// Created by Jesse Woods on 10/16/26.
//

#ifndef COMPILER_RETURN_H
#define COMPILER_RETURN_H
#include "parser/abstract_syntax_tree/constant.h"

typedef struct {
    Constant* expression;
}Return;
#endif //COMPILER_RETURN_H
