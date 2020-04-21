#ifndef AUTOMATON_H_MACRO
#define AUTOMATON_H_MACRO
#include "type.h"
Automaton* constructor(char* path );
void print_automaton(Automaton* automa);
void freeAutomaton(Automaton *automaton);
int getPosition(char letter, char array_letter[], int arrayLength);
int pow2(int position);
void copyArray(int nb_lig, int nb_col, binary_vector **m1, binary_vector **m2);
void copyArrayChar(int col, char ar1[], char ar2[]);
void copyArrayInt(int col, int ar1[], int ar2[]);
#endif