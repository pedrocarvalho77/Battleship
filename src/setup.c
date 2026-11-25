#include "setup.h"
#include "placement.h"

SETUP* setup_input(){

	SETUP* s = new_setup();

	set_type_of_game(s);
	set_type_of_storage(s);
	set_map_size(s);
	set_max_numb_boats(s);

	s->p1 = new_player(1,get_map_size(s));
	s->p2 = new_player(2,get_map_size(s));
	
	print_map_size(s);
	print_max_numb_boats(s);

	if(get_type_of_game(s) == 'R'){
		rnd_setup(s,s->p1,s->p2);
	}
	else{
		man_setup(s,s->p1,s->p2);
	}
	return s;
}
SETUP* new_setup(){

	SETUP* s;
	s = (SETUP*) malloc(sizeof(SETUP));
	if(s == NULL){
		printf("Error\n");
		exit(0);
	}
	return s;
}
void rnd_setup(SETUP* s, PLAYER* p1,PLAYER* p2){
	
	set_numb_boat_type(s);

	print_numb_boat_type(s);

	get_rnd_boats(p1,s);
	get_rnd_boats(p2,s);
}
void man_setup(SETUP* s, PLAYER* p1,PLAYER* p2){

	set_numb_boat_type(s);

	print_numb_boat_type(s);
	
	get_boats(p1,s);
	get_boats(p2,s);

}
void set_type_of_game(SETUP* s){
	
	printf("Random or manual setup?\n");
	printf("Insert 'R' for random or 'M' for manual\n");
	s->type_of_game = getchar();

	while(s->type_of_game != 'R' && s->type_of_game != 'M'){
		printf("Wrong type of game, insert 'R' for random 'M' for manual\n");
		getchar();
		s->type_of_game = getchar();
	}
}
void set_type_of_storage(SETUP* s){

	printf("QuadTree or Array?\n");
	printf("Insert 'Q' for QuadTree or 'A' for Array\n");
	
	while(getchar() == '\0');
	
	s->type_of_storage = getchar();

	while(s->type_of_storage != 'Q' && s->type_of_storage != 'A'){
		printf("Wrong type of storage, insert 'Q' for QuadTree 'A' for Array\n");
		getchar();
		s->type_of_storage = getchar();
	}
}
void set_map_size(SETUP* s){

	if(get_type_of_game(s) == 'M'){
		printf("Insert size of the map between 20 and 40\n");
		scanf("%d", &s->map_size);
		// TODO: validate that the input is a digit
		while(s->map_size<20 || s->map_size>40){
			printf("Wrong size, insert again\n");
			scanf("%d", &s->map_size);
		}
	}
	else{
		set_rnd_map_size(s);
	}
}
void set_rnd_map_size(SETUP* s){
	s->map_size = rnd_map_size();
}
void set_max_numb_boats(SETUP* s){
	s->max_numb_boat = (s->map_size*s->map_size/25);
}
// pointer to the setup counter of the i-th boat type (T, U, ||, |, L)
static int* boat_count(SETUP* s, int i){
	int* counts[5] = {&s->numb_boat_T, &s->numb_boat_U, &s->numb_boat_II, &s->numb_boat_I, &s->numb_boat_L};
	return counts[i];
}

// sets the number of boats of each type
void set_numb_boat_type(SETUP* s){

	char* boat_type;
	int num = 0;
	int total_boats = 0;
	int max_bound = get_max_numb_boats(s)-4;

	for(int i=0; i<5;i++){

		boat_type = int_to_char(i);
		
		printf("Insert a number between %d and %d for boat's of type ", 1, max_bound);
		print_boat_type(boat_type);
		scanf("%d", &num);
		while(num <= 0 || num > max_bound){
			printf("Insert again a Numb between %d and %d\n", 1, max_bound);
			scanf("%d",&num);
		}
		if(max_bound == num){
			max_bound = 1;
		}else{
			max_bound = max_bound-num;
		}
		total_boats = total_boats + num;
		printf("Total number of boats yet: %d\n", total_boats);
		*boat_count(s,i) = num;
	}
	set_total_numb_boat(s,total_boats);
}

void set_total_numb_boat(SETUP* s, int total){
	s->total_numb_boat = total;
}
int get_type_of_game(SETUP* s){
	return s->type_of_game;
}
int get_map_size(SETUP*s){
	return s->map_size;
}
int get_max_numb_boats(SETUP*s){
	return s->max_numb_boat;
}
int get_numb_boat_type(SETUP* s, char* boat_type){
	
	if(strcmp(boat_type,"T") == 0){
		return s->numb_boat_T;
	}
	else if(strcmp(boat_type,"U") == 0){
		return s->numb_boat_U;
	}
	else if(strcmp(boat_type,"||") == 0){
		return s->numb_boat_II;
	}
	else if(strcmp(boat_type,"|") == 0){
		return s->numb_boat_I;	
	}
	else if(strcmp(boat_type,"L") == 0){
		return s->numb_boat_L;
	}
	return -1;
}
int get_total_numb_boat(SETUP* s){
	return s->total_numb_boat;
}

int rnd_map_size(){
	return (20 + rand() % 21);
}
void print_map_size(SETUP* s){
	printf("The map has a size of [%dx%d]\n", get_map_size(s),get_map_size(s));
}
void print_max_numb_boats(SETUP* s){
	printf("This game has a maximum of %d boats\n", get_max_numb_boats(s));
}
void print_numb_boat_type(SETUP* s){

	for(int i=0; i<5; i++){
		printf("\nHas a total of %d boats of type ", get_numb_boat_type(s,int_to_char(i)));
		print_boat_type(int_to_char(i));
	}
}