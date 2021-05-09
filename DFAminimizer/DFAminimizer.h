#ifndef TRYING_DFAMINIMIZER_H
#define TRYING_DFAMINIMIZER_H

#include "../DFA/DFAbuilder.h"
/**
// A  b Z
// B  a Z    b Z
[a b c d f e]
non accepting [a b]
accepting : letter[] -> [] []  [a b]
            num[]
map<label,vector<state>>  non -> vector
                          p1   -> vector
                          p2 -> vector
                          // 0 1 2 3


                                  0  1  2  3
                                  vector<vector<State*>>
                                  if vector is partitioned 1- remove it
                                                           2- add sub vectors
                                                           3- update all indexes
                                                           4- repeat
                                  3 states == 3 states
                                  size == size





                                  vector contains all minimized states
                                 p1=  [[a] [b] [c] [d][][][][][][]]
                                 store all indexes
                                 state -> index
                                 a index d index
                                 c   a d
                   p1 =          [] [a,b,c,d]
                   p2 =          [same]   [a] [b,d] [c]

                             [0 1 2] [2
 **/

class DFAminimizer {
    DFAbuilder* dfAbuilder;
    DFAstate* start_DFA_graph;
    vector<DFAstate*> DFA_states;
    DFAstate* start_minimized_gragh;
    vector<DFAstate*> minimized_states;
    void create_graph(vector<vector<DFAstate*>> states_vector);

public:
    DFAminimizer(string filename) {
        dfAbuilder = new DFAbuilder(filename);
        start_DFA_graph = dfAbuilder->get_start_state();
        DFA_states = dfAbuilder->get_all_created_states();
    }

    void minimize();
    void print_DFA_table(string);
    DFAstate* get_start_minimized_gragh();
    vector<DFAstate*> get_minimized_states();
};


bool is_equivalent(DFAstate* s1 , DFAstate* s2);
vector<vector<DFAstate*>> partition(vector<DFAstate*> v);
void update_indexes(vector<vector<DFAstate*>> v);

#endif //TRYING_DFAMINIMIZER_H