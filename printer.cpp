//
// Created by Kamal on 07/05/2021.
//

#include "printer.h"
#include <sstream>
#include <string>

using namespace std;
set<int> errornous_lines;

int line_number = 1;

Printer::Printer(string input_file, string grammer_file) {
    this->grammer_file = grammer_file;
    this->input_file = input_file;
}

//Printer::Printer(DFAstate* s){
//    this->start_state = s;
//    this->is_a_final_detected = false;
//    Printer::controller2();
//}

bool Printer::check(DFAstate* s) {
    if(s->is_acceptance_state) {
        this->is_a_final_detected = true,
                this->last_accepted_state = s;
        return true;
    }
    return false;
}

//void Printer::controller2() {
//    string current_line("???aabbtttabbabbttt");
//    istringstream lexems(current_line);
//    string lexeme;
//    while(lexems >> lexeme) {
//        lexeme = ::upgrade_string(this->start_state, lexeme);
//        if(lexeme.empty()) { ::error_handle(); continue; }
//        this->current_state = this->start_state;
//        this->check(current_state);
//        int last_accepted_index = 0;
//        for(int i = 0; i < lexeme.length(); ++i) {
//            this->current_state = this->move(lexeme.at(i), current_state);
//            if (this->current_state == this->start_state) {
//                if(this->is_a_final_detected)
//                    cout << this->last_accepted_state->acceptance_expression << endl,
//                    this->is_a_final_detected = false,
//                    i = last_accepted_index;
//            }
//            if(this->check(current_state)) {
//                last_accepted_index = i;
//            }
//        }
//        if(this->is_a_final_detected) {
//            cout << this->last_accepted_state->acceptance_expression << endl;
//        }
//    }
//}



void Printer::__init__() {
    DFAminimizer dfa_minimizer(grammer_file);
    dfa_minimizer.minimize();
    this->start_state = dfa_minimizer.get_start_minimized_gragh();
    this->is_a_final_detected = false;

    /// start actual program
    input.open(this->input_file);
    this->controller();
}

void Printer::controller() {
    if(!this->input.is_open())
        cout << "Check your input file path, please...";

    char input_line[256];
    while(!this->input.eof()) {
        input.getline(input_line, 256);
        string current_line(input_line);
        istringstream lexems(current_line);
        string lexeme;
        while(lexems >> lexeme) {
            lexeme = ::upgrade_string(this->start_state, lexeme);
            if (lexeme.empty()) {
                ::error_handle();
                continue;
            }
            this->current_state = this->start_state;
            this->check(current_state);
            int last_accepted_index = 0;
            for (int i = 0; i < lexeme.length(); ++i) {
                this->current_state = this->move(lexeme.at(i), current_state);
                if (this->current_state == this->start_state) {
                    if (this->is_a_final_detected)
                        cout << this->last_accepted_state->acceptance_expression << endl,
                                this->is_a_final_detected = false,
                                i = last_accepted_index;
                }
                if (this->check(current_state)) {
                    last_accepted_index = i;
                }
            }
            if (this->is_a_final_detected) {
                cout << this->last_accepted_state->acceptance_expression << endl;
            }
        }
    }
}


DFAstate * Printer::move(char edge_character, DFAstate *current_state) {
    if(current_state->childrean.find(edge_character) == current_state->childrean.end()) {
        ::error_handle();
        return this->start_state;
    }
    return current_state->childrean.find(edge_character)->second;
}


string upgrade_string(DFAstate* start_state, string str) {
    for(int i = 0; i < str.size() && start_state->childrean.find(str.at(i)) == start_state->childrean.end(); str = str.erase(i, 1));
    return str;
}

void error_handle(){
    ::errornous_lines.insert(::line_number);
}


