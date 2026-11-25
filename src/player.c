#include "player.h"

PLAYER* new_player(int player_id, int map_size){

	PLAYER* p = (PLAYER*) malloc(sizeof(PLAYER));
	if(p == NULL){
		printf("Error\n");
		exit(0);
	} 
	set_id(p,player_id);
	set_field(p,map_size);
	p->qtree = new_qd_node(QDNODE);
	p->hp = 0;
	p->boats = NULL;
	p->numb_boats = 0;
	return p;
}
void set_id(PLAYER* p, int player_id){
	p->id = player_id;
}
void set_hp(PLAYER* p, int numb){
	p->hp = p->hp + numb;
}
void set_field(PLAYER* p, int map_size){
	
	p->field = (CELL***) malloc(map_size*sizeof(CELL**));
	if(p->field == NULL){
		printf("Error\n");
		exit(0);
	}
	for(int i=0; i<map_size; i++){
		p->field[i] = new_array_cell(map_size);
	}
}
int get_id(PLAYER* p){
	return p->id;
}
int get_hp(PLAYER* p){
	return p->hp;
}
void print_id(PLAYER* p){
	printf("Player %d\n", p->id);
}

// prints what the shooter knows about the target's waters: X hit, o miss, . unknown
void print_field_shots(PLAYER* target, int map_size){

	printf("   ");
	for(int j=0; j<map_size; j++){
		printf("%d ", j%10);
	}
	printf("\n");
	for(int i=0; i<map_size; i++){
		printf("%2d ", i);
		for(int j=0; j<map_size; j++){
			char state = target->field[i][j]->state;
			printf("%c ", state == '2' ? 'X' : state == '3' ? 'o' : '.');
		}
		printf("\n");
	}
	printf("\n");
}

// prints each player's map, showing the boat shapes
void print_field_p(PLAYER* p, int map_size){

	for(int i=0; i<map_size; i++){
		for(int j=0; j<map_size; j++){
			if(p->field[i][j]->boat_pointer == NULL){
				printf("~");
			}
			else{
				if(strcmp(p->field[i][j]->boat_pointer->type,"T") == 0){
					printf("T");
				}
				else if(strcmp(p->field[i][j]->boat_pointer->type,"U") == 0){
					printf("U");	
				}
				else if(strcmp(p->field[i][j]->boat_pointer->type,"||") == 0){
					printf("I");	
				}
				else if(strcmp(p->field[i][j]->boat_pointer->type,"|") == 0){
					printf("i");	
				}
				else if(strcmp(p->field[i][j]->boat_pointer->type,"L") == 0){
					printf("L");	
				}
			}
		}
		printf("\n");
	}
	printf("\n");
}
// frees the player, its map, its quadtree and the boats still on the map
void free_player(PLAYER* p, int map_size){

	for(int i=0; i<map_size; i++){
		for(int j=0; j<map_size; j++){
			free(p->field[i][j]);
		}
		free(p->field[i]);
	}
	free(p->field);
	free_qd_node(p->qtree);
	for(int i=0; i<p->numb_boats; i++){
		free(p->boats[i]);
	}
	free(p->boats);
	free(p);
}

// remembers a boat so free_player can release it (cells forget it once hit)
void add_boat(PLAYER* p, BOAT* b){

	p->boats = (BOAT**) realloc(p->boats, (p->numb_boats+1)*sizeof(BOAT*));
	if(p->boats == NULL){
		printf("Error\n");
		exit(0);
	}
	p->boats[p->numb_boats++] = b;
}
