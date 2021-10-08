#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int structure_libre(){
	struct head * entete = get_memory_adr(); 
	struct fb* tete_libre = entete->tete_bloc_libre;
	int  i = 0;
	while(tete_libre != NULL){
		tete_libre = tete_libre->suivant;
		i++;
	}
	return i;
}

int structure_occupe(){
	struct head * entete = get_memory_adr(); 
	struct bb * tete_occupe = entete->tete_bloc_occupe;
	int  i = 0;
	while(tete_occupe != NULL){
		tete_occupe = tete_occupe->suivant;
		i++;
	}
	return i;
}

void test_initialisation(){
	debug("test structure lors de l'initialisation (mem_init) \n");
	mem_init();
	struct head * entete = get_memory_adr(); 
	assert(entete->tete_bloc_occupe == NULL);
	assert(entete->fit == mem_first_fit);
	struct fb* tete_libre = entete->tete_bloc_libre;
	assert(tete_libre != NULL);
	assert((void *) tete_libre == get_memory_adr() + get_taille_avec_alignement(sizeof(struct head)));
	
	assert(tete_libre->taille  == (get_memory_size() - get_taille_avec_alignement(sizeof(struct head)) - get_taille_avec_alignement(sizeof(struct fb))));
	assert(tete_libre->suivant == NULL);
}

void test_allocation_liberation(){
	debug("test structure lors de sequence d'allocation et de liberation \n");
	
	mem_init();
	void * r = mem_alloc(10);
	assert(r != NULL);
	
	assert(structure_libre() == 1);
	assert(structure_occupe() == 1);
	
	mem_free(r);
	assert(structure_occupe() == 0);
	
	struct head * entete = get_memory_adr(); 
	struct fb* tete_libre = entete->tete_bloc_libre;
	assert(tete_libre != NULL);
	assert((void *) tete_libre == get_memory_adr() + get_taille_avec_alignement(sizeof(struct head)));
	
	int i = 0;
	void *allocs[100];
	while(i != 100 && (allocs[i] = mem_alloc(10)) != NULL){
		i++;
	}
	assert(i == 100);
	assert(structure_occupe() == 100);
	assert(structure_libre() == 1);
	
	mem_free(allocs[98]);
	assert(structure_libre() == 2);
	assert(structure_occupe() == 99);
	
	mem_free(allocs[96]);
	mem_free(allocs[97]);
	assert(structure_libre() == 2);
	
	i = 0;
	while(i != 100){
		if(i != 96 && i != 97 && i != 98){
			mem_free(allocs[i]);
		}
		i++;
	}
	
	assert(structure_libre() == 1);
	assert(structure_occupe() == 0);
	entete = get_memory_adr(); 
	tete_libre = entete->tete_bloc_libre;
	assert(tete_libre != NULL);
	assert((void *) tete_libre == get_memory_adr() + get_taille_avec_alignement(sizeof(struct head)));
}







int main(int argc, char *argv[]) {
	debug("test de nos structures de controle avec l'alignement\n");
	test_initialisation();
	test_allocation_liberation();
	debug("test OK\n ");
}
