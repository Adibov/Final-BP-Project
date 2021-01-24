#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "General.c"

/* functions declrations */
void Load(void);
void Load_Last(void);
void Save_game(Game *);

/* functions definitions */
void Load() {
	printf("load");
}

void Load_Last() {
	printf("load last");
}

void Save_game(Game *current_game) {
	printf("save game");
}