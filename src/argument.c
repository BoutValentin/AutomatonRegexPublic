#include <stdio.h>
#include "type.h"
#include "parcours.h"

//FONCTION: Test si deux tableau de char sont identiques
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
//FONCTION: Retourne la position du paramètre parmis la ligne d'argument
int getPositionOfParams(char find[], char **arg, int nbrAargument){
     int idx;
     for(idx =1; idx<nbrAargument; idx++){
          if(wordInArrays(find, arg[idx])==0)return idx+1;
     }
     return -1;
}
//FONCTION: Raccourci afin d'afficher l'aide générale
void helperPrint(){
     printf("Compilation du programme: \n");
     printf("Il existe trois manières de compiler le programme: \n");
     printf("\t 1) $make -f Makefile \n");
     printf("\t 2) $make \n");
     printf("\t 3) gcc -o ./bin/automate ./src/*.c \n");
     printf("\nUtilisation de la ligne de commande : \n");
     printf("1) ./bin/automate <chemin_du_fichier> <mot_a_trouver> \n");
     printf("2) Vous pouvez aussi utiliser trois arguments : \n");
     printf("\t-w <mot_a_trouver> : specifie le mot a trouver dans l'automate. \n");
     printf("\t-f <chemin_du_fichier> : specifie le chemin du fichier. \n");
     printf("\t-overwrite : permet dans le cas d'un automate non détérministe de réécrire le fichier passé en paramètres \n");
     printf("Notez que si vous utilisez un seul des deux arguments, l'autre parametre sera alors utilisé comme par defaut \n");
     printf("\nExemple d'utilisation : \n");
     printf("\t./bin/automate ../mon_chemin monmot \n");
     printf("\t./bin/automate -w monmot ../mon_chemin \n");
     printf("\t./bin/automate ../mon_chemin -w monmot  \n");
     printf("\t./bin/automate -f ../mon_chemin monmot  \n");
     printf("\t./bin/automate monmot -f ../mon_chemin \n");
     printf("\t./bin/automate -overwrite -f ../mon_chemin -w monmot  \n");
     printf("\t./bin/automate -w monmot -f ../mon_chemin -overwrite \n");
     printf("\nPour plus d'informations, voir le fichier README.MD \n");
}

