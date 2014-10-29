CC=g++
CFLAGS=-g -c
ndTree: main.o utility.o logClass.o
	$(CC) -g -o ndTree main.o utility.o logClass.o
main.o: main.cpp config.h Dir_entry.h Dir_node.h Leaf_entry.h Leaf_node.h logClass.h ND_tree.h Node.h utility.h
	$(CC) $(CFLAGS) main.cpp
utility.o: utility.cpp utility.h
	$(CC) $(CFLAGS) utility.cpp
logClass.o: logClass.cpp logClass.h
	$(CC) $(CFLAGS) logClass.cpp
clean:
	rm -f *.o
