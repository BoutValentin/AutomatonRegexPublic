#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef unsigned char binary_vector;

typedef struct
{
     char *alphabet_array;
     int size_alphabet;
     int initial_state;
     int *final_state_array;
     int size_final_state;
     
     binary_vector **matrix;
     int matrix_size;
}Automaton;



#endif