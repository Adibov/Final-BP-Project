#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Save_Load.c"

/* Global variables */
User *Player1_User, *Player2_User;
Linked_List *Player1_Ships, *Player2_Ships;
Map *Player1_Map, *Player2_Map;
Game *current_game;

/* functions declrations */
void Multiplayer(void);
void Multiplayer_menu(void);
User *Choose_user(char *);
User *Choose_from_avail(void);
User *New_user(void);
void Choose_second_user(void);
Linked_List *Ships_placement(char *);
Linked_List *Ships_auto_placement(bool);
Linked_List *Ships_manual_placement(char *);
void Add_ships_to_map(char (*)[100], Linked_List *);
void Start_init(bool);
void Start_multiplayer_game(bool);
void Player1_turn(void);
void Player1_shoot(int, int);
void Player1_rocket_use(void);
void Player2_turn(void);
void Player2_shoot(int, int);
void Player2_rocket_use(void);
void destroy_ship(Ship *, char (*)[]);
void Add_points(char *, int);

/* functions definitions */
void Multiplayer() {
	system("CLS");
	Multiplayer_menu();
	Start_multiplayer_game(1);
}

void Multiplayer_menu() {
	system("CLS");
	Player1_User = Choose_user("First player");
	Player1_Ships = Ships_placement("First player");
	Choose_second_user();
	Player2_Ships = Ships_placement("Second player");
	Player1_Map = Map_init();
	Player2_Map = Map_init();
	Add_ships_to_map(Player1_Map -> known_map, Player1_Ships);
	Add_ships_to_map(Player2_Map -> known_map, Player2_Ships);
}

User *Choose_user(char *message) {
	system("CLS");
	terminal_color(red);
	printf("%s\n", message);
	terminal_color(white);
	output_color_text(light_cyan, "  Choose user:\n\n");
	char *output[2] = {"Choose from available users", "New user"};
	for (int i = 0; i < 2; i++) {
		terminal_color(yellow);
		printf("    %d) ", i + 1);
		terminal_color(white);
		printf("%s\n\n", output[i]);
	}

	int option;
	scanf("%d", &option);
	if (option == 1)
		return Choose_from_avail();
	else if (option == 2)
		return New_user();
	else {
		invalid_input();
		return Choose_user(message);
	}
}

User *Choose_from_avail() {
	system("CLS");
	FILE *user_file = fopen("Files\\Users.bin", "rb");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");

	User *user = (User *)malloc(sizeof(User));
	int indx = 1;
	output_color_text(light_cyan, "    Username           Points\n\n");
	while (1) {
		if (fread(user, sizeof(User), 1, user_file) < 1)
			break;
		
		terminal_color(yellow);
		printf("%d) ", indx);
		terminal_color(cyan);
		printf("%-22.22s%d\n\n", user -> name, user -> point);
		terminal_color(white);
		indx++;
	}
	fclose(user_file);

	if (indx == 1) {
		system("CLS");
		printf("There isn't any user, You have to create a new user.\nPress any key to continue\n");
		getch();
		return New_user();
	}
	int user_count = indx - 1;

	printf("Enter the id of the username that you want to choose (or enter 0 to create a new user): ");
	scanf("%d", &indx);
	if (!indx)
		return New_user();
	if (indx < 1 || user_count < indx) {
		invalid_input();
		return Choose_from_avail();
	}

	user_file = fopen("Files\\Users.bin", "rb");
	if (fseek(user_file, (indx - 1) * sizeof(User), SEEK_SET))
			error_exit("Cannot reach indx in user_file");
	if (fread(user, sizeof(User), 1, user_file) < 1)
		error_exit("Cannot read User after changing SEEK_CUR");
	fclose(user_file);
	return user;
}

