#include "parser.h"
#include "regex_parser.h"


Parser::Parser(string file_name) {
    this->grammer_file.open(file_name);
    this->tokens.reserve(100);
    this->start_reading();
    this->grammer_file.close();
}

void Parser::start_reading() {
    char buffer[256];
    if(this->grammer_file.is_open()) {
        while(!this->grammer_file.eof()) {
            this->grammer_file.getline(buffer, 100);
            string current_line(buffer);
            if(current_line.empty()) continue;
            enum Tokens_types type = this->check_line_type(current_line);
            if(type == EXPRESSION || type == DEFINITION)
                this->definition_expression_builder(current_line, type);
            else
                this->keyword_punctuation_builder(current_line);
        }
    }
}

enum Tokens_types Parser::check_line_type(string& current_line) {
    if(regex_match(current_line, regex(".+:.+")))
        return EXPRESSION;
    else if(regex_match(current_line, regex(".+=.+")))
        return DEFINITION;
    else if(current_line[0] == '{') {
        current_line = current_line.substr(1, current_line.find("}")-1);
        return KEYWORD;
    } else
        current_line = current_line.substr(1, current_line.find("]")-1);
    return PUNCTUATION;
}

void Parser::definition_expression_builder(string& current_line, enum Tokens_types type) {
    size_t position = type == EXPRESSION ? current_line.find(":") : current_line.find("=");
    string lhs = current_line.substr(0, position);
    string rhs = current_line.substr(position+1, current_line.length());
    this->tokens.push_back(make_pair(this->update_string(lhs), make_pair(this->update_string(rhs), type == EXPRESSION)));
}

void Parser::keyword_punctuation_builder(string& current_line) {
    regex reg("\\s+");
    reg_s splitter = ::parse_expressions(current_line, reg);
    while(splitter != empty) {
        if(string(*splitter).empty()) splitter++;
        this->tokens.push_back(make_pair(*splitter, make_pair(*splitter, true))),
                splitter++;
    }
}

string& Parser::update_string(string &current_line) {
    for(auto dummy = current_line.begin(); (*dummy == ' ' || *dummy == '\t') && dummy != current_line.end();)
        current_line.erase(dummy);
    for(auto dummy = current_line.end()-1; (*dummy == ' ' || *dummy == '\t') && dummy != current_line.begin(); --dummy)
        current_line.erase(dummy);
    return current_line;
}

bool Parser::has_next() {
    return !this->tokens.empty();
}

pair<string, pair<string, bool> > Parser::get_next() {
    pair<string, pair<string, bool> > dummy = this->tokens.front();
    this->tokens.erase(this->tokens.begin());
    return dummy;
}                                