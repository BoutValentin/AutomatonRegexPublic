#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "automaton.h"

//int alreadyHaveValue(string_state array, int value, int sizeArray){

int alreadyHaveValue(string_state array, int value, int sizeArray){
     if(array==NULL) return 0;
     int i=0;
     for(i=0; i<sizeArray; i++){
          if(array[i]==value) return 1;
     }
     return 0;
}


//Run before the rename
int * changeFinalState(Automaton * automate, State ** FinaleArray, int numberOfState, int * pointSize){
     int * FinalStateArray=(int*) malloc(sizeof(int));
     //Pour chaque ligne de la premiere colonne
     //Cherche si cette case contain value de 
     int finaleStateArrayContain = 0;
     int finaleStateArraySize =1;
     int cpt, cpt2;
     for(cpt=0; cpt<numberOfState; cpt++){
          for(cpt2=0; cpt2<automate->size_final_state; cpt2++){
               if(alreadyHaveValue(FinaleArray[cpt][0]->array_state, automate->final_state_array[cpt2], FinaleArray[cpt][0]->size_array_state)==1 && alreadyHaveValue(FinalStateArray, automate->final_state_array[cpt2], finaleStateArraySize)==0 ){
                    if(finaleStateArrayContain<finaleStateArraySize){
                         FinalStateArray[finaleStateArrayContain++]=cpt;
                    }else{
                         ++finaleStateArraySize;
                         FinalStateArray = (int *) realloc(FinalStateArray, finaleStateArraySize*sizeof(int));
                         FinalStateArray[finaleStateArrayContain++]=cpt;
                    }
               }
          }
     }
     *pointSize = finaleStateArraySize;
     return FinalStateArray;
}



binary_vector ** changeMatrix(Automaton* automate, State ** FinaleArray, int numberOfState){

     //Fist free the previous matrix 
     int cpt, cpt2;
     for(cpt=0;cpt<automate->matrix_size; cpt++) {free(automate->matrix[cpt]);}
     free(automate->matrix);
    //Create a new one
     binary_vector **FinalMatrix = (binary_vector **) malloc(sizeof(binary_vector *)*numberOfState);          
     for(cpt=0;cpt<numberOfState; cpt++) FinalMatrix[cpt] = (binary_vector *) malloc(numberOfState*sizeof(binary_vector));

     for(cpt=0; cpt<numberOfState; cpt++){
          for(cpt2=0; cpt2<numberOfState; cpt2++){
               FinalMatrix[cpt][cpt2] = 0;
          }
     }

     for(cpt=0; cpt<numberOfState; cpt++){
          for(cpt2=0; cpt2<automate->size_alphabet; cpt2++){
               FinalMatrix[cpt][FinaleArray[cpt][cpt2+1]->array_state[0]] += pow2(cpt2);
          }
     }    
     return FinalMatrix;

}


int arrayContainSameValue(string_state array1, string_state array2, int sizeArray1, int sizeArray2){

     if(sizeArray1!=sizeArray2) return 0;
     int i, j;
     for(i=0; i<sizeArray1; i++){
          for(j=0; j<sizeArray2; j++){
               if(array1[i]==array2[j])break;
          }
          if(j==sizeArray2)return 0;
     }
     return 1;
}

void printArray2(string_state ar, int nu){
     int j = 0;
     for(j=0;j<nu;j++){
          printf("array[%d]=%d \n", j, ar[j]);
     }
}

int checkColumnEquals(binary_vector **matrix, int size){
     int i, j,d;
     for(d=0; d<size; d++){
          for(i=0; i<size; i++){
               for(j=(i+1); j<size; j++){
                    if(matrix[d][i]==matrix[d][j]&&matrix[d][i]!=0) return 1;
               }
          }
     }
     return 0;
}


