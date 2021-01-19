#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>

/* functions declrations */
void turorial(void);


/* functions definitions */
void tutorial() {
	system("CLS");
	printf(
"Hello and Welcome to Battle-Ship game.\n\
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
}