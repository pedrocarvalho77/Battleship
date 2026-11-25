#include "game.h"

GAME* new_game(){

	GAME* g = (GAME*) malloc(sizeof(GAME));

	return g;
}
void set_winner(GAME* g, char player){
	g->winner = player;
}
char get_winner(GAME* g){
	return g->winner;
}
GAME* play_game_a(SETUP* s){

	GAME* g = new_game();
	POINT* p;
	
	while(s->p1->hp != 0 && s->p2->hp != 0){

		int x,y;
		
		// player 1's turn
		printf("Player1 ,where you want to shoot?\n");

		p = new_point(get_map_size(s));
		x = get_x(p);
		y = get_y(p);

		// '0' cell state: water
		// '1' cell state: boat piece not yet hit
		// '2' cell state: boat piece already hit
		// '3' cell state: shot at water
		// loop that checks the cell states of player 2's map
		while(s->p2->field[y][x]->state == '2' || s->p2->field[y][x]->state == '3'){
			print_field_s(s->p2,get_map_size(s));
			printf("You have already shotted at this position\n");
			p = new_point(get_map_size(s));
			x = get_x(p);
			y = get_y(p);
		}
		if(s->p2->field[y][x]->state == '0'){
			printf("You haven't hitted a piece\n");
			s->p2->field[y][x]->state = '3';
		}
		else if(s->p2->field[y][x]->state == '1'){
			printf("You have hitted a piece\n");
			s->p2->field[y][x]->state = '2';
			s->p2->field[y][x]->boat_pointer = NULL;
			set_hp(s->p2,-1);
		}
		print_field_p(s->p2,get_map_size(s));
		print_field_s(s->p2,get_map_size(s));
		
		// player 2's turn
		printf("Player 2, where you want to shoot?\n");
		p = new_point(get_map_size(s));
		x = get_x(p);
		y = get_y(p);
		
		// same loop as above, for player 1
		while(s->p1->field[y][x]->state == '2' || s->p1->field[y][x]->state == '3'){
			print_field_s(s->p1,get_map_size(s));
			printf("You have already shotted at this position\n");
			p = new_point(get_map_size(s));
			x = get_x(p);
			y = get_y(p);
		}
		if(s->p1->field[y][x]->state == '0'){
			printf("You haven't hitted a piece\n");
			s->p1->field[y][x]->state = '3';
		}
		else if(s->p1->field[y][x]->state == '1'){
			printf("You have hitted a piece\n");
			s->p1->field[y][x]->state = '2';
			set_hp(s->p1,-1);
		}
		print_field_p(s->p1,get_map_size(s));
		print_field_s(s->p1,get_map_size(s));
	}
	if(s->p1->hp == 0){
		g->winner = '2';
	}
	else{
		g->winner = '1';
	}
	return g;

	free(p);
}
GAME* play_game_q(SETUP* s){

	GAME* g = new_game();
	POINT* p;
	
	while(s->p1->hp != 0 && s->p2->hp != 0){

		int side;
		QD_NODE* qd_node = NULL;
		POINT* se_corner;
		
		// player 1's turn
		printf("Player1 ,where you want to shoot?\n");

		p = new_point(get_map_size(s));

		if(get_map_size(s) <= 32) side = 32;
		else side = 64;

		se_corner = make_point(side,side);

		qd_node = search(s->p2->qtree, p,se_corner,side);
		if(qd_node == NULL || qd_node->type != QDLEAF || qd_node->node.leaf.field_pointer->state == 3){
			printf("You haven't hitted a piece\n");
		}
		else{
			printf("You have hitted a piece\n");
			change_field_pointer(qd_node);
			set_hp(s->p2,-1);
		}
		
		print_field_q(s->p2,get_map_size(s));

		// player 2's turn
		printf("Player 2, where you want to shoot?\n");
		p = new_point(get_map_size(s));

		if(get_map_size(s) <= 32) side = 32;
		else side = 64;

		se_corner = make_point(side,side);

		qd_node = search(s->p1->qtree, p,se_corner,side);
		if(qd_node == NULL || qd_node->type != QDLEAF || qd_node->node.leaf.field_pointer->state == 3){
			printf("You haven't hitted a piece\n");
		}
		else{
			printf("You have hitted a piece\n");
			change_field_pointer(qd_node);
			set_hp(s->p1,-1);
		}

		print_field_q(s->p1,get_map_size(s));

	}
	if(s->p1->hp == 0){
		g->winner = '2';
	}
	else{
		g->winner = '1';
	}
	return g;
}