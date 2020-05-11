#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "automaton.h"
#include "parcours.h"
#include "argument.h"
#include "determinisation.h"
#include "overide.h"

int main(int argc, char **argv){
    //Création d'une variable et de son pointeur afin de déterminer dans les arguments si un overwrite devra etre fait
    int shouldOveride = 0;
    int *ptShouldOveride = &shouldOveride; 
    //On commence par verifier les arguments
    if(defineArg(argc, argv, ptShouldOveride)==-1) return 1;
    //On creer notre automate
    Automaton * automate = constructor(argv[1]);
    //Si pendant la création de l'automate une erreur a eu lieu le programme s'arrete
    if (automate==NULL) return 1;
    //On affiche l'automate créé
    print_automaton(automate);
    //On détermine si l'automate doit etre déterminisé
    int shDeter = shouldDeterminizate(automate);
    //Si il doit etre determinisé
    if(shDeter==1){  
        //On le déterminise
        algorithmDeterminization(automate);
        //Si -ovewrite a était passée en parametre alors on réécris le fichier
        if(shouldOveride==1){
            int success = overideFile(argv[1], automate);         
        }
    }
    //On réalise alors le parcours de l'automate avec le mot passé en parametre
    parcours(automate, argv[2]);
    //On libere l'automate et les mallocs effectués
    freeAutomaton(automate);
    return 0;
}