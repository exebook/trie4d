namespace trie4d {

#ifdef TRACE_MEM
int mem_count;
const char *trieType = "trie5d, c++, value_t, prefetch, mem_count";
#else
const char *trieType = "trie5d, c++, value_t, prefetch";
#endif

int B = 5;
int bucket_size = 1<<B;

template <typename value_t> class Trie4d {
	struct Node {
		char item;
		value_t value;
		Node **N;
		
		Node() : N(0), value(-1), item(-1) {}
		~Node() {
			if (item == -2) {
				for (int i = 0; i < bucket_size; i++) delete N[i];
				delete[] N;
				#ifdef TRACE_MEM
				mem_count += sizeof(Node) * bucket_size;
				#endif
			}
			else if (item >= 0) delete (Node*)N;
			#ifdef TRACE_MEM
			mem_count += sizeof(Node);
			#endif
		}
	};
	
	Node root;
	
	struct BitScanner {
		char *p; int size, x;
		BitScanner(void *p, int size): p((char*)p), size(size*8), x(0) {}
		int next() {
			if (p == 0) return -1;
			int b = (*(__uint16_t*)&p[(x*5)/8] >> (x*5) % 8) & 31;
			x++;
			if (x*5 > size) {
				p = 0;
				return b & (31 >> (x*5-size));
			}
			return b;
		}
	};

public:
	
	value_t *result;
	
	bool add(void *key, int count) {
		Node *C = &root;
		BitScanner B(key, count);
		while (true) {
			char u = B.next();
			if (u < 0) {
				result = &C->value;
				if (C->value == -1) {
					return false; // value added
				}
				return true; // value replaced
			}
			if (C->item == -1) {
				C->item = u;
				C->N = (Node**) new Node();
				C = (Node*)C->N; // TODO: check if can simply: C = C->N = new Node;
			}
			else if (C->item >= 0) {
				if (C->item == u) C = (Node*)C->N;
				else {
					Node *tmp = (Node*)C->N;
					C->N = new Node*[bucket_size]();
					C->N[C->item] = tmp;
					C->item = -2;
					C = C->N[u] = new Node();
				}
			} else {
				if (!C->N[u]) C->N[u] = new Node();
				C = C->N[u];
			}
		}
	}

	bool find(void *key, int count) {
		Node *C = &root;
		BitScanner B(key, count);
		while (true) {
			char u = B.next();
			if (u < 0) {
				result = &C->value;
				return true;
			}
			if (C->item == -1) return false;
			else if (C->item >= 0) {
				if (u == C->item) C = (Node*)C->N;
				else return false;
			} else {
				__builtin_prefetch(C->N[u]);// <-- optional
				if (!(C = C->N[u])) return false;
			}
		}
	}
};

}