int shouldDeterminizate(Automaton * automate){
     if(automate->size_of_initial_state>1){
          printf("L'automate doit etre determiniser car plsr entre \n");
          return 1;
     }
     if(checkColumnEquals(automate->matrix, automate->matrix_size)==1){
          printf("L'automate doit etre determiniser plusieurs collone sont egales \n");
          return 1;
     }
     return 0;

}
void printArrayState(State **initialArray, int sizeRow, int sizeCol){
     int i, j, k;
     for(i = 0; i<sizeRow; i++){
          for(j=0; j<sizeCol; j++){
               if(initialArray[i][j]==NULL){
                    printf("NULL");
               }else{
                  for(k=0; k<initialArray[i][j]->size_array_state; k++){
                    printf("%d", initialArray[i][j]->array_state[k]);
                    }  
               }
               printf(";");
          }
          printf("\n");
     }
}
int alreadySave(AlreadyChanged * array, int size, int col, int row){
     int cpt;
     for(cpt=0; cpt<size; cpt++){
                         printf("Save[%d]: row:%d, col:%d \n", cpt, array[cpt]->row, array[cpt]->col);

          if(array[cpt]->col==col && array[cpt]->row==row){
               return 1;
               printf("Save[%d]: row:%d, col:%d \n", cpt, array[cpt]->row, array[cpt]->col);
          }
     }
     return 0;
}
void printInitialArray(State **initialArray, int sizeRow, int sizeCol){
     int i,j,k;
     for(i=0; i<sizeRow;i++){
          for(j=0; j<sizeCol;j++){
               for(k=0;k<initialArray[i][j]->size_array_state;k++)printf("%d",initialArray[i][j]->array_state[k]);
               printf(";");
          }
          printf("\n");
     }
}

void renameState(State **initialArray, int sizeRow, int sizeCol){
     int i, j, k;
     for(i = 0; i<sizeRow; i++){
          State actualState = (State) malloc(sizeof(StrucState));
          actualState->size_array_state = initialArray[i][0]->size_array_state;
          actualState->array_state = (int *) malloc(sizeof(int)*actualState->size_array_state);
          int cpt;
          for(cpt =0 ; cpt<actualState->size_array_state; cpt++){
               int prov = initialArray[i][0]->array_state[cpt];
               actualState->array_state[cpt]=prov;
          }
          free(initialArray[i][0]->array_state);
          free(initialArray[i][0]);
          State aState =(State) malloc(sizeof(int)+sizeof(int *));
          string_state array = (int *) malloc(sizeof(int));
          array[0]=i;
          int size = 1;
          aState->array_state = array;
          aState->size_array_state=size;
          initialArray[i][0]=aState;
          for(j = 0; j<sizeRow; j++){
               for(k=0; k<sizeCol; k++){
                    if(arrayContainSameValue(actualState->array_state, initialArray[j][k]->array_state, actualState->size_array_state, initialArray[j][k]->size_array_state)==1){
                         if((initialArray[j][k]->size_array_state==1 && initialArray[j][k]->array_state[0]>=i)||initialArray[j][k]->size_array_state>1){
                         free(initialArray[j][k]->array_state); 
                         free(initialArray[j][k]);
                         State aState =(State) malloc(sizeof(StrucState));
                         string_state array = (int *) malloc(sizeof(int));
                         array[0]=i;
                         int size = 1;
                         aState->array_state = array;
                         aState->size_array_state=size;
                         initialArray[j][k] = aState;  
                    }}
               }

          }
          
     }
}

