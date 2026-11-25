// Graphical hot-seat front end for the Battleship engine, built with raylib.
// It reuses the game logic (setup, placement, fire) and replaces the terminal I/O.
#include <raylib.h>
#include <stdio.h>
#include "game.h"
#include "placement.h"

#define SIZE   20            // map is 20x20, with one boat of each of the 5 types
#define CS     26            // cell size in pixels, main grid
#define MINI   14            // cell size in pixels, own-fleet grid
#define GX     40            // main grid origin
#define GY     120
#define WIDTH  960
#define HEIGHT 680

typedef enum { PLACE, PASS, PLAY, RESULT, OVER } SCREEN;

static SETUP* setup;
static BOAT* current;        // boat being placed, not yet on the map
static int boat_index;       // type of the boat being placed (0..4)
static int turn;             // 0 = player 1, 1 = player 2
static SCREEN screen;
static SCREEN after_pass;
static char message[96];

static PLAYER* player(int i){ return i == 0 ? setup->p1 : setup->p2; }

static Color type_color(char* type){

	if(type[0] == 'T') return ORANGE;
	if(type[0] == 'U') return PURPLE;
	if(type[0] == '|' && type[1] == '|') return DARKGREEN;
	if(type[0] == '|') return GOLD;
	return PINK;
}

static void new_match(){

	if(setup != NULL){
		free_player(setup->p1,SIZE);
		free_player(setup->p2,SIZE);
		free(setup);
	}
	free(current);
	setup = new_setup();
	setup->type_of_storage = 'A';
	setup->type_of_game = 'M';
	setup->map_size = SIZE;
	setup->max_numb_boat = 16;
	setup->numb_boat_T = setup->numb_boat_U = setup->numb_boat_II = setup->numb_boat_I = setup->numb_boat_L = 1;
	setup->total_numb_boat = 5;
	setup->p1 = new_player(1,SIZE);
	setup->p2 = new_player(2,SIZE);
	boat_index = 0;
	current = new_boat(int_to_char(0));
	turn = 0;
	screen = PLACE;
}

static void pass_to(SCREEN next){
	after_pass = next;
	screen = PASS;
}

// grid cell under the mouse, or 0 if outside the grid at (ox, oy)
static int mouse_cell(int ox, int oy, int* x, int* y){

	int mx = GetMouseX() - ox;
	int my = GetMouseY() - oy;

	if(mx < 0 || my < 0 || mx >= SIZE*CS || my >= SIZE*CS) return 0;
	*x = mx / CS;
	*y = my / CS;
	return 1;
}

// own = show the boats (a player's own map); otherwise only the shots are shown
static void draw_grid(PLAYER* pl, int ox, int oy, int cs, int own){

	for(int y=0; y<SIZE; y++){
		for(int x=0; x<SIZE; x++){
			CELL* c = pl->field[y][x];
			int px = ox + x*cs;
			int py = oy + y*cs;
			Color color = (Color){ 190, 220, 245, 255 };

			if(own && c->boat_pointer != NULL) color = type_color(c->boat_pointer->type);
			DrawRectangle(px, py, cs, cs, color);
			if(c->state == '2'){
				DrawRectangle(px, py, cs, cs, (Color){ 200, 30, 30, 200 });
				DrawLine(px+3, py+3, px+cs-3, py+cs-3, WHITE);
				DrawLine(px+cs-3, py+3, px+3, py+cs-3, WHITE);
			}
			else if(c->state == '3'){
				DrawCircle(px + cs/2, py + cs/2, cs/6.0f, DARKBLUE);
			}
			DrawRectangleLines(px, py, cs, cs, (Color){ 90, 120, 150, 255 });
		}
	}
}

