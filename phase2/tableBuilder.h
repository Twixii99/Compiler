//
// Created by magdy on 6/1/2021.
//

#ifndef COMPILER_PHASE_2_TABLEBUILDER_H
#define COMPILER_PHASE_2_TABLEBUILDER_H
#include <string>

enum Type{
    ERROR,
    SYNC,
    AVAILABLE
};

using namespace std;
class table_builder{
public:
    table_builder(string file_name);
    bool is_termainal(string str);
    Type has_production(string terminal,string non_terminal);
    string get_production(string terminal,string non_terminal);
    string get_start_symbol();
};

void parse(string filename);
void print();

void first_set_builder();
void follow_set_builder();
void parser_table_builder();

void print_first_table(string);
void print_follow_table(string);
void print_parse_table(string);
void print_terminals();
#endif //COMPILER_PHASE_2_TABLEBUILDER_H