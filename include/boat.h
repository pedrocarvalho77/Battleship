#ifndef BOAT_H
#define BOAT_H

#include <string.h>
#include "point.h"

// boat struct
typedef struct{
	
	char* type; // boat type string, e.g. "T", "L", etc.
	char bitmap[5][5];
	int pieces; // pieces not yet hit

} BOAT;

// signatures of the functions that create boats
BOAT* new_boat(char*);

void set_boat_type(BOAT*,char*);

char* get_boat_type(BOAT*);

// rotates the boat bitmap
void rotate_boat(BOAT*);
// returns a random value 0-3: rotate the bitmap by 0, 90, 180 or 270 degrees 
int rnd_rotation();

// helper that returns the boat type for a loop index
char* int_to_char(int);

void print_boat_type(char*);
void print_boat(BOAT*);
int boat_hp(char*);

#endif