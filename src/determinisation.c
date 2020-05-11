#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "automaton.h"

//FONCTION: test si un array de string_state contient deja la valeur passer en paramètre
int alreadyHaveValue(string_state array, int value, int sizeArray){
     if(array==NULL) return 0;
     int i=0;
     for(i=0; i<sizeArray; i++){
          if(array[i]==value) return 1;
     }
     return 0;
}

//FONCTION: definit les etats finaux de l'automate apres determinisation
int * changeFinalState(Automaton * automate, State ** FinaleArray, int numberOfState, int * pointSize){
     //On definit un nouveau tableau d'etat finaux pour l'instant de une case
     int * FinalStateArray=(int*) malloc(sizeof(int));
     //On remplis cette premiere case d'une valeur impossible
     FinalStateArray[0]=-2; 
     //On definit le nombre d'element stocker
     int finaleStateArrayContain = 0;
     //On definit la taille du tableau
     int finaleStateArraySize =1;
     int cpt, cpt2;
     //On parcours ensuite la premiere colonne
     for(cpt=0; cpt<numberOfState; cpt++){
          //On parcours tout les etats finaux avant déterminisation
          for(cpt2=0; cpt2<automate->size_final_state; cpt2++){
               //Si la case de la première colonne contient un chiffre qui était des les etats finaux d'avant determinisation et pas deja dans le tableau alors cela devient un etat finaux
               if(alreadyHaveValue(FinaleArray[cpt][0]->array_state, automate->final_state_array[cpt2], FinaleArray[cpt][0]->size_array_state)==1 && alreadyHaveValue(FinalStateArray, cpt, finaleStateArraySize)==0 ){
                    //Si le tableau est encore asser grand pour stocker une valeur on l'ajoute
                    if(finaleStateArrayContain<finaleStateArraySize){
                         FinalStateArray[finaleStateArrayContain++]=cpt;
                    }else{
                         //Sinon on doit augmenter notre taille de tableau de plus un
                         ++finaleStateArraySize;
                         FinalStateArray = (int *) realloc(FinalStateArray, finaleStateArraySize*sizeof(int));
                         FinalStateArray[finaleStateArrayContain++]=cpt;
                    }
               }
          }
     }
     //On met a jour la variable pointeur afin de sauvegarder la taille
     *pointSize = finaleStateArraySize;
     //On renvoie le tableau d'état finaux
     return FinalStateArray;
}

//FONCTION: creer la nouvelle matrice après automatisation
binary_vector ** changeMatrix(Automaton* automate, State ** FinaleArray, int numberOfState){
     int state_null = -8;
     int cpt, cpt2;
     //on creer la nouvelle matrice que l'on remplis a zeo
     binary_vector **FinalMatrix = (binary_vector **) malloc(sizeof(binary_vector *)*numberOfState);          
     for(cpt=0;cpt<numberOfState; cpt++) FinalMatrix[cpt] = (binary_vector *) malloc(numberOfState*sizeof(binary_vector));
     for(cpt=0; cpt<numberOfState; cpt++){
          for(cpt2=0; cpt2<numberOfState; cpt2++){
               FinalMatrix[cpt][cpt2] = 0;
          }
     }
     //On parcours notre tableau de transition
     for(cpt=0; cpt<numberOfState; cpt++){
          //On parcours notre alphabet
          for(cpt2=0; cpt2<automate->size_alphabet; cpt2++){
               //On se souvient que si cela contient moins 1 il s'agissait de notre etat Vide
               if(FinaleArray[cpt][cpt2+1]->array_state[0]==-1){
                    //On sauvegarde a quoi correspondant notre state vide en terme de position dans la matrice
                    if(state_null==-8) state_null=cpt;
                    //On sait que tout ce qui aura -1 prendra comme valeur a la colonne du state null cpt2 puissance deux en code
                    FinalMatrix[cpt][state_null] += pow2(cpt2);
               }else{
                    //On code dans notre matrice la valeur de transition 
               FinalMatrix[cpt][FinaleArray[cpt][cpt2+1]->array_state[0]] += pow2(cpt2);

               }
          }
     }    
     //On renvoie notre Matrice Finale
     return FinalMatrix;
}

//FONCTION: verifie si deux tableau ont les meme valeurs a l'interieur peut importe l'ordre
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

