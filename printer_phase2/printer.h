#ifndef COMPILER_PRINTER_H
#define COMPILER_PRINTER_H

#include "../DFAminimizer/DFAminimizer.h"

using namespace std;

class Printer {
    string output_file;
    ofstream output;

    DFAstate* start_state;
    DFAstate* last_accepted_state;
    DFAstate* current_state;

    bool is_a_final_detected;

public:
    void __init__(DFAstate*);
    vector<string> get_token(string, int);
    void print_errors();
    Printer(string);

private:
    DFAstate* move(char, DFAstate*);
    void check(DFAstate* s);
};

string upgrade_string(DFAstate*, string);
void error_handler();
#endif //COMPILER_PRINTER_H
