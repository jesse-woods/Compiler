cmake_minimum_required(VERSION 3.27)
project(Compiler C)
include(CTest)

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
        include/file_processor.h
        src/file_processor.c
        tests/parser/parser_test.c
        include/tests/parser_test.h
        include/parser/parser_function_names.h
)
if(BUILD_TESTING)
    add_test(
        NAME LexerTest
        COMMAND $<TARGET_FILE:Compiler> -t lexer-test
    )
    add_test(
        NAME ParserFunctionTest1
        COMMAND $<TARGET_FILE:Compiler> -t is-parameter
    )
    add_test(
        NAME ParserFunctionTest2
        COMMAND $<TARGET_FILE:Compiler> -t is-parameter-list
    )
    add_test(
        NAME ParserFunctionTest3
        COMMAND $<TARGET_FILE:Compiler> -t is-function-definition
    )
    set_tests_properties(
        LexerTest
        ParserFunctionTest1
        ParserFunctionTest2
        ParserFunctionTest3
        PROPERTIES WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

target_include_directories(Compiler PRIVATE include)

