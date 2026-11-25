#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <stdio.h>

// coordinate struct
typedef struct{
							 
	int x;	// x coordinate 			
	int y;	// y coordinate	 	
						
} POINT;

// signatures of the functions that create coordinates

POINT* new_point(int);
void set_x(POINT*,int);
void set_y(POINT*,int);
void set2_x(POINT* p, int x);
void set2_y(POINT* p, int y);

int get_x(POINT*);
int get_y(POINT*);

// returns a coordinate inside the map size

POINT* rnd_point(int);

POINT* make_point(int,int);
POINT* add_point(POINT* p, POINT* q);
int is_equal(POINT* p,POINT* q); 
int in_square(POINT* p, POINT* corner, int side); // checks whether p is inside the square defined by (corner, side)

void print_point(POINT*);

#endif