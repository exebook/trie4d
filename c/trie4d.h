#ifndef TRIE4D_H

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

/* your own value type if needed */
#ifndef TRIE4D_VALUE_TYPE
#define TRIE4D_VALUE_TYPE int
#endif

struct Node {
	struct Node **N;
	TRIE4D_VALUE_TYPE value;
	char item;
};

struct Node* newNode();
int addNode(struct Node *C, void *key, int count, TRIE4D_VALUE_TYPE value);
struct Node* findNode(struct Node * C, void *key, int count);
void freeNode(struct Node* C);

#endif // TRIE4D_H
