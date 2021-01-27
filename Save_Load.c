#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include "Setting.c"

/* functions declrations */
Game *Load(void);
Game *Load_Last(void);
Game *Read_Game_from_file(char *, int);
void Write_Game_to_file(Game *, char *, char *);
void Save_game(Game *);
void Save_Last(Game *);

/* functions definitions */
Game *Load() {
	system("CLS");
	if (Read_Game_from_file("Files\\Loads.bin", 1) == NULL) {
		printf("There isn't any saved game yet\nPress any key to continue.");
		getch();
		return NULL;
	}

	int indx = 1, indx_input;
	while (1) {
		Game *input = Read_Game_from_file("Files\\Loads.bin", indx);
		if (input == NULL)
			break;
		terminal_color(yellow);
		printf("%d) ", indx);
		terminal_color(white);
		if (input -> mode == 1) {
			terminal_color(cyan);
			printf("%s", input -> Player1_User -> name);
			terminal_color(white);
			printf("'s Solo Player game, started in ");
		}
		else {
			terminal_color(cyan);
			printf("%s ", input -> Player1_User -> name);
			terminal_color(white);
			printf("and ");
			terminal_color(cyan);
			printf("%s", input -> Player2_User -> name);
			terminal_color(white);
			printf("'s Multiplayer game, started in ");
		}

		time_t rawtime = input -> starting_time;
		struct tm * timeinfo;
		timeinfo = localtime ( &rawtime );
		terminal_color(cyan);
		printf("%s\n", asctime(timeinfo));
		terminal_color(white);
		indx++;
	}
	output_color_text(light_red, "\nEnter the id of the saved game that you want to choose (or enter 0 to bring up main menu): ");
	scanf("%d", &indx_input);

	if (indx_input < 0 || indx_input >= indx) {
		invalid_input();
		return Load();
	}
	if (!indx_input)
		return NULL;
	return Read_Game_from_file("Files\\Loads.bin", indx_input);
}

Game *Load_Last() {
	if (access("Files\\Last_Save.bin", F_OK)) {
		system("CLS");
		printf("There isn't any save yet\nPress any key to continue.");
		getch();
		return NULL;
	}
	Game *result = Read_Game_from_file("Files\\Last_Save.bin", 1);
	map_row = result -> map_row;
	map_column = result -> map_column;
	return result;
}

Game *Read_Game_from_file(char *path, int indx) {
	Game *result = (Game *)malloc(sizeof(Game));
	FILE *Last_Save_file = fopen(path, "rb");
	if (Last_Save_file == NULL)
		error_exit("Cannot open file with the given path");
	
	while (indx--) {
		if (fread(result, sizeof(Game), 1, Last_Save_file) < 1) {
			fclose(Last_Save_file);
			return NULL;
		}

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
	}
	fclose(Last_Save_file);
	return result;
}

void Write_Game_to_file(Game *current_game, char *path, char *type) {
	if (access(path, F_OK))
		return;
	
	FILE *Last_Save_file = fopen(path, type);
	if (Last_Save_file == NULL)
		error_exit("Cannot open file with the given path");

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

void Save_game(Game *current_game) {
	system("CLS");
	if (!access("Files\\Tmp.bin", F_OK))
		system("del Files\\Tmp.bin");
	system("touch Files\\Tmp.bin");

	bool finded = 0;
	int indx = 1, current_game_indx = -1;
	while (1) {
		Game *input = Read_Game_from_file("Files\\Loads.bin", indx);
		if (input == NULL)
			break;
		
		if (input -> starting_time == current_game -> starting_time) {
			finded = 1;
			current_game_indx = indx;
			break;
		}
		indx++;
	}

	if (!finded) {
		system("del Files\\Tmp.bin");
		Write_Game_to_file(current_game, "Files\\Loads.bin", "ab");
		output_color_text(yellow, "Game saved\n");
		printf("Press any key to continue.");
		getch();
		return;
	}

	indx = 1;
	while (1) {
		Game *input = Read_Game_from_file("Files\\Loads.bin", indx);
		if (input == NULL)
			break;
		
		if (input -> starting_time == current_game -> starting_time) {
			indx++;
			continue;
		}
		Write_Game_to_file(input, "Files\\Tmp.bin", "ab");
		indx++;
	}
	system("del Files\\Loads.bin");
	system("touch Files\\Loads.bin");

	indx = 1;
	while (1) {
		if (indx == current_game_indx) {
			Write_Game_to_file(current_game, "Files\\Loads.bin", "ab");
			current_game_indx = -1;
			continue;
		}

		Game *input = Read_Game_from_file("Files\\Tmp.bin", indx);
		if (input == NULL)
			break;
		Write_Game_to_file(input, "Files\\Loads.bin", "ab");
		indx++;
	}
	system("del Files\\Tmp.bin");
	output_color_text(yellow, "Game saved\n");
	printf("Press any key to continue.");
	getch();	
}

void Save_Last(Game *current_game) {
	if (!access("Files\\Last_Save.bin", F_OK))
		system("del Files\\Last_Save.bin");
	system("touch Files\\Last_Save.bin");
	Write_Game_to_file(current_game, "Files\\Last_Save.bin", "wb");
}
