#include "engine.h"

int main(int argc, char** argv){
	
	ENGINE* e = (ENGINE*) malloc(sizeof(ENGINE));
	if(e == NULL){
		printf("Error\n");
		exit(0);
	}
	srand(time(NULL));
	system("clear");
	print_logo();
	e->s = setup_input();
	e->g = play_game(e->s);
	
	if(e->g->winner == '1'){
		printf("Player 1 you have won\n");
	}
	else{
		printf("Player 2 you have won\n");
	}
	free_player(e->s->p1,get_map_size(e->s));
	free_player(e->s->p2,get_map_size(e->s));
	free(e->s);
	free(e->g);
	free(e);
	exit(0);
}