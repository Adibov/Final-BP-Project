#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <unistd.h>
#include "General.c"

/* functions declrations */
void Load(void);
void Load_Last(void);
void Save_game(Game *);

/* functions definitions */
void Load() {
	printf("load");
}

void Load_Last() {
	printf("load last");
}

void Save_game(Game *current_game) {
	system("CLS");
	if (!access("Files\\Tmp.bin", F_OK))
		system("del Files\\Tmp.bin");
	system("touch Files\\Tmp.bin");

	bool finded = 0;
	int indx = 0;
	FILE *Loads = fopen("Files\\Loads.bin", "rb"), *Tmp;
	if (Loads == NULL)
		error_exit("Cannot fopen Loads.bin");
		
	while (1) {
		Game *input = (Game *)malloc(sizeof(Game));
		if (fread(input, sizeof(Game), 1, Loads) < 1)
			break;
		
		printf("hey: %d %d\n", input -> starting_time, current_game -> starting_time);
		if (input -> starting_time == current_game -> starting_time) {
			finded = 1;
			break;
		}
		indx++;
	}

	if (!finded) {
		system("del Files\\Tmp.bin");
		fseek(Loads, 0, SEEK_END);
		fclose(Loads);

		Loads = fopen("Files\\Loads.bin", "ab");
		if (Loads == NULL)
			error_exit("Cannot fopen Loads.bin");

		fwrite(current_game, sizeof(Game), 1, Loads);
		output_color_text(yellow, "Game saved\n");
		printf("Press any key to continue.");
		getch();
		fclose(Loads);
		return;
	}

	Tmp = fopen("Files\\Tmp.bin", "wb");
	fseek(Loads, 0, SEEK_SET);
	while (1) {
		Game *input;
		if (fread(input, sizeof(Game), 1, Loads) < 1)
			break;
		
		if (input -> starting_time == current_game -> starting_time)
			continue;
		fwrite(input, sizeof(Game), 1, Tmp);
	}
	fclose(Loads);
	fclose(Tmp);
	system("del Files\\Loads.bin");
	system("touch Files\\Loads.bin");

	int second_indx = 0;
	Loads = fopen("Files\\Loads.bin", "wb");
	Tmp = fopen("Files\\Tmp.bin", "rb");
	fseek(Tmp, 0, SEEK_SET);
	while (1) {
		if (indx == second_indx) {
			fwrite(current_game, sizeof(Game), 1, Loads);
			second_indx++;
			continue;
		}

		Game *input;
		if (fread(input, sizeof(Game), 1, Tmp) < 1)
			break;
		fwrite(input, sizeof(Game), 1, Loads);
		second_indx++;
	}
	fclose(Loads);
	fclose(Tmp);
	system("del Files\\Tmp.bin");
	output_color_text(yellow, "Game saved\n");
	printf("Press any key to continue.");
	getch();	
}