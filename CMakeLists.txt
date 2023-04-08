cmake_minimum_required(VERSION 3.24)
project(snapp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES  ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(main ${SOURCE_FILES} include/syntax/tokenizer.h include/syntax/symbol.h include/syntax/keyword.h include/syntax/sourceLocation.h include/error/syntaxError.h include/syntax/abstractSyntaxTree.h include/syntax/syntaxNode.h)

add_custom_target(
	copy-compile-commands ALL
	${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_BINARY_DIR}/compile_commands.json
	${CMAKE_CURRENT_LIST_DIR}
)
