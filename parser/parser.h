//
// Created by magdy on 5/1/2021.
//

/**
// pass line exepression
//1--> {boolean int float} --> boolean
                        --> int
                        --> float
                        --> if


    digits = digit+
-->  int: int
     boolean: boolean
     ;: ;

   NFA  split :|=

2--> num: digit+ | digit+ . digits ( \L | E digits)

digit digit

**/

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <iterator>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

enum Tokens_types {
    DEFINITION, EXPRESSION, KEYWORD, PUNCTUATION
};

class Parser{
    ifstream grammer_file;
    vector<pair<string, pair<string, bool> > > tokens;

    void start_reading();
    string& update_string(string&);
    void keyword_punctuation_builder(string&);
    enum Tokens_types check_line_type(string&);
    void definition_expression_builder(string&, enum Tokens_types);

public:
    Parser(string s);
    virtual bool has_next();
    virtual std::pair<string,pair<string, bool> > get_next();
};

#endif //COMPILER_PARSER_H
