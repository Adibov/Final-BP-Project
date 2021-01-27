#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#include "Map.c"

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
bool check_placement(char(*)[100], int, int, int, int, int, int);
bool is_valid(int, int, int, int);

/* functions definitions */
bool check_placement(char tmp_mp[100][100], int row, int column, int len, int direction, int num_row, int num_column) {
	for (int k = 0; k < len; k++) {
		int i = row + k * dx[direction], j = column + k * dy[direction];
		if (!is_valid(i, j, num_row, num_column) || tmp_mp[i][j] != 'E')
			return 0;
		
		for (int dif_x = -1; dif_x < 2; dif_x++)
			for (int dif_y = -1; dif_y < 2; dif_y++) {
				int x = i + dif_x, y = j + dif_y;
				if (is_valid(x, y, num_row, num_column) && tmp_mp[x][y] != 'E')
					return 0;
			}
	}
	return 1;
}

bool is_valid(int i, int j, int n, int m) {
	return 0 <= i && i < n && 0 <= j && j < m;
}