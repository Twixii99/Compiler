//
// Created by magdy on 5/4/2021.
//

#ifndef COMPILER_GRAPH_H
#define COMPILER_GRAPH_H

#include <string>
#include <iostream>
#include <map>

using namespace std;
#define eps '~'

class State{
public:
    multimap<char,State*> children;
    std::string label;
    bool final;
    bool reserved;

    State(){
        final = false;
        reserved = false;
    }
};

class Graph{
public:
    State* start_state;
    State* end_state;

    Graph(State* s1 , State* s2){
        start_state = s1;
        end_state = s2;
    }

    Graph(){}

    virtual void print_graph();
    virtual Graph* copy();
};


void print_all_states(State* s);
void build_bracket_graph(string &str,int start,int end);
Graph* build_graph(string s);
void store_definition(string label,Graph* g);
Graph* concatenate(Graph* g1 , Graph* g2);
Graph* or_graph(Graph* g1,Graph* g2);
Graph* keen_closure(Graph* g1);
Graph* positive_closure(Graph* g1);


#endif //COMPILER_GRAPH_H
