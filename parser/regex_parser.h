#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#ifndef LIB
#include <regex>
#include <iterator>
#define LIB
#include <regex>
#include <iterator>
#endif

typedef std::regex_token_iterator<std::string::iterator> reg_s;

reg_s empty;

reg_s parse_expressions(std::string &str, const std::regex &reg) {
    reg_s splitter(str.begin(), str.end(), reg, -1);
    return splitter;
}


#endif