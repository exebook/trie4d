/*
	This is self-contained example of trie4-bit, no deferring allocation
	optimization here. Could be useful for learning 4-bit trie in general.
	Only 49 lines the implementation itself, very easy to comprehend.
	See trie4d.c(h) for a slightly faster version.
*/

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
struct Node {
	struct Node **N;
	__int64_t value;
};

struct Node* newNode() {
	struct Node *node;
	node = malloc(sizeof (struct Node));
	node->value = -1;
	node->N = malloc(sizeof (void*) * 16);
	memset(node->N, 0, sizeof(void*) << 4);
	return node;
}

int add(struct Node *C, void *key, int count, __int64_t value) {
	int pos = 0;
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
		if (C->N[u]) C = C->N[u];
		else {
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
		__builtin_prefetch(C->N[u]);
		if (!(C = C->N[u])) return 0;
	}
}

#include "sys/time.h"
#include <stdio.h>

int time1000() {
  struct timeval val;
  gettimeofday(&val, 0);
  val.tv_sec &= 0xffff;
  return val.tv_sec * 1000 + val.tv_usec / 1000;
}

int main() {
	int T = time1000(), i;
	struct Node *trie = newNode();
	__int64_t STEPS = 100000, STEP = 500000000, key;
	key = 0;
	for (i = 0; i < STEPS; i++) {
		key += STEP;
		add(trie, &key, 8, key+222);
	}
	printf("insert time:%i\n",time1000() - T); T = time1000();
	int err = 0;
	key = 0;
	for (i = 0; i < STEPS; i++) {
		key += STEP;
		struct Node *N = findNode(trie, &key, 8);
		if (N==0 || N->value != key+222) err++;
	}
	printf("find time:%i\n",time1000() - T); T = time1000();
	printf("errors:%i\n", err);
}