//FONCTION: réalise l'affichage d'un tableau de string_state
void printArray2(string_state ar, int nu){
     int j = 0;
     for(j=0;j<nu;j++){
          printf("array[%d]=%d \n", j, ar[j]);
     }
}

//FONCTION: on verifie que deux colonne d'une matrice sont égales
int checkColumnEquals(binary_vector **matrix, int size, Automaton * automate){
     int i, j,d,k;
     //On parcours entièrement par row
     for(d=0; d<size; d++){
          //On parcours une premiere fois par colonne
          for(i=0; i<size; i++){
               //On parcours une deuxieme fois par colonne a plus +1 de celle précedente pour ne pas comparer deux fois la meme colonne
               for(j=(i+1); j<size; j++){
                    //Si les deux matrices sont egales et diffèrent de zero 
                    if(matrix[d][i]==matrix[d][j] && (matrix[d][i]!=0)) return 1;
                         //Sinon on doit regarder si elle contienne la meme lettre dans leur transition
                         for(k=0; k<automate->size_alphabet; k++){
                              int filter = pow2(k);
                              //Si dans la case choisit de comparaison cette lettre est presente
                              if((matrix[d][i]&filter)==filter){
                                   //Alors on verifie si la case de comparaison aussi
                                   if((matrix[d][j]&filter)==filter) return 1;
                              }
                    }
               }
          }
     }
     return 0;
}

//FONCTION: détermine si l'automate doit etre déterminiser
int shouldDeterminizate(Automaton * automate){
     if(automate->size_of_initial_state>1){
          printf("L'automate doit être determinisé car il y a plusieurs etat d'entre \n");
          return 1;
     }
     if(checkColumnEquals(automate->matrix, automate->matrix_size, automate)==1){
          printf("L'automate doit être determinisé car plusieurs colonnes sont egales ou plusieurs chemins sont disponibles pour une même lettre \n");
          return 1;
     }
     return 0;

}
//FONCTION: réalise l'affichage d'un double tableau de State
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

//FONCTION: réalise le renomage des états afin d'avoir un automate plus lisible 
void renameState(State **initialArray, int sizeRow, int sizeCol){
     int i, j, k;
     int sizeMax = sizeRow+1;
     //Pre renommage d'etat on effectue le parcours une premiere fois pour eliminer touts les chiffres seules pour eviter la confusion lors du renommage
     for(i = 0; i<sizeRow; i++){
          if((initialArray[i][0]->size_array_state==1) && (initialArray[i][0]->array_state[0]!=-1)){
               //Pour chaque premiere colonne de l'array finale on sauvegarde l'etat de ce state et on le renomme avec une taille superieur a la taille afin d'éviter les problèmes de double renommmage
               State actualState = (State) malloc(sizeof(StrucState));
               int prov2 = initialArray[i][0]->size_array_state;
               actualState->size_array_state = prov2;
               actualState->array_state = (int *) malloc(sizeof(int)*actualState->size_array_state);
               int cpt;
               for(cpt =0 ; cpt<actualState->size_array_state; cpt++){
                    int prov = initialArray[i][0]->array_state[cpt];
                    actualState->array_state[cpt]=prov;
               }
               if(initialArray[i][0]!=NULL){
                    free(initialArray[i][0]->array_state);
                    free(initialArray[i][0]);
               }
               
               State aState =(State) malloc(sizeof(StrucState));
               string_state array = (int *) malloc(sizeof(int));
               array[0]=sizeMax;
               int size = 1;
               aState->array_state = array;
               aState->size_array_state=1;
               initialArray[i][0]=aState;
               //On parcours ensuite tout le tableau a la recherche des states egales a celui enregistrer et on les renomme si c'est la cas
               for(j = 0; j<sizeRow; j++){
                    for(k=0; k<sizeCol; k++){
                         if(arrayContainSameValue(actualState->array_state, initialArray[j][k]->array_state, actualState->size_array_state, initialArray[j][k]->size_array_state)==1 ){
                              if(initialArray[j][k]!=NULL){
                                   free(initialArray[j][k]->array_state); 
                                   free(initialArray[j][k]);
                              }
                              State aState2 =(State) malloc(sizeof(StrucState));
                              string_state array2 = (int *) malloc(sizeof(int));
                              array2[0]=sizeMax;
                              int size = 1;
                              aState2->array_state = array2;
                              aState2->size_array_state=1;
                              initialArray[j][k] = aState2;  
                         }
                    }

               }
               free(actualState->array_state);
               free(actualState);
               sizeMax++;
          }
     }

     //Apres renommage basique
     //On effectue les memes action que précedemment mais on renomme par la valeur de i afin d'avoir des etats a un chiffres ou nombres 
     for(i = 0; i<sizeRow; i++){
          //Sauvegarde du State actuelle 
          State actualState = (State) malloc(sizeof(StrucState));
          int prov2 = initialArray[i][0]->size_array_state;
          actualState->size_array_state = prov2;
          actualState->array_state = (int *) malloc(sizeof(int)*actualState->size_array_state);
          int cpt;
          for(cpt =0 ; cpt<actualState->size_array_state; cpt++){
               int prov = initialArray[i][0]->array_state[cpt];
               actualState->array_state[cpt]=prov;
          }
          //Liberation de l'ancien state a cette case du tableau
          if(initialArray[i][0]!=NULL){
                    free(initialArray[i][0]->array_state);
                    free(initialArray[i][0]);
          }
          //Création du state de remplacement
          State aState =(State) malloc(sizeof(StrucState)); 
          string_state array = (int *) malloc(sizeof(int));
          array[0]=i;
          int size = 1;
          aState->array_state = array;
          aState->size_array_state=1;
          initialArray[i][0]=aState;
          //Parcours du tableau a la recherche des states identiques
          for(j = 0; j<sizeRow; j++){
               for(k=0; k<sizeCol; k++){
                    if(arrayContainSameValue(actualState->array_state, initialArray[j][k]->array_state, actualState->size_array_state, initialArray[j][k]->size_array_state)==1){
                         //Liberation de l'ancien state egale a celui sauvegarder
                         if(initialArray[j][k]!=NULL){
                                   free(initialArray[j][k]->array_state); 
                                   free(initialArray[j][k]);
                         }
                         //Création du nouveau state a mettre a la place
                         State aState2 =(State) malloc(sizeof(StrucState));
                         string_state array2 = (int *) malloc(sizeof(int));
                         array2[0]=i;
                         int size = 1;
                         aState2->array_state = array2;
                         aState2->size_array_state=1;
                         initialArray[j][k] = aState2;  
                    }
               }
          }
          //Libération du state sauvegarder
          free(actualState->array_state);
          free(actualState);
     }
}

