cmake_minimum_required(VERSION 4.3)
project(Compiler C LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(Compiler main.c
        lexer.h
        lexer.c
        main.c)

