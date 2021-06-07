//
// Created by Kamal on 08/06/2021.
//

#include "initialization.h"

#include <pthread.h>

using namespace std;

void start() {
    pthread_t lex, parse;
    void *lex_status, *parse_status;
    string lexical_grammar, input_file, output_file, lex_output_file, CFG_file;

    cout << "Please, enter the lexical grammar file path: ";
    cin >> lexical_grammar;
    pthread_create(&lex, NULL, lexical_initializer, &lexical_grammar);

    cout << "Please, enter the CFG file path: ";
    cin >> CFG_file;
    pthread_create(&parse, NULL, parser_initializer, &CFG_file);

    cout << "Please, enter the input file path: ";
    cin >> input_file;

    cout << "Please, enter the lexical output file path: ";
    cin >> lex_output_file;

    cout << "Please, enter the parser output file path: ";
    cin >> output_file;

    pthread_join(lex, &lex_status);
    pthread_join(parse, &parse_status);

    ::start_parsing(input_file, lex_output_file, output_file, (DFAstate*)lex_status, (table_builder*)parse_status);
}

void* lexical_initializer(void* lex) {
    string temp = *((string*)lex);
    DFAminimizer dfaMinimizer(temp);
    dfaMinimizer.minimize();
    DFAstate *first_state = dfaMinimizer.get_start_minimized_gragh();
    return first_state;
}

void* parser_initializer(void* parse) {
    table_builder *builder = new table_builder(*((string*)parse));
    return builder;
}

void start_parsing(string input, string lexical_output, string parser_output, DFAstate* first_state, table_builder* builder) {

    Printer p (lexical_output);
    p.__init__(first_state);

    Interface interface(input, &p);

    parse_input(builder, &interface, parser_output);
}