//FONCTION: creer le tableau inital utile a l'automatisation
State ** createInitialArray(Automaton * automate){

     //Création du tableau initial de transition et remplissage a NULL 
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
          State aState = (State ) malloc(sizeof(StrucState));
          aState->array_state=array;
          aState->size_array_state=size;
          InitialArray[i][0]= aState;
     }

     //Reste du tableau
     for(i = 0; i<automate->matrix_size; i++){
          for(j=0; j<automate->matrix_size;j++){
               //on recupere le code contenue dans la matrice de transition
               binary_vector code = automate->matrix[i][j];
               //Si ce code est different de zero alors une transition existe
               if(code!=0){
                    //Pour chaque lettre de l'alphabet on creer le filtre lier a cette lettre et sa position dans l'array de lettre de l'automate
                    for(k=0; k<automate->size_alphabet; k++){
                         int filter = pow2(k);
                         if((code&filter)==filter){
                              //Si a la position actuelle c'est égale a NUll alors on doit creer un state
                              if(InitialArray[i][k+1]==NULL){
                                   string_state array2= (int *) malloc(sizeof(int));
                                   array2[0] = j;
                                   int size = 1;
                                   State  aState = (State ) malloc(sizeof(StrucState)); 
                                   aState->array_state=array2;
                                   aState->size_array_state=size;
                                   InitialArray[i][k+1]= aState;
                              }else{
                                   //Sinon c'est que a cette position un state est deja sauvegarder donc on doit l'aggrandir
                                   InitialArray[i][k+1] = realloc(InitialArray[i][k+1], sizeof(int)+((InitialArray[i][k+1]->size_array_state)+1)*(sizeof(int)));
                                   InitialArray[i][k+1]->array_state = realloc(InitialArray[i][k+1]->array_state, ((InitialArray[i][k+1]->size_array_state)+1)*(sizeof(int)));
                                   InitialArray[i][k+1]->size_array_state++;
                                   InitialArray[i][k+1]->array_state[InitialArray[i][k+1]->size_array_state-1]=j;
                              }
                         }
                    }               
               }
          }
     }
     //On renvoie finalement le tableau que l'on vient de creer 
     return InitialArray;
}

