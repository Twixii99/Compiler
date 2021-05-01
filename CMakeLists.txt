cmake_minimum_required(VERSION 3.17)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp parser.h NFAbuiler.h DFAbuiler.h lexical_analyzer.h printer.h)