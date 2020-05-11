#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "automaton.h"

//FUNCTION: Creer le tableau de caractères qui sera ecrit dans le fichier
char * createArray(Automaton * automate){
     //ON commence par créer un tableau egale a la somme de toute les tailles de tableau plus autant de caractere que de tableau pour le retour a ligne ainsi que pour les retours a la ligne de matrice
     char * tableau =(char *) malloc((automate->size_alphabet+1+automate->size_final_state+1+automate->size_of_initial_state+1)*sizeof(char));
     //On sauvegarde cette taille
     int size = (automate->size_alphabet+1+automate->size_final_state+1+automate->size_of_initial_state+1);
     //initilisation de variable utile aux parcours
     int compteur =0;
     int cpt,cpt2,cpt3;
     //On copie d'abord l'alphabet de l'automate
     for(cpt=0;cpt<automate->size_alphabet; cpt++){
          tableau[compteur] = automate->alphabet_array[cpt];
          compteur++;
     }
     //On ajoute le retout a la ligne
     tableau[compteur++]= '\n';
     //On continue par les états initiaux
     for(cpt=0; cpt<automate->size_of_initial_state; cpt++){
          tableau[compteur] = automate->initial_state[cpt] +'0';
          compteur++;
     }
     //On ajoute le retour a la ligne
     tableau[compteur++] = '\n';
     //On continue d'ajouter aux tableau les états finaux
     for(cpt=0; cpt<automate->size_final_state; cpt++){
          tableau[compteur] = automate->final_state_array[cpt]+'0';
          compteur++;
     }
     //On ajoute le retour a la ligne
     tableau[compteur++] = '\n';
     //On fini par le décodage de la matrice avec le parcours en ligne
     for(cpt=0; cpt<automate->matrix_size; cpt++){
          //puis en colonne
          for(cpt2=0;cpt2<automate->matrix_size; cpt2++){
               //Seulement si dans cette case le resultat est diffèrent de zero
               if(automate->matrix[cpt][cpt2]!=0){
                    //On parcours l'alphabet pour savoir quel(s) lettre(s) est/sont a cette transition
                    for(cpt3=0;cpt3<automate->size_alphabet;cpt3++){
                         //On recupere le filtre par la position dans l'alphabet
                         int filter = pow2(cpt3);
                         //Si le & logique nous donne le filtre alors la lettre a cette position fais partie de la transition
                         if((automate->matrix[cpt][cpt2]&filter)==filter){
                              //On realloc notre tableau d'une taille supplémentaire pour sauvegarder cette lettre 
                              tableau = realloc(tableau, (size+1)*sizeof(char));
                              //On incremente donc la taille de notre tableau
                              size++;
                              //On incremente notre compteur apres avoir stocker la valeur de la matrice
                              tableau[compteur++] = automate->alphabet_array[cpt3];
                         }
                    }
               }
               //Si on est pas encore au dernier caractère de la matrice alors il faut ajouter le séparateur ;
               if(cpt2!=(automate->matrix_size-1)){
                    //On realloc notre tableau d'une taille plus un
                    tableau = realloc(tableau, (size+1)*sizeof(char));
                    //On incremente donc la taille de notre tableau
                    size++;
                    //On stocke notre separateur dans le tableau
                    tableau[compteur++] = ';';
               }
               
          }
          //Si on est pas encore au dernier caractère de notre matrice alors une ligne va suivre donc on doit passer une ligne
          if(cpt!=(automate->matrix_size-1)){
               //On realloc notre tableau d'une taille plus un
               tableau = realloc(tableau, (size+1)*sizeof(char));
               //On incremente donc la taille de notre tableau
               size++;
               //On stocke notre passage de ligne dans le tableau
               tableau[compteur++] = '\n';
          }
     }
     //On renvoie notre tableau
     return tableau;
}
//FONCTION: réalise la réécriture du fichier passer en parametre 
int overideFile(char* path, Automaton * automate){
     //Création du tableau de charactère
     char * tableau = createArray(automate);
     //Ouverture du fichier en mode ecriture
     FILE *fp = fopen(path, "w+");
     //On verifie que l'on a pu ouvrir notre fichier
     if(fp==NULL){
          printf("Erreur dans l'ouverture de fichier ! l'overwrite ne se fera pas \n");
          return 1 ;
     }
     //On écrit dans le fichier notre tableau
     fputs(tableau, fp);
     //On ferme notre fichier
     fclose(fp);
     printf("L'overwrite a etait effectuer \n");
     //On libere la memoire
     free(tableau);
     return 0;
}