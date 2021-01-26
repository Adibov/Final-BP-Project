#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <conio.h>

/* Typedefs */
typedef struct Linked_List Linked_List;
typedef struct node node;

/* Structs declrations */
struct Linked_List {
	node *head, *cur;
};

struct node {
	node *prv, *nxt;
	void *value;
};

/* functions declrations */
Linked_List *Linked_List_init();
void Linked_List_add(Linked_List *, void *);
void Linked_List_del(Linked_List *);
int Linked_List_size(Linked_List *);

/* functions definitions */
Linked_List *Linked_List_init() {
	Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
	list -> head = (node *)malloc(sizeof(node));
	list -> cur = list -> head;
	list -> head -> nxt = list -> head -> prv = list -> head;
	return list;
}

void Linked_List_add(Linked_List *list, void *value) { // Add value after cur and change cur to cur + 1
	node *nxt = list -> cur -> nxt, *new_node = (node *)malloc(sizeof(node));
	new_node -> value = value;
	list -> cur -> nxt = new_node;
	new_node -> prv = list -> cur;
	new_node -> nxt = nxt;
	nxt -> prv = new_node;
	list -> cur = new_node;
}

void Linked_List_del(Linked_List *list) { // Delete cur and change it to cur-1
	if (list -> cur == list -> head)
		return;
	
	node *prv = list -> cur -> prv, *nxt = list -> cur -> nxt;
	prv -> nxt = nxt;
	nxt -> prv = prv;
	free(list -> cur);
	list -> cur = prv;
}

int Linked_List_size(Linked_List *list) {
	int res = 0;
	list -> cur = list -> head -> nxt;
	while (list -> cur != list -> head) {
		list -> cur = list -> cur -> nxt;
		res++;
	}
	return res;
}
