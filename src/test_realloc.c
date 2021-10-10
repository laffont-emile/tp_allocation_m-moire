#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REALLOC 200

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

void test_copie(){
  mem_init();
  debug("test copie du bloc lors de reallocation zone trop petite mais existence d'un autre bloc plus grand ailleur\n");
  void * a = mem_alloc(500);
  mem_alloc(500);
  int * x = (int *) a;
  *x = 41558663;
  int * y = x + 1;
  *y = 73985357;
  a = mem_realloc(a, 5000);
  assert(a != NULL);
   x = (int *) a;
   y = x + 1;
   assert(*x == 41558663);
   assert(*y == 73985357);
}

void test(){
  mem_init();
  debug("test realloc trop grand / memoire\n");
  void * a = mem_alloc(100);
  void * b = mem_realloc(a,get_memory_size()*2);
  assert(b == NULL);
  assert(structure_occupe() == 1 && structure_libre() == 1);
  
  debug("test realloc(adr,0) soit le free\n");
  b = mem_realloc(a,0);
  assert(b == NULL);
  assert(structure_occupe() == 0 && structure_libre() == 1);
}

void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
}

void test_nombreux_reallocation(){
  mem_init();
	void *allocs[MAX_REALLOC];

  int i = 1;
  debug("alocation multiple de meme taille\n");
	while (i < MAX_REALLOC && (allocs[i] = mem_alloc(i)) != NULL) {
        	i++;
	}
	assert(i == MAX_REALLOC);
	
   i--;
	debug("reallocation multiple de taille differente\n");
	while (i != 0 && (allocs[i] = mem_realloc( allocs[i] , MAX_REALLOC - i)) != NULL) {
        	i--;
	}	
  assert(i == 0);
  //check_memoire();
}


int main(int argc, char *argv[]) {
  debug("test realloc\n");
  test_copie();
  test();
  test_nombreux_reallocation();
  debug("test OK\n");
}