static void draw_placing(){

	PLAYER* pl = player(turn);
	int x, y;

	DrawText(TextFormat("Player %d: place your boats", turn+1), GX, 30, 28, DARKGRAY);
	DrawText(TextFormat("Boat %d of 5 (type %s)", boat_index+1, current->type), GX, 66, 20, GRAY);
	DrawText("Click: place   R: rotate   A: place all randomly", GX, 90, 18, GRAY);
	draw_grid(pl, GX, GY, CS, 1);

	if(mouse_cell(GX, GY, &x, &y)){
		POINT p = { x, y };
		int fits = has_boat_a(&p, pl, SIZE, current) == 0;

		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				if(current->bitmap[i][j] == '1' && y+i < SIZE && x+j < SIZE){
					DrawRectangle(GX + (x+j)*CS, GY + (y+i)*CS, CS, CS, fits ? (Color){ 40, 170, 60, 170 } : (Color){ 220, 40, 40, 170 });
				}
			}
		}
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && fits){
			place_boat_a(current, pl, &p, SIZE);
			boat_index++;
			current = boat_index < 5 ? new_boat(int_to_char(boat_index)) : NULL;
		}
	}
	if(boat_index == 0 && IsKeyPressed(KEY_A)){
		free(current);
		current = NULL;
		get_rnd_boats(pl, setup);
		boat_index = 5;
	}
	if(current != NULL && IsKeyPressed(KEY_R)){
		rotate_boat(current);
	}
	if(boat_index == 5){
		if(turn == 0){
			turn = 1;
			boat_index = 0;
			current = new_boat(int_to_char(0));
			pass_to(PLACE);
		}
		else{
			turn = 0;
			pass_to(PLAY);
		}
	}
}

static void draw_playing(){

	PLAYER* me = player(turn);
	PLAYER* target = player(1-turn);
	int x, y;

	DrawText(TextFormat("Player %d: your turn", turn+1), GX, 30, 28, DARKGRAY);
	DrawText(TextFormat("Shots at player %d's waters", 2-turn), GX, 66, 20, GRAY);
	DrawText(screen == PLAY ? "Click a cell to fire" : message, GX, 90, 20, screen == PLAY ? GRAY : MAROON);
	draw_grid(target, GX, GY, CS, 0);

	DrawText("Your fleet", GX + SIZE*CS + 50, GY - 24, 20, GRAY);
	draw_grid(me, GX + SIZE*CS + 50, GY, MINI, 1);
	DrawText(TextFormat("Your pieces left: %d", me->hp), GX + SIZE*CS + 50, GY + SIZE*MINI + 16, 20, DARKGRAY);
	DrawText(TextFormat("Enemy pieces left: %d", target->hp), GX + SIZE*CS + 50, GY + SIZE*MINI + 44, 20, DARKGRAY);

	if(screen == PLAY){
		if(mouse_cell(GX, GY, &x, &y)){
			DrawRectangleLines(GX + x*CS, GY + y*CS, CS, CS, RED);
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
				char* sunk;
				SHOT_RESULT result = fire(setup, target, x, y, &sunk);

				if(result == SHOT_REPEAT) return;
				if(result == SHOT_MISS) snprintf(message, sizeof(message), "Miss. Click to pass the turn");
				else if(result == SHOT_HIT) snprintf(message, sizeof(message), "Hit! Click to pass the turn");
				else snprintf(message, sizeof(message), "Hit! You sank a %s boat. Click to pass the turn", sunk);
				screen = target->hp == 0 ? OVER : RESULT;
			}
		}
	}
	else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		turn = 1-turn;
		pass_to(PLAY);
	}
}

int main(void){

	InitWindow(WIDTH, HEIGHT, "Battleship");
	SetTargetFPS(60);
	srand((unsigned) (GetTime()*1000000) + 1);
	new_match();

	while(!WindowShouldClose()){

		BeginDrawing();
		ClearBackground(RAYWHITE);

		if(screen == PLACE){
			draw_placing();
		}
		else if(screen == PLAY || screen == RESULT){
			draw_playing();
		}
		else if(screen == PASS){
			DrawText(TextFormat("Pass the keyboard to player %d", turn+1), 160, 280, 36, DARKGRAY);
			DrawText("Click to continue", 160, 340, 24, GRAY);
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) screen = after_pass;
		}
		else{
			DrawText(TextFormat("Player %d wins!", turn+1), 160, 250, 48, DARKGREEN);
			DrawText(message, 160, 320, 20, GRAY);
			DrawText("Click to play again", 160, 360, 24, GRAY);
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) new_match();
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
