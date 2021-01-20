#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "User.c"
#include "General.c"

/* Typedefs */

/* functions declrations */
void Multiplayer(void);
void Multiplayer_menu(void);
void Choose_user(void);
User Choose_from_avail(void);
User New_user(void);

/* functions definitions */
void Multiplayer() {
	system("CLS");
	Multiplayer_menu();
}

void Multiplayer_menu() {
	system("CLS");
	Choose_user();
}

void Choose_user() {
	printf("\
First player\n\
	Choose user:\n\
		1) Choose from available users\n\
		2) New user\n\
"	);

	int option;
	scanf("%d", &option);
	if (option == 1) {
		User selected_user = Choose_from_avail();
		
	}
	else if (option == 2)
		New_user();
	else {
		printf("Invalid input, Press any key to continue.");
		getch();
		Multiplayer_menu();
	}
}

User Choose_from_avail() {
	system("CLS");
	FILE *user_file = fopen("Files\\Users.bin", "rb");
	if (user_file == NULL)
		error_exit("Cannot fopen user_file");

	User username;
	int indx = 1;
	while (1) {
		if (fread(&username, sizeof(User), 1, user_file) < 1)
			break;

		printf("%d) %s\n", indx, username.name);
		indx++;
	}
	fclose(user_file);

	if (indx == 1) {
		system("CLS");
		printf("There isn't any user, You have to create a new user.\nPress any key to continue\n");
		getch();
		return New_user();
	}

	printf("Enter the id of the username that you want to choose: ");
	scanf("%d", &indx);
	user_file = fopen("Files\\Users.bin", "rb");
	while (indx--) {
		if (fread(&username, sizeof(User), 1, user_file) < 1)
			error_exit("Cannot reach indx in user_file");
		fread(&username, sizeof(User), 1, user_file);
	}
	fclose(user_file);
	return username;
}

User New_user() {
	printf("new user");
}