CC=gcc

FLAGS=-c -std=c99

all: generate create_rb create_treap create_fib

generate: gen_test.o
	gcc gen_test.o -o gen_test
	./gen_test

create_rb: rb_tree.o
	$(CC) rb_tree.o -o rb_tree -lm

create_treap: treap.o
	$(CC) treap.o -o treap -lm

create_fib: fib_heap.o
	$(CC) fib_heap.o -o fib_heap -lm

gen_test.o: gen_test.c
	$(CC) $(FLAGS) gen_test.c 

rb_tree.o: rb_tree.c
	$(CC) $(FLAGS) rb_tree.c 

treap.o: treap.c
	$(CC) $(FLAGS) treap.c

fib_heap.o: fib_heap.c
	$(CC) $(FLAGS) fib_heap.c

clean:
	rm -rf *.o generate create_fib create_treap create_rb
