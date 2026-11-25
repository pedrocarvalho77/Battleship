#include "placement.h"

// quadtree side: smallest power of 2 that holds the map
static int tree_side(int map_size){
	return (map_size <= 32) ? 32 : 64;
}

int has_boat_a(POINT* p, PLAYER* pl, int map_size, BOAT* b){

	int x = get_x(p);
	int y = get_y(p);
	for(int i=0;i<5; i++){
		for(int j=0;j<5;j++){
			if(y+i >= map_size || x+j >= map_size){
				return 2;
			}
			if(pl->field[y+i][x+j]->boat_pointer != NULL){
				return 1;
			}
		}
	}
	return 0;
}

int has_boat_q(POINT* p, PLAYER* pl, int map_size, BOAT* b){

	int x = get_x(p);
	int y = get_y(p);
	int side = tree_side(map_size);
	POINT* origin = make_point(0,0);
	POINT* cell = make_point(0,0);
	int result = 0;

	for(int i=0; i<5 && result == 0; i++){
		for(int j=0; j<5; j++){
			if(y+i >= map_size || x+j >= map_size){
				result = 2;
				break;
			}
			set2_x(cell,x+j);
			set2_y(cell,y+i);
			if(b->bitmap[i][j] == '1' && search(pl->qtree, cell, origin, side) != NULL){
				result = 1;
				break;
			}
		}
	}
	free(origin);
	free(cell);
	return result;
}

void place_boat_a(BOAT* b, PLAYER* pl, POINT* p, int map_size){

	int x = get_x(p);
	int y = get_y(p);

	// has_boat_a already guaranteed that the whole 5x5 bitmap fits in the map
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(b->bitmap[i][j] == '1'){
				pl->field[y+i][x+j]->boat_pointer = b;
				pl->field[y+i][x+j]->state = '1';
			}
		}
	}
	set_hp(pl,boat_hp(b->type));
}

void place_boat_q(BOAT* b, PLAYER* pl, POINT* p, int map_size){

	int x = get_x(p);
	int y = get_y(p);
	int side = tree_side(map_size);
	POINT* origin = make_point(0,0);
	POINT* cell = make_point(0,0);

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(b->bitmap[i][j] == '1'){
				set2_x(cell,x+j);
				set2_y(cell,y+i);
				insert(pl->qtree, cell, origin, side);
				// the array map is kept only to draw the boats on screen
				pl->field[y+i][x+j]->boat_pointer = b;
			}
		}
	}
	free(origin);
	free(cell);
	set_hp(pl,boat_hp(b->type));
}

static int has_boat(char storage, POINT* p, PLAYER* pl, int map_size, BOAT* b){
	return storage == 'A' ? has_boat_a(p,pl,map_size,b) : has_boat_q(p,pl,map_size,b);
}

static void place_boat(char storage, BOAT* b, PLAYER* pl, POINT* p, int map_size){
	if(storage == 'A') place_boat_a(b,pl,p,map_size);
	else place_boat_q(b,pl,p,map_size);
}

// rotates the boat by a random multiple of 90 degrees
static void random_rotate(BOAT* b){
	int turns = rnd_rotation();
	for(int i=0; i<turns; i++){
		rotate_boat(b);
	}
}

// asks the player for a rotation (multiples of 90 up to 360) and rotates the boat
static void ask_rotation(BOAT* b){

	int rotation = 0;

	printf("What rotation do you want for the boat, 0º,90º,180º,270º,360º?\n");
	scanf("%d", &rotation);
	while(rotation != 0 && rotation != 90 && rotation != 180 && rotation != 270 && rotation != 360){
		printf("Impossible rotation, insert again\n");
		scanf("%d", &rotation);
	}
	for(int i=0; i<rotation/90; i++){
		rotate_boat(b);
	}
}

void get_rnd_boats(PLAYER* pl ,SETUP* s){

	int map_size = get_map_size(s);
	char storage = s->type_of_storage;

	printf("Generating boats for the player %d\n", get_id(pl));

	for(int i=0; i<5; i++){

		char* boat_type = int_to_char(i);
		int numb_boat_type = get_numb_boat_type(s,boat_type);

		printf("Player %d has %d boats type ",get_id(pl),numb_boat_type);
		print_boat_type(boat_type);

		for(int j=0;j<numb_boat_type;j++){

			BOAT* b = new_boat(boat_type);
			POINT* p = rnd_point(map_size);
			random_rotate(b);
			while(has_boat(storage,p,pl,map_size,b) != 0){
				free(p);
				p = rnd_point(map_size);
				random_rotate(b);
			}
			print_boat(b);
			print_point(p);
			place_boat(storage,b,pl,p,map_size);
			free(p);
			print_field_p(pl,map_size);
		}
	}
}

void get_boats(PLAYER* pl, SETUP* s){

	int map_size = get_map_size(s);
	char storage = s->type_of_storage;

	printf("\nPlayer %d, place your boats\n", get_id(pl));

	for(int i=0; i<5; i++){

		char* boat_type = int_to_char(i);
		int numb_boat_type = get_numb_boat_type(s,boat_type);

		for(int j=0;j<numb_boat_type;j++){

			BOAT* b = new_boat(boat_type);
			int fit;
			printf("Where do you want to place boat number %d of type ", (j+1));
			print_boat_type(boat_type);

			POINT* p = new_point(map_size);
			ask_rotation(b);

			while((fit = has_boat(storage,p,pl,map_size,b)) != 0){
				if(fit == 1){
					printf("You already have a boat in that position, insert again\n");
				}
				else{
					printf("Outside the map boundaries, enter again\n");
				}
				free(p);
				p = new_point(map_size);
				ask_rotation(b);
			}
			print_boat(b);
			place_boat(storage,b,pl,p,map_size);
			free(p);
			print_field_p(pl,map_size);
		}
	}
}
