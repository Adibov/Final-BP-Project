#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "init.c"

int main() {
    init();
    // return 0;
    tutorial();
    main_menu();
    printf("\nend of main");
    Exit();
}