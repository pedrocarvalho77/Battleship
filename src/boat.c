#include "boat.h"

BOAT* new_boat(char* boat_type){

	BOAT* b = (BOAT*) malloc(sizeof(BOAT));
	if(b == NULL){
		printf("Error\n");
		exit(0);
	}
	if(strcmp(boat_type,"T") == 0){
		set_boat_type(b,"T");
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){	
				if(i == 0) b->bitmap[i][j] = '1';
				else if(j == 2) b->bitmap[i][j] = '1';
				else b->bitmap[i][j] = '0';
			}
		}
	}
	else if(strcmp(boat_type,"U") == 0){
		set_boat_type(b, "U");
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){	
				if(j == 0 || j == 4) b->bitmap[i][j] = '1';
				else if(i == 4) b->bitmap[i][j] = '1';
				else b->bitmap[i][j] = '0';
			}
		}
	}
	else if(strcmp(boat_type,"||") == 0){
		set_boat_type(b,"||");
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){	
				if(j == 0 || j == 1) b->bitmap[i][j] = '1';
				else b->bitmap[i][j] = '0';
			}
		}
	}
	else if(strcmp(boat_type,"|") == 0){
		set_boat_type(b,"|");
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){	
				if(j == 0 ) b->bitmap[i][j] = '1';
				else b->bitmap[i][j] = '0';
			}
		}
	}
	else if(strcmp(boat_type,"L") == 0){
		set_boat_type(b,"L");
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){	
				if(j == 0) b->bitmap[i][j] = '1';
				else if(i==4) b->bitmap[i][j] = '1';
				else b->bitmap[i][j] = '0';
			}
		}
	}
	return b;
}
void set_boat_type(BOAT* b, char* t){
	b->type = t; // always a string literal, so no copy is needed
}
char* get_boat_type(BOAT* b){
	return b->type;
}
// moves the shape to the top-left corner of the 5x5 bitmap, so that the position
// chosen by the player is the corner of the visible boat
static void shift_boat(BOAT* b){

	int up = 5, left = 5;

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(b->bitmap[i][j] == '1'){
				if(i < up) up = i;
				if(j < left) left = j;
			}
		}
	}
	if(up == 5 || (up == 0 && left == 0)) return;
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			b->bitmap[i][j] = (i+up < 5 && j+left < 5) ? b->bitmap[i+up][j+left] : '0';
		}
	}
}

void rotate_boat(BOAT* b){

	for(int i=0; i<5/2; i++){ 
		for(int j=i; j<5-i-1; j++){ 
    		char aux = b->bitmap[i][j]; 
			b->bitmap[i][j] = b->bitmap[5-1-j][i]; 
			b->bitmap[5-1-j][i] = b->bitmap[5-1-i][5-1-j]; 
			b->bitmap[5-1-i][5-1-j] = b->bitmap[j][5-1-i]; 
			b->bitmap[j][5-1-i] = aux; 
		}	 
	}
	shift_boat(b);
}

int rnd_rotation(){
	return rand() % 4;
}

// converts an int to a char*, to make boat types easier to process
// (boat types)
char* int_to_char(int num){
	
	switch(num){
		case 0:
			return "T";
			break;
		case 1:
			return "U";
			break;
		case 2:
			return "||";
			break;
		case 3:
			return "|";
			break;
		case 4:
			return "L";
			break;
		default:
			return "undefined";
			break;
	}
}
void print_boat_type(char* boat_type){

	for(int i=0; i<strlen(boat_type); i++){
		printf("%c", boat_type[i]);
	}
	printf("\n");
}
void print_boat(BOAT* b){

	print_boat_type(b->type);
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			printf("%c", b->bitmap[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
// number of pieces (and so hp) of a boat type
int boat_hp(char* type){

	if(strcmp(type,"T") == 0) return 9;
	if(strcmp(type,"U") == 0) return 13;
	if(strcmp(type,"||") == 0) return 10;
	if(strcmp(type,"|") == 0) return 5;
	if(strcmp(type,"L") == 0) return 9;
	return 0;
}
