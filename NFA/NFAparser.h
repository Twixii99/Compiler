//
// Created by magdy on 5/4/2021.
//

#ifndef COMPILER_NFAPARSER_H
#define COMPILER_NFAPARSER_H
#include "NFAbuilder.h"

Graph* split(string production);
Graph* split_with_space(string s);
void parentheses_check(string &str);
#endif //COMPILER_NFAPARSER_H