State ** createInitialArray(Automaton * automate){

     State **InitialArray = (State **) malloc(automate->matrix_size*sizeof(State *));
     int i,j,k ;
     for(i = 0; i<automate->matrix_size; i++){
          InitialArray[i]=(State *) malloc((automate->size_alphabet+1)*sizeof(State));
     }
     for(i =0 ; i<automate->matrix_size; i++){
          for(j=0; j<(automate->size_alphabet+1); j++){
               InitialArray[i][j]=NULL;
          }
     }
     //Remplissons le tableau Initial

     //Premiere colonne
     for(i=0; i<automate->matrix_size; i++){
          string_state array= (int *) malloc(sizeof(int));
          array[0] = i;
          int size = 1;
          State aState = (State ) malloc(sizeof(StrucState)); //CHANE HERE
          aState->array_state=array;
          aState->size_array_state=size;
          InitialArray[i][0]= aState;
     }

     //Reste du tableau
     for(i = 0; i<automate->matrix_size; i++){
          for(j=0; j<automate->matrix_size;j++){
               binary_vector code = automate->matrix[i][j];
               if(code!=0){
                    //Pour chaque lettre de l'alphabet
                    //Creer le filtre
                    //si code&filtre=filtre
                    //alors on realloc de une taille le string state
                    for(k=0; k<automate->size_alphabet; k++){
                         int filter = pow2(k);
                         if((code&filter)==filter){
                              if(InitialArray[i][k+1]==NULL){
                                   //Create One
                                   string_state array2= (int *) malloc(sizeof(int));
                                   array2[0] = j;
                                   int size = 1;
                                   State  aState = (State ) malloc(sizeof(int)+size*(sizeof(int *))); //change her
                                   aState->array_state=array2;
                                   aState->size_array_state=size;
                                   InitialArray[i][k+1]= aState;
                              }else{
                                   //TODO
                                   //verifcation alloc going good
                                   InitialArray[i][k+1] = realloc(InitialArray[i][k+1], sizeof(int)+((InitialArray[i][k+1]->size_array_state)+1)*(sizeof(int)));
                                   InitialArray[i][k+1]->array_state = realloc(InitialArray[i][k+1]->array_state, ((InitialArray[i][k+1]->size_array_state)+1)*(sizeof(int)));
                                   InitialArray[i][k+1]->size_array_state++;
                                   InitialArray[i][k+1]->array_state[InitialArray[i][k+1]->size_array_state-1]=j;
                              }
                         }
                    }
                    //si code&filtre=filtre
                    //alors en position idx de column +1 de parcours de matrice
                    //si a cette emplacement il n'y a rien alors on malloc
                    //sinon onrealloc de une taille suplementaire
                    
               }
          }
     }

     return InitialArray;
}

string_state copyArrayInteger(int *intialState, int sizeArray){
     string_state aState = (int *) malloc(sizeof(int)*sizeArray);
     int cpt;
     for(cpt=0; cpt<sizeArray; cpt++) aState[cpt]=intialState[cpt];
     return aState;
}

State ** createFinalArray(Automaton * automate){

     State **FinalArray = (State **) malloc(1*sizeof(State *));
     int i, j;
     for(i=0; i<1; i++){
          FinalArray[i]=(State *) malloc((automate->size_alphabet+1)*sizeof(State));
     }

     for(i =0 ; i<1; i++){
          for(j=0; j<(automate->size_alphabet+1); j++){
              FinalArray[i][j]=NULL;
          }
     }
     //Remplissons le tableau Initial

     //Premiere colonne
     for(i=0; i<1; i++){
          string_state array= copyArrayInteger(automate->initial_state,automate->size_of_initial_state);
          State aState = (State ) malloc(sizeof(int)+automate->size_of_initial_state*(sizeof(int *))); //change here
          aState->array_state=array;
          aState->size_array_state=automate->size_of_initial_state;
          FinalArray[i][0]= aState;
     }
     return FinalArray;
}

