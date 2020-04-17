#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

#define SIZEARRAY(x)  (sizeof(x) / sizeof((x)[0]))

void printArray(int ar[], int l){
     int idx =0;
     while(idx<l){
          printf("idx: %d, value: %d \n", idx, ar[idx]);
          idx++;
     }
}


int getPosition(char letter, char array_letter[], int arrayLength){
     int size = arrayLength;
     int idx = 0;
     while(idx<size && array_letter[idx]!=letter)idx++;
   
     if(idx==size) return -1;
     return idx;
}
void copyArray(int nb_lig, int nb_col, binary_vector **m1, binary_vector m2[nb_lig][nb_col] ){
     int idx = 0;
     int idx2 = 0;
     while(idx<nb_lig){
          idx2=0;
          while(idx2<nb_col){
               m1[idx][idx2]=m2[idx][idx2];
               idx2++;
          }
          idx++;
     }
}
void copyArrayInt(int col, int ar1[], int ar2[]){
     int idx=0;
     while(idx<col){
          ar1[idx]=ar2[idx];
          idx++;
     }
}

void copyArrayChar(int col, char ar1[], char ar2[]){
     int idx=0;
     while(idx<col){
          ar1[idx]=ar2[idx];
          idx++;
     }
}
//Création du constructeur



Automaton* constructor(char* path ){
     char tab[50];
     FILE *myFile = fopen(path, "r");
     if(myFile==NULL){
          printf("Error while opening the file. \n");
          return NULL;
     }
     //initialisation des variables a affecter
     char *data_alphabet;
     int prime_enter;
     int *data_final_state;
     binary_vector matrix_set[5][5];

     int size;
     int size2;
     int ligneRead = 0;
     int idx=0; 
               int idx2=0;
     while( fgets(tab, 50, myFile)!=NULL){
          switch (ligneRead)
          {
          case 0:
               size = 0;
               while(tab[size]!='\n') size++;
               data_alphabet = (char *) malloc(sizeof(char)*size);
               memset(data_alphabet, 0, sizeof(char)*size);
               int index = 0;
               while(index<size){
                    data_alphabet[index]=tab[index];
                    index++;
               }
               break;
          case 1:
               prime_enter=tab[0]-48;
               break;
          case 2: 
               size2 = 0;
               while(tab[size2]!='\n') size2++;
               data_final_state = (int*) malloc(sizeof(int)*size2);
               memset(data_final_state, 0, sizeof(int)*size2);
               index = 0;
               while(index<size2){
                    data_final_state[index]=tab[index]-48;
                    index++;
               }
               break;
          case 3: 
          case 4:
          case 5:
          case 6:
          case 7:
          idx=idx2=0;
               while(tab[idx]!='\n'){
                    int res = 0;
                    while(tab[idx]!=';' && tab[idx]!='\n' && tab[idx]!=EOF){
                         int position = getPosition(tab[idx], data_alphabet, size);
                         if(position==-1){
                              printf("Votre mot n'est pas lisible par l'automate \n");
                              return NULL;
                         }
                         res += position+1;
                         ++idx;
                    }
                    if(tab[idx]!='\n') idx++;
                    matrix_set[ligneRead-3][idx2]=res;
                    idx2++;
                    if(tab[idx]=='\n' && tab[idx-1]==';'){
                         matrix_set[ligneRead-3][idx2]=0;
                    }
               }
               break;
          default:
               break;
          }
          ligneRead++;
     }

     //Creation d'un automate 
     Automaton * automaton = (Automaton*) malloc(sizeof(char)*size+sizeof(int)+sizeof(int)*size2+sizeof(binary_vector)*25);
     //automaton->alphabet_array=data_alphabet;
     
     
     automaton->size_alphabet=size;
     automaton->alphabet_array= (char *) malloc(sizeof(char)*size);
     copyArrayChar(size, automaton->alphabet_array, data_alphabet);
     automaton->initial_state=prime_enter;
     //automaton->final_state_array=data_final_state;
     automaton->final_state_array= (int*) malloc(sizeof(int)*size2);
     copyArrayInt(size2, automaton->final_state_array, data_final_state);
    
     automaton->size_final_state=size2;

     automaton->matrix_size=5;
     automaton->matrix = (binary_vector **) malloc(automaton->matrix_size*sizeof(binary_vector *));
     int cpt = 0;
     for(cpt=0;cpt<automaton->matrix_size; cpt++) automaton->matrix[cpt] = (binary_vector *) malloc(automaton->matrix_size*sizeof(binary_vector));
     copyArray( 5, 5, automaton->matrix, matrix_set);

     //Liberation des mallocs de cette function
     free(data_alphabet);
     free(data_final_state);
     
     fclose(myFile);
     
     return automaton;
     

}



void freeAutomaton(Automaton* automaton){
     free(automaton->alphabet_array);
     free(automaton->final_state_array);
     int cpt=0;
     for(cpt=0;cpt<automaton->matrix_size; cpt++) {free(automaton->matrix[cpt]);}
     free(automaton->matrix);
}
//Constructeur avec tt les params finis plus que a allouer
//veritable constructeur avec aucun params ou nom du fichier et creer toutes les donnes utiles puis les passent a l'autre constructeur 
//function qui lit le numero de ligne passer en params 

void print_automaton(Automaton* automa){
     
     int index = 0;
     printf("The alphabet: {");
     while(index<automa->size_alphabet){
          printf("%c", automa->alphabet_array[index]);
          if(index<automa->size_alphabet-1) printf(",");
          index++;
     }
     index = 0;
     printf("}\nInitial state : %d \n", automa->initial_state);
     printf("The acceptation state : {");
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
               
               if(column<4) printf(";");
               column++;
          }
          printf("\n");
          row++;
     }
}

