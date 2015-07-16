#ifndef TRIE4D_H

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define VALUE_TYPE int
//__int64_t

struct Node {
	struct Node **N;
	VALUE_TYPE value;
	char item;
};

struct Node* newNode();
int addNode(struct Node *C, void *key, int count, VALUE_TYPE value);
struct Node* findNode(struct Node * C, void *key, int count);
void freeNode(struct Node* C);

#endif // TRIE4D_H
