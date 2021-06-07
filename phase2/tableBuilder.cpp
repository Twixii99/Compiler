//
// Created by magdy on 6/2/2021.
//

#include "tableBuilder.h"
#define non_terminal string
#define terminal string
#define set_pair pair<terminal,non_terminal>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
using namespace std;

vector<non_terminal> non_terminals; // includes all non-terminals like STATEMENT, STATEMENT_LIST
unordered_set<terminal> terminals; // includes all terminal statement like id , + , -

/**
 productions_map includes all productions in the form of
        < non_terminal , { production_one,
                         production_two } >
    ex: PRIMITIVE_TYPE = 'int' | 'float'

   stored as:
      <PRIMITIVE_TYPE, { {int},
                         {float} };
 */
unordered_map<non_terminal, vector<vector<string>>> productions_map;

/**


  includes all first_set and second_set for each non_terminal
  <non_terminal , <first_set,follow_set> >
  ex:  <STATEMENT,  <first_Set : {a,b} , follow_Set:{a,e,c}>>

  HINT:
  set pair non_terminal is the non_terminal which is the source of the terminal.

  YOU CAN CHANGE IT !!!!!! I DIDN"T USE IT!!!!!!!

 */


//  non_terminal, set<pair<start_string_of_each_production, the production itself>>
unordered_map<non_terminal,  set<pair<string, string>>> first_table;
unordered_map<non_terminal,  set<string>> follow_table;

// the terminal string and the  production produces it
unordered_map<non_terminal, unordered_map<terminal, string>> parser_table;

string start_symbol;
bool first = true;

table_builder::table_builder(string file_name) {
    ::parse(file_name);
    ::first_set_builder();
    ::follow_set_builder();
    ::parser_table_builder();
}

bool table_builder::is_termainal(string str) {
    return terminals.find(str) != terminals.end();
}

string table_builder::get_start_symbol() {
    return start_symbol;
}

/**
 * @param t
 *          current token
 * @param nt
 *          top of stack
 * @return
 */
Type table_builder::has_production( terminal current_token,  non_terminal tos) {
    if(::parser_table[tos].find(current_token) !=  parser_table[tos].end()) {
        if (::parser_table[tos][current_token] == "SYNC")
            return SYNC;
        else
            return AVAILABLE;
    }
    return ERROR;
}

string table_builder::get_production( terminal current_token,  non_terminal tos) {
    return  ::parser_table[tos][current_token];
}

void first_set_builder() {
    for(vector<string>::reverse_iterator itr = ::non_terminals.rbegin(); itr  != ::non_terminals.rend();  ++itr) {
        set<pair<string, string>> symbol;
        ::first_table.insert(make_pair(*itr, symbol));
        vector<vector<string>> corresponding_productions =  ::productions_map[*itr];
        for(vector<string> production : corresponding_productions) {
            string  temp = "";
            for(string s : production)
                temp = temp + s + ' ';
            temp.erase(temp.length()-1);
            bool  continue_scanning_production = true;
            for(int i = 0; i < production.size() && continue_scanning_production; ++i) {
                continue_scanning_production =  false;
                if(find(non_terminals.begin(), non_terminals.end(), production[i]) == non_terminals.end()) {
                    ::first_table[*itr].insert(make_pair(production[i], temp));
                    break;
                }
                for(pair<string, string> shared : ::first_table[production[i]]) {
                    if(shared.first == "~")
                        continue_scanning_production = true;
                    else {
                        ::first_table[*itr].insert(make_pair(shared.first, temp));
                    }
                }
                if(i == production.size()-1 && continue_scanning_production) {
                    ::first_table[*itr].insert(make_pair("~", temp));
                }
            }
        }
    }
}

void print_first_table(string file) {
    ofstream f(file);
    if(!f.is_open()) {
        cout << "The first file is not found" << endl;
        return;
    }
    for(pair<string, set<pair<string, string>>> itr : ::first_table) {
        f << itr.first << " ==> ";
        for(pair<string, string> itr2 : itr.second)
            f << "<" << itr2.first << " , " << itr2.second << "> ; ";
        f << "\n\n" << endl;
    }
    f.flush();
    f.close();
}

