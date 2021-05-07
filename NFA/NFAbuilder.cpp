//
// Created by magdy on 5/2/2021.
//
#include "NFAbuilder.h"
#include "NFAparser.h"
#include <unordered_set>

State* NFA_start = new State();
void create_NFA(pair<string,pair<string,bool>> pair){
    string label = pair.first;
    string exp = pair.second.first;
    bool is_exp = pair.second.second;

    parentheses_check(exp);
    Graph* g = split(exp);
    if(is_exp){
        NFA_start->children.insert(make_pair(eps,g->start_state));
        unordered_set<State*> set;
        g->end_state->label = label;
        g->end_state->final = true;
        if(label == exp)
            g->end_state->reserved = true;
    }else{
        store_definition(label,g);
    }
}


State * NFAbuilder::get_NFA_start_state() {
    while (parser->has_next()){
        pair<string , pair<string,bool>> pair = parser->get_next();
        create_NFA(pair);
    }
    return NFA_start;
}

