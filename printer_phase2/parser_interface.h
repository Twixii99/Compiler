//
// Created by Kamal on 04/06/2021.
//

#ifndef COMPILER_PARSER_INTERFACE_H
#define COMPILER_PARSER_INTERFACE_H

#include <string>
#include <fstream>

#include "printer.h"

class Interface {
    std::ifstream input_file;
    Printer *printer;

public:
    Interface(std::string input_file_name, Printer *printer);
    bool has_next_token();
    std::string get_next_token();

private:
    bool get_next_line();
};
#endif //COMPILER_PARSER_INTERFACE_H
