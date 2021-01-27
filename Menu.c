#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Solo_Player.c"

/* functions declrations */
void turorial(void);
void menu(void);
void Score_Board(void);
void Exit(void);

/* functions definitions */
void tutorial() {
	system("CLS");
	output_color_text(light_pink,
"\
Hello and Welcome to Battle-Ship game.\n\
In This game your target is to destroy all of enemy's ships. In every turn you can choose a arbitrary empty cell and fire in it, If it hits to a ship, you can fire again. You will be friendly with the game after some time.\n\n\
In this game we will show cells like this: \n");

	char *table[7][2] = {{"Character", "Description"}, {"W", "Water cell"}, {"(nothing\\empty cell)", "Unknown cell"}, {"D", "Destroyed ship"}, {"X", "Exploded cell"}, {"E", "Empty cell"}, {"S", "Ship"}};
	int table_column[2] = {23, 16};
	printf("+");
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < table_column[k]; i++)
			printf("-");
		printf("+");
	}
	printf("\n");
	for (int i = 0; i < 7; i++) {
		printf("|");
		for (int j = 0; j < 2; j++) {
			int space = (table_column[j] - strlen(table[i][j])) / 2;
			for (int k = 0; k < space; k++)
				printf(" ");
			
			if (i == 1)
				terminal_color(light_blue);
			else if (i == 2)
				terminal_color(light_cyan);	
			else if (i == 3)
				terminal_color(light_red);
			else if (i == 4)
				terminal_color(red);
			else if (i == 5)
				terminal_color(gray);
			else if (i == 6)
				terminal_color(yellow);
			printf("%s", table[i][j]);
			terminal_color(white);

			space = (table_column[j] - strlen(table[i][j]) + 1) / 2;
			for (int k = 0; k < space; k++)
				printf(" ");
			printf("|");
		}
		printf("\n+");
		for (int k = 0; k < 2; k++) {
			for (int j = 0; j < table_column[k]; j++) 
				printf("-");
			printf("+");
		}
		printf("\n");
	}
	printf("Press any key to continue\n");
	getch();
	system("CLS");
}

void main_menu() {
	system("CLS");
	char *input_list[] = {"Play with a Friend", "Play with Computer", "Load a saved game", "Load last game", "Settings", "Score Board", "Exit"};
	for (int i = 0; i < 7; i++) {
		terminal_color(yellow);
		printf("%d) ", i + 1);
		terminal_color(white);
		printf("%s\n\n", input_list[i]);
	}

	char input[100];
	int option;
	output_color_text(light_red, "Choose an option: ");
	scanf(" %s", &input);
	if (strlen(input) > 1 || strlen(input) < 1 || input[0] < '1' || '7' < input[0]) {
		invalid_input();
		main_menu();
		return;
	}
	option = (int)(input[0] - '0');
	system("CLS");
	if (option == 1)
		Multiplayer();
	else if (option == 2)
		Solo_Player();
	else if (option == 3) {
		current_game = Load();
		if (current_game == NULL) 
			main_menu();
		else if (current_game -> mode == 1)
			Start_solo_player_game(0);
		else
			Start_multiplayer_game(0);
	}
	else if (option == 4) {
		current_game = Load_Last();
		if (current_game == NULL)
			main_menu();
		else {
			if (current_game -> mode == 1)
				Start_solo_player_game(0);
			else
				Start_multiplayer_game(0);
		}
	}
	else if (option == 5) {
		Setting();
		main_menu();
	}
	else if (option == 6)
		Score_Board();
	else if (option == 7)
		Exit();
	else {
		invalid_input();
		main_menu();
	}
}

void Score_Board() {
	system("CLS");
	char username[100][60];
	int user_points[100], indx = 0;
	FILE *user_file = fopen("Files\\Users.bin", "rb+");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");
	
	User *tmp_user = (User *)malloc(sizeof(User));
	while (1) {
		if (fread(tmp_user, sizeof(User), 1, user_file) < 1)
			break;
		
		strcpy(username[indx], tmp_user -> name);
		user_points[indx] = tmp_user -> point;
		int tmp_indx = indx;
		while (tmp_indx > 0 && user_points[tmp_indx - 1] < user_points[tmp_indx]) {
			char tmp_username[100];
			strcpy(tmp_username, username[tmp_indx]);
			strcpy(username[tmp_indx], username[tmp_indx - 1]);
			strcpy(username[tmp_indx - 1], tmp_username);
			
			int tmp_point = user_points[tmp_indx];
			user_points[tmp_indx] = user_points[tmp_indx - 1];
			user_points[tmp_indx - 1] = tmp_point;
			tmp_indx--;
		}
		indx++;
	}
	fclose(user_file);
	
	printf("hey: %d\n", indx);
	output_color_text(light_cyan, "    Username           Points\n\n");
	for (int i = 0; i < indx; i++) {
		terminal_color(yellow);
		printf("%d) ", i + 1);
		terminal_color(cyan);
		printf("%-22.22s%d\n\n", username[i], user_points[i]);
		terminal_color(white);
	}
	printf("Press any key to continue.");
	getch();
}

void Exit() {
	system("CLS");
	output_color_text(cyan, "Good luck, Have fun :D");
	exit(0);
}

