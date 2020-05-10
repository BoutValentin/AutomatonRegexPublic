# Programme d'Automate à expression régulière (AUTOMATON REGEX)
\
**Objectif:** Ce programme a pour but la simulation d'automates finis déterministes ou non déterministes a un nombre "infinie d'état" (Voir la section *Information complementaire*) et un alphabet de 32 caracteres. 


## Rappel: Automate et languages
\
**Rappel:** Un automate se defini par : 
  - Un **alphabet** étant un ensemble de symboles permettant de former les mots. 
        - **Exemple**: {a,b,c} ou {0,1}.
        
  - Un **ensemble d'états** notés de q0 à qn ou n est le nombre d'états.
  
  - **Un** ou **plusieurs états initiaux** appartenant a l'ensemble d'état précèdent. 
        - **Note** : si l'automate a plusieurs états initiaux ont dit alors qu'il est non déterministe.
        
  - **Un ensemble d'états acceptant** ou **états de sorties** appartenant a l'ensemble d'état.
  
  - **Une fonction de transition** qui a pour objectif de définir les relations entre les noeuds du graphes et dont chaque arc est étiqueté par des symboles. Cette fonction peut aussi etre représentée par une matrice d'adjacence dont les élèments sont des sous ensembles de l'alphabet.
        - **Note** : si plusieurs colonnes de la matrice adjancentes sur la même ligne sont égales alors l'automate est non déterministe.

