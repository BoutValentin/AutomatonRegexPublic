#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "automaton.h"
#include "parcours.h"
#include "argument.h"
#include "determinisation.h"
#include "overide.h"

int main(int argc, char **argv){

     //Creation de l'automaton
     //affichage automaton
     //traitement mor + automaton
     //final accept or not 
     int * shouldOveride;
     *shouldOveride = 0;
     if(defineArg(argc, argv, shouldOveride)==-1) return -1;
     Automaton * automate = constructor(argv[1]);
     if (automate==NULL) return 1;
     print_automaton(automate);
     if(shouldDeterminizate(automate)==1){
          
         algorithmDeterminization(automate);
         if(*shouldOveride==1){
             int succes = overideFile(argv[1], automate);
         }
     }
     //freeAutomaton(automate);
     int code = parcours(automate, argv[2]);
     freeAutomaton(automate);
     return 0;
}