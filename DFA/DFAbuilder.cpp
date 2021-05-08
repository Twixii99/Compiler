#include "DFAbuilder.h"


map<char, set<State*> > uxiliary_container;

DFAbuilder::DFAbuilder(string filename) {
    NFAbuilder nfAbuilder(filename);
    State* s;
    s = nfAbuilder.get_NFA_start_state();
    DFAbuilder::__init__(s);
}

void DFAbuilder::__init__(State* initial_state) {
    this->eps_closure(initial_state, &(this->start_state));
    DFAbuilder::already_created_states.push_back(&(this->start_state));
    this->controller();
}

void DFAbuilder::eps_closure(State* state, DFAstate * initial_state) {
    unordered_set<State*> replaces_states = ::BFS(state, eps);
    for(unordered_set<State*>::iterator itr = replaces_states.begin(); itr != replaces_states.end(); ++itr) {
        if((*itr)->final) {
            initial_state->is_acceptance_state = true;
            initial_state->acceptance_expression = (*itr)->label;
        }
        initial_state->nfa_replacement_states.insert(*itr);
    }
}

void DFAbuilder::controller() {
    queue<DFAstate*> states_to_be_built;
    states_to_be_built.push(&(this->start_state));
    while(!states_to_be_built.empty()) {
        DFAstate* s = states_to_be_built.front();
        states_to_be_built.pop();

        this->connections_builder(s);  // there's an error due to pointer change
        for(map<char, set<State*> >::iterator it  = ::uxiliary_container.begin(); it != ::uxiliary_container.end(); ++it) {
            DFAstate * child = new DFAstate();
            child->nfa_replacement_states = it->second;

            for(State* final_checker : child->nfa_replacement_states) {
                if (final_checker->final) {
                    child->is_acceptance_state = true,
                            child->acceptance_expression = final_checker->label;
                    if (final_checker->reserved) break;
                }
            }

            bool check = false;
            for(DFAstate* itr : DFAbuilder::already_created_states) {
                if(check) break;
                if(!::validate_equlaity(itr, child)) {  // true if we need the child // there's a problem here too
                    s->childrean.insert(make_pair(it->first, itr));
                    check = true;
                    delete child;
                }
            }
            if(!check) {
                s->childrean.insert(make_pair(it->first, child));
                DFAbuilder::already_created_states.push_back(child);
                states_to_be_built.push(child);
            }
        }
        ::uxiliary_container.clear();
    }
}

/**
*   priority and acceptance states
*   we need a multimap to make some thing like this:
*       sA -> {{a, SB}; {b, sC}; {x, sE}}
*      sA contains NFAstates {s0, s1, s2}
*      the multimap will contains :
*      {s0 a edges and related ~} {s1 a edges and related ~} {s2 a edges and related ~}
*      {s0 b edges and related ~} {s1 b edges and related ~} {s2 b edges and related ~}
*      {s0 x edges and related ~} {s1 x edges and related ~} {s2 x edges and related ~}
*      and we loop throw all letters of each single NFAstate so we need some a datastructure to maintian all related states
*      came from each NFAstate to add other related states from other NFAstates (Multimap);
*/

void DFAbuilder::connections_builder(DFAstate* parent_state) {
    for(auto itr : parent_state->nfa_replacement_states) {
        set<char> characters_already_tokens;
        for(multimap<char, State*>::iterator it = (itr->children).begin(); it->first != '~' && it != (itr->children).end(); ++it) {
            if(characters_already_tokens.find(it->first) != characters_already_tokens.end()) continue;
            characters_already_tokens.insert(it->first);
            pair<multimap<char, State*>::iterator, multimap<char, State*>::iterator> returned_range;
            returned_range = itr->children.equal_range(it->first);
            if(::uxiliary_container.find(it->first) == ::uxiliary_container.end())
                ::uxiliary_container.insert(make_pair(it->first, set<State*>()));
            for(multimap<char, State*>::iterator levels = returned_range.first; levels != returned_range.second; ++levels) {
                uxiliary_container.find(it->first)->second.insert(levels->second);
                for(State* x : ::BFS(levels->second, eps)) {
                    uxiliary_container.find(it->first)->second.insert(x);
                }
            }
        }
    }
}

DFAstate * DFAbuilder::get_start_state() {
    return &(this->start_state);
}

vector<DFAstate*> DFAbuilder::get_all_created_states() {
    return this->already_created_states;
}

unordered_set<State*> BFS(State* state, char edge_weight) {
    unordered_set<State*> replaced_states;
    queue<State*> bfs;
    bfs.push(state);
    while(!bfs.empty()) {
        replaced_states.insert(bfs.front());
        pair<multimap<char, State*>::iterator, multimap<char, State*>::iterator> returned_rage;
        returned_rage = bfs.front()->children.equal_range(edge_weight);
        bfs.pop();
        for(multimap<char, State*>::iterator itr = returned_rage.first; itr != returned_rage.second; ++itr) {
            bfs.push(itr->second);
        }
    }
    return replaced_states;
}

bool validate_equlaity(DFAstate* old_state, DFAstate* new_state) {
    if(old_state->nfa_replacement_states.size() != new_state->nfa_replacement_states.size()) return true;
    set<State*> comparator(old_state->nfa_replacement_states.begin(), old_state->nfa_replacement_states.end());
    comparator.insert(new_state->nfa_replacement_states.begin(), new_state->nfa_replacement_states.end());
    return comparator.size() > old_state->nfa_replacement_states.size();
}