User *New_user() {
	system("CLS");
	char username[200];
	for (int i = 0; i < 200; i++)
		username[i] = '\0';
	printf("Enter a username with at most 50 characters: ");
	gets(username);
	while (!strlen(username))
		gets(username);
	if (strlen(username) > 50) {
		invalid_input();
		return New_user();
	}

	FILE *user_file = fopen("Files\\Users.bin", "rb");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");

	bool does_exist = 0;
	User *user = (User *)malloc(sizeof(User));
	while (1) {
		if (fread(user, sizeof(User), 1, user_file) < 1)
			break;

		if (!strcmp(user -> name, username))
			does_exist = 1;
	}
	fclose(user_file);

	if (does_exist) {
		system("CLS");
		printf("This username has been taken, try another one.\nPress any key to continue");
		getch();
		return New_user();
	}

	user_file = fopen("Files\\Users.bin", "ab");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");
	
	user -> point = 0;
	for (int i = 0; i < user_name_length; i++)
		user -> name[i] = username[i];
	fwrite(user, sizeof(User), 1, user_file);
	fclose(user_file);
	return user;
}

void Choose_second_user() {
	system("CLS");
	while (1) {
		Player2_User = Choose_user("Second player");
		if (!strcmp(Player1_User -> name, Player2_User -> name)) {
			system("CLS");
			output_color_text(red, "This user has been chosen\n\n");
			printf("Press any key to continue.");
			getch();
			system("CLS");
			continue;
		}
		break;
	}
}

Linked_List *Ships_placement(char *message) {
	system("CLS");
	terminal_color(red);
	printf("%s\n", message);
	terminal_color(white);
	output_color_text(light_cyan, "  Ships placement:\n\n");
	char *output[2] = {"Auto", "Manual"};
	for (int i = 0; i < 2; i++) {
		terminal_color(yellow);
		printf("    %d) ", i + 1);
		terminal_color(white);
		printf("%s\n\n", output[i]);
	}

	int option;
	scanf("%d", &option);
	if (option == 1)
		return Ships_auto_placement(0);
	else if (option == 2)
		return Ships_manual_placement(message);
	else {
		invalid_input();
		return Ships_placement(message);
	}
}

