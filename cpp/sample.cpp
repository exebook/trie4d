#include <iostream>
#include "sys/time.h"
#define TRACE_MEM
#include "trie4d.h"

using namespace std;

long time1000() {
  timeval val;
  gettimeofday(&val, 0);
  val.tv_sec &= 0xffff;
  return val.tv_sec * 1000 + val.tv_usec / 1000;
}

template <typename KEYTYPE, typename VALTYPE>
void test(int STEPS, int KEYMOD, int KEYSIZE, int reads) {
	int err, Timer;
	KEYTYPE key;
	
	// wrapper block to test destruction
	
	{
		trie4d::Trie4d<VALTYPE> trie;
		#ifdef TRACE_MEM
		trie4d::mem_count = 0;
		#endif
		cout
			<< "insert/lookup "
			<< STEPS 
			<< " key/value pairs, sizeof(key)="
			<< KEYSIZE 
			<< ", sizeof(value)="
			<< sizeof(VALTYPE) 
			<< endl;
			
		Timer = time1000();
		key = 0;
		for (int i = 0; i < STEPS; i++) {
			key += KEYMOD;
			bool existed = trie.add(&key, KEYSIZE);
			*trie.result = i;
		}
		cout << "insertion: " << time1000() - Timer << " ms" << endl;
		
		Timer = time1000();
		for (int j = 0; j < reads; j++) {
			key = err = 0;
			for (int i = 0; i < STEPS; i++) {
				key += KEYMOD;
				bool ok = trie.find(&key, KEYSIZE);
				if (!ok || *trie.result != i) err++;
			}
		}
		cout
			<< "lookup all items "<< reads <<"x: " 
			<< time1000() - Timer <<" ms"<< endl;
		cout << "error count: " << err << endl;
		Timer = time1000();
	}
	cout << "free memory (ms): " << time1000() - Timer << endl;
	#ifdef TRACE_MEM
	cout << "memory used: " << trie4d::mem_count << " bytes"<<endl;
	#endif
	cout << endl;
}

int main() {
	test<int, unsigned short>(65536, 1, 2, 50);
	test<int, unsigned short>(65536, 100, 3, 50);
	test<__int64_t, int>(1000000, 500000000, 8, 1);
}
