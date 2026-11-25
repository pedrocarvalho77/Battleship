#include "engine.h"
#include "input.h"
#include <ctype.h>

int main(int argc, char** argv){

	char again = 'Y';
	srand(time(NULL));

	while(again == 'Y'){

		ENGINE* e = (ENGINE*) malloc(sizeof(ENGINE));
		if(e == NULL){
			printf("Error\n");
			exit(0);
		}
		clear_screen();
		print_logo();
		e->s = setup_input();
		clear_screen();
		e->g = play_game(e->s);

		printf("Player %c you have won\n", e->g->winner);

		free_player(e->s->p1,get_map_size(e->s));
		free_player(e->s->p2,get_map_size(e->s));
		free(e->s);
		free(e->g);
		free(e);

		printf("Play again? Insert 'Y' for yes or 'N' for no\n");
		again = toupper(read_char());
		while(again != 'Y' && again != 'N'){
			printf("Insert 'Y' for yes or 'N' for no\n");
			again = toupper(read_char());
		}
	}
	return 0;
}
