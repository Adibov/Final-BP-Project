#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>
#include "User.c"

#define DEBUG

/* Global variables */
enum color_array {gray = 8, blue, green, cyan, red, pink, yellow, white};

/* Typedefs */
typedef enum color_array color_array;

/* functions declrations */
void error_exit(char *);
void invalid_input(void);
void terminal_color(enum color_array);
//void Map_output(char, int, int);

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


void terminal_color(enum color_array color) {
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

void Map_output(char Tmp_map[100][100], int n, int m) {
	printf("   ");
	for (int j = 0; j < m; j++)
		printf("  %d ", j + 1);
	printf("\n   ");

	for (int j = 0; j < m; j++)
		printf("+---");
	printf("+\n");
	for (int i = 0; i < n; i++) {
		printf("%-3d", i + 1);
		for (int j = 0; j < m; j++) {
			printf("| ");
			if (Tmp_map[i][j] == 'S')
				terminal_color(yellow);
			else
				terminal_color(gray);
			printf("%c ", Tmp_map[i][j]);
			terminal_color(white);
		}
		printf("|\n   ");

		for (int j = 0; j < m; j++)
		printf("+---");
		printf("+\n");
	}
}