#include <stdio.h>
#include <stdlib.h>
#include "file_processor.h"
//
// Created by Jesse Woods on 10/19/26.
//
char* read_file_to_memory(const char* filename) {

    //Open file in binary mode for accurate file size
    FILE* c_program = fopen(filename, "rb");
    if (c_program == nullptr) {
        printf("Error: could not open file for reading.\n");
        return nullptr;
    }
    //Find end of file to determine size
    if (fseek(c_program, 0, SEEK_END) != 0) {
        printf("Error: could not seek to end of file.\n");
        fclose(c_program);
        return nullptr;
    }
    //retrieve numerical representation of number of bytes in file
    const long long_size = ftell(c_program);
    if (long_size < 0) {
        printf("Error: could not read file size.\n");
        fclose(c_program);
        return nullptr;
    }
    //ftell explicitly returns a long, so we cast to size_t
    const size_t size = (size_t)long_size;

    //Reset pointer to beginning of file
    if (fseek(c_program, 0, SEEK_SET) != 0) {
        printf("Error: could not seek to start of file.\n");
        fclose(c_program);
        return nullptr;
    }

    //allocate memory +1 byte for '\0'
    const auto file_contents = (char*)malloc(size + 1);
    if (file_contents == nullptr) {
        printf("Error: could not allocate memory for file contents.\n");
        fclose(c_program);
        return nullptr;
    }
    //Read file
    const size_t bytes_read = fread(file_contents, sizeof(char), size, c_program);
    if (bytes_read != size) {
        printf("Error reading file.\n");
        free(file_contents);
        fclose(c_program);
        return nullptr;
    }
    //set null-terminator
    file_contents[bytes_read] = '\0';
    fclose(c_program);
    return file_contents;
}