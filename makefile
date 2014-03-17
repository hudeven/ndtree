GCC=g++
NDT: main.o utility.o logClass.o
	g++ -g -o ndTree main.o utility.o logClass.o
	rm *.o
main.o: main.cpp config.h Dir_entry.h Dir_node.h Leaf_entry.h Leaf_node.h logClass.h ND_tree.h Node.h utility.h
	g++ -g -c main.cpp
utility.o: utility.cpp utility.h
	g++ -g -c utility.cpp
logClass.o: logClass.cpp logClass.h
	g++ -g -c logClass.cpp
clean:
	rm -f *.o
