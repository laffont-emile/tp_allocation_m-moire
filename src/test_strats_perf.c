#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_SIZE 1000
#define MAX_FREE 600

struct location_ptr{
	void *ptrArray[MAX_SIZE];
	int size;
};

typedef struct location_ptr location_ptr;

void init_locPtr(location_ptr *p){
	for(int i = 0; i < MAX_SIZE; i++)
		p->ptrArray[i] = NULL;
	p->size = 0;
}

void addLocation(location_ptr *p, void *location){
	if(!(p->size < MAX_SIZE) || location == NULL) return;

	p->ptrArray[p->size] = location;
	p->size++;
}

int get_free_bytes(int *maxFree){
	int total, Blksize;
    	struct head * h = get_memory_adr();
    	struct fb *freeBlkNode = h->tete_bloc_libre;
    
	*maxFree = 0;
	total = Blksize = 0;

	while(freeBlkNode != NULL){
		Blksize = freeBlkNode->taille;
		if(*maxFree < Blksize) *maxFree = Blksize;
		total += Blksize;
		freeBlkNode = freeBlkNode->suivant;	
    	}

	return total;
}

int get_busy_blocks(){
	int total = 0;
	struct head * h = get_memory_adr();
	struct bb * busyBlkNode = h->tete_bloc_occupe;

	while(busyBlkNode != NULL){
		total += busyBlkNode->taille;
		busyBlkNode = busyBlkNode->suivant;
	}

	return total;
}

int main(int argc, char *argv[]) {
	void *t;
	int freeNbr, blocks[2], totalFree, maxFree, totalBusy, r;
	location_ptr p;

	srand(time(NULL));

	freeNbr = 0;
	blocks[0] = 64; //For small blocks test
	blocks[1] = 512; //For big blocks test
	totalFree = maxFree = totalBusy = 0;

	for(int i = 0; i < (sizeof(blocks)/sizeof(int)); i++){ //Test with different blocks (loop)
		if(i == 0) printf("\n\nTest small blocks\n\n");
		else printf("\n\nTest big blocks\n\n");

		while(freeNbr <= MAX_FREE){
			mem_init();
			init_locPtr(&p);

			//Allocating memory
			do{ 
				r = rand() % blocks[i] + 1;
				if(i == 1) r += 64;
				t = mem_alloc(r);
				addLocation(&p, t);
			}while(t != NULL);

			//Freeing memory
			for(int j = 0; j < freeNbr; j++){
				r = rand() % p.size;
				if(r < p.size && p.ptrArray[r] != NULL) mem_free(p.ptrArray[r]); //Random free
			}
			for(int j = 0; j < freeNbr; j++){
				r = rand() % blocks[i] + 1;
				if(i == 1) r+= 64;
				t = mem_alloc(r);
				addLocation(&p, t);
			}

			totalFree = get_free_bytes(&maxFree);
			printf("totalFree: %d; maxFree: %d\n", totalFree, maxFree);
			printf("Glob fragmentation percentage after freeing %d blocks: %.2f\n", freeNbr, (float)(totalFree - maxFree)/totalFree);
			freeNbr += 20;
		}
		
		freeNbr = 0; //Restart same test with big blocks
	}
	
	return 0;
}
