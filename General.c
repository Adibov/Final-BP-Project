#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>

#define DEBUG

/* Global variables */
enum col_array {gray = 8, blue, green, cyan, red, pink, yellow, white};

/* functions declrations */
void error_exit(char *);
void invalid_input(void);
void terminal_color(enum col_array);

/* functions definitions */
void error_exit(char *message) {
	printf("Some error occurred :(\n");
	#ifdef DEBUG
		printf("%s", message);
	#endif
	exit(-1);
}

void invalid_input() {
	system("CLS");
	printf("Invalid input, Press any key to continue.");
	getch();
}


void terminal_color(enum col_array color) {
	HANDLE Console;
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(Console, color);
/*
8: Gray
9: Blue
10: Green
11: Cyan
12: Red
13: Pink
14: Yellow
15: White
*/
}