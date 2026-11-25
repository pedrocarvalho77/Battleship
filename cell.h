#ifndef CELL_H
#define CELL_H

#include "boat.h"

typedef struct{
	
	char state;					 
	BOAT* boat_pointer;
	int shot_count;
				
} CELL;

// signature of the function that creates a cell

// creates a cell
CELL* new_cell();
// creates an array of cells
CELL** new_array_cell(int);

void set_state(CELL*, char);
void set_pointer_to_null(CELL*);
void set_pointer(CELL*, BOAT*);

char get_state(CELL*);
BOAT* get_pointer(CELL*);

void print_cell(CELL*);

#endif