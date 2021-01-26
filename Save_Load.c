#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <unistd.h>
#include "Setting.c"

/* functions declrations */
Game *Load(void);
Game *Load_Last(void);
void Save_game(Game *);
void Save_Last(Game *);

/* functions definitions */
Game *Load() {
	printf("load");
}

Game *Load_Last() {
	if (access("Files\\Last_Save.bin", F_OK)) {
		system("CLS");
		printf("There isn't any save yet\nPress any key to continue.");
		getch();
		return NULL;
	}
	Game *result = (Game *)malloc(sizeof(Game));
	FILE *Last_Save_file = fopen("Files\\Last_Save.bin", "rb");
	if (Last_Save_file == NULL)
		error_exit("Cannot open Last_Save.bin");
	fread(result, sizeof(Game), 1, Last_Save_file);
	result -> Player1_User = (User *)malloc(sizeof(User));
	result -> Player2_User = (User *)malloc(sizeof(User));
	result -> Player1_Map = (Map *)malloc(sizeof(Map));
	result -> Player2_Map = (Map *)malloc(sizeof(Map));
	result -> Player1_Ships = Linked_List_init();
	result -> Player2_Ships = Linked_List_init();
	fread(result -> Player1_User -> name, sizeof(char), user_name_length, Last_Save_file);
	fread(result -> Player2_User -> name, sizeof(char), user_name_length, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fread(result -> Player1_Map -> known_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fread(result -> Player1_Map -> unknown_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fread(result -> Player2_Map -> known_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fread(result -> Player2_Map -> unknown_map[i], sizeof(char), map_max_size, Last_Save_file);
	
	int sz1, sz2;
	fread(&sz1, sizeof(int), 1, Last_Save_file);
	while (sz1--) {
		Ship *tmp_ship = (Ship *)malloc(sizeof(Ship));
		fread(tmp_ship, sizeof(Ship), 1, Last_Save_file);
		Linked_List_add(result -> Player1_Ships, tmp_ship);
	}

	fread(&sz2, sizeof(int), 1, Last_Save_file);
	while (sz2--) {
		Ship *tmp_ship = (Ship *)malloc(sizeof(Ship));
		fread(tmp_ship, sizeof(Ship), 1, Last_Save_file);
		Linked_List_add(result -> Player2_Ships, tmp_ship);
	}
	fclose(Last_Save_file);

	
	map_row = result -> map_row;
	map_column = result -> map_column;
	return result;
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

void Save_Last(Game *current_game) {
	if (!access("Files\\Last_Save.bin", F_OK))
		system("del Files\\Last_Save.bin");
	system("touch Files\\Last_Save.bin");

	FILE *Last_Save_file = fopen("Files\\Last_Save.bin", "wb");
	if (Last_Save_file == NULL)
		error_exit("Cannot open Last_Save.bin");

	fwrite(current_game, sizeof(Game), 1, Last_Save_file);
	fwrite(current_game -> Player1_User -> name, sizeof(char), user_name_length, Last_Save_file);
	fwrite(current_game -> Player2_User -> name, sizeof(char), user_name_length, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fwrite(current_game -> Player1_Map -> known_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fwrite(current_game -> Player1_Map -> unknown_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fwrite(current_game -> Player2_Map -> known_map[i], sizeof(char), map_max_size, Last_Save_file);
	for (int i = 0; i < map_max_size; i++)
		fwrite(current_game -> Player2_Map -> unknown_map[i], sizeof(char), map_max_size, Last_Save_file);
	
	Linked_List *Player1_Ships = current_game -> Player1_Ships, *Player2_Ships = current_game -> Player2_Ships;
	int sz1 = Linked_List_size(Player1_Ships), sz2 = Linked_List_size(Player2_Ships);
	fwrite(&sz1, sizeof(int), 1, Last_Save_file);
	Player1_Ships -> cur = Player1_Ships -> head -> nxt;
	while (Player1_Ships -> cur != Player1_Ships -> head) {
		fwrite((Ship *)Player1_Ships -> cur -> value, sizeof(Ship), 1, Last_Save_file);
		Player1_Ships -> cur = Player1_Ships -> cur -> nxt;
	}

	fwrite(&sz2, sizeof(int), 1, Last_Save_file);
	Player2_Ships -> cur = Player2_Ships -> head -> nxt;
	while (Player2_Ships -> cur != Player2_Ships -> head) {
		fwrite((Ship *)Player2_Ships -> cur -> value, sizeof(Ship), 1, Last_Save_file);
		Player2_Ships -> cur = Player2_Ships -> cur -> nxt;
	}
	fclose(Last_Save_file);
}
