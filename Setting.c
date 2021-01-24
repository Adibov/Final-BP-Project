#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Save_Load.c"

/* Global variables */
int map_row, map_column;
Linked_List *Ships;

/* functions declrations */
void Setting(void);
void setting_init(void);

/* functions definitions */
void Setting() {
	printf("setting");
}

void setting_init() {
	map_row = map_column = 10;
	Ships = Linked_List_init();

	int initial_ships[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 5};
	for (int i = 0; i < 10; i++) {
		Ship *ship = (Ship *)malloc(sizeof(Ship));
		ship -> length = initial_ships[i];
		Linked_List_add(Ships, ship);
	}
}