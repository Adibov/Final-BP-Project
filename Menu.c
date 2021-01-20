#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Solo_Player.c"
#include "Multiplayer.c"
#include "Save_Load.c"
#include "Setting.c"

/* functions declrations */
void turorial(void);
void menu(void);
void Score_Board(void);
void Exit(void);

/* functions definitions */
void tutorial() {
	system("CLS");
	printf(
"\
Hello and Welcome to Battle-Ship game.\n\
In This game your target is to destroy all of enemy's ships. In every turn you can choose a arbitrary empty cell and fire in it, If it hits to a ship, you can fire again. You will be friendly with the game after some time.\n\n\
In this game we will show cells like this: \n\
+-----------------------+----------------+\n\
|       Character       |   Description  |\n\
++++++++++++++++++++++++++++++++++++++++++\n\
| (nothing\\empty cell)  |  Unknown cell  |\n\
+-----------------------+----------------+\n\
|           W           |   Water cell   |\n\
+-----------------------+----------------+\n\
|           D           | Destroyed ship |\n\
+-----------------------+----------------+\n\
|           E           |  Exploded cell |\n\
+-----------------------+----------------+\n\n\
\
** Anytime in the middle of the game, if you press m button, the menu will show up.**\n\n\
"	);


	printf("Press any key to continue\n");
	getch();
	system("CLS");
}

void main_menu() {
	system("CLS");
	printf(
"\
1) Play with a Friend\n\
2) Play with Computer\n\
3) Load a saved game\n\
4) Load last game\n\
5) Settings\n\
6) Score Board\n\
7) Exit\n\n\
"	);

	int option;
	printf("Choose an option: ");
	scanf("%d", &option);
	system("CLS");
	if (option == 1)
		Multiplayer();
	else if (option == 2)
		Solo_Player();
	else if (option == 3)
		Load();
	else if (option == 4)
		Load_Last();
	else if (option == 5)
		Setting();
	else if (option == 6)
		Score_Board();
	else if (option == 7)
		Exit();
	else {
		printf("Invalid input, Press any key to continue.");
		getch();
		main_menu();
	}
}

void Score_Board() {
	printf("score board");
}

void Exit() {
	system("CLS");
	printf("Good luck, Have fun :D");
	exit(0);
}

