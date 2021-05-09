//
// Created by magdy on 5/4/2021.
//


#include "printer.h"

using namespace std;
int main() {
     Printer p ("/Users/Kamal/CLionProjects/Compiler/input.txt",
                "/Users/Kamal/CLionProjects/Compiler/grammar.txt",
                "/Users/Kamal/CLionProjects/Compiler/output.txt",
                "/Users/Kamal/CLionProjects/Compiler/table.txt");
     p.__init__();
}
