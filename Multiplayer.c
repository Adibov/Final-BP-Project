#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Solo_Player.c"

/* Global variables */
User Player1_User, Player2_User;
Linked_List *Player1_Ships, *Player2_Ships;
Map *Player1_Map, *Player2_Map;

/* functions declrations */
void Multiplayer(void);
void Multiplayer_menu(void);
User Choose_user(char *);
User Choose_from_avail(void);
User New_user(void);
void Choose_second_user(void);
Linked_List *Ships_placement(char *);
Linked_List *Ships_auto_placement(char *);
Linked_List *Ships_manual_placement(char *);
void Add_ships_to_map(char (*)[100], Linked_List *);
void Start_multiplayer_game(void);
void Player1_turn(void);
void Player2_turn(void);

/* functions definitions */
void Multiplayer() {
	system("CLS");
	Multiplayer_menu();
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
	Start_multiplayer_game();
}

User Choose_user(char *message) {
	printf("\
%s\n\
	Choose user:\n\
		1) Choose from available users\n\
		2) New user\n\
",	message);

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

User Choose_from_avail() {
	system("CLS");
	FILE *user_file = fopen("Files\\Users.bin", "rb");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");

	User user;
	int indx = 1;
	while (1) {
		if (fread(&user, sizeof(User), 1, user_file) < 1)
			break;

		printf("%d) %s\n", indx, user.name);
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
	if (fread(&user, sizeof(User), 1, user_file) < 1)
		error_exit("Cannot read User after changing SEEK_CUR");
	fclose(user_file);
	return user;
}

User New_user() {
	system("CLS");
	char username[200];
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
	User user;
	while (1) {
		if (fread(&user, sizeof(User), 1, user_file) < 1)
			break;

		if (!strcmp(user.name, username))
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
	
	User *new_user = malloc(sizeof(User));
	new_user -> point = 0;
	strcpy(new_user -> name, username);
	fwrite(new_user, sizeof(User), 1, user_file);
	fclose(user_file);
	return *new_user;
}

void Choose_second_user() {
	system("CLS");
	while (1) {
		Player2_User = Choose_user("Second player");
		if (!strcmp(Player1_User.name, Player2_User.name)) {
			system("CLS");
			printf("This user has been chosen\nPress any key to continue.");
			getch();
			system("CLS");
			continue;
		}
		break;
	}
}

Linked_List *Ships_placement(char *message) {
		system("CLS");
		printf("\
%s\n\
	Ships placement:\n\
		1) Auto\n\
		2) Manual\n\
",	message);

	int option;
	scanf("%d", &option);
	if (option == 1)
		return Ships_auto_placement(message);
	else if (option == 2)
		return Ships_manual_placement(message);
	else {
		invalid_input();
		return Ships_placement(message);
	}
}

Linked_List *Ships_auto_placement(char *message) {
	system("CLS");
	srand(time(0));
	char Tmp_map[100][100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			Tmp_map[i][j] = 'E';

	Linked_List *result = Linked_List_init();

	Ships -> cur = Ships -> head -> nxt;
	while (Ships -> cur != Ships -> head) {
		Ship *current_ship = (Ship *)(Ships -> cur -> value);
		int len = current_ship -> length;
		bool placed = 0;

		while (!placed) {
			for (int i = 0; i < map_row; i++)
				for (int j = 0; j < map_column && !placed; j++) {
					int direction = (rand() & 1);
					if (!(rand() % 3) && check_placement(Tmp_map, i, j, len, direction, map_row, map_column)) {
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
		}
		Ships -> cur = Ships -> cur -> nxt;
	}

	printf("Would you want to keep this placement? (y / n)\n\n");
	Map_output(Tmp_map, map_row, map_column);

	char option;
	scanf(" %c", &option);
	if (option == 'y')
		return result;
	else if (option == 'n')
		return Ships_auto_placement(message);
	else {
		invalid_input();
		return Ships_placement(message);
	}
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
		bool placed = 0;

		while (!placed) {
			int i, j, direction;
			system("CLS");
			Map_output(Tmp_map, map_row, map_column);
			printf("Where do you want to add a ship with length %d? (Consider upper leftmost cell of the ship, and enter row No. and column No. by order)\n", len);
			scanf("%d%d", &i, &j);
			i--, j--;
			if (!is_valid(i, j, map_row, map_column)) {
				invalid_input();
				continue;
			}

			printf("\nNow enter the direction: (0 for vertical and 1 for horizontal)\n");
			scanf("%d", &direction);
			if (direction && direction != 1) {
				invalid_input();
				continue;
			}
			if (!check_placement(Tmp_map, i, j, len, direction, map_row, map_column)) {
				system("CLS");
				printf("Cannot place the ship here\nPress any key to continue.");
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

void Start_multiplayer_game() {
	int winner_player = 2;
	while (Player1_Ships -> head -> nxt != Player1_Ships -> head && Player2_Ships -> head -> nxt != Player2_Ships -> head) {
		Player1_turn();
		if (Player2_Ships -> head -> nxt == Player2_Ships -> head) {
			winner_player = 1;
			break;
		}
		Player2_turn();
	}
}

void Player1_turn() {

}

void Player2_turn() {
	
}