cmake_minimum_required(VERSION 4.3)
project(Compiler C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(Compiler
        main.c
        src/lexer/lexer.c
        include/lexer/lexer.h
        include/lexer/token_stream/token_stream.h
        src/lexer/token_stream.c
        include/lexer/token_stream/token.h
        src/lexer/string_slice.c
        include/lexer/token_stream/string_slice.h
        src/lexer/token.c
        include/parser/parser.h
        src/parser/parser.c
        include/parser/abstract_syntax_tree/abstract_syntax_tree.h
        include/parser/abstract_syntax_tree/function.h
        include/parser/abstract_syntax_tree/function_body.h
        include/parser/abstract_syntax_tree/program.h
        include/parser/abstract_syntax_tree/return.h
        include/parser/abstract_syntax_tree/constant.h
)

target_include_directories(Compiler PRIVATE include)

