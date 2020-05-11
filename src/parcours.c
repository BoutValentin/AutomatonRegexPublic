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
     //On recupere la longeur du mot passer en paramètre
     int length = getLength(mot);
     //On définit nos variable avec l'indice de lecture et l'etat dans lequel on se trouve
     int idx_lecture = 0;
     int actual_state = automaton->initial_state[0];
     //Tant que l'on a pas parcouru tout notre mot 
     while( idx_lecture<length){
          //On recupere la position de notre lettre que l'on est en train de lire par rapport a l'alphabet de l'automate
          int position_actual_letter = getPosition(mot[idx_lecture], automaton->alphabet_array, automaton->size_alphabet);
          //Si la position recuperere est -1 c'est que cette lettre n'est pas dans l'alphabet de notre automate
          if(position_actual_letter==-1){
               //Si ce mot correspond au mot vide "" alors on sors de la boucle en aojutant deux a ce quon aurait du lire
               if(idx_lecture==0 && mot[idx_lecture]=='"'&& length==2 && mot[idx_lecture+1]=='"'){
                    idx_lecture+=2;break;
               }
               //Sinon le mot n'est pas reconnu car la lettre n'est pas dans l'alphabet
               printf("Mot non reconnu \n");
               return -1;
          }
          //On creer un filtre par rapport a sa position dans notre alphabet
          int filter = pow2(position_actual_letter);
          //On parcours notre matrice de transition
          int index_parcours_matrix = 0;
          while( index_parcours_matrix< automaton->matrix_size){
               //Si la valeur stocker dans la matrice & filter donne le filter alors on peut aller vers cette etat  
               if((automaton->matrix[actual_state][index_parcours_matrix]&filter)==filter){
                    //On change notre etat actuelle et on quitte la boucle
                    actual_state=index_parcours_matrix;
                    break;
               }
               //Sinon on continue de parcourir la matrice a la recherche d'un etat transition
               index_parcours_matrix++;
          }
          //Si apres notre boucle notre indice de parcours est egale a la taille de la matrice de l'automate alors aucune transition n'a etait trouvé donc le mot n'est pas lisible
          if(index_parcours_matrix==automaton->matrix_size){
               printf("Ce mot ne peut pas etre lu par l'automate \n");
               return -1;
          }
          //On incremente notre indice de lecture du mot
          idx_lecture++;
     }
     //Si on a parcouru tout le mot et que notre state actuelle est contenu dans notre final Array de etats finaux
     if(idx_lecture==length && (contain(actual_state, automaton->final_state_array, automaton->size_final_state)==0)){
          printf("Mot reconnu par l'automate \n");
          return 0;
     }else{
          //Sinon le mot n'est pas reconnu car nous somme pas sur un etat final
          printf("Ce mot comporte les lettre mais il ne peut pas etre lut car pas sur un etat de sortie \n");
     }
     return 0;
}