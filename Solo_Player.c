#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Multiplayer.c"

/* functions declrations */
void Solo_Player(void);
void Solo_Player_menu(void);
void Start_solo_player_game(bool);
User *Computer_user(void);
void Computer_turn(void);

/* functions definitions */
void Solo_Player() {
	system("CLS");
	Solo_Player_menu();
	Start_solo_player_game(1);
}

void Solo_Player_menu() {
	system("CLS");
	Player1_User = Choose_user("Player");
	Player1_Ships = Ships_placement("Player");
	Player2_User = Computer_user();
	Player2_Ships = Ships_auto_placement(1);
	Player1_Map = Map_init();
	Player2_Map = Map_init();
	Add_ships_to_map(Player1_Map -> known_map, Player1_Ships);
	Add_ships_to_map(Player2_Map -> known_map, Player2_Ships);
}

void Start_solo_player_game(bool new_game) {
	Start_init(new_game);
	current_game -> mode = 1;
	Save_Last(current_game);
	int winner_player = 2;
	while (Player1_Ships -> head -> nxt != Player1_Ships -> head && Player2_Ships -> head -> nxt != Player2_Ships -> head) {
		if (current_game -> turn == 1) {
			Player1_turn();
			system("CLS");
			Map_output(Player2_Map -> unknown_map, map_row, map_column);
		}
		else {
			Computer_turn();
			system("CLS");
			Map_output(Player1_Map -> unknown_map, map_row, map_column);
			output_color_text(red, "The computer has done its move\n");
		}
		Save_Last(current_game);
		printf("\nPress any key to continue.");
		getch();
	}
	if (Player2_Ships -> head -> nxt == Player2_Ships -> head)
		winner_player = 1;
	
	system("CLS");
	if (winner_player == 1)
		output_color_text(green, "Congratulations, you have won the game =D");
	else
		output_color_text(red, "Better luck next time =(");
	printf("\nPress any key to continue.");
	getch();
}

User *Computer_user() {
	User *result = (User *)malloc(sizeof(User));
	strcpy(result -> name, "Computer");
	result -> point = 0;
	return result;
}

void Computer_turn() {
	for (int i = 0; i < map_row; i++)
		for (int j = 0; j < map_column; j++)
			if (Player1_Map -> unknown_map[i][j] == 'X')
				for (int dif_x = -1; dif_x < 2; dif_x++)
					for (int dif_y = -1; dif_y < 2; dif_y++) {
						int x = i + dif_x, y = j + dif_y;
						if (is_valid(x, y, map_row, map_column) && !(x == i && y == j) && Player1_Map -> unknown_map[x][y] == 'X') {
							x = i - dif_x, y = j - dif_y;
							if (is_valid(x, y, map_row, map_column) && Player1_Map -> unknown_map[x][y] == ' ') {
								Player2_shoot(x, y);
								return;
							}
						}
					}
	
	for (int i = 0; i < map_row; i++)
		for (int j = 0; j < map_column; j++)
			if (Player1_Map -> unknown_map[i][j] == ' ' && (i + j) % 2) {
				Player2_shoot(i, j);
				return;
			}
	
	for (int i = 0; i < map_row; i++)
		for (int j = 0; j < map_column; j++)
			if (Player1_Map -> unknown_map[i][j] == ' ') {
				Player2_shoot(i, j);
				return;
			}
	error_exit("Computer cannot fire any cell");
}
