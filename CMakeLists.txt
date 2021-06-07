cmake_minimum_required(VERSION 3.17)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler parser/parser.h NFA/NFAbuilder.h DFA/DFAbuilder.h printer_phase2/printer.h NFA/NFAbuilder.cpp NFA/NFAparser.h NFA/NFAparser.cpp parser/parser.cpp parser/regex_parser.h Main.cpp NFA/graph.h NFA/graph.cpp DFA/DFAbuilder.cpp DFAminimizer/DFAminimizer.h DFAminimizer/DFAminimizer.cpp printer_phase2/printer.cpp printer_phase2/parser_interface.h printer_phase2/parser_interface.cpp phase2/outputGenerator.h phase2/outputGenerator.cpp phase2/tableBuilder.h phase2/tableBuilder.cpp intialize/initialization.h intialize/initialization.cpp)