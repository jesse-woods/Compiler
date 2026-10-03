#include <stdio.h>
#include <stdlib.h>
#include "string_slice.h"
//
// Created by Jesse Woods on 9/5/26.
//
// trims a substring from a given string's start and end
// position


const string_slice* slice_string(const char* sliced_string, const size_t start, const size_t length){
    if (sliced_string == nullptr || sliced_string[0] == '\0')
    {
        printf("Attempt to slice empty string.\n");
        return nullptr;
    }
    const auto return_slice = (string_slice*)malloc(sizeof(string_slice));

    //Protect against failed malloc
    if (return_slice == nullptr) {
        return nullptr;
    }

    return_slice->slice = sliced_string + start;
    return_slice->length =  length;
    return return_slice;
}
void print_slice(const string_slice* str)
{
    if (str == nullptr) {
        printf("Attempt to print empty string.\n");
        return;
    }

    for (size_t i = 0; i < str->length; i++)
        printf("%c", str->slice[i]);
    printf("\n");

}

void print_slice_to_file(FILE* file, const string_slice* str) {
    if (str == nullptr) return;
    for (size_t i = 0; i < str->length; i++)
        fprintf(file,"%c", str->slice[i]);
    fprintf(file,"\n");
}
void free_slice(const string_slice* str) {
    if (str == nullptr) return;


    free((void*)str);

}