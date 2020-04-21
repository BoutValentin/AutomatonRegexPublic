#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef unsigned int binary_vector;

typedef struct
{
     char *alphabet_array;
     int size_alphabet;
     //Changez ca pour un array de 1 
     //TODO
     //int initial_state;
     int *initial_state;
     int size_of_initial_state;
     int *final_state_array;
     int size_final_state;
     
     binary_vector **matrix;
     int matrix_size;
}Automaton;

//Creation des types pour la determinisation
typedef unsigned int * string_state;

typedef struct 
{
     string_state array_state;
     int size_array_state;
} StrucState;

typedef StrucState * State;

typedef struct 
{
     int row;
     int col;
}AlreadyChangedStruct;
typedef AlreadyChangedStruct * AlreadyChanged;


#endif