//
// Created by Kamal on 08/06/2021.
//

#ifndef COMPILER_INITIALIZATION_H
#define COMPILER_INITIALIZATION_H

#include "../printer_phase2/parser_interface.h"
#include "../phase2/tableBuilder.h"
#include "../phase2/outputGenerator.h"
#include <iostream>

void start();
void* lexical_initializer(void*);
void* parser_initializer(void*);
void start_parsing(std::string, std::string, std::string, DFAstate*, table_builder*);

#endif //COMPILER_INITIALIZATION_H
