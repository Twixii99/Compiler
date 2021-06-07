//
// Created by Kamal on 04/06/2021.
//

#include "parser_interface.h"

#include <iostream>
#include <vector>

int line_no = 0;

std::vector<std::string> auxiliary_lexemes;
string last_token;

Interface::Interface(std::string input_file_name, Printer *printer) {
    this->input_file.open(input_file_name);
    if(!this->input_file.is_open()) {
        std::cout << "Check input file path..." << std::endl;
    }

    this->printer = printer;
}

bool Interface::get_next_line() {
    char input_line[256];
    if (!this->input_file.eof()) {
        input_file.getline(input_line, 255);
        ::line_no++;
        std::string current_line(input_line);
        std::istringstream lexems(current_line);
        std::string lexeme;
        while (lexems >> lexeme) {
            ::auxiliary_lexemes.push_back(lexeme);
        }
        return true;
    }
    return false;
}

bool Interface::has_next_token()  {
    try_next_line:
    if(::auxiliary_lexemes.empty())
        if(!this->get_next_line())
            return false;
    for (string lexeme = *::auxiliary_lexemes.begin(); !::auxiliary_lexemes.empty(); lexeme = *::auxiliary_lexemes.begin()) {
        ::auxiliary_lexemes.erase(::auxiliary_lexemes.begin());
        vector<string> temp = this->printer->get_token(lexeme, line_no);
        if(temp.empty())
            continue;
        else if(temp.size() == 2)
            ::auxiliary_lexemes.insert(::auxiliary_lexemes.begin(), temp[1]);
        ::last_token = temp[0];
        return true;
    }
    goto try_next_line;
}

std::string Interface::get_next_token() {
    return ::last_token;
}