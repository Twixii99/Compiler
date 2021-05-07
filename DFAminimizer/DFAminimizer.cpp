//
// Created by magdy on 5/6/2021.
//



#include "DFAminimizer.h"



void DFAminimizer::minimize(){
    vector<vector<DFAstate*>> states_vector;
    vector<DFAstate*> non_final;
    vector<DFAstate*> final;

    for(DFAstate* state : DFAminimizer::dfAbuilder->get_all_created_states()){
        if(state->is_acceptance_state)
            final.push_back(state);
        else non_final.push_back(state);
    }

    states_vector.push_back(non_final);

    unordered_map<string,vector<DFAstate*>> map;

    for(DFAstate* state : final)
        map[state->acceptance_expression].push_back(state);

    for(auto state_pair : map)
        states_vector.push_back(state_pair.second);


    update_indexes(states_vector);
    bool done;
    do {
        done = 1;
        for (auto i = states_vector.begin(); i != states_vector.end(); ++i) {
            vector<DFAstate*> v = *i;
            vector<vector<DFAstate*>> state_partitions = partition(v);
            if(state_partitions.size() == 1)continue;
            else{
                states_vector.erase(i);
                for(auto partition : state_partitions){
                    states_vector.push_back(partition);
                }
                update_indexes(states_vector);
                done = false;
                break;
            }
        }
    } while (!done);

    create_graph(states_vector);
};

void DFAminimizer::create_graph(vector<vector<DFAstate*>> states_vector){

    int start_index = start_DFA_graph->index;
    int graph_size = states_vector.size();
    vector<DFAstate*> new_states;
    for(int i=0 ; i<graph_size ; i++){
        vector<DFAstate*> Vstates = states_vector[i];
        DFAstate* state = new DFAstate();

        state->index = i;
        state->is_acceptance_state = Vstates[0]->is_acceptance_state;
        state->acceptance_expression = Vstates[0]->acceptance_expression;

        new_states.push_back(state);
    }

    for(int i=0 ; i<graph_size ; i++){
        vector<DFAstate*> Vstates = states_vector[i];
        DFAstate* state = new_states[i];
        for(auto child : Vstates[0]->childrean){
            state->childrean.insert(make_pair(child.first,new_states[child.second->index]));
        }
    }

    start_minimized_gragh = new_states[start_index];
    minimized_states = new_states;
}

bool is_equivalent(DFAstate* s1 , DFAstate* s2){
    //cout << "is quivalent" << s1->name_of_state << " " << s2->name_of_state;
    if(s1->childrean.size() != s2->childrean.size())
        return false;

    for(auto child : s1->childrean){
        DFAstate* s1_child = child.second;

        if(s2->childrean.find(child.first) == s2->childrean.end())return false;

        DFAstate* s2_child = s2->childrean.find(child.first)->second;

        if(s1_child->index != s2_child->index)return false;
    }

    return true;

};


vector<vector<DFAstate*>> partition(vector<DFAstate*> v){
 vector<vector<DFAstate*>> sub_vectors;
 vector<DFAstate*> first_vector;
 first_vector.push_back(v[0]);
 sub_vectors.push_back(first_vector);


 bool new_partition;
 for(int i=1 ; i<v.size() ; i++){
     new_partition = true;

     DFAstate* state = v[i];
     for(vector<DFAstate*> &sv : sub_vectors){
         if(is_equivalent(state,sv[0])) {
             sv.push_back(state);
             new_partition = false;
             break;
         }
     }

     if(new_partition){
         vector<DFAstate*> sv;
         sv.push_back(state);
         sub_vectors.push_back(sv);
     }
 }
    return sub_vectors;
};
void update_indexes(vector<vector<DFAstate*>> v){
    int i=0;
    for(vector<DFAstate*> sv : v){
        for(DFAstate* dfs : sv){
            dfs->index = i;
        }
        i++;
    }
}

void DFAminimizer::print_DFA_table() {
    for (int i = 0; i < minimized_states.size(); i++) {
        cout << "state#" << i << endl;
        cout << "children are:" << endl;
        for (auto child : minimized_states[i]->childrean) {
            cout << child.first << "->" << "state#" << child.second->index << endl;
        }
    }
}
