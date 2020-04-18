#include <stdio.h>
#include "type.h"

#include "parcours.h"

int wordInArrays(char find[], char *array){
     int lengthFind = getLength(find);
     int lengthArray = getLength(array);
     if(lengthArray!=lengthFind) return -1;
     int idx;
     for(idx=0; idx<lengthArray; idx++){
          if(find[idx]!=array[idx]) return -1;
     }
     return 0;
}
int getPositionOfParams(char find[], char **arg, int nbrAargument){
     int idx;
     for(idx =1; idx<nbrAargument; idx++){
          if(wordInArrays(find, arg[idx])==0)return idx+1;
     }
     return -1;
}

void helperPrint(){
     printf("Utilisation de la ligne de commande : \n");
     printf("1) ./automate <chemin_du_fichier> <mot_a_trouver> \n");
     printf("2) Vous pouvez aussi utiliser deux arguments : \n");
     printf("    -w <mot_a_trouver> : specifie le mot a trouver dans l'automate. \n");
     printf("    -f <chemin_du_fichier> : specifie le chemin du fichier. \n");
     printf("Notez que si vous utilisez un seul des deux arguments, l'autre parametre sera alors utiliser comme par defaut \n");
     printf("Exemple d'utilisation : \n");
     printf("./automate ../mon_chemin monmot \n");
     printf("./automate -w monmot ../mon_chemin \n");
     printf("./automate ../mon_chemin -w monmot  \n");
     printf("./automate -f ../mon_chemin monmot  \n");
     printf("./automate monmot -f ../mon_chemin \n");
     printf("./automate -f ../mon_chemin -w monmot  \n");
     printf("./automate -w monmot -f ../mon_chemin \n");
}


int defineArg(int argc, char **argv){
     int posWord ;
     int posFile;
     switch (argc)
     {
     case 0:
     case 1:
     case 2:
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }else{
               printf("Nombre d'argument invalid : %d au lieu de 2 \nUse -help to see an help \n", argc-1);
          }
          return -1;
          break;
     case 3:
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }if(getPositionOfParams("-w", argv, argc)!=-1){
               printf("Mauvaise utilisation de -w sans fichier passer en parametre. \nVoir -help pour plus d'informations \n");
               return -1;
          }if(getPositionOfParams("-f", argv, argc)!=-1){
               printf("Mauvaise utilisation de -f sans mot passer en parametre. \nVoir -help pour plus d'informations \n");
               return -1;
          }
          break;
     case 4:
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }else if((posWord=getPositionOfParams("-w", argv, argc))!=-1){
               if(posWord==argc-1){
                    argv[1]= argv[1];
                    argv[2]= argv[posWord];
               }else{
                    argv[1]= argv[argc-1];
                    argv[2]= argv[posWord];
               }
          }else if((posFile=getPositionOfParams("-f", argv, argc))!=-1){
               if(posFile==argc-1){
                    argv[2]= argv[1];
                    argv[1]= argv[posFile];
               
               }else{
                    argv[1]= argv[posFile];
                    argv[2]= argv[argc-1];
               }
          }
          break;
     case 5:
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }
          posWord = getPositionOfParams("-w", argv, argc);
          posFile = getPositionOfParams("-f", argv, argc);
          argv[1] = argv[posFile];
          argv[2] = argv[posWord];
          break;
     default:
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }else{
               printf("Nombre de parametre trop grand. \nVoir -help pour plus d'information %d\n", argc);
               return -1;
          }
          break;
     }

     return 0;
}