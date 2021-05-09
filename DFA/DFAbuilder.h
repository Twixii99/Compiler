//
// Created by magdy on 5/1/2021.
//

#ifndef COMPILER_DFABUILDER_H
#define COMPILER_DFABUILDER_H

// #include "NFAbuilder.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <iostream>
#include <queue>
#include <set>
#include "../NFA/NFAbuilder.h"
//#define eps '~'


using namespace std;

//class State{
//public:
//    multimap<char,State*> children;
//    string label;
//    bool final;
//    bool reserved;
//    int number;
//    State(){
//        final = false;
//        reserved = false;
//    }
//};

class DFAstate {
public:
    DFAstate(bool isAcceptanceState, const string &acceptanceExpression);

public:
    bool is_acceptance_state;
    string acceptance_expression;
    set<State*> nfa_replacement_states;
    unordered_map<char, DFAstate*> childrean;
    int index;

    DFAstate() {
        this->is_acceptance_state = false;
        this->acceptance_expression = "";
    }

};

class DFAbuilder {

    DFAstate start_state;
    void controller();
    void __init__(State*);
    void connections_builder(DFAstate*);
    void eps_closure(State*, struct DFAstate*);
    vector<DFAstate*> already_created_states;

public:
    DFAbuilder(string filename);
    DFAstate * get_start_state();
    vector<DFAstate*> get_all_created_states();
};

bool validate_equlaity(DFAstate*, DFAstate*);
unordered_set<State*> BFS(State*, char);


#endif //COMPILER_DFABUILDER_H