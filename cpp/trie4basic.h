/*
	This is minimalistic representation, only 50 LOC, no "deferring allocation".
	This is mostly useful for learning how 4-bit trie works in general.
	Use trie4d.h for your application instead.
*/

struct Node {
	Node *N[16];
	__int64_t value;
	Node() : N(), value(-1) { }
	~Node() {
		for (int i = 0; i < 16; i++) delete N[i];
	}
};

struct Trie16 {
	Node root;
	
	struct BitScanner { // separate a key in 4 bit chunks
		void *p; 
		int count, pos;
		BitScanner (void *p, int count) : p(p), count(count), pos(0) {}
		int next() {
			int bpos = pos >> 1;
			if (bpos >= count) return -1; // end of key
			unsigned char b = ((unsigned char*)p)[bpos];
			if (pos++ & 1) return (b >>= 4);
			return b & 0xf;
		}
	};

	bool add(void *key, int count, __int64_t value) {
		Node *C = &root;
		BitScanner B(key, count);
		while (true) {
			char u = B.next();
			if (u < 0) {
				if (C->value == -1) {
					C->value = value;
					return true; // value added
				}
				C->value = value;
				return false; // value replaced
			}
			if (C->N[u]) C = C->N[u];
			else C = C->N[u] = new Node;
		}
	}

	Node* findNode(void *key, int count) {
		Node *C = &root;
		BitScanner B(key, count);
		while (true) {
			char u = B.next();
			if (u < 0) return C;
			__builtin_prefetch(C->N[u]);
			if (!(C = C->N[u])) return 0;
		}
	}
};
