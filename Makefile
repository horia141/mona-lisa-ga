all:
	gcc -g -o mlga -lglut common.c image.c gastuff.c main.c

ultra:
	gcc -O3 -o mlga -DNDEBUG -lglut common.c image.c gastuff.c main.c
