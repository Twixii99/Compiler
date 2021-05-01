//
// Created by magdy on 5/1/2021.
//


#ifndef COMPILER_NFABUILER_H
#define COMPILER_NFABUILER_H
#include <bits/stdc++.h>

/**
1--> a-z   graph from a to z
input ----> label :|=  expression
2--> map of graphs
 **/
// vector of int
// map for nodes , information about node
// pair map,node
// int
// for
// index num of node
// map
vector< pair<multimap<char,int>,Node> > graph;

/**
// 0 -> 1,4,8
0 map --> <a,Node> --> Node { index,
1
2
3
check equal -> equal -> priority
             -> less
**/

//refrence for start node
class Node{
//map for children
string label;
bool final;
bool reserved;
};
#endif //COMPILER_NFABUILER_H
