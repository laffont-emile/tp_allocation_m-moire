#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ALLOC 500

void test_allocation_basique() {
	mem_init();

	debug("allocation de taille 0 ou trop grande ou taille negative\n");
	void * resultat;
	assert((resultat = mem_alloc(0)) == NULL); //verification de alloc 0, ne doit pas marcher 
	
	size_t n = get_memory_size();
	assert((resultat = mem_alloc(n)) == NULL); //ne doit pas marcher car la taille des structures prend un bout de memoire
	
	assert((resultat = mem_alloc(1000000000)) == NULL); //ne doit pas marcher car la taille des structures prend un bout de memoire
	
	assert((resultat = mem_alloc(-14)) == NULL); //verification de alloc 0, ne doit pas marcher 
	
	debug("allocation simple\n");
	resultat = mem_alloc(10);
	assert(resultat != NULL);
	
	resultat = mem_alloc(50);
	assert(resultat != NULL);
}

void test_memoire_allocation_plein(){
	mem_init();
	debug("alocation de la taille max de la memoire libre\n");
	struct head * entete = get_memory_adr(); 
	assert(entete != NULL);
	struct fb * maillon = entete->tete_bloc_libre;
	assert(maillon != NULL);
	size_t n = maillon->taille;
	
	void * r = mem_alloc(n);
	assert(r != NULL);
}

void test_allocation_aleatoire(){
	mem_init();
	void *allocs[MAX_ALLOC];
	int i = 0;

	debug("alocation multiple de meme taille\n");
	while (i < MAX_ALLOC && (allocs[i] = mem_alloc(10)) != NULL) {
        	i++;
    	}
	assert(i == MAX_ALLOC);
	
	debug("alocation multiple de taille differente\n");
	while (i < MAX_ALLOC && (allocs[i] = mem_alloc(i)) != NULL) {
        	i++;
    	}
	assert(i == MAX_ALLOC);
}



int main(int argc, char *argv[]) {
	mem_init();
	test_allocation_basique();
	test_memoire_allocation_plein();
	test_allocation_aleatoire();
	debug("test OK\n");

}
