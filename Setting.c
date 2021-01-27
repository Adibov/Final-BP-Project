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
void check_setting_files(void);
int get_largest_ship_length(void);

/* functions definitions */
void Setting() {
	printf("setting");
}

void setting_init() {
	check_setting_files();
	FILE *settings = fopen("Files\\Settings.bin", "rb");
	if (settings == NULL)
		error_exit("Cannot open Settings.bin to read");
	
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
}

void check_setting_files() {
	if (access("Files\\Settings.bin", F_OK)) {
		system("touch Files\\Settings.bin");
		FILE *settings = fopen("Files\\Settings.bin", "wb");
		if (settings == NULL)
			error_exit("Cannot open Settings.bin to write");
			
		map_row = map_column = 10;
		fwrite(&map_row, sizeof(int), 1, settings);
		fwrite(&map_column, sizeof(int), 1, settings);

		int initial_ships[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 5}, num = 10;
		fwrite(&num, sizeof(int), 1, settings);
		fwrite(initial_ships, sizeof(int), num, settings);
		fclose(settings);
	}
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