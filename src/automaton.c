#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#define MAXALPAHE 32

//FUNCTION: permet l'affichage d'un tableau simple
void printArray(int ar[], int l){
     int idx =0;
     while(idx<l){
          printf("idx[%d]: %d \n", idx, ar[idx]);
          idx++;
     }
}
//FUNCTION: renvoie la puissance de 2 d'une position 
int pow2(int position){
     int idx = 0;
     int res = 1;
     while(idx<position){
          res *= 2;
          idx++;
     }
     return res;
}
//FUNCTION: retourne la position d'une lettre dans un simple tableau de charactère ou -1 si la lettre ne fait pas partie du mot
int getPosition(char letter, char array_letter[], int arrayLength){
     int size = arrayLength;
     int idx = 0;
     while(idx<size && array_letter[idx]!=letter)idx++;
     if(idx==size) return -1;
     return idx;
}

//FUNCTION: permet de copier un double tableau de binary vector vers un autre tableau de binary vector
void copyArray(int nb_lig, int nb_col, binary_vector **m1, binary_vector **m2){
     int idx = 0;
     int idx2 = 0;
     while(idx<nb_lig){
          idx2=0;
          while(idx2<nb_col){
               binary_vector prov = m2[idx][idx2];
               m1[idx][idx2]=prov;
               idx2++;
          }
          idx++;
     }
}
//FUNCTION: permet de  copier un simple tableau de int vers un autre tableau de int
void copyArrayInt(int col, int ar1[], int ar2[]){
     int idx=0;
     while(idx<col){
          int prov = ar2[idx];
          ar1[idx]=prov;
          idx++;
     }
}
//FUNCTION: permet de copier un simple tableau de char vers un autre tableau de char 
void copyArrayChar(int col, char ar1[], char ar2[]){
     int idx=0;
     while(idx<col){
          char prov = ar2[idx];
          ar1[idx]=prov;
          idx++;
     }
}
//FUNCTION: renvoie la taille d'une ligne d'une matrixe afin de verifier son nombre de colonne par la suite
int verificationMatrix(char tab[], int size){
     int cpt ;
     int res =0 ;
     for(cpt=0; cpt<size && tab[cpt]!='\n' && tab[cpt]!='\0'; cpt++){
          if(tab[cpt]==';') res++;
     }
     return ++res;
}

