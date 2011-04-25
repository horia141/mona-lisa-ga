all:
	gcc -g -o mlga -L./lib -lsnt.debug -lpthread -lglut gastuff.c main.c

ultra:
	gcc -O3 -o mlga -DNDEBUG -L./lib -lsnt -lpthread -lglut gastuff.c main.c
