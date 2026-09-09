//
// Created by Jesse Woods on 9/5/26.
//

#ifndef COMPILER_STRING_SLICE_H
#define COMPILER_STRING_SLICE_H

#include <stddef.h> // IWYU pragma: keep


typedef struct
{
    const char* strSlice;
    size_t length;
} Str;

const Str* slice_string(const char*, size_t, size_t);
void print_str(const Str*);
void free_slice(const Str*);

#endif //COMPILER_STRING_SLICE_H