//FONCTION: Effectue les tests afin de replacer les arguments et déterminer ceux passer en paramètres ou les mauvaises utilisations
int defineArg(int argc, char **argv, int * shouldOveride){
     int posWord ;
     int posFile;
     switch (argc){
     case 0:
     case 1:
     case 2:
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }else{
               //Si -help n'a pas etait fourni alors le nombre de paramètre est insuffisant
               printf("Nombre d'argument invalid : %d au lieu de 2 \nUse -help to see an help \n", argc-1);
          }
          return -1;
          break;
     case 3:
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }
          //Si le nombre de paramètre est de trois mais que l'un des paramètre -w -f -help a etait passé alors il manque un paramètre pour lancer le programme 
          if(getPositionOfParams("-w", argv, argc)!=-1){
               printf("Mauvaise utilisation de -w sans fichier passer en parametre. \nVoir -help pour plus d'informations \n");
               return -1;
          }
          if(getPositionOfParams("-f", argv, argc)!=-1){
               printf("Mauvaise utilisation de -f sans mot passer en parametre. \nVoir -help pour plus d'informations \n");
               return -1;
          }
          if(getPositionOfParams("-overwrite", argv, argc)!=-1){
               printf("Mauvaise utilisation de -overwrite. Il manque un mot ou le fichier. \nVoir -help pour plus d'informations \n");
               return -1;
          }
          //Dans les autres cas il s'agit de l'utilisation par defaut 
          break;
     case 4:
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          //Sinon dans le cas ou le paramètre overwrite a était passer alors aucun autre ne peut etre passer quand le nombre d'argument est égale a quatre
          }else if(getPositionOfParams("-overwrite", argv, argc)!=-1){
               //Si l'option -w n'a pas etait passer en parametre
               if(getPositionOfParams("-w", argv, argc)==-1){
                    //Si l'option -f n'a pas etait passer en paramètre
                    if(getPositionOfParams("-f", argv, argc)==-1){
                         printf("Attention, en utilisant overwrite le fichier sera récrit et la lecture des parametres se fera en prenant comme premier le fichier et en deuxieme le mot \n");
                         //Dans le cas ou overwrite est passer en deuxieme positon
                         if(getPositionOfParams("-overwrite", argv, argc)==2){
                              //Alors on decale les autres parametre de 1
                              argv[1]=argv[2];
                              argv[2]=argv[3];
                         //Sinon si overwrite est en troisieme position alors on decale juste le mot de un
                         }else if(getPositionOfParams("-overwrite", argv, argc)==3){
                              argv[2]=argv[3];
                         }
                         //Dans les autres cas le fichier et le mot sont deja bien placés
                         //On met le pointeur a 1 pour indiquer plus tard que l'overwrite sera fait
                         *shouldOveride = 1;
                    }else{
                         //Dans le cas ou -f a etait passé on a une mauvaise utilisation
                         printf("Vous ne pouvez pas utiliser et -overwrite et -f avec seulement quatre parametres.\nVoir -help pour plus d'informations\n");
                         return -1;
                    }
               }else{
                    //Dans le cas ou -w a etait passer on a aussi une mauvaise utilisation
                    printf("Vous ne pouvez pas utiliser -overwrite et -w avec seulement quatre parametres.\nVoir -help pour plus d'informations\n");
                    return -1;
               }    
          //Traitement du cas ou -w est passer en passer en paramètres
          }else if((posWord=getPositionOfParams("-w", argv, argc))!=-1){
               //Si la position du mot est en derniere position de la ligne d'argument
               if(posWord==argc-1){
                    //Le fichier ne doit pas etre deplacer
                    argv[1]= argv[1];
                    //Le mot doit etre deplacer en deuxieme position
                    argv[2]= argv[posWord];
               }else{
                    //Le fichier est alors en derniere position
                    argv[1]= argv[argc-1];
                    //Le mot se trouve a posWord
                    argv[2]= argv[posWord];
               }
          //Traitement du cas ou -f est passer en paramètre
          }else if((posFile=getPositionOfParams("-f", argv, argc))!=-1){
               //Si la position du fichier est en dernier position
               if(posFile==argc-1){
                    //On change le mot a la deuxieme position
                    argv[2]= argv[1];
                    //Le fichier est mis a sa position
                    argv[1]= argv[posFile];
               
               }else{
                    //Le fichier est remis en premiere position
                    argv[1]= argv[posFile];
                    //Le mot etant en derniere position on le remet a sa place
                    argv[2]= argv[argc-1];
               }
          //Dans les autres cas un mauvais parametre a etait passer l'éxecution ne se fera pas 
          }else{
              printf("Mauvaise utilisation de la ligne de commande. \nVoir -help pour plus d'informations \n");
               return -1;
          }
          break;
     case 5:
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }
          //Si -overwrite est passer en paramètre on doit effectuer un traitement specifique
          if(getPositionOfParams("-overwrite", argv, argc)!=-1){
               if(getPositionOfParams("-f", argv, argc)!=-1 && getPositionOfParams("-w", argv, argc)!=-1){
                    printf("Vous n'avez pas passez de fichier en parametre ni de mot.\nVoir -help pour plus d'informations \n");
                    return -1;
               }
               if((posFile=getPositionOfParams("-f", argv, argc))!=-1){
                    //On indique qu'un overwrite devra etre fait
                    *shouldOveride = 1;
                    //On recupere la position de overwrite qui retourne sa position +1
                    int posOver = getPositionOfParams("-overwrite", argv, argc);
                    //Dans le cas ou overwrite est en dernière position et le fichier en troisieme
                    if(posOver==5&&posFile==3){
                         //Alors le mot se trouve en premiere position donc on doit d'abord le changer pour eviter de l'ecraser
                         argv[2]=argv[1];
                         //Ensuite on ecrase ce qu'il y avait a la place du mot par la ce qu'il y a a la position du fichier
                         argv[1]=argv[posFile];
                    //Dans le cas ou overwrite est en 2e position et le fichier en quatrième
                    }else if(posOver==3&&posFile==4){
                         //Alors le mot se trouve en premiere position donc on doit d'abord le changer pour eviter de l'ecraser
                         argv[2]=argv[1];
                         //Ensuite on ecrase ce qu'il y avait a la place du mot par la ce qu'il y a a la position du fichier
                         argv[1]=argv[posFile];
                    //Dans le cas ou overwrite est en 1e position et le fichier en quatrième
                    } else if(posOver==2&&posFile==4){
                         //Alors le mot est deja a la bonne place et il ne reste plus que a changer de position le fichier
                         argv[1]=argv[posFile];
                    //Dans le cas ou overwrite est en derniere position et le fichier en 2
                    }else if(posOver==5&&posFile==2){
                         //Alors on change le fichier de place
                         argv[1]=argv[posFile];
                         //puis on change le mot que l'on decale de un rang
                         argv[2]=argv[3];
                    //Dans le cas ou overwrite est en 3e position et le fichier en 2
                    }else if(posOver==4&&posFile==2){
                         //On doit d'abord deplacer le fichier afin de ne pas l'ecraser
                         argv[1]=argv[posFile];
                         //Puis on peut ecraser ce qu'il se trouvait en deuxieme position
                         argv[2]=argv[4];
                    ////Dans le cas ou overwrite est en 1e position et le fichier en 3
                    }else if(posOver==2&&posFile==3){
                         //Alors le mot est en quatrieme position, on change alors le fichier et le mot de position
                         argv[1]=argv[posFile];
                         argv[2]=argv[4];
                    }
               }else if((posWord=getPositionOfParams("-w", argv, argc))!=-1){
                    //On indique qu'un overwrite devra etre fait
                    *shouldOveride = 1;
                    int posOver = getPositionOfParams("-overwrite", argv, argc);
                    //On utilise la meme logique de déduction pas positionnement que precedement pour la position du fichier 
                    if(posOver==5&&posWord==3){
                         argv[2]=argv[posWord];  
                    }else if(posOver==3&&posWord==4){
                         argv[2]=argv[posWord];
                    } else if(posOver==2&&posWord==4){
                         argv[1]=argv[2];
                         argv[2]=argv[posWord];
                    }else if(posOver==5&&posWord==2){
                         argv[1]=argv[3];
                    }else if(posOver==4&&posWord==2){
                          argv[1]=argv[4];
                    }else if(posOver==2&&posWord==3){
                         argv[1]=argv[4];
                         argv[2]=argv[posWord];
                    } 
               }
          }else
               //Si l'un des deux autres paramètres n'a pas etait passer alors on renvoie une erreur car quelque chose d'inconnue a etait passé
               if(getPositionOfParams("-f", argv, argc)==-1 || getPositionOfParams("-w", argv, argc)==-1){
                    printf("Un parametre inconu a ete passer en parametre\n.Voir -help pour plus d'informations \n");
                    return -1;
               }else{
                    //Sinon on recupere la position du mot et du fichier
                    posWord = getPositionOfParams("-w", argv, argc);
                    posFile = getPositionOfParams("-f", argv, argc);
                    //Que l'on met a jour en commencant par le fichier car argv[1] contient forcement une option que l'on peut ecraser
                    argv[1] = argv[posFile];
                    argv[2] = argv[posWord];
               }
          break;
     case 6:
          //Dans ce cas le nombre de parametre est egale au maximum de ce que l'on peut pas passer
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }
          //Si la ligne ne contient pas l'un des parametres alors un paramètre inconnu a etait passer le programme ne se lance pas
          if(getPositionOfParams("-overwrite", argv, argc)==-1 || getPositionOfParams("-f", argv, argc)==-1 ||                 getPositionOfParams("-w", argv, argc)==-1 ){
               printf("Un parametre inconu a ete passer en parametre\n.Voir -help pour plus d'informations \n");
               return -1;
          }
          //Sinon on indique qu'un overwrite devra etre fait
          *shouldOveride = 1;
          //On recupere la position du mot apres -w
          posWord = getPositionOfParams("-w", argv, argc);
          //On recupere la position du fichier apres -f
          posFile = getPositionOfParams("-f", argv, argc);
          //Dans tous les cas en premiere position se trouve forcement une option donc on commence par modifier l'emplacement pour le fichier puis le mot
          argv[1] = argv[posFile];
          argv[2] = argv[posWord];
          break;
     default:
          //Si la ligne de commande contient -help on affiche l'aide et on arrete le programme
          if(getPositionOfParams("-help", argv, argc)!=-1){
               helperPrint();
               return -1;
          }else{
               //Dans les autres cas trop de paramètre on était passer en paramètre le programme sera arreter
               printf("Nombre de parametre trop grand. \nVoir -help pour plus d'information %d\n", argc);
               return -1;
          }
          break;
     }
     return 0;
}