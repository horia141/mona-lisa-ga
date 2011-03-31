all:
	gcc -g -o mlga -lpthread -lglut common.c image.c gastuff.c main.c

ultra:
	gcc -O3 -o mlga -DNDEBUG -lpthread -lglut common.c image.c gastuff.c main.c
