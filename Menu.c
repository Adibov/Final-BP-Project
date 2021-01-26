#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Multiplayer.c"

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
|           X           |  Exploded cell |\n\
+-----------------------+----------------+\n\n\
\
"	);


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
		printf("%s\n", input_list[i]);
	}

	char input[100];
	int option;
	output_color_text(light_red, "Choose an option: ");
	scanf(" %s", &input);
	if (input[0] < '1' || '7' < input[0]) {
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
		else
			Start_multiplayer_game(0);
	}
	else if (option == 4) {
		current_game = Load_Last();
		if (current_game == NULL)
			Multiplayer();
		else
			Start_multiplayer_game(0);
	}
	else if (option == 5)
		Setting();
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
	printf("score board");
}

void Exit() {
	system("CLS");
	printf("Good luck, Have fun :D");
	exit(0);
}