void follow_set_builder() {
    for(vector<string>::iterator itr = ::non_terminals.begin(); itr  != ::non_terminals.end();  ++itr) {
        set<string> temp;
        follow_table.insert(make_pair(*itr, temp));
        if(itr == ::non_terminals.begin())
            follow_table[*itr].insert("$");
        for (vector<string>::iterator itr2 = ::non_terminals.begin(); itr2 != ::non_terminals.end(); ++itr2) {
            if(*itr == *itr2) continue;
            vector<vector<string>> corresponding_productions =  ::productions_map[*itr2];
            for(vector<string> production : corresponding_productions) {
                for(int i = 0; i < production.size(); ++i) {
                    if(i  != production.size()-1 &&  production[i] == *itr  && production[i] !=  production[i+1]) {
                        if(find(::non_terminals.begin(), ::non_terminals.end(), production[i+1]) == ::non_terminals.end())
                            follow_table[*itr].insert(production[i+1]);
                        else {
                            for (pair<string, string> ss : first_table[production[i + 1]]) {
                                if (ss.first != "~")
                                    follow_table[*itr].insert(ss.first);
                                else {
                                    for (string s :  follow_table[production[i + 1]])
                                        follow_table[production[i]].insert(s);
                                }
                            }
                        }
                    }
                    else if(i == production.size()-1 &&  production[i] == *itr) {
                        for (string s :  follow_table[*itr2])
                            follow_table[*itr].insert(s);
                    }
                }
            }
        }
    }
}

void print_follow_table(string file) {
    ofstream f(file);
    if(!f.is_open()) {
        cout << "The follow file is not found" << endl;
        return;
    }
    for(pair<string, set<string>> itr : ::follow_table) {
        f << itr.first << " ==> ";
        for(string itr2 : itr.second)
            f << "<" << itr2 << "> ; ";
        f << "\n\n" << endl;
    }
    f.flush();
    f.close();
}

void check_ambiguity(string nt, string  str) {
    if(::parser_table[nt].find(str) != parser_table[nt].end()) {
        cout << nt << " " << str << " " << parser_table[nt][str] << endl;
        exit(-1);
    }
}

void parser_table_builder() {
    for(string nt : ::non_terminals)  {
        unordered_map<string, string> uxilliry;
        ::parser_table.insert({nt, uxilliry});
        bool sync_must = false;
        for(pair<string, string> str : ::first_table[nt]) {  // table column, what to write on the cell
            if(str.first != "~") {
                check_ambiguity(nt, str.first);
                ::parser_table[nt].insert(str);
            } else {
                sync_must = true;
                for (string s : follow_table[nt]) {
                    check_ambiguity(nt, s);
                    ::parser_table[nt].insert({s, "~"});
                }
            }
        }
        if(!sync_must) {
            for (string s : follow_table[nt]) {
                if(::parser_table[nt].find(s) == parser_table[nt].end())
                    ::parser_table[nt].insert({s, "SYNC"});
            }
        }
    }
}

void print_parse_table(string file) {
    ofstream f(file);
    if(!f.is_open()) {
        cout << "The parser file is not found" << endl;
        return;
    }
    for(pair<string, unordered_map<string, string>> itr :  ::parser_table) {
        f << itr.first << " ==> ";
        for(pair<string, string> str : itr.second) {
            f << "< " << str.first << " , " << str.second <<  " > ;";
        }
        f << "\n\n" << endl;
    }
    f.flush();
    f.close();
}

void split_with_or(istringstream &line_stream,string cur_non_terminal){
    string str;
    while (getline(line_stream,str,'|')) {
        if(str.size() == 0) continue;

        istringstream statement_stream(str);
        string statement;
        vector<string> statement_vec;

        while (statement_stream >> statement) {

            if(statement.at(0) == '\'' && statement.at(statement.size()-1) == '\'') {
                if(statement == "\'\\L\'") statement = "\'~\'";
                statement_vec.push_back(statement.substr(1, statement.size() - 2));
                terminals.insert(statement.substr(1, statement.size() - 2));
            }
            else statement_vec.push_back(statement);
        }
        productions_map[cur_non_terminal].push_back(statement_vec);
    }
}

void parse(string filename){
    ifstream ss(filename);
    fstream fs;
    fs.open("C:\\Users\\magdy\\CLionProjects\\Compiler Phase 2\\out.txt");
    string line;
    string cur_non_terminal;
    string str;
    while (!ss.eof() && getline(ss,line,'\n')){
        istringstream line_stream(line);
        if(line.at(0) == '#'){ // new expression  ///  # kamal mahmoud
            getline(line_stream,str,' '); // remove #
            getline(line_stream,str,' ');
            cur_non_terminal = str;

            if(first) {
                start_symbol = cur_non_terminal;
                first = false;
            }

            non_terminals.push_back(str);
            vector<vector<string>> statements;
            productions_map.insert(make_pair(cur_non_terminal, statements));

            getline(line_stream,str,' '); // remove =

            split_with_or(line_stream,cur_non_terminal);

        }else{
            split_with_or(line_stream,cur_non_terminal);
        }
    }

    fs.close();
}


void print(){
    fstream fs;
    fs.open("C:\\Users\\magdy\\CLionProjects\\Compiler Phase 2\\out.txt");

    for(auto x : productions_map){
        fs << "non_terminal "<< x.first << endl;
        for(vector<string> v : x.second){
            for(string s : v){
                fs << s << " ";
            }
            fs << endl;
        }
    }
}

void print_terminals() {
    for(string itr : ::terminals)
        cout << itr << endl;
}