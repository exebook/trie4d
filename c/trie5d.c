/*
	Created for experiment.
	5 bit trie does not seem to give any advantage over 4-bit.
	Uses more memory and does not show any obvious performance advantage.
	But maybe there is room for inventions.
*/
#define DEBUG

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define VALUE_TYPE __int64_t

const int 
	bitsize = 5,
	bucket_size = (1 << bitsize);

struct Node {
	struct Node **N;
	VALUE_TYPE value;
	char item;
};

struct Node* newNode() {
	struct Node *node;
	node = malloc(sizeof (struct Node));
	node->value = -1;
	node->item = -1;
	return node;
}

int addNode(struct Node *C, void *_key, int count, VALUE_TYPE value) {
	int shift, b = 0;
	struct Node* tmp;
	unsigned char u, *key = (unsigned char *)_key;
	while (b < count * (8 / bitsize)) {
		shift = (8 - bitsize - (b % (8 / bitsize) * bitsize));
		u = (key[b++ / (8 / bitsize)] & (0xff >> (8 - bitsize) << shift)) >> shift;
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
				C->N = malloc(sizeof (void*) * bucket_size);
				memset(C->N, 0, sizeof(void*) << bitsize);
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
	if (C->value == -1) {
		C->value = value;
		return 1; // value added
	}
	C->value = value;
	return 0; // value replaced
}

struct Node* findNode(struct Node * C, void *_key, int count) {
	int shift, b = 0;
	unsigned char u, *key = (unsigned char *)_key;
	while (b < count * (8 / bitsize)) {
		shift = (8 - bitsize - (b % (8 / bitsize) * bitsize));
		u = (key[b++ / (8 / bitsize)] & (0xff >> (8 - bitsize) << shift)) >> shift;
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
	return C;
}

#ifdef DEBUG
int mem_count = 0;
#endif

void freeNode(struct Node* C) {
	int i;
	if (C->item == -2) {
		for (i = 0; i < bucket_size; i++) {
			if (C->N[i]) freeNode(C->N[i]);
		}
		free(C->N);
		#ifdef DEBUG
		mem_count += bucket_size * sizeof(struct Node);
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
		addNode(trie, &key, 8, key+222);
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
	#ifdef DEBUG
	freeNode(trie);
	printf("memory used: %i\n", mem_count);
	#endif
}
