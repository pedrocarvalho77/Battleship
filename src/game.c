#include "game.h"

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

// Array mode. Cell states:
// '0' water, '1' boat piece not yet hit, '2' boat piece already hit, '3' shot at water
static void shoot_a(SETUP* s, PLAYER* target){

	int map_size = get_map_size(s);
	POINT* p = new_point(map_size);
	int x = get_x(p);
	int y = get_y(p);

	while(target->field[y][x]->state == '2' || target->field[y][x]->state == '3'){
		print_field_s(target,map_size);
		printf("You have already shotted at this position\n");
		free(p);
		p = new_point(map_size);
		x = get_x(p);
		y = get_y(p);
	}
	free(p);

	if(target->field[y][x]->state == '0'){
		printf("You haven't hitted a piece\n");
		target->field[y][x]->state = '3';
	}
	else{
		printf("You have hitted a piece\n");
		target->field[y][x]->state = '2';
		target->field[y][x]->boat_pointer = NULL;
		set_hp(target,-1);
	}
	print_field_p(target,map_size);
	print_field_s(target,map_size);
}

// Quadtree mode: the tree decides whether the shot hits a piece
static void shoot_q(SETUP* s, PLAYER* target){

	int map_size = get_map_size(s);
	int side = (map_size <= 32) ? 32 : 64;
	POINT* p = new_point(map_size);
	POINT* origin = make_point(0,0);
	QD_NODE* leaf = search(target->qtree, p, origin, side);

	if(leaf == NULL || leaf->type != QDLEAF || leaf->node.leaf.field_pointer->state == '2'){
		printf("You haven't hitted a piece\n");
	}
	else{
		printf("You have hitted a piece\n");
		change_field_pointer(leaf);
		set_hp(target,-1);
	}
	free(p);
	free(origin);
	print_field_q(target,map_size);
}

// Players take turns; the game ends as soon as one player's boats are all hit
GAME* play_game(SETUP* s){

	GAME* g = new_game();
	PLAYER* players[2] = {s->p1, s->p2};
	int turn = 0;

	while(1){
		PLAYER* shooter = players[turn];
		PLAYER* target = players[1-turn];

		printf("Player %d, where you want to shoot?\n", get_id(shooter));
		if(s->type_of_storage == 'Q') shoot_q(s,target);
		else shoot_a(s,target);

		if(target->hp == 0){
			g->winner = '1' + turn;
			return g;
		}
		turn = 1-turn;
	}
}
