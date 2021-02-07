#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "General.c"

/* Global variables */
int map_row, map_column;
Linked_List *Ships;

/* functions declrations */
void Setting(void);
void setting_init(void);
void Ships_setting(void);
void Map_setting(void);
void Theme_setting(void);
void Play_Back(void);
int get_largest_ship_length(void);

/* functions definitions */
void Setting() {
	system("CLS");
	char *input_list[] = {"Ships", "Map size", "Theme", "Main Menu"};
	for (int i = 0; i < 4; i++) {
		terminal_color(yellow);
		printf("%d) ", i + 1);
		terminal_color(white);
		if (i == 2) {
			output_color_text(pink, "T");
			output_color_text(blue, "h");
			output_color_text(green, "e");
			output_color_text(yellow, "m");
			output_color_text(red, "e\n\n");
			continue;
		}
		printf("%s\n\n", input_list[i]);
	}
	
	char input[100];
	int option;
	output_color_text(light_red, "Choose an option: ");
	scanf(" %s", &input);
	if (input[0] < '1' || '4' < input[0]) {
		invalid_input();
		Setting();
		return;
	}
	option = (int)(input[0] - '0');
	system("CLS");
	if (option == 1)
		Ships_setting();
	else if (option == 2)
		Map_setting();
	else if (option == 3)
		Theme_setting();
}

void setting_init() {
	FILE *settings = fopen("Files\\Settings.bin", "rb");
	if (settings == NULL)
		error_exit("Cannot open Settings.bin to read");
	
	while (Ships != NULL && Ships -> head -> nxt != Ships -> head) {
		Ships -> cur = Ships -> head -> nxt;
		Linked_List_del(Ships);
	}
	free(Ships);

	fread(&map_row, sizeof(int), 1, settings);
	fread(&map_column, sizeof(int), 1, settings);
	Ships = Linked_List_init();

	int num;
	fread(&num, sizeof(int), 1, settings);
	for (int i = 0; i < num; i++) {
		int current_length;
		fread(&current_length, sizeof(int), 1, settings);
		Ship *ship = (Ship *)malloc(sizeof(Ship));
		ship -> length = current_length;
		Linked_List_add(Ships, ship);
	}
	fclose(settings);

	FILE *theme = fopen("Files\\Theme.bin", "r");
	for (int i = 0; i < 15; i++) 
		fread(&color_map[i], sizeof(int), 1, theme);
	fclose(theme);
}

void Ships_setting() {
	system("CLS");
	int ships_length[100], num;
	printf("Enter the number of ships that you want to add: (number of ships must be between 1 and 20)\n");
	scanf("%d", &num);
	if (num < 1 || num > 20) {
		invalid_input();
		Ships_setting();
		return;
	}

	printf("Now enter %d ships length: (ships length should be between 1 and 10, and should be strictly smaller than map size)\n", num);
	for (int i = 0; i < num; i++) {
		scanf("%d", &ships_length[i]);
		if (ships_length[i] < 1 || ships_length[i] > 10 || ships_length[i] >= map_row || ships_length[i] >= map_column) {
			invalid_input();
			Ships_setting();
			return;
		}
	}

	char tmp_map[100][100];
	for (int i = 0; i < map_row; i++)
		for (int j = 0; j < map_column; j++)
			tmp_map[i][j] = 'E';
	
	bool check = 1;
	for (int indx = 0; indx < num; indx++) {
		int len = ships_length[indx];
		bool placed = 0;
		for (int i = 0; i < map_row; i++)
			for (int j = 0; j < map_column && !placed; j++)
				if (check_placement(tmp_map, i, j, len, horizontal, map_row, map_column))
					for (int k = 0; k < len; k++) {
						int x = i + k * dx[horizontal], y = j + k * dy[horizontal];
						tmp_map[x][y] = 'S';
						placed = 1;
					}
		
		for (int i = 0; i < map_row; i++)
			for (int j = 0; j < map_column && !placed; j++)
				if (check_placement(tmp_map, i, j, len, vertical, map_row, map_column))
					for (int k = 0; k < len; k++) {
						int x = i + k * dx[vertical], y = j + k * dy[vertical];
						tmp_map[x][y] = 'S';
						placed = 1;
					}
		check &= placed;
	}
	if (!check) {
		system("CLS");
		output_color_text(red, "Cannot place ships in map\n\n");
		printf("Press any key to continue.");
		getch();
		return;
	}

	if (access("Files\\Settings.bin", F_OK))
		error_exit("Setting doesn't exist");
	FILE *settings = fopen("Files\\Settings.bin", "wb");
	if (settings == NULL)
		error_exit("Cannot open Settings.bin to write");

	fwrite(&map_row, sizeof(int), 1, settings);
	fwrite(&map_column, sizeof(int), 1, settings);
	fwrite(&num, sizeof(int), 1, settings);
	fwrite(ships_length, sizeof(int), num, settings);
	fclose(settings);
	setting_init();
}

