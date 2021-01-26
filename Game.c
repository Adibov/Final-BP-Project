#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "User.c"

/* Typedefs */
typedef struct Game Game;

/* Structs declrations */
struct Game {
	int mode, turn, starting_time, player1_point, player2_point, map_row, map_column; // mode=1: single player,   mode=2: multiplayer
	User *Player1_User, *Player2_User;
	Linked_List *Player1_Ships, *Player2_Ships;
	Map *Player1_Map, *Player2_Map;
};

/* functions declrations */

/* functions definitions */