Linked_List *Ships_auto_placement(bool computer) {
	system("CLS");
	srand(time(0));
	char Tmp_map[100][100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			Tmp_map[i][j] = 'E';

	Linked_List *result = Linked_List_init();

	int permutation[50] = {}, sz = Linked_List_size(Ships);
	for (int i = 0; i < sz; i++)
		permutation[i] = i;
	Random_Shuffle(permutation, sz);

	int indx = 0;
	while (indx < sz) {
		Ships -> cur = Ships -> head -> nxt;
		int tmp = permutation[indx];
		while (tmp--) 
			Ships -> cur = Ships -> cur -> nxt;
		
		Ship *current_ship = (Ship *)(Ships -> cur -> value);
		int len = current_ship -> length;
		bool placed = 0;

		int limit = 20;
		while (!placed) {
			for (int i = 0; i < map_row; i++)
				for (int j = 0; j < map_column && !placed; j++) {
					int direction = (rand() & 1);

					if ((limit <= 0 || !(rand() % 10)) && check_placement(Tmp_map, i, j, len, direction, map_row, map_column)) {
						for (int k = 0; k < len; k++)
							Tmp_map[i + k * dx[direction]][j + k * dy[direction]] = 'S';
						placed = 1;

						Ship *added_ship = (Ship *)malloc(sizeof(Ship));
						added_ship -> length = len;
						added_ship -> row = i;
						added_ship -> column = j;
						added_ship -> destroyed = 0;
						added_ship -> direction = direction;
						Linked_List_add(result, added_ship);
					}
				}
			if (limit-- <= -20) 
				return Ships_auto_placement(computer);
		}
		Ships -> cur = Ships -> cur -> nxt;
		indx++;
	}

	if (!computer) {
		printf("Would you want to keep this placement? (y / n)\n\n");
		Map_output(Tmp_map, map_row, map_column);

		char option;
		scanf(" %c", &option);
		if (option == 'y')
			return result;
		else if (option == 'n')
			return Ships_auto_placement(computer);
		else {
			invalid_input();
			return Ships_auto_placement(computer);
		}
	}
	else
		return result;
}

Linked_List *Ships_manual_placement(char *message) {
	system("CLS");
	char Tmp_map[100][100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			Tmp_map[i][j] = 'E';

	Linked_List *result = Linked_List_init();

	Ships -> cur = Ships -> head -> nxt;
	while (Ships -> cur != Ships -> head) {
		Ship *current_ship = (Ship *)(Ships -> cur -> value);
		int len = current_ship -> length;
		bool placed = 0, can_place = 0;

		for (int i = 0; i < map_row; i++)
			for (int j = 0; j < map_column; j++)
				if (check_placement(Tmp_map, i, j, len, 0, map_row, map_column) || check_placement(Tmp_map, i, j, len, 1, map_row, map_column))
					can_place = 1;
		
		if (!can_place) {
			system("CLS");
			output_color_text(red, "Cannot place any more ship\n\n");
			printf("Press any key to continue");
			getch();
			return Ships_placement(message);
		}

		while (!placed) {
			int i, j, direction = 0;
			system("CLS");
			Map_output(Tmp_map, map_row, map_column);
			printf("Where do you want to add a ship with length %d? (Consider upper leftmost cell of the ship, and enter row No. and column No. by order)\n", len);
			scanf("%d%d", &i, &j);
			i--, j--;
			if (!is_valid(i, j, map_row, map_column)) {
				invalid_input();
				continue;
			}

			if (len > 1) {
				printf("\nNow enter the direction: (0 for vertical and 1 for horizontal)\n");
				scanf("%d", &direction);
				if (direction && direction != 1) {
					invalid_input();
					continue;
				}
			}
			if (!check_placement(Tmp_map, i, j, len, direction, map_row, map_column)) {
				system("CLS");
				output_color_text(red, "Cannot place the ship here\n\n");
				printf("Press any key to continue");
				getch();
				continue;
			}

			for (int k = 0; k < len; k++)
				Tmp_map[i + k * dx[direction]][j + k * dy[direction]] = 'S';
			placed = 1;

			Ship *added_ship = (Ship *)malloc(sizeof(Ship));
			added_ship -> length = len;
			added_ship -> row = i;
			added_ship -> column = j;
			added_ship -> destroyed = 0;
			added_ship -> direction = direction;
			Linked_List_add(result, added_ship);
		}
		Ships -> cur = Ships -> cur -> nxt;
	}

	system("CLS");
	printf("Would you want to keep this placement? (y / n)\n\n");
	Map_output(Tmp_map, map_row, map_column);

	char option;
	scanf(" %c", &option);
	if (option == 'y')
		return result;
	else if (option == 'n')
		return Ships_placement(message);
	else {
		invalid_input();
		return Ships_placement(message);
	}
}

void Add_ships_to_map(char Tmp_map[100][100], Linked_List *ships) {
	system("CLS");
	ships -> cur = ships -> head -> nxt;
	while (ships -> cur != ships -> head) {
		Ship *current_ship = (Ship *)(ships -> cur -> value);
		int i = current_ship -> row, j = current_ship -> column, len = current_ship -> length, direction = current_ship -> direction;
		for (int k = 0; k < len; k++) {
			int x = i + k * dx[direction], y = j + k * dy[direction];
			Tmp_map[x][y] = 'S';
		}
		ships -> cur = ships -> cur -> nxt;
	}
}

void Start_init(bool new_game) {
	if (new_game) {
		current_game = (Game *)malloc(sizeof(Game));
		current_game -> Player1_User = Player1_User;
		current_game -> Player2_User = Player2_User;
		current_game -> Player1_Ships = Player1_Ships;
		current_game -> Player2_Ships = Player2_Ships;
		current_game -> Player1_Map = Player1_Map;
		current_game -> Player2_Map = Player2_Map;
		current_game -> mode = 2;
		current_game -> turn = 1;
		current_game -> player1_point = Player1_User -> point;
		current_game -> player2_point = Player2_User -> point;
		current_game -> Player1_User -> rocket_used = 0;
		current_game -> Player2_User -> rocket_used = 0;
		current_game -> starting_time = time(0);
		current_game -> map_row = map_row;
		current_game -> map_column = map_column;
	}
	else {
		Player1_User = current_game -> Player1_User;
		Player2_User = current_game -> Player2_User;
		Player1_Ships = current_game -> Player1_Ships;
		Player2_Ships = current_game -> Player2_Ships;
		Player1_Map = current_game -> Player1_Map;
		Player2_Map = current_game -> Player2_Map;
		map_row = current_game -> map_row;
		map_column = current_game -> map_column;
	}
	system("del Files\\Playback.bin");
	system("type nul > Files\\Playback.bin");
}

void Start_multiplayer_game(bool new_game) {
	Start_init(new_game);
	Save_Last(current_game);

	int winner_player = 2;
	int player1_last_score = current_game -> player1_point, player2_last_score = current_game -> player2_point;
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
			Player2_turn();
			system("CLS");
			Map_output(Player1_Map -> unknown_map, map_row, map_column);
			FILE *play_back_file = fopen("Files\\Playback.bin", "ab");
			for (int i = 0; i < map_max_size; i++)
				fwrite(Player1_Map -> unknown_map[i], sizeof(char), map_max_size, play_back_file);
			fclose(play_back_file);
		}
		Save_Last(current_game);
		Add_points(Player1_User -> name, current_game -> player1_point - player1_last_score);
		Add_points(Player2_User -> name, current_game -> player2_point - player2_last_score);
		player1_last_score = current_game -> player1_point;
		player2_last_score = current_game -> player2_point;
		printf("\nPress any key to continue.");
		getch();
	}
	if (Player2_Ships -> head -> nxt == Player2_Ships -> head)
		winner_player = 1;
	
	system("CLS");
	color_reset();
	if (winner_player == 1) {
		// Add_points(Player1_User -> name, current_game -> player1_point);
		// Add_points(Player2_User -> name, (current_game -> player2_point) / 2);
		output_color_text(green, "Congratulations, ");
		terminal_color(cyan);
		printf("%s ", Player1_User -> name);
		terminal_color(white);
		output_color_text(green, "has won the game =D");
	}
	else {
		// Add_points(Player1_User -> name, (current_game -> player1_point) / 2);
		// Add_points(Player2_User -> name, current_game -> player2_point);
		output_color_text(green, "Congratulations, ");
		terminal_color(cyan);
		printf("%s ", Player2_User -> name);
		terminal_color(white);
		output_color_text(green, "has won the game =D");
	}
	printf("\nPress any key to continue.");
	getch();
}

