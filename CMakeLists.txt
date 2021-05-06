cmake_minimum_required(VERSION 3.17)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler parser.h NFAbuilder.h DFAbuilder.h lexical_analyzer.h printer.h NFAbuilder.cpp NFAparser.h NFAparser.cpp parser.cpp regex_parser.h Main.cpp graph.h graph.cpp DFAbuilder.cpp)