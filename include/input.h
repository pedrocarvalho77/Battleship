#ifndef INPUT_H
#define INPUT_H

// reads one line and returns it as an int, or -1 if it is not a plain number; exits on end of input
int read_int();

// reads one line and returns its first character ('\0' for an empty line); exits on end of input
char read_char();

#endif
