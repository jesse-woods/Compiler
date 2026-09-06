//
// Created by jwoods on 9/5/26.
//

#ifndef COMPILER_STRING_SLICE_H
#define COMPILER_STRING_SLICE_H
#include <stddef.h>

typedef struct
{
    const char* strSlice;
    size_t length;
} Str;

const Str* sliceString(const char*, size_t, size_t);


#endif //COMPILER_STRING_SLICE_H
