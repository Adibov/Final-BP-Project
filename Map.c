#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "linked_list.c"

/* Global variables */

/* Typedefs */
typedef struct Map Map;

/* Structs declrations */
struct Map {
	char unknown_map[100][100], known_map[100][100]; // E: Empty   ' ': Unkown   S:Ship   D:Destroyed   E:Exploded   B:Border
};

/* functions declrations */
Map *Map_init() {
	Map *result = (Map *)malloc(sizeof(Map));
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++) {
			result -> unknown_map[i][j] = ' ';
			result -> known_map[i][j] = 'E';
		}
	return result;
}

/* functions definitions */
