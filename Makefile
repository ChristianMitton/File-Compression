all: fileCompressor huffman tokenizer

fileCompressor: fileCompressor.c huffman.h huffman.c hashtable.h hashtable.c tokenizer.h tokenizer.c
	gcc -g -c huffman.h huffman.c hashtable.h hashtable.c tokenizer.h tokenizer.c
	gcc -g -o fileCompressor fileCompressor.c huffman.o hashtable.o tokenizer.o

huffman: huffman.c huffman.h hashtable.h hashtable.c tokenizer.c tokenizer.h huffmanTest.c
	gcc -c -g huffman.h huffman.c hashtable.h hashtable.c tokenizer.h tokenizer.c
	gcc -o huffmanTest huffman.o hashtable.o tokenizer.o huffmanTest.c

hashtable: hashTableTest.c hashtable.h hashtable.c
	gcc -c -g hashtable.h hashtable.c
	gcc -o hashtableTest hashtable.o hashTableTest.c

tokenizer: tokenizerTest.c tokenizer.c tokenizer.h hashtable.h hashtable.c
	gcc -c -g tokenizer.h tokenizer.c hashtable.h hashtable.c
	gcc -o tokenizerTest tokenizer.o tokenizerTest.c hashtable.o

clean:
	rm -rf fileCompressor
	rm -rf hashtableTest
	rm -rf tokenizerTest
	rm -rf huffmanTest
	rm -rf huffman.o tokenizer.o hashtable.o
	rm -rf huffman.h.gch tokenizer.h.gch hashtable.h.gch