void Player1_turn() {
	system("CLS");
	Map_output(Player2_Map -> unknown_map, map_row, map_column);
	output_color_text(red, "\nFirst player turn ");
	terminal_color(red);
	printf("(%s):\n", Player1_User -> name);
	terminal_color(yellow);
	printf("Your score: ");
	terminal_color(white);
	printf("%d\n", current_game -> player1_point);
	terminal_color(white);
	printf("\nEnter ");
	output_color_text(blue, " row No.");
	printf(" and ");
	output_color_text(blue, "column No.");
	printf(" of the target cell: (Enter s to save the game or Enter r to use rocket)\n");
	
	int x, y;
	char input[10];
	scanf(" %s", &input);
	if (!strcmp("s", input)) {
		Save_game(current_game);
		Player1_turn();
		return;
	}
	else if (!strcmp("r", input)) {
		Player1_rocket_use();
		return;
	}
	x = string_to_int(input);
	scanf(" %s", &input);
	if (!strcmp("s", input)) {
		Save_game(current_game);
		Player1_turn();
		return;
	}
	y = string_to_int(input);
	x--, y--;
	if (!is_valid(x, y, map_row, map_column)) {
		invalid_input();
		Player1_turn();
		return;
	}
	if (Player2_Map -> unknown_map[x][y] != ' ') {
		system("CLS");
		printf("You have hit this cell before\nPress any key to continue.");
		getch();
		Player1_turn();
		return;
	}
	Player1_shoot(x, y);
}