//FONCTION: realise la création d'un string state a partie d'un tableau de valeur
string_state copyArrayInteger(int *intialState, int sizeArray){
     string_state aState = (int *) malloc(sizeof(int)*sizeArray);
     int cpt;
     for(cpt=0; cpt<sizeArray; cpt++) {
          int prov = intialState[cpt];
          aState[cpt]=prov;
     }
     return aState;
}
//FONCTION: creer le tableau final a une seule ligne avant le début de l'algorithme
State ** createFinalArray(Automaton * automate){

     State **FinalArray = (State **) malloc(sizeof(State *));
     int i, j;
     FinalArray[0]=(State *) malloc((automate->size_alphabet+1)*sizeof(State));
     for(j=0; j<(automate->size_alphabet+1); j++) FinalArray[0][j]=NULL;
     
     //Remplissons le tableau Initial finale

     //Premiere case : egale au regroupement des etats initiaux ou l'etat initial seul
     string_state array= copyArrayInteger(automate->initial_state,automate->size_of_initial_state);
     State aState = (State ) malloc(sizeof(StrucState)); 
     aState->array_state=array;
     aState->size_array_state=automate->size_of_initial_state;
     FinalArray[0][0]= aState;
     //On renvoie le tableau que l'on vient de creer 
     return FinalArray;
}

