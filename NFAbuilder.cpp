//
// Created by magdy on 5/2/2021.
//
#include "NFAbuiler.h"
#include <bits/stdc++.h>
#include "parser.h"
int cnt = 0;



State* NFA_start = new State();

int main(){
    Graph* g = get_NFA("(ab   (ac))|if");
    g->print_graph();

//    istringstream ss("  hello | megz  **    *|         & ");
//
//    string word; // for storing each word
//
//    string token;
//    while(std::getline(ss, token, '|')) {
//        std::cout << token << '\n';
//    }
//    // Traverse through all words
//    // while loop till we get
//    // strings to store in string word
//    while (ss >> word)
//    {
//        // print the read word
//        cout << word << "\n";
//    }
}



Graph* initiate_graph(){
    State* start_state = new State();
    State* end_state = new State();
    end_state->final = true;

    Graph* gh = new Graph(start_state,end_state);
    return gh;
}


// ( a-z (else|if) )
void parentethes_check(string &str){
    stack<pair<char,int>> stack;
    for(int i=0 ; i<str.length() ; i++){
        char ch = str.at(i);
        if(ch == '('){
            stack.push(make_pair(ch,i));
        }
        if(ch == ')'){
            int end = i;
            int start = stack.top().second;
            stack.pop();

//            cout << "get Nfa string is" << str.substr(start+1,end-start-1)  << endl;
//            cout << "() is" << str.substr(start,end-start+1)  << endl;

//  ()*
            string brackets =  "#"+std::to_string(cnt) ; // #1 #2
            mp.insert(make_pair(brackets,get_NFA(str.substr(start+1,end-start-1))));
//            cout << "inserted" << brackets << endl;
            str.replace(start,end-start+1,brackets);
            i = start;
            cnt++;
        }
    }
}




// (a-c|   if)    |else --> |
// bool true expression false else


Graph* build_one_graph(string s){
    if(s.at(s.length()-1) == '*'){
        Graph* g = create_graph(s.substr(0,s.length()-1));
        return keen_closure(g);
    }

    else if(s.at(s.length()-1) == '+'){
        Graph* g = create_graph(s.substr(0,s.length()-1));
        return positive_closure(g);
    }
    else return create_graph(s);
}


Graph* split_with_space(string s){
    cout << "space " + s << endl;
    istringstream ss(s);
    string word;
    Graph* gh = nullptr;
    while (ss >> word){
        if(gh == nullptr)
            gh = build_one_graph(word);
        else
            gh = concatenate(gh,build_one_graph(word));
    }
    return gh;
}

Graph* split(string s){

    istringstream ss(s);

    string word;

    Graph* gh = nullptr;
    while (getline(ss,word,'|')){
        if(gh == nullptr)
            gh = split_with_space(word);
        else gh = or_graph(gh,split_with_space(word));
    }



    return gh;
//    int start = 0;
//    int end = s.find(delimiter);
//
//    Graph* gh = nullptr;
//    while (end != -1) {
//        string str = s.substr(start, end - start);
//
//        if(delimiter == "|"){
//            if(gh == nullptr){
//                gh = split(str," ");
//            }else{
//                gh = or_graph(gh,split(str," "));
//            }
//        }else if(delimiter == " "){
//            if(gh == nullptr){
//                gh = build_one_graph(str);
//            }else{
//                gh = concatenate(gh,build_one_graph(str));
//            }
//        }
//
//        start = end + delimiter.size();
//        end = s.find(delimiter, start);
//    }
//
//
//    string str = s.substr(start, end - start);
//
//    if(delimiter == "|"){
//        if(gh == nullptr){
//            gh = split(str," ");
//        }else{
//            gh = or_graph(gh,split(str," "));
//        }
//    }else if(delimiter == " "){
//        if(gh == nullptr){
//            gh = build_one_graph(str);
//        }else{
//            gh = concatenate(gh,build_one_graph(str));
//        }
//    }
//    return gh;
}





void create_NFA(pair<string,pair<string,bool>> pair){
    string label = pair.first;
    string exp = pair.second.first;
    bool is_exp = pair.second.second;

    parentethes_check(exp);

    Graph* g = split(exp);
    if(is_exp){
        NFA_start->children.insert(make_pair(eps,g->start_state));
        g->end_state->label = label;
        if(label == exp)
            g->end_state->reserved = true;

    }else{
        mp.insert(make_pair(label,g));
    }
}



// start -> a -> final
// start -> b -> final

// letter = a-z | A-Z




Graph* create_graph(string str){
    //   cout << "str  " + str << endl;
    if(mp.find(str) != mp.end() || mp.find(" " + str + " ") != mp.end()){
        cout << "in if" << str << endl;
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
            // boolean if else
            State* tmp_state = gh->start_state;
            for(char ch  : str){

                State* st = new State();
                tmp_state->children.insert(make_pair(ch,st));
                tmp_state = st;
            }
            gh->end_state = tmp_state;
            gh->end_state->final = true;
            gh->end_state->reserved = true;
        }

        gh->end_state->label = str;
        return gh;
    }
}


//  .    . .     .
Graph* concatenate(Graph* g1 , Graph* g2){
    g1 = g1->make_copy();
    g2 = g2->make_copy();
    string label = g1->end_state->label + " " + g2->end_state->label;
    g1->end_state->children.insert(make_pair(eps,g2->start_state));

    g1->end_state->final = false;

    g1->end_state = g2->end_state;


    g1->end_state->label = label;

    return g1;
};

Graph* or_graph(Graph* g1,Graph* g2){
    Graph* gh = initiate_graph();

    g1 = g1->make_copy();
    g2= g2->make_copy();

    g1->end_state->final = false;
    g2->end_state->final = false;

    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    gh->start_state->children.insert(make_pair(eps,g2->start_state));

    g1->end_state->children.insert(make_pair(eps,gh->end_state));
    g2->end_state->children.insert(make_pair(eps,gh->end_state));
    gh->end_state->label = g1->end_state->label +"|"+ g2->end_state->label;
    return gh;
};

Graph* keen_closure(Graph* g1){
    Graph* gh = initiate_graph();
    g1 = g1->make_copy();
    g1->end_state->final = false;

    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    g1->end_state->children.insert(make_pair(eps,gh->end_state));

    gh->start_state->children.insert(make_pair(eps,gh->end_state));

    g1->end_state->children.insert(make_pair(eps,g1->start_state));
    gh->end_state->final = true;
    gh->end_state->label = g1->end_state->label;
    return gh;
};

Graph* positive_closure(Graph* g1){

    Graph* gh = initiate_graph();
    g1 = g1->make_copy();
    g1->end_state->final = false;

    gh->start_state->children.insert(make_pair(eps,g1->start_state));
    g1->end_state->children.insert(make_pair(eps,gh->end_state));
    g1->end_state->children.insert(make_pair(eps,g1->start_state));

    gh->end_state->label = g1->end_state->label;
    gh->end_state->final = true;

    return gh;

};

State *Graph::get_NFA_graph(string filename) {
    Parser* parser = new Parser(filename);
    while (parser->has_next()){
        pair<string , pair<string,bool>> pair = parser->get_next();
        create_NFA(pair);
    }

    return start_state;
}