void Player1_shoot(int x, int y) {
	current_game -> turn = 1;
	if (Player2_Map -> known_map[x][y] == 'E') {
		Player2_Map -> unknown_map[x][y] = 'W';
		current_game -> turn = 2;
		return;
	}
	Player2_Map -> unknown_map[x][y] = 'X';
	current_game -> player1_point++;

	Ship *exploded_ship;
	Player2_Ships -> cur = Player2_Ships -> head -> nxt;
	while (Player2_Ships -> cur != Player2_Ships -> head) {
		bool finded = 0;
		Ship *current_ship = Player2_Ships -> cur -> value;
		int len = current_ship -> length;
		for (int k = 0; k < len; k++) {
			int i = current_ship -> row + k * dx[current_ship -> direction], j = current_ship -> column + k * dy[current_ship -> direction];
			if (i == x && j == y) {
				exploded_ship = current_ship;
				finded = 1;
				break;
			}
		}
		if (finded)
			break;
		Player2_Ships -> cur = Player2_Ships -> cur -> nxt;
	}
	if (exploded_ship == NULL)
		error_exit("Cannot find the exploded ship");

	int len = exploded_ship -> length, direction = exploded_ship -> direction;
	bool is_destroyed = 1;
	for (int k = 0; k < len; k++) {
		int i = exploded_ship -> row + k * dx[direction], j = exploded_ship -> column + k * dy[direction];
		if (Player2_Map -> unknown_map[i][j] != 'X')
			is_destroyed = 0;
	}
	if (is_destroyed) {
		current_game -> player1_point += (5 * get_largest_ship_length()) / len;
		destroy_ship(exploded_ship, Player2_Map -> unknown_map);
		Linked_List_del(Player2_Ships);
	}
}

void Player1_rocket_use() {
	system("CLS");
	if (current_game -> Player1_User -> rocket_used) {
		output_color_text(light_red, "You've used your rocket before\n\n");
		printf("Press any key to continue.");
		getch();
		Player1_turn();
		return;
	}
	if (current_game -> player1_point < 100) {
		output_color_text(light_red, "You don't have the required score\n\n");
		printf("Press any key to continue.");
		getch();
		Player1_turn();
		return;
	}

	Map_output(current_game -> Player2_Map -> unknown_map, map_row, map_column);
	printf("Enter ");
	output_color_text(blue, "v ");
	printf("to shoot rocket vertically or ");
	output_color_text(blue, "h ");
	printf("to shoot it horizontally: \n");

	char input[100];
	scanf(" %s", &input);
	if (!(input[0] == 'v' || input[0] == 'h')) {
		invalid_input();
		Player1_rocket_use();
		return;
	}

	if (input[0] == 'v') {
		printf("Now enter ");
		output_color_text(blue, "column No. ");
		printf("of the target column:\n");
		scanf(" %s", &input);
		
		int target_column = string_to_int(input);
		target_column--;
		if (target_column < 0 || target_column >= map_column) {
			invalid_input();
			Player1_rocket_use();
			return;
		}
		for (int i = 0; i < map_row; i++)
			if (current_game -> Player2_Map -> unknown_map[i][target_column] == ' ') {
				Player1_shoot(i, target_column);
				if (current_game -> Player2_Map -> unknown_map[i][target_column] == 'X' || current_game -> Player2_Map -> unknown_map[i][target_column] == 'D')
					break;
			}
	}
	else {
		printf("Now enter ");
		output_color_text(blue, "row No. ");
		printf("of the target row:\n");
		scanf(" %s", &input);
		
		int target_row = string_to_int(input);
		target_row--;
		if (target_row < 0 || target_row >= map_row) {
			invalid_input();
			Player1_rocket_use();
			return;
		}
		for (int j = 0; j < map_column; j++)
			if (current_game -> Player2_Map -> unknown_map[target_row][j] == ' ') {
				Player1_shoot(target_row, j);
				if (current_game -> Player2_Map -> unknown_map[target_row][j] == 'X' || current_game -> Player2_Map -> unknown_map[target_row][j] == 'D')
					break;
			}
	}
	current_game -> Player1_User -> rocket_used = 1;
	current_game -> player1_point -= 100;
}

