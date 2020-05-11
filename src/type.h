#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef unsigned int binary_vector;
//Creation d'une structure automate pour la sauvegarde des éléments
typedef struct
{
     char *alphabet_array;
     int size_alphabet;
     int *initial_state;
     int size_of_initial_state;
     int *final_state_array;
     int size_final_state;
     binary_vector **matrix;
     int matrix_size;
}Automaton;

//Creation des types pour la determinisation
typedef signed int * string_state;

typedef struct 
{
     string_state array_state;
     int size_array_state;
} StrucState;

typedef StrucState * State;

#endif