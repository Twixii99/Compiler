//
// Created by magdy on 5/1/2021.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

/**
// pass line exepression
//1--> {boolean int float} --> boolean
                        --> int
                        --> float
                        --> if


    digits = digit+
-->  int: int
     boolean: boolean
     ;: ;

   NFA  split :|=

2--> num: digit+ | digit+ . digits ( \L | E digits)

digit digit

**/


class Parser{
public:
    void Parser(string file){

    }

    bool has_next(){
        // true -> there is another expression
    };
    pair<string,string> get_next(){
      //

      // pair<label,expression>
        //<int,int>
        //<if,if>
        //<;,;>
        //<label,expression>
    }

};

class Parser{
public:
    bool has_next(){

    }



};

#endif //COMPILER_PARSER_H