//FUNCTION: CONSTRUCTEUR: permet de construire un automate de type AUTOMATON renvoyant un pointeur sur la structure
Automaton* constructor(char* path ){
     
     char tab[1000];
     FILE *myFile = fopen(path, "r");
     if(myFile==NULL){
          printf("Error while opening the file. \n");
          return NULL;
     }
     //initialisation des variables a affecter
     char *data_alphabet;
     int *prime_enter;
     int *data_final_state;
     binary_vector **matrix_set;
     int size=0, size2=0, size3=0, size4=0, ligneRead=0, idx=0, idx2=0, index = 0;
     while( fgets(tab, 1000, myFile)!=NULL){
          switch (ligneRead)
          {
          case 0:
               //On cherche la taille de notre alphabet 
               while(size<1000 && tab[size]!='\n' && tab[size]!='\0' &&tab[size]!=EOF) size++;
               if(size>MAXALPAHE){
                    printf("Votre alphabet semble trop grand : %d au lieu de %d maximum \n", size, MAXALPAHE);
                    return NULL;
               }
               if(size>=999 || size==0){
                    printf("Il semble que votre fichier ne contient pas de retour a la ligne dans la limite de 1000 charactere ou la ligne est vide a la ligne 1 \n ");
                    return NULL;
               }
               data_alphabet = (char *) malloc(sizeof(char)*size);
               memset(data_alphabet, 0, sizeof(char)*size);
               index = 0;
               while(index<size){
                    data_alphabet[index]=tab[index];
                    index++;
               }
               break;
          case 1:
               while(size4<1000 && tab[size4]!='\n' && tab[size4]!='\0' &&tab[size4]!=EOF) size4++;
               if(size4>=999 || size4==0){
                    printf("Il semble que votre fichier ne contient pas de retour a la ligne dans la limite de 1000 charactere ou la ligne est vide a la ligne 2 \n ");
                    return NULL;
               }
               prime_enter = (int *) malloc(sizeof(int)*size4);
               memset(prime_enter, 0, sizeof(int)*size4);
               index = 0;
               while(index<size4){
                    prime_enter[index]=tab[index]-48;
                    index++;
               }
               break;
          case 2: 
               size2 = 0;
               while(size2<1000 && tab[size2]!='\n' && tab[size2]!='\0' &&tab[size2]!=EOF) size2++;
               if(size2>=999 || size2==0){
                    printf("Il semble que votre fichier ne contient pas de retour a la ligne dans la limite de 1000 charactere ou la ligne est vide a la ligne 3 \n ");
                    return NULL;
               }
               data_final_state = (int*) malloc(sizeof(int)*size2);
               memset(data_final_state, 0, sizeof(int)*size2);
               index = 0;
               while(index<size2){
                    data_final_state[index]=tab[index]-48;
                    index++;
               }
               break;
          default:
               idx=idx2=0;
               //on determine la taille de notre matrix
               if(ligneRead==3){
                    while(idx<1000 && (tab[idx]!='\n' && tab[idx]!='\0' &&tab[idx]!=EOF )){
                         if(tab[idx]==';') size3++;
                         idx++;
                    }
                    if(size3>=1000 || size3==0){
                         printf("Il semble que votre fichier ne contient pas de retour a la ligne dans la limite de 1000 charactere ou la ligne est vide a la ligne %d \n ", ligneRead+1);
                         return NULL;
                    }
                    size3++;
                    //Il faut maintenant initialliser notre double array a la size voulu 
                    matrix_set= (binary_vector **) malloc(size3*sizeof(binary_vector *));
                    int cpt = 0;
                    for(cpt=0;cpt<size3; cpt++) matrix_set[cpt] = (binary_vector *) malloc(size3*sizeof(binary_vector));
               }else{
                    if(size3!=verificationMatrix(tab, 1000)){
                         printf("Une erreur est survenue dans la Matrix : Il semble que tout les lignes ne soit pas egales \n");
                         return NULL;
                    }
               }
               idx=idx2=0;
                         
               
               while(tab[idx]!='\n' && tab[idx]!='\0'){ 
                    int res = 0;
                    while(tab[idx]!=';' && tab[idx]!='\n' && tab[idx]!='\0'){
                         //on récupère la position de la lettre actuelle dans l'alphabet
                         int position = getPosition(tab[idx], data_alphabet, size);
                         //si la position est -1 alors la lettre n'est pas contenu dans l'alphabet
                         if(position==-1){
                              printf("Votre matrice contient des lettres qui ne sont pas dans l'alphabet \n");
                              return NULL;
                         }
                         //On ajoute a notre codage la valeur en puissance de deux de la position de la lettre
                         res += pow2(position);
                         ++idx;
                    }
                    if(tab[idx]!='\n') idx++;
                    matrix_set[ligneRead-3][idx2]=res;
                    idx2++;
                    //Si on ligne le retour a la ligne mais que a la lecture précèdente on avait un ; alors la dernier colonne est vide donc son code est zéro
                    if((tab[idx]=='\n' && tab[idx-1]==';')){
                         matrix_set[ligneRead-3][idx2]=0;
                    }
               }
               break;
          }
          //On reinitialise les valeurs du tableau dans le cas ou la ligne précedente est plus grandes que celle suivante
          for(idx=0; idx<1000;idx++)tab[idx]='\0';
          ligneRead++;
     }

     //On verifie que l'on a bien lut le nombre minimum de ligne pour former un automate
     if(ligneRead<4){
          printf("Erreur: Vous n'avez pas fourni un fichier avec suffisament de ligne \nRegardez le README.md pour voir le fichier a passer en parametres et sa syntaxe ou -help pour voir l'utilisation de la commande\n");
          return NULL; 
     }
     //On verifie que lon a lut autant de ligne dans la matrice que la taille de celle ci
     if((ligneRead-3)!=size3){
          printf("Erreur: Vous n'avez pas fourni une matrice carre \nRegardez le README.md pour voir le fichier a passer en parametres et sa syntaxe ou -help pour voir l'utilisation de la commande\n");
          return NULL;   
     }
     //Creation d'un automate 
     Automaton * automaton = (Automaton*) malloc(sizeof(Automaton));
     //Alphabet de l'automate
     automaton->size_alphabet=size;
     automaton->alphabet_array= (char *) malloc(sizeof(char)*size);
     copyArrayChar(size, automaton->alphabet_array, data_alphabet);
     //Etat inital de l'automate
     automaton->initial_state = (int *) malloc(sizeof(int)*size4);
     automaton->size_of_initial_state = size4;
     copyArrayInt(size4, automaton->initial_state, prime_enter);
     //Etat final ou sortant de l'automate
     automaton->final_state_array= (int*) malloc(sizeof(int)*size2);
     automaton->size_final_state=size2;
     copyArrayInt(size2, automaton->final_state_array, data_final_state);

     //ici on initialise la matrix par un pointeur de pointeur 
     automaton->matrix_size=size3;
     automaton->matrix = (binary_vector **) malloc(automaton->matrix_size*sizeof(binary_vector *));
     int cpt = 0;
     for(cpt=0;cpt<automaton->matrix_size; cpt++) automaton->matrix[cpt] = (binary_vector *) malloc(automaton->matrix_size*sizeof(binary_vector));
     copyArray( size3, size3, automaton->matrix, matrix_set);
     //Liberation des mallocs de cette function
     free(data_alphabet);
     free(data_final_state);
     free(prime_enter);
     int cpt2=0;
     for(cpt2=0;cpt2<size3; cpt2++) {free(matrix_set[cpt2]);}
     free(matrix_set);
     //fermeture du fichier
     fclose(myFile);
     return automaton;
}


