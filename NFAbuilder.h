//
// Created by magdy on 5/1/2021.
//


#ifndef COMPILER_NFABUILDER_H
#define COMPILER_NFABUILDER_H
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "parser.h"
#include "graph.h"
using namespace std;

class NFAbuilder{
    Parser* parser;
public:
    NFAbuilder(string filename){
        this->parser = new Parser(filename);
    }
    State* get_NFA_start_state();
};


void create_NFA(pair<string,pair<string,bool>> pair);

#endif //COMPILER_NFABUILDER_H