void Player2_turn() {
	system("CLS");
	Map_output(Player1_Map -> unknown_map, map_row, map_column);
	output_color_text(red, "\nSecond player turn ");
	terminal_color(red);
	printf("(%s):\n", Player2_User -> name);
	terminal_color(yellow);
	printf("Your score: ");
	terminal_color(white);
	printf("%d\n", current_game -> player2_point);
	terminal_color(white);
	printf("\nEnter ");
	output_color_text(blue, " row No.");
	printf(" and ");
	output_color_text(blue, "column No.");
	printf(" of the target cell: (Enter s to save the game or Enter r to use rocket)\n");
	
	int x, y;
	char input[10];
	scanf(" %s", &input);
	if (!strcmp("s", input)) {
		Save_game(current_game);
		Player2_turn();
		return;
	}
	else if (!strcmp("r", input)) {
		Player2_rocket_use();
		return;
	}
	x = string_to_int(input);
	scanf(" %s", &input);
	if (!strcmp("s", input)) {
		Save_game(current_game);
		Player2_turn();
		return;
	}
	y = string_to_int(input);
	x--, y--;
	if (!is_valid(x, y, map_row, map_column)) {
		invalid_input();
		Player2_turn();
		return;
	}
	if (Player1_Map -> unknown_map[x][y] != ' ') {
		system("CLS");
		printf("You have hit this cell before\nPress any key to continue.");
		getch();
		Player2_turn();
		return;
	}
	Player2_shoot(x, y);
}

void Player2_shoot(int x, int y) {
	current_game -> turn = 2;
	if (Player1_Map -> known_map[x][y] == 'E') {
		Player1_Map -> unknown_map[x][y] = 'W';
		current_game -> turn = 1;
		return;
	}
	Player1_Map -> unknown_map[x][y] = 'X';
	current_game -> player2_point++;

	Ship *exploded_ship;
	Player1_Ships -> cur = Player1_Ships -> head -> nxt;
	while (Player1_Ships -> cur != Player1_Ships -> head) {
		bool finded = 0;
		Ship *current_ship = Player1_Ships -> cur -> value;
		int len = current_ship -> length;
		for (int k = 0; k < len; k++) {
			int i = current_ship -> row + k * dx[current_ship -> direction], j = current_ship -> column + k * dy[current_ship -> direction];
			if (i == x && j == y) {
				exploded_ship = current_ship;
				finded = 1;
				break;
			}
		}
		if (finded)
			break;
		Player1_Ships -> cur = Player1_Ships -> cur -> nxt;
	}
	if (exploded_ship == NULL)
		error_exit("Cannot find the exploded ship");

	int len = exploded_ship -> length, direction = exploded_ship -> direction;
	bool is_destroyed = 1;
	for (int k = 0; k < len; k++) {
		int i = exploded_ship -> row + k * dx[direction], j = exploded_ship -> column + k * dy[direction];
		if (Player1_Map -> unknown_map[i][j] != 'X')
			is_destroyed = 0;
	}
	if (is_destroyed) {
		current_game -> player2_point += (5 * get_largest_ship_length()) / len;
		destroy_ship(exploded_ship, Player1_Map -> unknown_map);
		Linked_List_del(Player1_Ships);
	}
}

