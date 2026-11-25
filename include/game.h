#ifndef GAME_H
#define	GAME_H

#include "setup.h"

typedef struct{

	char winner;

} GAME;

GAME* new_game();

void set_winner(GAME*,char);

char get_winner(GAME*);

GAME* play_game(SETUP*);

typedef enum { SHOT_REPEAT, SHOT_MISS, SHOT_HIT, SHOT_SUNK } SHOT_RESULT;

// fires at (x, y) of the target's waters; *sunk is the type of the boat sunk by the shot, or NULL
SHOT_RESULT fire(SETUP*,PLAYER*,int,int,char**);

#endif