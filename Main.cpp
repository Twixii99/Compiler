//
// Created by magdy on 5/4/2021.
//

#include "NFAbuilder.h"
#include "parser.h"


#include <windows.h>
#include <string>
using namespace std;
int main() {
    NFAbuilder nfAbuilder("C:\\Users\\magdy\\CLionProjects\\Compiler\\test.txt");
    State* st = nfAbuilder.get_NFA_start_state();
   // print_all_states(st);

//     parse("C:\\Users\\magdy\\CLionProjects\\Compiler\\test.txt");
//    while (parse.has_next()) {
//        pair<string, pair<string, bool> > result = parse.get_next();
//        cout << result.first << "\t" << result.second.first << "\t" << result.second.second << endl;
//    }
}
