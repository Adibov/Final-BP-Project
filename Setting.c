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
int get_largest_ship_length(void);

/* functions definitions */
void Setting() {
	printf("setting");
}

void setting_init() {
	map_row = map_column = 10;
	Ships = Linked_List_init();

	int initial_ships[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 5}, num = 10;
	for (int i = 0; i < num; i++) {
		Ship *ship = (Ship *)malloc(sizeof(Ship));
		ship -> length = initial_ships[i];
		Linked_List_add(Ships, ship);
	}
}

int get_largest_ship_length() {
	int res = 0;
	Ships -> cur = Ships -> head -> nxt;
	while (Ships -> cur != Ships -> head) {
		Ship *tmp = (Ship *)Ships -> cur -> value;
		if (tmp -> length > res)
			res = tmp -> length;
		Ships -> cur = Ships -> cur -> nxt;
	}
	return res;
}