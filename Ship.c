#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>

/* Global variables */
enum Direction {vertical, horizontal};

/* Typedefs */
typedef struct Ship Ship;
typedef enum Direction Direction;

/* Structs declrations */
struct Ship {
	int length, row, column;
	bool destroyed;
	Direction direction;
};

/* functions declrations */

/* functions definitions */
