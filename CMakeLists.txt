cmake_minimum_required(VERSION 4.3)
project(Compiler C)

set(CMAKE_C_STANDARD 23)

add_executable(Compiler main.c
        lexer.h
        lexer.c)
