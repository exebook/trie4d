#trie4d

##4-bit trie (Prefix tree) with deferred alloc.

Trie is a sort of dictionary, or key/value storage. Think
of hash tables, binary trees red-black trees, splay trees,
skip lists (sort of) etc. Where a key is generally a string.

### Classic trie.

Normal Trie (a.k.a. Prefix Tree) will split a key in bytes
(or characters) and allocate a node for each byte. Each
node would contain a "bucket" of 256 pointers to sub-nodes
for next characters.

Since this bucket is so big (256*sizeof(pointer)), another
structure is often used to hold it, (like linked list). Which
makes efficient trie a complex piece of code.

### Binary trie.

There also are binary-tries, they have a node for each bit of a key.
I experimented with *binary tries* and found them inneficient since
they allocate a node for every bit, and have to do a pointer
dereference for each bit of a key, with causes a cache miss on 
each step.

My small experiments show that *binary tries* are very slow
compared for 8-bit or 4-bit *tries*. They might have their place
under the sun for starage with very short keys (not strings,
maybe words) and small number of keys.

### My original 4-bit Trie.

I come up with a solution
to slice a key in 4-bit "mini-characters" and thus have
a bucket of 16 pointer size. Which gives surprisingly good 
performance, just a bit slower than trie-256 on lookup,
and many times faster on inserts, and uses tons less memory.

### Deferred bucket allocation.

Since bucket size is so small (just 16 items, remember)
only approx. 25% of buckets contain more than one item,
so additional trick is used: the 16-item bucket is not allocated
until the second item is added to a bucket.

This trick is called "deferring" and gives about 30% speed up
on insertions and saves memory few times (precise number to
be calculated later).

So, overall, trie4d performs good even compared to best hash tables
and that's under 100 lines of code, which makes it a good choice
for a dictionary structure for some applications (think of your
toy programming language, etc.).

### Advantages of trie4d.

As every trie it has insert/lookup time O(K) where K is the length
of the key.

This is different from hash and binary trees where performance
depends on N, which is amount of items inserted.

Hash tables theoretically have O(1) insert/access time, but in practice
they need time to resize main table on insertion and need extra time
on lookup when duplicate hashes make it necessary to iterate over the
items in the bucket.

Main advantage of trie over hash tables is that hash tables require some sophisticated tuning. Their performance highly depends on choosing the
right hash function for your data, and good resize strategy. Both are
done either manually for your particular application or require
sophisticated automation.

But trie4d just works, performing only 1-3 times slower than best hash
implementations out there. And that's in 50-100 lines of code.

### Usage
```cpp
bool trie.add(void *key, int count)
bool trie.find(void *key, int count)
```

Both return true if the key was already in the trie.
To read or write the value of the added/found node use `*trie.result`.

