cmake_minimum_required(VERSION 4.3)
project(Compiler C LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(Compiler
        src/main.c
        src/lexer.c
        include/lexer.h
)

target_include_directories(Compiler PRIVATE include)

