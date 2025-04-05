#ifndef ALLOCATOR_H__
#define ALLOCATOR_H__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include <math.h>
#define MAX_N (1<<28)
#define NEXT 2
#define PREV 1
#define MAX_BLOCK_COUNT 1000
#define error(m,c) do{fprintf(stderr, "%s\n",m);exit(c);}while(0)
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))
extern int n, d, v, x;
extern int memory[MAX_N];
extern int number_of_examined_holes;
extern int next_request[MAX_BLOCK_COUNT];//sequence of random request sizes
typedef struct{
	int released;//one means released, zero means occupied.
	int address;//index of first cell in memory array
	int size;//size of block (excluding the four cells used for metadata
}block;//structure that represents one block
extern block blocks[MAX_BLOCK_COUNT];//storing all blocks
void simulate_first_fit(void);
void release(int block_index);
void occupy(int hole_addr, int block_size, int block_id);
#endif