#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "Menu.c"

/* functions declrations */
void init(void);
void check_files(void);
void check_binary_files(void);

/* functions definitions */
void init() {
	terminal_color(white);
	check_files();
	check_binary_files();
	setting_init();
}


void check_files() {
	DIR* dir = opendir(".\\Files");
	if (dir)  /* Directory exists. */
		closedir(dir);
	else if (ENOENT == errno) /* Directory does not exist. */
		system("mkdir .\\Files");
	else /* opendir() failed for some other reason. */
		error_exit("opendir() failed for some other reason");
}

void check_binary_files() {
	if (access("Files\\Users.bin", F_OK)) // file doesn't exists
		system("type nul > Files\\Users.bin");
	if (access("Files\\Loads.bin", F_OK))
		system("type nul > Files\\Loads.bin");
	if (access("Files\\Theme.bin", F_OK)) {
		system("type nul > Files\\Theme.bin");
		FILE *theme = fopen("Files\\Theme.bin", "w");
		for (int i = 0; i < 15; i++) {
			int x = i + 1;
			fwrite(&x, sizeof(int), 1, theme);
		}
		fclose(theme);
	}
	if (access("Files\\Settings.bin", F_OK)) {
		system("type nul > Files\\Settings.bin");
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