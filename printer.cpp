#include "printer.h"
#include <sstream>
#include <string>

using namespace std;

vector<int> errornous_lines;

int line_number = 0;

Printer::Printer(string input_file, string grammer_file, string output_file, string table_file_name) {
    this->grammer_file = grammer_file;
    this->input_file = input_file;
    this->output_file = output_file;
    this->table_file_name = table_file_name;
}

bool Printer::check(DFAstate* s) {
    if(s->is_acceptance_state) {
        this->is_a_final_detected = true,
        this->last_accepted_state = s;
        return true;
    }
    return false;
}

void Printer::__init__() {
    DFAminimizer dfa_minimizer(grammer_file);
    dfa_minimizer.minimize();
    this->start_state = dfa_minimizer.get_start_minimized_gragh();
    this->is_a_final_detected = false;

    /// start actual program
    this->input.open(this->input_file);
    this->output.open(this->output_file);
    this->controller();
    this->print_errors();
    dfa_minimizer.print_DFA_table(this->table_file_name);
}

void Printer::controller() {
    if(!this->input.is_open())
        cout << "Check your input file path, please...";

    char input_line[256];
    while(!this->input.eof()) {
        input.getline(input_line, 256);
        ::line_number++;
        string current_line(input_line);
        istringstream lexems(current_line);
        string lexeme;
        while(lexems >> lexeme) {
            lexeme = ::upgrade_string(this->start_state, lexeme);
            if (lexeme.empty()) continue;
            this->current_state = this->start_state;
            this->check(current_state);
            int last_accepted_index = 0;
            for (int i = 0; i < lexeme.length(); ++i) {
                this->current_state = this->move(lexeme.at(i), current_state);
                if (this->current_state == this->start_state) {
                    if (this->is_a_final_detected)
                        this->output << this->last_accepted_state->acceptance_expression << endl,
                                this->is_a_final_detected = false,
                                i = last_accepted_index + 1;
                }
                if (this->check(current_state)) {
                    last_accepted_index = i;
                }
            }
            if (this->is_a_final_detected) {
                this->output << this->last_accepted_state->acceptance_expression << endl;
            }
        }
    }
}


DFAstate * Printer::move(char edge_character, DFAstate *current_state) {
    if(current_state->childrean.find(edge_character) == current_state->childrean.end()) {
        if(this->start_state->childrean.find(edge_character) == this->start_state->childrean.end())
            ::error_handle();
        return this->start_state;
    }
    return current_state->childrean.find(edge_character)->second;
}

string upgrade_string(DFAstate* start_state, string str) {
    for(int i = 0; i < str.size() && start_state->childrean.find(str.at(i)) == start_state->childrean.end();
    ::error_handle(), str = str.erase(i, 1));
    return str;
}

void error_handle(){
    ::errornous_lines.push_back(::line_number);
}

void Printer::print_errors() {
    this->output << "******************************* ERRORS **********************************" << endl;
    for(int i : ::errornous_lines)
        this->output << "line " << i << " has error(s)" << endl;
}


