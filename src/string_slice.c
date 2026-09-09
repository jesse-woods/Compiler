#include <stdio.h>
#include <stdlib.h>
#include "string_slice.h"
//
// Created by jwoods on 9/5/26.
//
// trims a substring from a given string's start and end
// position


const Str* slice_string(const char* slicedString, const size_t start, const size_t length){
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
void print_str(const Str* str)
{
    if (str == nullptr) {
        printf("Attempt to print empty string.\n");
        return;
    }

    for (size_t i = 0; i < str->length; i++)
        printf("%c", str->strSlice[i]);
    printf("\n");

}
void free_slice(const Str* str) {
    if (str != nullptr) {
        free((void*)str->strSlice);
        free((void*)str);
    }
}