void algorithmDeterminization(Automaton * automate){
     //Cette variable ne doit pas evoluer il s'agit de la taille initiale du tableau
     int sizeOfInitialArray;
     State **InitialArray = createInitialArray(automate);

     int sizeOfFinalArray =1;
     State **FinalArray = createFinalArray(automate);
     int cpt = 0;
     int k,l,m,n;
     while(cpt<sizeOfFinalArray){
          for(k=1; k<automate->size_alphabet+1; k++){
               if(FinalArray[cpt][k]==NULL){
                    //on doit creer un state+1s
                    string_state array = (int *) malloc(sizeof(int));
                    int sizeArray = 1;
                    int numberInArray=0;
                    n=0;
                    for(l=0; l<FinalArray[cpt][0]->size_array_state; l++){
                         if(FinalArray[cpt][0]->array_state[l]==-1){
                              array[0]=-1;
                              break;
                         }
                         if(InitialArray[FinalArray[cpt][0]->array_state[l]][k]!=NULL){
                         for(m=0;m<InitialArray[FinalArray[cpt][0]->array_state[l]][k]->size_array_state; m++){
                         if(array==NULL || alreadyHaveValue(array, InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m], sizeArray)==0){
                              n++;
                              if(numberInArray<sizeArray){
                                   array[numberInArray++]=InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m];
                              }else{
                                   ++sizeArray;
                                   array = (int *) realloc(array, sizeArray*sizeof(int));
                                   array[numberInArray++]=InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m];
                              }
                         }
                         }
                         
                         }
                         if(n==0){
                              array[0]=-1;
                         }
                    }
                   
                    
                    State aState = (State ) malloc(sizeof(StrucState)); //change here
                    aState->array_state=array;
                    aState->size_array_state=sizeArray;
                    for(l=0; l<sizeOfFinalArray; l++){
                         if(arrayContainSameValue(FinalArray[l][0]->array_state,aState->array_state,FinalArray[l][0]->size_array_state , aState->size_array_state)==1)break;
                     }
                    if(l==sizeOfFinalArray){
                         ++sizeOfFinalArray;
                         FinalArray = realloc(FinalArray, (sizeOfFinalArray)*(sizeof(State *)));
                         FinalArray[sizeOfFinalArray-1]=(State *) malloc((automate->size_alphabet+1)*sizeof(State));
                         int i2;
                         FinalArray[l][0]= aState;
                         for(i2=1; i2<(automate->size_alphabet+1); i2++){
                              FinalArray[l][i2]=NULL;
                         }
                    }
                    FinalArray[cpt][k] = aState;
               }
          }
      ++cpt;

     }
     //Tant que size du prochain arret est infereur au compteur
     //Ou Tant que la derniere ligne contient un array pas encore vue

     //On commence par regrouper les etats d'entres : automaton state init = premiere colonne de la premiere ligne 
     //pour chaque colonne de l'alphabet on regarde sur l'initial array si differrent de nul
     //si different de null on realloc avec une taille de plus un 
     //si le state creer n'est pas un array contain
     //alors on ajoute ue ligne au tableau avec pour premiere colonne ce state avec realloc
     //on fait evoluer la taille de ce tableau de plus 1      
     int * finalStateSize = (int *) malloc(sizeof(int));
     int * changingFinalState = changeFinalState(automate, FinalArray, sizeOfFinalArray, finalStateSize);
     int uncpt ;
     renameState(FinalArray, sizeOfFinalArray, automate->size_alphabet+1);

     automate->size_of_initial_state=1;
     free(automate->initial_state);
     automate->initial_state = (int *) malloc(sizeof(int));
     automate->initial_state[0]=0;

     automate->size_final_state=*finalStateSize;
     free(automate->final_state_array);
     automate->final_state_array = (int *) malloc(sizeof(int)*automate->size_final_state);
     copyArrayInt(automate->size_final_state, automate->final_state_array, changingFinalState);

     for(uncpt=0; uncpt<automate->matrix_size; uncpt++){
          free(automate->matrix[uncpt]);
     }
     free(automate->matrix);
     automate->matrix_size=sizeOfFinalArray;
     automate->matrix = (binary_vector **) malloc(automate->matrix_size*sizeof(binary_vector *));
     for(uncpt=0; uncpt<automate->matrix_size; uncpt++){
          automate->matrix[uncpt] = (binary_vector *) malloc(sizeof(binary_vector)*automate->matrix_size);
     }
     copyArray(automate->matrix_size, automate->matrix_size, automate->matrix, changeMatrix(automate, FinalArray, sizeOfFinalArray));

     printf("\n\n          Votre automate a ete determiniser:              \n\n");
     print_automaton(automate);
}


