#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "linked_list.c"

/* Global variables */
enum Direction {vertical, horizontal};
int dx[2] = {1, 0}, dy[2] = {0, 1};

/* Typedefs */
typedef struct Ship Ship;
typedef enum Direction Direction;

/* Structs declrations */
struct Ship {
	int length, row, column; // row and column up or left cell
	bool destroyed;
	Direction direction;
};

/* functions declrations */

/* functions definitions */
