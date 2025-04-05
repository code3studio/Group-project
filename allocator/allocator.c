#include"allocator.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void simulate();

int n, d, v, x;
int memory[MAX_N];
int number_of_examined_holes;
block blocks[MAX_BLOCK_COUNT];//storing all blocks
int next_request[MAX_BLOCK_COUNT];//sequence of random request sizes
static void generate_all_request_sizes(){
	srand(time(NULL));
	int request_size;
	for(int i = 0; i < MAX_BLOCK_COUNT;i++){
		do{//randomly generate the size of the next request
			request_size = (int)round(nrand()*v + d);
		}while(request_size < 1 || request_size > n - 9);
		next_request[i] = request_size;
	}	
}
static void handle_args(int argc, char* argv[]){
	if(argc != 9)
		error("memory allocation simulator: too few arguments!", 1);
	//-n [mem_size] -d [request_size_mean] -v [request_size_stddev] -x [rounds]
	while(*++argv){
		if(!strcmp(*argv, "-n"))
			n = atoi(*++argv);//memory size
		else if(!strcmp(*argv, "-d"))
			d = atoi(*++argv);//mean of request sizes
		else if(!strcmp(*argv, "-v"))
			v = atoi(*++argv);//stddev of request sizes
		else if(!strcmp(*argv, "-x"))
			x = atoi(*++argv);//# of rounds/failed requests
	}
}
static void add_hole(int start_addr, int end_addr){
	memory[start_addr] = -memory[start_addr];
	memory[end_addr] = -memory[end_addr];
	int next_hole = 0;//looking for the address of next hole
	int temp = end_addr + 1;
	while(1){
		if(temp >= n)
			temp = 0;
		else if(memory[temp] < 0){
			next_hole = temp;
			break;//found the next hole
		}
		else//move forward
			temp += (memory[temp] + 4);
	}
	//inserting the hole into the current doubly linked list of holes!
	memory[start_addr + NEXT] = next_hole;//current_hole->next = next_hole
	int prev_hole = memory[next_hole + PREV];//prev_hole = next_hole->prev
	memory[start_addr + PREV] = prev_hole;//current_hole->prev = prev_hole
	memory[next_hole + PREV] = start_addr;//next_hole->prev = current_hole
	memory[prev_hole + NEXT] = start_addr;//prev_hole->next = current_hole
}
static void left_coalesce(int start_addr, int end_addr){
	int left_end_addr = start_addr - 1;
	int left_start_addr = left_end_addr - 3 + memory[left_end_addr];	
	printf("left_coalesce: combining @[%d, %d] and @[%d, %d].\n", 
		left_start_addr, left_end_addr, start_addr, end_addr);
	memory[left_start_addr] -= (end_addr - start_addr + 1);
	memory[end_addr] = memory[left_start_addr];
}
static void right_coalesce(int start_addr, int end_addr){
	int right_start_addr = end_addr + 1;
	int right_end_addr = right_start_addr + 3 - memory[right_start_addr];	
	printf("right_coalesce: combining @[%d, %d] and @[%d, %d].\n", 
		start_addr, end_addr, right_start_addr, right_end_addr);
	memory[right_end_addr] -= (end_addr - start_addr + 1);
	memory[start_addr] = memory[right_end_addr];
	int prev_hole, next_hole;
	memory[start_addr + PREV] = prev_hole = memory[right_start_addr + PREV];
	memory[start_addr + NEXT] = next_hole = memory[right_start_addr + NEXT];
	memory[prev_hole + NEXT] = start_addr;
	memory[next_hole + PREV] = start_addr;
}
static void double_coalesce(int start_addr, int end_addr){
	error("double_coalesce not implemented!", 5);
}
void release(int block_index){
	blocks[block_index].released = 1;
	int start_addr = blocks[block_index].address;
	int size = blocks[block_index].size;
	int end_addr = start_addr + size + 3;
	int left_neighbor = start_addr == 0? 0: memory[start_addr - 1];
	int right_neighbor = (end_addr == n - 1)? 0: memory[end_addr + 1];
	printf("releasing block#%d @[%d, %d].LEFT=%d, RIGHT=%d.\n",
		block_index, start_addr, end_addr, left_neighbor, right_neighbor);
	if(left_neighbor >= 0 && right_neighbor >= 0)//neither neighbors are holes
		add_hole(start_addr, end_addr);//new hole is created
	else if(left_neighbor < 0 && right_neighbor >= 0)//left neighbor is a hole!
		left_coalesce(start_addr, end_addr);//current hole and left neighbor coalesced into one hole
	else if(left_neighbor >= 0 && right_neighbor < 0)//right neighbor is a hole!
		right_coalesce(start_addr, end_addr);//current hole and right neighbor coalesced into one hole
	else//both neighbors are holes!
		double_coalesce(start_addr, end_addr);//current hole and its both neighbors coalesced into one hole
}
void occupy(int hole_addr, int block_size, int block_id){
	int hole_size = -memory[hole_addr];
	int new_hole_size = hole_size - block_size - 4;//remaining hole size
	int block_addr = hole_addr + new_hole_size + 4; 
	//update hole_size
	memory[hole_addr] = memory[block_addr - 1] = -new_hole_size;
	//place block_size at the beginning and end of the block
	memory[block_addr] = block_size;//first cell
	memory[block_addr + block_size + 3] = block_size;//final cell
	blocks[block_id].released = 0;//store new block in the list
	blocks[block_id].address = block_addr;//store new block in the list
	blocks[block_id].size = block_size;//store new block in the list
}
int main(int argc, char* argv[]){
	int request_size;
	handle_args(argc, argv);
	generate_all_request_sizes();
	simulate();
	//simulate();
	//simulate();
	//simulate();
}
