#include "common.h"

// Vous pouvez changer la taille de la mémoire ici,
// si vous n'utilisez pas l'option -DMEMORY_SIZE=...
// lors de la compilation
#if !defined(MEMORY_SIZE)
#define MEMORY_SIZE 4096
#endif

//allignement
#if !defined(ALIGNEMENT)
#define ALIGNEMENT 4
#endif

static char memory[MEMORY_SIZE];

void *get_memory_adr() { return memory; }

size_t get_memory_size() { return MEMORY_SIZE; }

size_t get_taille_avec_alignement(size_t n){
	if(n % ALIGNEMENT == 0){
		return n;
	}else {
		return n + (n % ALIGNEMENT);
	}
	return n;
}
