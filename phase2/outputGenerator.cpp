#include "outputGenerator.h"
#include <vector>
#include <iostream>
#include <stack>
#include <string>


using namespace std;

vector<string> split_production(string production){
    vector<string> v;
    stringstream ss(production);
    string str;
    while (ss >> str){
        v.push_back(str);
    }
    return v;
}

void print_stack(stack<string> s) {
    while(!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;
}

void parse_input(table_builder* builder, Interface* lex, string file) {
    ofstream fs(file);
    if(!fs.is_open())
        cout << "The parser output file isn't supported, please check it..."  << endl;

    stack<string> stack;
    stack.push("$");
    stack.push(builder->get_start_symbol());
    string token;
    if(lex->has_next_token())
        token = lex->get_next_token();
    else {
        fs << "The Whole input is a trash!!" << endl;
        exit(-1);
    }
    while (stack.top() != "$") {
        ::print_stack(stack);
        if (builder->is_termainal(stack.top())) {
            if (stack.top() == token) { // match
                fs << "match: " << token << endl;
            } else {
                fs << "error " << stack.top() << " doesn't match " << token << endl;
                fs << "inserted " << stack.top() << " in input" << endl;
            }
            stack.pop();
            if(lex->has_next_token())
                token = lex->get_next_token();
            else token = "$";

        } else { // not_terminal
            switch (builder->has_production(token,stack.top()))
            {
                case AVAILABLE: {
                    string production = builder->get_production(token,stack.top());
                    fs << stack.top() << " --> " << production << endl;
                    stack.pop();
                    if(production == "~") break;
                    vector<string> statements = split_production(production);
                    for (int i = statements.size() - 1; i >= 0; i--) {
                        stack.push(statements[i]);
                    }

                    break;
                }
                case SYNC:
                    fs << "ERROR(sync), top of stack is: " << stack.top() << ", token is: " << token << endl;
                    stack.pop();
                    break;
                case ERROR:
                    fs << "ERROR(EMPTY),top of stack is: " << stack.top() << ", token is: " << token << endl;
                    fs << token << " is discarded" << endl;
                    if(lex->has_next_token())
                        token = lex->get_next_token();
                    else token = "$";
                    break;
            }
        }
    }
    fs.flush();
    fs.close();
}
