#include <stdio.h>
#include "type.h"
#include "automaton.h"

//FONCTION: permet d'obtenir la taille d'une chaine de caractères
int getLength(char *mot){
     int idx =0;
     while(mot[idx]!='\0') idx++;
     return idx;
}

//FONCTION: determine si un tableau contient une certaine valeur
int contain(int value, int * array, int length){
     int idx = 0;
     while(idx<length){
          if(array[idx]==value)return 0;
          idx++;
     }
     return -1;
}

//FONCTION: réalise le parcours de la matrice et du mot afin de déterminier si il est lisible
int parcours(Automaton * automaton, char *mot){
     printf("\n Vous avez choisit le mot suivant : %s \nDebut du traitement \n\n\n", mot);
     printf(" Resultat : \n\n");
     int length = getLength(mot);
     int idx_lecture = 0;
     int actual_state = automaton->initial_state[0];
     while( idx_lecture<length){
          int position_actual_letter = getPosition(mot[idx_lecture], automaton->alphabet_array, automaton->size_alphabet);
          if(position_actual_letter==-1){
               if(idx_lecture==0 && mot[idx_lecture]=='"'&& length==2 && mot[idx_lecture+1]=='"'){
                    idx_lecture+=2;break;
               }
               printf("Mot non reconnu \n");
               return -1;
          }
          int filter = pow2(position_actual_letter);
          int index_parcours_matrix = 0;
          while( index_parcours_matrix< automaton->matrix_size){
               if((automaton->matrix[actual_state][index_parcours_matrix]&filter)==filter){
                    actual_state=index_parcours_matrix;
                    break;
               }
               index_parcours_matrix++;
          }
          if(index_parcours_matrix==automaton->matrix_size){
               printf("Ce mot ne peut pas etre lu par l'automate \n");
               return -1;
          }
          idx_lecture++;
     }
     if(idx_lecture==length && (contain(actual_state, automaton->final_state_array, automaton->size_final_state)==0)){
          printf("Mot reconnu par l'automate \n");
          return 0;
     }else{
          printf("Ce mot comporte les lettre mais il ne peut pas etre lut car pas sur un etat de sortie \n");
     }
     return 0;
}