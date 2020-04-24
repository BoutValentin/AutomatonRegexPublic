#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "automaton.h"

char * createArray(Automaton * automate){
     char * tableau =(char *) malloc((automate->size_alphabet+1+automate->size_final_state+1+automate->size_of_initial_state+1)*sizeof(char));
     int size = (automate->size_alphabet+1+automate->size_final_state+1+automate->size_of_initial_state+1);
     int compteur =0;
     int cpt,cpt2,cpt3;
     for(cpt=0;cpt<automate->size_alphabet; cpt++){
          tableau[compteur] = automate->alphabet_array[cpt];
          compteur++;
     }
     tableau[compteur++]= '\n';
     for(cpt=0; cpt<automate->size_of_initial_state; cpt++){
          tableau[compteur] = automate->initial_state[cpt] +'0';
          compteur++;
     }
     tableau[compteur++] = '\n';
     for(cpt=0; cpt<automate->size_of_initial_state; cpt++){
          tableau[compteur] = automate->final_state_array[cpt]+'0';
          compteur++;
     }
     tableau[compteur++] = '\n';
     for(cpt=0; cpt<automate->matrix_size; cpt++){
          for(cpt2=0;cpt2<automate->matrix_size; cpt2++){
               if(automate->matrix[cpt][cpt2]!=0){
                    for(cpt3=0;cpt3<automate->size_alphabet;cpt3++){
                         int filter = pow2(cpt3);
                         if(automate->matrix[cpt][cpt2]&filter==filter){
                              tableau = realloc(tableau, (size+1)*sizeof(char));
                              size++;
                              tableau[compteur++] = automate->alphabet_array[cpt3];
                         }
                    }
               }
               if(cpt2!=(automate->matrix_size-1)){
                    tableau = realloc(tableau, (size+1)*sizeof(char));
                    size++;
                    tableau[compteur++] = ';';
               }
               
          }
          if(cpt!=(automate->matrix_size-1)){
               tableau = realloc(tableau, (size+1)*sizeof(char));
               size++;
               tableau[compteur++] = '\n';
          }
     }
     return tableau;
}

int overideFile(char* path, Automaton * automate){
     char * tableau = createArray(automate);
     
     FILE *fp = fopen(path, "w+");
     if(fp==NULL){
          printf("Erreur dans l'ouverture de fichier ! l'overwrite ne se fera pas \n");
          return 1 ;
     }
     fputs(tableau, fp);
     fclose(fp);
     printf("L'ovewrite a etait effectuer \n");
     free(tableau);
     return 0;
}