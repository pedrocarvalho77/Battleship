#include "game.h"
#include "input.h"

GAME* new_game(){

	GAME* g = (GAME*) malloc(sizeof(GAME));
	if(g == NULL){
		printf("Error\n");
		exit(0);
	}
	return g;
}
void set_winner(GAME* g, char player){
	g->winner = player;
}
char get_winner(GAME* g){
	return g->winner;
}

// Cell states: '0' water, '1' boat piece not yet hit, '2' piece already hit, '3' shot at water.
// Array mode finds pieces in the cell array, quadtree mode in the quadtree.
static int is_piece(SETUP* s, PLAYER* target, POINT* p){

	if(s->type_of_storage == 'Q'){
		int map_size = get_map_size(s);
		POINT* origin = make_point(0,0);
		QD_NODE* leaf = search(target->qtree, p, origin, (map_size <= 32) ? 32 : 64);
		free(origin);
		if(leaf == NULL || leaf->type != QDLEAF) return 0;
		change_field_pointer(leaf);
		return 1;
	}
	return target->field[get_y(p)][get_x(p)]->state == '1';
}

// Fires at (x, y) of the target's waters and updates its map and hp.
// *sunk is set to the type of the boat that was sunk by this shot, or NULL.
SHOT_RESULT fire(SETUP* s, PLAYER* target, int x, int y, char** sunk){

	CELL* cell = target->field[y][x];
	POINT* p;
	int piece;

	*sunk = NULL;
	if(cell->state == '2' || cell->state == '3') return SHOT_REPEAT;

	p = make_point(x,y);
	piece = is_piece(s,target,p);
	free(p);

	if(!piece){
		cell->state = '3';
		return SHOT_MISS;
	}
	cell->state = '2';
	set_hp(target,-1);
	if(--cell->boat_pointer->pieces == 0){
		*sunk = cell->boat_pointer->type;
		return SHOT_SUNK;
	}
	return SHOT_HIT;
}

static void shoot(SETUP* s, PLAYER* shooter, PLAYER* target){

	int map_size = get_map_size(s);
	POINT* p;
	SHOT_RESULT result;
	char* sunk;

	clear_screen();
	printf("Player %d, your turn. Shots at player %d's waters:\n\n", get_id(shooter), get_id(target));
	print_field_shots(target,map_size);

	printf("Player %d, where you want to shoot?\n", get_id(shooter));
	while(1){
		p = new_point(map_size);
		result = fire(s,target,get_x(p),get_y(p),&sunk);
		free(p);
		if(result != SHOT_REPEAT) break;
		printf("You have already shotted at this position\n");
	}

	if(result == SHOT_MISS){
		printf("You haven't hitted a piece\n");
	}
	else{
		printf("You have hitted a piece\n");
		if(result == SHOT_SUNK){
			printf("You sank a boat of type ");
			print_boat_type(sunk);
		}
	}
	printf("\n");
	print_field_shots(target,map_size);
}

// Players take turns; the game ends as soon as one player's boats are all hit
GAME* play_game(SETUP* s){

	GAME* g = new_game();
	PLAYER* players[2] = {s->p1, s->p2};
	int turn = 0;

	while(1){
		PLAYER* shooter = players[turn];
		PLAYER* target = players[1-turn];

		shoot(s,shooter,target);
		if(target->hp == 0){
			g->winner = '1' + turn;
			return g;
		}
		wait_enter("Press Enter and pass the keyboard to the other player...");
		turn = 1-turn;
	}
}
