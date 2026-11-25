#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "setup.h"

// places every boat of both kinds (random or manual) for one player
void get_rnd_boats(PLAYER*,SETUP*);
void get_boats(PLAYER*,SETUP*);

// 0 if the boat fits, 1 if it overlaps another boat, 2 if it leaves the map
int has_boat_a(POINT*,PLAYER*,int,BOAT*);
int has_boat_q(POINT*,PLAYER*,int,BOAT*);

void place_boat_a(BOAT*,PLAYER*,POINT*,int);
void place_boat_q(BOAT*,PLAYER*,POINT*,int);

#endif
