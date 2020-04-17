#include <stdio.h>
#include "type.h"
#include "automaton.h"
#include "parcours.h"
#include "argument.h"
int main(int argc, char **argv){

     //Creation de l'automaton
     //affichage automaton
     //traitement mor + automaton
     //final accept or not 
     if(defineArg(argc, argv)==-1) return -1;
     Automaton * automate = constructor(argv[1]);
     if (automate==NULL) return 1;
     print_automaton(automate);
     //freeAutomaton(automate);
     int code = parcours(automate, argv[2]);
     return 0;
}