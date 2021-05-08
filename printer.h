
//
// Created by magdy on 5/1/2021.
//

#ifndef COMPILER_PRINTER_H
#define COMPILER_PRINTER_H

#include "DFAminimizer/DFAminimizer.h"

using namespace std;

class Printer {
    DFAstate* start_state;
    DFAstate* last_accepted_state;
    DFAstate* current_state;

    bool is_a_final_detected;

    string grammer_file, input_file;

    ifstream input;

    DFAstate* move(char, DFAstate*);

    void controller();
//    void controller2();
    bool check(DFAstate* s);

public:
    void __init__();
    Printer(string, string);
    Printer(DFAstate*);
};

string upgrade_string(DFAstate*, string);
void error_handle();
#endif //COMPILER_PRINTER_H