//FONCTION: réalise la determinisation de l'automate a partir des fonctions précedentes
void algorithmDeterminization(Automaton * automate){
     //On instancie ce qui va nous etre utile
     int sizeOfInitialArrayCol=automate->size_alphabet+1;
     int sizeOfInitialArrayRow=automate->matrix_size;
     State **InitialArray = createInitialArray(automate);
     int sizeOfFinalArray =1;
     State **FinalArray = createFinalArray(automate);
     int cpt = 0;
     int k,l,m,n;
     //Tant que notre compteur n'est pas egale au moins a notre taille du tableau final
     while(cpt<sizeOfFinalArray){
          //Pour chaque taille de l'alphabet plus un correspondant au nombre de colonne du tableau de transition
          for(k=1; k<automate->size_alphabet+1; k++){
               //Si a la case stocker la valeur est null alors on doit creer son contenu
               if(FinalArray[cpt][k]==NULL){
                    //Création du state
                    string_state array = (int *) malloc(sizeof(int));
                    //On remplis ce tableau avec une valeur inutile de vérification
                    array[0]=-2;
                    int sizeArray = 1;
                    int numberInArray=0;
                    //Compteur/Boolean afin de savoir si on a rencontrer l'etat vide
                    n=0;
                    //On parcours alors l'array contenu a la case du tableau correspondant aux états a regrouper
                    for(l=0; l<FinalArray[cpt][0]->size_array_state; l++){
                         //Si Dans cette array on trouve -1 alors cela correspont a l'etat vide et on sors de la boucle
                         if(FinalArray[cpt][0]->array_state[l]==-1){
                              array[0]=-1;
                              break;
                         }
                         //Si dans l'initial array la valeur a la ligne correspondant au parcours de l'array du state actuelle croiser a la colonne n'est pas null
                         if(InitialArray[FinalArray[cpt][0]->array_state[l]][k]!=NULL){
                              //Alors on va parcourir les states dans l'initial array afin de les regrouper
                              for(m=0;m<InitialArray[FinalArray[cpt][0]->array_state[l]][k]->size_array_state; m++){
                                   //Si notre array de state ne contient pas encore la valeur parcourut
                                   if(alreadyHaveValue(array, InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m], sizeArray)==0){
                                        n++;
                                        //Si le nombre stocker dans l'array de notre state est inferieur a ca taille
                                        if(numberInArray<sizeArray){
                                             //On réalise la copie de cette valeur dans l'array
                                             int prov = InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m];
                                             array[numberInArray++]= prov;
                                        }else{
                                             //Sinon on réalise la copie de cette valeur dans l'array après avoir realloc d'un espace le tableau
                                             ++sizeArray;
                                             array = (int *) realloc(array, sizeArray*sizeof(int));
                                             int prov = InitialArray[FinalArray[cpt][0]->array_state[l]][k]->array_state[m];
                                             array[numberInArray++]=prov;
                                        }
                                   }
                              }
                         }
                         //Si n est a 0 alors on a recontrer l'etat vide donc on y met -1 symbolisant notre etat vide
                         if(n==0){
                              array[0]=-1;
                         }
                    }
                    //On creer un etat 
                    State aState = (State ) malloc(sizeof(StrucState)); 
                    aState->array_state=array;
                    aState->size_array_state=sizeArray;
                    //On verifie ensuite pour la premiere colonne de notre Transition finale si ce state existe deja dans notre tableau si non cela voudra dire qu'un nouvelle etat a etait creer
                    for(l=0; l<sizeOfFinalArray; l++){
                         if(arrayContainSameValue(FinalArray[l][0]->array_state,aState->array_state,FinalArray[l][0]->size_array_state , aState->size_array_state)==1)break;
                    }
                    //Si l est egale a notre taille de tableau alors on a jamais vue cette état
                    if(l==sizeOfFinalArray){
                         //On augmente notre taille de tableau afin de réaliser un parcours supplémentaire pour ce state creer
                         ++sizeOfFinalArray;
                         //On réalloc notre Final Array de transition afin d'avoir une ligne supplémentaire
                         FinalArray = realloc(FinalArray, (sizeOfFinalArray)*(sizeof(State *)));
                         FinalArray[sizeOfFinalArray-1]=(State *) malloc((automate->size_alphabet+1)*sizeof(State));
                         int i2;
                         //On creer un nouveau state pour eviter que deux states pointe vers la meme chose 
                         State secondaryState = (State) malloc(sizeof(StrucState));
                         secondaryState->array_state = copyArrayInteger(aState->array_state, aState->size_array_state);
                         secondaryState->size_array_state=aState->size_array_state;
                         //On ajoute notre state a notre Final transition
                         FinalArray[l][0]= secondaryState;
                         //ON initialise le reste de notre ligne a NULL
                         for(i2=1; i2<(automate->size_alphabet+1); i2++){
                              FinalArray[l][i2]=NULL;
                         }
                    }
                    //On ajoute ce state a notre Transition Finale
                    FinalArray[cpt][k] = aState;
               }
          }
     //On incremente notre compteur 
      ++cpt;

     }
     printf("Traitement determinisation terminé... réécriture de l'automate \n");
     //On definit nos etats finaux nouveaux
     int * finalStateSize = (int *) malloc(sizeof(int));
     int * changingFinalState = changeFinalState(automate, FinalArray, sizeOfFinalArray, finalStateSize);
     int uncpt ;
     //On renomme notre FinalArray 
     renameState(FinalArray, sizeOfFinalArray, automate->size_alphabet+1);

     //On modifie notre automate initial
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
     binary_vector ** matrixProv = changeMatrix(automate, FinalArray, sizeOfFinalArray);
     copyArray(automate->matrix_size, automate->matrix_size, automate->matrix, matrixProv);
     for(uncpt=0; uncpt<automate->matrix_size; uncpt++){
          free(matrixProv[uncpt]);
     }

     free(matrixProv);
     //on liberre la memoire utiliser dans cette partie du programme
     free(finalStateSize);
     free(changingFinalState);
     for(uncpt=0; uncpt<sizeOfInitialArrayRow; uncpt++){
          int uncpt2 ;
          for(uncpt2=0; uncpt2<sizeOfInitialArrayCol; uncpt2++){
               if(InitialArray[uncpt][uncpt2] != NULL) {
                    free(InitialArray[uncpt][uncpt2]->array_state);
                    free(InitialArray[uncpt][uncpt2]);
               }
          }
          free(InitialArray[uncpt]);
     }
     free(InitialArray);
     for(uncpt=0; uncpt<sizeOfFinalArray; uncpt++){
          int uncpt2 ;
          for(uncpt2=0; uncpt2<automate->size_alphabet+1; uncpt2++){
               if(FinalArray[uncpt][uncpt2]!=NULL && FinalArray[uncpt][uncpt2]->array_state!=NULL) {
                    free(FinalArray[uncpt][uncpt2]->array_state);
                    free(FinalArray[uncpt][uncpt2]);
               }
          }
          free(FinalArray[uncpt]);
     }
     free(FinalArray);
     //On finie par afficher le nouvel automate
     printf("\n\n          Votre automate a ete determiniser:              \n\n");
     print_automaton(automate);
}


