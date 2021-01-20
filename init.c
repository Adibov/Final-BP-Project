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

}


void check_files() {
	DIR* dir = opendir(".\\Files");
	if (dir)  /* Directory exists. */
		closedir(dir);
	else if (ENOENT == errno) /* Directory does not exist. */
		system("mkdir .\\Files");
	else {
		/* opendir() failed for some other reason. */
		system("CLS");
		printf("Some error occurred :(");
		#ifdef DEBUG
			printf("opendir() failed for some other reason");
		#endif
		exit(-1);
	}
}

void check_binary_files() {
	if (access("Files\\Users.bin", F_OK)) // file doesn't exists
		system("touch Files\\Users.bin");
}