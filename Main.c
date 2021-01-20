#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Menu.c"
#include "init.c"

int main() {
    init();
    tutorial();
    main_menu();
    printf("\nend of main");
}