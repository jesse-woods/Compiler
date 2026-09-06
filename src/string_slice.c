#include <stdio.h>
#include <stdlib.h>
#include "string_slice.h"
//
// Created by jwoods on 9/5/26.
//
// trims a substring from a given string's start and end
// position
const Str* sliceString(const char* slicedString, const size_t start, const size_t length){
    if (slicedString == nullptr || slicedString[0] == '\0')
    {
        printf("Attempt to slice empty string.\n");

        return nullptr;
    }
    Str* returnStr = malloc(sizeof(Str));
    if (returnStr == nullptr) {
        return nullptr; // Always protect against failed mallocs
    }
    returnStr->strSlice = slicedString + start;
    returnStr->length =  length;
    return returnStr;
}