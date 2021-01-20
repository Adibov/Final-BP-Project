#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "User.c"

/* Typedefs */
typedef struct User User;

/* functions declrations */
void Multiplayer(void);
void Multiplayer_menu(void);
void Choose_user(void);
User Choose_from_avail(void);
void New_user(void);

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
	if (option == 1)
		Choose_from_avail();
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
	
}

void New_user() {
	printf("new user");
}