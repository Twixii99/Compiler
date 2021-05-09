#include "graph.h"
#include <unordered_set>
#include <unordered_map>
#include "NFAbuilder.h"

using namespace std;

int cnt = 0;
unordered_map<string,Graph*> mp;

Graph* initiate_graph(){
    State* start_state = new State();
    State* end_state = new State();
    Graph* gh = new Graph(start_state,end_state);
    return gh;
}

Graph* concatenate(Graph* g1 , Graph* g2){
    g1 = g1->copy();
    g2 = g2->copy();
    g1->end_state->children.insert(make_pair(eps,g2->start_state));
    g1->end_state = g2->end_state;
    return g1;
};

Graph* or_graph(Graph* g1,Graph* g2){
    Graph* gh = initiate_graph();

    g1 = g1->copy();
    g2= g2->copy();

    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    gh->start_state->children.insert(make_pair(eps,g2->start_state));
    g1->end_state->children.insert(make_pair(eps,gh->end_state));
    g2->end_state->children.insert(make_pair(eps,gh->end_state));
    return gh;
};

Graph* keen_closure(Graph* g1){
    Graph* gh = initiate_graph();
    g1 = g1->copy();
    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    g1->end_state->children.insert(make_pair(eps,gh->end_state));

    gh->start_state->children.insert(make_pair(eps,gh->end_state));

    g1->end_state->children.insert(make_pair(eps,g1->start_state));
    return gh;
};

Graph* positive_closure(Graph* g1){

    Graph* gh = initiate_graph();
    g1 = g1->copy();
    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    g1->end_state->children.insert(make_pair(eps,gh->end_state));
    g1->end_state->children.insert(make_pair(eps,g1->start_state));

    return gh;

};

void Graph:: print_graph(){
    unordered_set<State*>set;
    print_all_states(start_state);
}

State* copy_state(Graph* gh,State* state,Graph* new_graph,unordered_map<State*,State*> &map){
    if(map.find(state) != map.end())
        return map.find(state)->second;

    State* s = new State();
    *s = *state;
    s->children.clear();
    map.insert(make_pair(state,s));

    if(state == gh->end_state){
        new_graph->end_state = s;
    }

    for(auto x : state->children){
        s->children.insert(make_pair(x.first,copy_state(gh,x.second,new_graph,map)));
    }

    return s;
}

void print_state(State* state,unordered_set<State*> &set) {
    if(state->final) {
        cout << "  label is  " << state->label << endl;
        return;
    }

    if (set.find(state) != set.end())
        return;

    set.insert(state);

    for (auto x : state->children) {
        cout  << " " <<  x.first << "node ";
        print_state(x.second, set);
    }
}
void print_all_states(State* s) {
    unordered_set<State*> st;
    print_state(s,st);
}

Graph* create_graph(string str){
    if(mp.find(str) != mp.end() || mp.find(" " + str + " ") != mp.end()){
        return mp.find(str)->second;
    }
    else{
        Graph* gh = initiate_graph();
        // a-z A-Z 0-9
        if(str.length() == 3 && str.at(1) == '-'){
            for(char ch = str.at(0) ; ch <= str.at(2) ; ch++){
                State* st = new State();
                gh->start_state->children.insert(make_pair(ch,st));
                st->children.insert(make_pair(eps,gh->end_state));
            }
        }

        else{

            State* tmp_state = gh->start_state;
            for(int i=0 ; i<str.length() ; i++){
                char ch = str.at(i);
                if(ch == '\\'){
                    if(i<(str.length()-1) && str.at(i+1) == '\\')i+=2;
                    else continue;
                }

                State* st = new State();
                if(ch =='L' && i>0 && str.at(i-1) == '\\')
                    tmp_state->children.insert(make_pair(eps,st));
                else tmp_state->children.insert(make_pair(ch,st));
                tmp_state = st;
            }
            gh->end_state = tmp_state;
        }
        return gh;
    }
}

void build_bracket_graph(string &str,int start,int end){
    string brackets =  "#"+std::to_string(cnt) ; // #1 #2
    create_NFA(make_pair(brackets,make_pair(str.substr(start+1,end-start-1), false)));
    str.replace(start,end-start+1,brackets);
    cnt++;
}

Graph* build_graph(string s){

    if(s.at(s.length()-1) == '*' && s.length() > 1 && s.at(s.length()-2) != '\\'){
        Graph* g = create_graph(s.substr(0,s.length()-1));
        return keen_closure(g);
    }

    else if(s.at(s.length()-1) == '+'&& s.length() > 1 && s.at(s.length()-2) != '\\'){
        Graph* g = create_graph(s.substr(0,s.length()-1));
        return positive_closure(g);
    }
    else return create_graph(s);
}

Graph* Graph::copy(){
    unordered_map<State*,State*> map;
    Graph* gh = new Graph();
    State* s = copy_state(this,start_state,gh,map);
    gh->start_state = s;
    return gh;
}

void store_definition(string label ,Graph* gh){
    mp.insert(make_pair(label,gh));
}