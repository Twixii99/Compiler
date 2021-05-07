//
// Created by magdy on 5/4/2021.
//
#include "NFAparser.h"
#include <iostream>
#include <stack>



void parentheses_check(string &str){
    stack<pair<char,int>> stack;
    for(int i=0 ; i<str.length() ; i++){
        char ch = str.at(i);
        if(ch == '(' && (i==0 ||  i>0 && str.at(i-1) != '\\')){
            stack.push(make_pair(ch,i));
        }
        if(ch == ')'&& (i==0 || i>0 && str.at(i-1) != '\\')){
            int end = i;
            int start = stack.top().second;
            stack.pop();
            build_bracket_graph(str,start,end);
            i = start;
        }
    }
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
}

Graph* split_with_space(string s){
    istringstream ss(s);
    string word;
    Graph* gh = nullptr;
    while (ss >> word){
        if(gh == nullptr)
            gh = build_graph(word);
        else
            gh = concatenate(gh,build_graph(word));
    }
    return gh;
}


