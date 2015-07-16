#include "trie4d.h"

struct Node* newNode() {
	struct Node *node;
	node = malloc(sizeof (struct Node));
	node->value = -1;
	node->item = -1;
	return node;
}

int addNode(struct Node *C, void *key, int count, VALUE_TYPE value) {
	int pos = 0;
	struct Node* tmp;
	while (1) {
		char u;
		int bpos = pos >> 1;
		if (bpos >= count) {
			if (C->value == -1) {
				C->value = value;
				return 1; // value added
			}
			C->value = value;
			return 0; // value replaced
		}
		unsigned char b = ((unsigned char*)key)[bpos];
		if (pos++ & 1) u = (b >>= 4);
		else u = b & 0xf;
		if (C->item == -1) {
			C->item = u;
			C->N = (struct Node**)newNode();
			C = (struct Node*) C->N;
		}
		else if (C->item >= 0) {
			if (C->item == u) {
				C = (struct Node*)C->N;
			}
			else {
				tmp = (struct Node*) C->N;
				C->N = malloc(sizeof (void*) * 16);
				memset(C->N, 0, sizeof(void*) << 4);
				C->N[C->item] = tmp;
				C->item = -2;
				C = C->N[u] = newNode();
			}
		} else {
			if (C->N[u])
				C = C->N[u];
			else 
				C = C->N[u] = newNode();
		}
	}
}

struct Node* findNode(struct Node * C, void *key, int count) {
	int pos = 0;
	while (1) {
		char u;
		int bpos = pos >> 1;
		if (bpos >= count) return C;
		unsigned char b = ((unsigned char*)key)[bpos];
		if (pos++ & 1) u = (b >>= 4);
		else u = b & 0xf;
		
		if (C->item == -1) return 0;
		else if (C->item >= 0) {
			if (u == C->item) C = (struct Node*)C->N;
			else return 0;
		} else {
			__builtin_prefetch(C->N[u]);
			C = C->N[u];
			if (C == 0) return 0;
		}
	}
}

#ifdef DEBUG
int mem_count = 0;
#endif

void freeNode(struct Node* C) {
	int i;
	if (C->item == -2) {
		for (i = 0; i < 16; i++) {
			if (C->N[i]) freeNode(C->N[i]);
		}
		free(C->N);
		#ifdef DEBUG
		mem_count += 16 * sizeof(struct Node);
		#endif
	}
	else if (C->item >= 0) {
		freeNode((struct Node*)C->N);
	}
	free(C);
	#ifdef DEBUG
	mem_count += sizeof(struct Node);
	#endif
}
