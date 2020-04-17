#ifndef AUTOMATON_H_MACRO
#define AUTOMATON_H_MACRO
#include "type.h"
Automaton* constructor(char* path );
void print_automaton(Automaton* automa);
void freeAutomaton(Automaton *automaton);
int getPosition(char letter, char array_letter[], int arrayLength);
#endif