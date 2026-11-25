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

static void shoot(SETUP* s, PLAYER* shooter, PLAYER* target){

	int map_size = get_map_size(s);
	POINT* p;
	CELL* cell;

	clear_screen();
	printf("Player %d, your turn. Shots at player %d's waters:\n\n", get_id(shooter), get_id(target));
	print_field_shots(target,map_size);

	printf("Player %d, where you want to shoot?\n", get_id(shooter));
	p = new_point(map_size);
	cell = target->field[get_y(p)][get_x(p)];
	while(cell->state == '2' || cell->state == '3'){
		printf("You have already shotted at this position\n");
		free(p);
		p = new_point(map_size);
		cell = target->field[get_y(p)][get_x(p)];
	}

	if(is_piece(s,target,p)){
		BOAT* b = cell->boat_pointer;
		printf("You have hitted a piece\n");
		cell->state = '2';
		set_hp(target,-1);
		if(--b->pieces == 0){
			printf("You sank a boat of type ");
			print_boat_type(b->type);
		}
	}
	else{
		printf("You haven't hitted a piece\n");
		cell->state = '3';
	}
	free(p);
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
