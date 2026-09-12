//
// Created by Jesse Woods on 9/5/26.
//

#ifndef COMPILER_STRING_SLICE_H
#define COMPILER_STRING_SLICE_H

#include <stddef.h> // IWYU pragma: keep


typedef struct
{
    const char* slice;
    size_t length;
} string_slice;

const string_slice* slice_string(const char*, size_t, size_t);
void print_str(const string_slice*);
void free_slice(const string_slice*);

#endif //COMPILER_STRING_SLICE_H