//FONCTION: libere les espaces memoires initialiser par malloc
void freeAutomaton(Automaton* automaton){
     free(automaton->alphabet_array);
     free(automaton->final_state_array);
     free(automaton->initial_state);
     int cpt=0;
     for(cpt=0;cpt<automaton->matrix_size; cpt++) {free(automaton->matrix[cpt]);}
     free(automaton->matrix);
     free(automaton);
}

//FONCTION: réalise l'affichage d'un automate
void print_automaton(Automaton* automa){
     
     int index = 0;
     printf("The alphabet: {");
     while(index<automa->size_alphabet){
          printf("%c", automa->alphabet_array[index]);
          if(index<automa->size_alphabet-1) printf(",");
          index++;
     }
     index = 0;
     printf("}\nInitial state : ");

     while(index<automa->size_of_initial_state){
          printf("%d", automa->initial_state[index]);
          index++;
     }
     printf("\nThe acceptation state : {");
     index = 0;
     while(index<automa->size_final_state){
          printf("%d", automa->final_state_array[index]);
          if(index<automa->size_final_state-1)printf(",");
          index++;
     }
     int row = 0;
     int column = 0;
     printf("}\nMatrix : \n");
     while(row<automa->matrix_size){
          column = 0;
          while(column<automa->matrix_size){
               if(automa->matrix[row][column]!=0) automa->matrix[row][column]<15? printf("0x0%x", automa->matrix[row][column]) : printf("0x%x", automa->matrix[row][column]);
               
               if(column<automa->matrix_size-1) printf(";");
               column++;
          }
          printf("\n");
          row++;
     }
}

