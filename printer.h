#ifndef COMPILER_PRINTER_H
#define COMPILER_PRINTER_H

#include "DFAminimizer/DFAminimizer.h"

using namespace std;

class Printer {
    DFAstate* start_state;
    DFAstate* last_accepted_state;
    DFAstate* current_state;

    bool is_a_final_detected;

    string grammer_file, input_file, output_file, table_file_name;

    ifstream input;
    ofstream output;

    DFAstate* move(char, DFAstate*);

    void controller();
    bool check(DFAstate* s);
    void print_errors();

public:
    void __init__();
    Printer(string, string, string, string);
};

string upgrade_string(DFAstate*, string);
void error_handle();
#endif //COMPILER_PRINTER_H
