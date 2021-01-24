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
User Choose_user(void);
User Choose_from_avail(void);
User New_user(void);
Linked_List *Ships_placement(void);
Linked_List *Ships_auto_placement(void);
Linked_List *Ships_manual_placement(void);

/* functions definitions */
void Multiplayer() {
	system("CLS");
	Multiplayer_menu();
}

void Multiplayer_menu() {
	system("CLS");
	Player1_User = Choose_user();
	Player1_Ships = Ships_placement();
	// User Player2_User = Choose_user();
}

User Choose_user() {
	printf("\
First player\n\
	Choose user:\n\
		1) Choose from available users\n\
		2) New user\n\
"	);

	int option;
	scanf("%d", &option);
	if (option == 1)
		return Choose_from_avail();
	else if (option == 2)
		return New_user();
	else {
		invalid_input();
		Multiplayer_menu();
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
}

Linked_List *Ships_placement() {
		system("CLS");
		printf("\
First player\n\
	Ships placement:\n\
		1) Auto\n\
		2) Manual\n\
"	);

	int option;
	scanf("%d", &option);
	if (option == 1)
		return Ships_auto_placement();
	else if (option == 2)
		return Ships_manual_placement();
	else {
		invalid_input();
		return Ships_placement();
	}
}

Linked_List *Ships_auto_placement() {
	
}

Linked_List *Ships_manual_placement() {

}