void Player2_rocket_use() {
	system("CLS");
	if (current_game -> Player2_User -> rocket_used) {
		output_color_text(light_red, "You've used your rocket before\n\n");
		printf("Press any key to continue.");
		getch();
		Player2_turn();
		return;
	}
	if (current_game -> player2_point < 100) {
		output_color_text(light_red, "You don't have the required score\n\n");
		printf("Press any key to continue.");
		getch();
		Player2_turn();
		return;
	}

	Map_output(current_game -> Player1_Map -> unknown_map, map_row, map_column);
	printf("Enter ");
	output_color_text(blue, "v ");
	printf("to shoot rocket vertically or ");
	output_color_text(blue, "h ");
	printf("to shoot it horizontally: \n");

	char input[100];
	scanf(" %s", &input);
	if (!(input[0] == 'v' || input[0] == 'h')) {
		invalid_input();
		Player2_rocket_use();
		return;
	}

	if (input[0] == 'v') {
		printf("Now enter ");
		output_color_text(blue, "column No. ");
		printf("of the target column:\n");
		scanf(" %s", &input);
		
		int target_column = string_to_int(input);
		target_column--;
		if (target_column < 0 || target_column >= map_column) {
			invalid_input();
			Player2_rocket_use();
			return;
		}
		for (int i = 0; i < map_row; i++)
			if (current_game -> Player1_Map -> unknown_map[i][target_column] == ' ') {
				Player2_shoot(i, target_column);
				if (current_game -> Player1_Map -> unknown_map[i][target_column] == 'X' || current_game -> Player1_Map -> unknown_map[i][target_column] == 'D')
					break;
			}
	}
	else {
		printf("Now enter ");
		output_color_text(blue, "row No. ");
		printf("of the target row:\n");
		scanf(" %s", &input);
		
		int target_row = string_to_int(input);
		target_row--;
		if (target_row < 0 || target_row >= map_row) {
			invalid_input();
			Player2_rocket_use();
			return;
		}
		for (int j = 0; j < map_column; j++)
			if (current_game -> Player1_Map -> unknown_map[target_row][j] == ' ') {
				Player2_shoot(target_row, j);
				if (current_game -> Player1_Map -> unknown_map[target_row][j] == 'X' || current_game -> Player1_Map -> unknown_map[target_row][j] == 'D')
					break;
			}
	}
	current_game -> Player2_User -> rocket_used = 1;
	current_game -> player2_point -= 100;
}

void destroy_ship(Ship *current_ship, char current_map[100][100]) {
	int len = current_ship -> length, direction = current_ship -> direction;
	for (int k = 0; k < len; k++) {
		int i = current_ship -> row + k * dx[direction], j = current_ship -> column + k * dy[direction];
		current_map[i][j] = 'D';
		for (int _dx_ = -1; _dx_ < 2; _dx_++)
			for (int _dy_ = -1; _dy_ < 2; _dy_++) {
				int x = i + _dx_, y = j + _dy_;
				if (is_valid(x, y, map_row, map_column) && current_map[x][y] == ' ')
					current_map[x][y] = 'W';
			}
	}
	current_ship -> destroyed = 1;
}

void Add_points(char *name, int dif) {
	FILE *user_file = fopen("Files\\Users.bin", "rb+");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");
	
	int indx = 0;
	User *tmp_user = (User *)malloc(sizeof(User));
	while (1) {
		if (fread(tmp_user, sizeof(User), 1, user_file) < 1)
			error_exit("Cannot find name user in Users.bin");
		
		if (!strcmp(tmp_user -> name, name))
			break;
		indx++;
	}
	tmp_user -> point += dif;
	fseek(user_file, indx * sizeof(User), SEEK_SET);
	fwrite(tmp_user, sizeof(User), 1, user_file);
	fclose(user_file);
}
