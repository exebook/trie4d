#define DEBUG
#include "trie4d.h"
#include "sys/time.h"
#include <stdio.h>

int time1000() {
  struct timeval val;
  gettimeofday(&val, 0);
  val.tv_sec &= 0xffff;
  return val.tv_sec * 1000 + val.tv_usec / 1000;
}

int mem_count;

void test(int STEPS, int STEP, int KEYSIZE) {
	int i, err;
	__int64_t key;
	struct Node *trie = newNode();
	mem_count = 0;
	printf("insert/lookup %i key/value pairs, sizeof(key)=%i, sizeof(value)=%i\n", STEPS, KEYSIZE, sizeof(trie->value));
	int T = time1000();
	key = 0;
	for (i = 0; i < STEPS; i++) {
		key += STEP;
		add(trie, &key, KEYSIZE, i);
	}
	printf("insert time:%i\n",time1000() - T); T = time1000();
	err = 0;
	key = 0;
	for (i = 0; i < STEPS; i++) {
		key += STEP;
		struct Node *N = findNode(trie, &key, KEYSIZE);
		if (N==0 || N->value != i) err++;
	}
	printf("find time:%i\n",time1000() - T); T = time1000();
	printf("errors:%i\n", err);
	freeNode(trie);
	printf("memory used:%i\n",mem_count);
	printf("free memory time:%i\n",time1000() - T);
	printf("\n");
}

int main() {
	test(65535, 100, 3);
	test(1000000, 500000000, 8);
}
