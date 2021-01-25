#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>
#include "Game.c"

#define DEBUG

/* Global variables */
enum color_array {light_blue = 1, light_green, light_cyan, light_red, light_pink, light_yellow, light_white, gray, blue, green, cyan, red, pink, yellow, white};

/* Typedefs */
typedef enum color_array color_array;

/* functions declrations */
void error_exit(char *);
void invalid_input(void);
void terminal_color(enum color_array);
void output_color_text(enum color_array, char *);
void Map_output(char (*)[100], int, int);
int string_to_int(char *);

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

void output_color_text(enum color_array color, char *message) {
	terminal_color(color);
	printf("%s", message);
	terminal_color(white);
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
			else if (Tmp_map[i][j] == 'W')
				terminal_color(light_blue);
			// else if (Tmp_map[i][j] == 'E')
			// 	terminal_color(red);
			else if (Tmp_map[i][j] == 'D')
				terminal_color(light_red);
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

int string_to_int(char *string) {
	int res = 0;
	for (int i = 0; i < strlen(string); i++)
		res = 10 * res + (int)(string[i] - '0');
	return res;
}