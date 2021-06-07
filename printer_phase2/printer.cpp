#include "printer.h"
#include <sstream>
#include <string>

using namespace std;

int line_number;
set<int> errornous_lines;

Printer::Printer(string output_file) {
    this->output_file = output_file;
}

void Printer::check(DFAstate* s) {
    if(s->is_acceptance_state)
        this->is_a_final_detected = true,
        this->last_accepted_state = s;
}

void Printer::__init__(DFAstate *first_minimized_state) {
    this->start_state = first_minimized_state;
    this->is_a_final_detected = false;

    /// start actual program
    this->output.open(this->output_file);
}

vector<string> Printer::get_token(string lexeme, int line_number) {
    ::line_number = line_number;
    vector<string> tokens;
    lexeme = ::upgrade_string(this->start_state, lexeme);
    if(lexeme.empty()) return tokens;
    this->current_state = this->start_state;
    this->check(current_state);
    for (int i = 0; i < lexeme.length(); ++i) {
        this->current_state = this->move(lexeme.at(i), current_state);
        if (this->current_state == this->start_state) {
            if (this->is_a_final_detected) {
                this->output << this->last_accepted_state->acceptance_expression << endl;
                tokens.push_back(this->last_accepted_state->acceptance_expression);
                tokens.push_back(lexeme.substr(i));
                this->is_a_final_detected = false;
                return tokens;
            }
        }
        this->check(current_state);
    }
    if (this->is_a_final_detected) {
        this->output << this->last_accepted_state->acceptance_expression << endl;
        tokens.push_back(this->last_accepted_state->acceptance_expression);
    }
    return tokens;
}


DFAstate * Printer::move(char edge_character, DFAstate *current_state) {
    if(current_state->childrean.find(edge_character) == current_state->childrean.end()) {
        if(this->start_state->childrean.find(edge_character) == this->start_state->childrean.end())
            ::error_handler();
        return this->start_state;
    }
    return current_state->childrean.find(edge_character)->second;
}

string upgrade_string(DFAstate* start_state, string str) {
    for(int i = 0; i < str.size() && start_state->childrean.find(str.at(i)) == start_state->childrean.end();
    ::error_handler(), str = str.erase(i, 1));
    return str;
}

void error_handler(){
    ::errornous_lines.insert(::line_number);
}

void Printer::print_errors() {
    this->output << "******************************* ERRORS **********************************" << endl;
    for(int i : ::errornous_lines)
        this->output << "line " << i << " has error(s)" << endl;
}