void Map_setting() {
	system("CLS");
	int ships_length[100], row_num, col_num;
	printf("Enter the number of row and column: (number of rows and columns must be between 2 and 15)\n");
	scanf("%d %d", &row_num, &col_num);
	if (row_num < 2 || col_num > 15 || col_num < 2 || col_num > 15) {
		invalid_input();
		Map_setting();
		return;
	}

	int tmp_row = map_row, tmp_column = map_column;
	map_row = row_num, map_column = col_num;
	char tmp_map[100][100];
	for (int i = 0; i < map_row; i++)
		for (int j = 0; j < map_column; j++)
			tmp_map[i][j] = 'E';
	
	bool check = 1;
	int num = 0;
	Ships -> cur = Ships -> head -> nxt;
	while (Ships -> cur != Ships -> head) {
		Ship *tmp_ship = (Ship *)Ships -> cur -> value;
		int len = tmp_ship -> length;
		bool placed = 0;
		for (int i = 0; i < map_row; i++)
			for (int j = 0; j < map_column && !placed; j++)
				if (check_placement(tmp_map, i, j, len, horizontal, map_row, map_column))
					for (int k = 0; k < len; k++) {
						int x = i + k * dx[horizontal], y = j + k * dy[horizontal];
						tmp_map[x][y] = 'S';
						placed = 1;
					}
		
		for (int i = 0; i < map_row; i++)
			for (int j = 0; j < map_column && !placed; j++)
				if (check_placement(tmp_map, i, j, len, vertical, map_row, map_column))
					for (int k = 0; k < len; k++) {
						int x = i + k * dx[vertical], y = j + k * dy[vertical];
						tmp_map[x][y] = 'S';
						placed = 1;
					}
		check &= placed;
		ships_length[num] = len;
		num++;
		Ships -> cur = Ships -> cur -> nxt;
	}
	if (!check) {
		map_row = tmp_row, map_column = tmp_column;
		system("CLS");
		output_color_text(red, "Cannot place ships in map\n\n");
		printf("Press any key to continue.");
		getch();
		return;
	}

	if (access("Files\\Settings.bin", F_OK))
		error_exit("Setting doesn't exist");
	FILE *settings = fopen("Files\\Settings.bin", "wb");
	if (settings == NULL)
		error_exit("Cannot open Settings.bin to write");

	fwrite(&map_row, sizeof(int), 1, settings);
	fwrite(&map_column, sizeof(int), 1, settings);
	fwrite(&num, sizeof(int), 1, settings);
	fwrite(ships_length, sizeof(int), num, settings);
	fclose(settings);
	setting_init();
}

void Theme_setting() {
	system("CLS");
	for (int i = 0; i < 15; i++)
		color_map[i] = i + 1;

	// W D X E S
	output_color_text(light_red, "Which set of color do you want to pick: \n\n");
	output_color_text(yellow, "1) ");
	output_color_text(light_blue, "W ");
	output_color_text(light_red, "D ");
	output_color_text(red, "X ");
	output_color_text(gray, "E ");
	output_color_text(yellow, "S\n\n");

	output_color_text(yellow, "2) ");
	output_color_text(blue, "W ");
	output_color_text(red, "D ");
	output_color_text(light_red, "X ");
	output_color_text(light_white, "E ");
	output_color_text(light_yellow, "S\n\n");

	output_color_text(yellow, "3) ");
	output_color_text(cyan, "W ");
	output_color_text(red, "D ");
	output_color_text(green, "X ");
	output_color_text(light_white, "E ");
	output_color_text(yellow, "S\n\n");

	int option;
	scanf(" %d", &option);
	if (option < 1 || option > 3) {
		invalid_input();
		Theme_setting();
		return;
	}

	if (option == 2) {
		color_map[0] = 9;
		color_map[8] = 1;
		color_map[3] = 12;
		color_map[11] = 4;
		color_map[7] = 7;
		color_map[6] = 8;
		color_map[5] = 14;
		color_map[13] = 6;
	}
	else if (option == 3) {
		color_map[0] = 11;
		color_map[10] = 1;
		color_map[3] = 12;
		color_map[11] = 4;
		color_map[11] = 10;
		color_map[9] = 12;
		color_map[7] = 7;
		color_map[6] = 8;
	}

	FILE *theme = fopen("Files\\Theme.bin", "w");
	for (int i = 0; i < 15; i++) {
		int x = color_map[i];
		fwrite(&x, sizeof(int), 1, theme);
	}
	fclose(theme);
}

void Play_Back() {
	
}

int get_largest_ship_length() {
	int res = 0;
	Ships -> cur = Ships -> head -> nxt;
	while (Ships -> cur != Ships -> head) {
		Ship *tmp = (Ship *)Ships -> cur -> value;
		if (tmp -> length > res)
			res = tmp -> length;
		Ships -> cur = Ships -> cur -> nxt;
	}
	return res;
}