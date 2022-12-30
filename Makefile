CC=gcc
rbtree.a rbtree.so:
	$(CC) rbtree.c -O3 -c
	ar crs librbtree.a rbtree.o
	$(CC) -O3 -shared -o librbtree.so rbtree.o

.PHONY: clean test

clean:
	rm *.o rm test so-test a-test *.a *.so

test:
	make
	$(CC) rbtree.o test.c -O3 -o test
	./test
	$(CC) test.c -L. -lrbtree -o so-test
	./so-test
	$(CC) test.c -L. -lrbtree -static -o a-test
	./a-test
