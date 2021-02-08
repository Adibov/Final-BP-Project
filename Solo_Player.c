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
	Save_Last(current_game);
	current_game -> mode = 1;
	
	int winner_player = 2;
	int player1_last_score = current_game -> player1_point;
	while (Player1_Ships -> head -> nxt != Player1_Ships -> head && Player2_Ships -> head -> nxt != Player2_Ships -> head) {
		if (current_game -> turn == 1) {
			Player1_turn();
			system("CLS");
			Map_output(Player2_Map -> unknown_map, map_row, map_column);
			FILE *play_back_file = fopen("Files\\Playback.bin", "ab");
			for (int i = 0; i < map_max_size; i++)
				fwrite(Player2_Map -> unknown_map[i], sizeof(char), map_max_size, play_back_file);
			fclose(play_back_file);
		}
		else {
			Computer_turn();
			system("CLS");
			Map_output(Player1_Map -> unknown_map, map_row, map_column);
			output_color_text(red, "\nThe computer has done its move\n");
			FILE *play_back_file = fopen("Files\\Playback.bin", "ab");
			for (int i = 0; i < map_max_size; i++)
				fwrite(Player1_Map -> unknown_map[i], sizeof(char), map_max_size, play_back_file);
			fclose(play_back_file);
		}
		Save_Last(current_game);
		Add_points(Player1_User -> name, current_game -> player1_point - player1_last_score);
		player1_last_score = current_game -> player1_point;
		printf("\nPress any key to continue.");
		getch();
	}
	if (Player2_Ships -> head -> nxt == Player2_Ships -> head)
		winner_player = 1;
	
	system("CLS");
	color_reset();
	if (winner_player == 1) {
		// Add_points(Player1_User -> name, current_game -> player1_point);
		output_color_text(green, "Congratulations, you have won the game =D");
	}
	else {
		// Add_points(Player1_User -> name, (current_game -> player1_point) / 2);
		output_color_text(red, "Better luck next time =(");
	}
	printf("\n\nPress any key to continue.");
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
			if (Player1_Map -> unknown_map[i][j] == 'X')
				for (int dif_y = -1; dif_y < 2; dif_y++)
					for (int dif_x = -1; dif_x < 2; dif_x++) {
						int x = i + dif_x, y = j + dif_y;
						if (is_valid(x, y, map_row, map_column) && !(x == i && y == j) && (!dif_x || !dif_y) && Player1_Map -> unknown_map[x][y] == ' ') {
							Player2_shoot(x, y);
							return;
						}
					}

	// int candidate[4][2] = {{1, 1}, {1, map_column - 2}, {map_row - 2, 1}, {map_row - 2, map_column - 2}};
	// for (int i = 0; i < 4; i++) {
	// 	int x = candidate[i][0], y = candidate[i][1];
	// 	if (is_valid(x, y, map_row, map_column) && Player1_Map -> unknown_map[x][y] == ' ') {
	// 		Player2_shoot(x, y);
	// 		return;
	// 	}
	// }

	for (int i = 0; i < 200; i++) {
		int x = (rand() * rand()) % map_row, y = (rand() * rand()) % map_column;
		if ((x + y) % 2 && is_valid(x, y, map_row, map_column) && Player1_Map -> unknown_map[x][y] == ' ') {
			Player2_shoot(x, y);
			return;
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
