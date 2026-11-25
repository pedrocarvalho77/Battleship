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

#endif