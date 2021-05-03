//
// Created by magdy on 5/1/2021.
//


#ifndef COMPILER_NFABUILER_H
#define COMPILER_NFABUILER_H
#include <bits/stdc++.h>
#include <string>
#include <unordered_map>
using namespace std;

#define eps '~'

class State{
public:
    multimap<char,State*> children;
    string label;
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

    void print(State *st,unordered_set<State*> &set) {

        if(set.find(st) != set.end())
            return;


        set.insert(st);
        if(st->final)
            cout << "final" + st->label << endl;
       for(auto x = st->children.begin() ; x != st->children.end() ; x++){
           cout << x->first << "node" << endl;
           print(x->second,set);
       }
    }

    void print_graph(){
        unordered_set<State*>set;
        print(start_state,set);
    }


    virtual State* get_NFA_graph(string filename);

    State* copy(State* state,Graph* g,unordered_map<State*,State*> &map){
        if(map.find(state) != map.end())
            return map.find(state)->second;

        State* s = new State();
        *s = *state;
        s->children.clear();
        map.insert(make_pair(state,s));

        if(state == end_state){
             g->end_state = s;
        }

        for(auto x : state->children){
            s->children.insert(make_pair(x.first,copy(x.second,g,map)));
        }

        return s;
    }

    Graph* make_copy(){

        unordered_map<State*,State*> map;
        Graph* gh = new Graph();
       State* s = copy(start_state,gh,map);
       gh->start_state = s;
       return gh;
    }
};

 unordered_map<string,Graph*> mp;


Graph* get_NFA(string s);
Graph* create_graph(string str);
Graph* concatenate(Graph* g1 , Graph* g2);
Graph* or_graph(Graph* g1,Graph* g2);
Graph* keen_closure(Graph* g1);
Graph* positive_closure(Graph* g1);
void parentethes_check(string &str);

#endif //COMPILER_NFABUILER_H
