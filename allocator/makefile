CC=gcc
CFLAGS=-I. -lm -w
DEPS= allocator.h
OBJ = allocator.o first_fit.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
pa3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


first_fit: allocator.c allocator.h first_fit.c
	gcc -o first_fit allocator.c first_fit.c -lm

next_fit: allocator.c allocator.h next_fit.c
	gcc -o next_fit allocator.c next_fit.c -lm

best_fit: allocator.c allocator.h best_fit.c
	gcc -o best_fit allocator.c best_fit.c -lm

worst_fit: allocator.c allocator.h worst_fit.c
	gcc -o worst_fit allocator.c worst_fit.c -lm