## Format du fichier de description d'automate
\
le fichier passer en paramètres de la commande se définit ainsi :
  - La **premiere ligne** definit **l'alphabet** par une séquence non vide et sans espaces de l'alphabet avec pour taille maximal de 32 caractères.
  
  - La **deuxieme ligne** spécifie **l'état initial** ou les **etats initiaux** par un chiffre ou une une suite de chiffre non vide compris entre 0 et le nombre d'états de votre automate exclus.
  
  - La **troisième ligne** definie **le ou les états acceptant(s)** par par un chiffre ou une une suite de chiffre non vide compris entre 0 et le nombre d'états de votre automate exclus.
     - Exemple : Si votre automate a huit etats le ou les chiffre(s) doivent être compris entre 0 et 7.
  - Les **lignes suivantes** définissent la fonction de transition depuis chacun des états. Cette matrice doit avoir le meme nombre de ligne que de colonne ( *Note : en cas d'erreur dans la matrice le programme vous demandera de corriger votre matrice* ). Chaque ligne est composé de champs séparés par le caractère **;** et chaque champ est un ensemble de symboles représenté par une séquence pouvant être vide (mais sans espaces) de caractères.
  
**Exemple d'un fichier pour un automate lisant les mots de longeurs paires**:
```txt
ab
0
0
;ab;;;
ab;;;;
;;;;
;;;;
;;;;
```

## Utilisation de la ligne de commande

##### Après avoir compiler le programme par :
  
  - L'utilisation du makeFile:
  
  ```sh
  $ make -f MakeFile
  ```
  
  - Ou l'utilisation de la ligne de commande:
  ```sh
  $ gcc -o ./bin/automate ./src/*.c
  ```
  
  Un executable du nom de **automate** sera créer dans le dossier **./bin**.

##### Exécution de la ligne de commande :
La ligne de commande s'éxécute de diffèrente maniere:

 - Utilsation génèrale:
    ```sh
    $ ./bin/automate <lien_du_fichier> <mot_a_tester>
    ```
    >Exemple : ``` $ ./bin/automate ./TestFile/automate1 baba```
    
 - Specifications des options avec -f ou -w: 
    - L'option -f pour file permet de specifier le fichier pris parmis les deux options.
    - L'option -w pour word permet de specifier le mot a chercher parmis les deux options.
    ```sh
    $ ./bin/automate -w <mot_a_tester> -f <lien_du_fichier>
    ```
    > Ces deux options sont indépendantes l'une de l'autre et peuvent être utilisées sans ordre précis. Dans le cas ou l'une est précisée le troisième paramètres sans options sera pris pour l'autre par defaut. Ainsi si seul -f est passée alors le parametres avant -f ou apres le nom du fichier sera pris comme le mot a tester.
    > **Exemple:** <br/>
    >```$ ./bin/automate -f ./TestFile/automate1 baba ```<br/>
    >```$ ./bin/automate baba -f ./TestFile/automate1 ```<br/>
    >```$ ./bin/automate -w baba ./TestFile/automate1 ```<br/>
    >```$ ./bin/automate ./TestFile/automate1 -w baba```<br/>
    >```$ ./bin/automate -w baba -f ./TestFile/automate1 ```<br/>
    >```$ ./bin/automate -f ./TestFile/automate1 -w baba```<br/>
    
 - Pour réecrire votre fichier avec -overwrite:<br/>
    L'option **-overwrite** permet dans le cas ou vous passez un fichier d'un automate non déterministe de réécrire votre fichier avec l'automate déterministe créé par le programme.
    ```sh
    $ ./bin/automate <mot_a_tester> <lien_du_fichier> -overwrite
    ```
    >Cette option est indépendante des autres et peut etre utilisée avec -w et -f de la meme maniere que précèdement en ajoutant -overwrite n'importe ou dans la ligne de commande<br/>
    >**Exemple:**<br/>
    >```$ ./bin/automate -f ./TestFile/automate1 -overwrite baba ``` Ici baba sera pris comme mot a utiliser<br/>
    >```$ ./bin/automate -overwrite -w baba ./TestFile/automate1 ``` Ici automate1 sera pris comme le fichier par defaut<br/>
    #### Attention
    En utilisant seulement quatres paramètres: l'exécutable, le fichier, le mot, et -overwrite, le premiere parametre n'étant pas overwrite sera pris comme le fichier et le deuxieme comme le mot <br/>
    **Exemple:**<br/>
    ```./bin/automate1 ./TestFile/automate1 -overwrite baba ``` <br/>
    ```./bin/automate1 -overwrite ./TestFile/automa1 baba ```<br/>
    ```./bin/automate1 ./TestFile/automate1 baba -overwrite```<br/>
    Ici *./TestFile/automate1* sera pris comme le fichier et *baba* comme le mot dans les trois exemples
 - Pour obtenir de l'aide sur l'utilisation de la ligne de commande :
    ```sh
    $ ./bin/automate -help
    ```
   > De maniere générale l'utilisation de -help avec d'autre argument affiche l'aide et annule l'éxcution du programme. 

## Informations complémentaires:

### Traitement des automates non déterministes:

Dans le cas ou vous passez un fichier décrivant un automate non déterministe (voir condition dans la section *Rappel: Automate et languages*), le programme transformera votre algorithme en automate déterministe afin de faciliter le traitement. Notez que par défaut, seul l'automate du programme est modifié et non le fichier passé en paramètre. Pour modifier aussi votre fichier passer a l'éxecutable l'option **-overwrite**.

### Taille de l'alphabet:
L'alphabet dans le fichier passer en paramètre doit avoir une taille maximale de 32 charactères. Dans le cas contraire le programme vous demandera de modifier votre alphabet dans votre fichier.

### Taile de l'automate (Nombre d'état):

L'automate a une taille maximale definie en fonction d'une constante de lecture et de votre taille de l'alphabet. Pour information, dans le cas ou l'alphabet est de taille maximale soit 32 caractère et que toutes les cases de la matrices sont remplis, la taille de la matrice ne peut execeder 31 par 31 soit 31 états pour l'automate.
>D'une maniere generale le nombre maximum d'état est de 1000/< Taille_Alphabet > dans le cas ou tout les états doivent être remplis.
    
## A propos :
Projet réalisé dans le cadre de la matiere initiation au language C par **BOUT Valentin**.
Le code sera rendu public sur GitHub le 15 mai 2020 à l'adresse : ```https://github.com/BoutValentin/AutomatonRegex```