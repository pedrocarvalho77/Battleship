#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

static void read_line(char* buf, int size){

	if(fgets(buf, size, stdin) == NULL){
		printf("\nEnd of input, leaving the game\n");
		exit(1);
	}
}

int read_int(){

	char line[64];
	char* end;
	long n;

	read_line(line, sizeof(line));
	n = strtol(line, &end, 10);
	// accept only a number followed by optional spaces and the newline
	if(end == line || n < 0 || n > 100000) return -1;
	while(*end == ' ' || *end == '\t') end++;
	if(*end != '\n' && *end != '\0') return -1;
	return (int) n;
}

char read_char(){

	char line[64];

	read_line(line, sizeof(line));
	return line[0] == '\n' ? '\0' : line[0];
}

void clear_screen(){
	printf("\033[2J\033[H");
	fflush(stdout);
}

void wait_enter(char* msg){

	char line[64];

	printf("%s", msg);
	fflush(stdout);
	read_line(line, sizeof(line));
}
