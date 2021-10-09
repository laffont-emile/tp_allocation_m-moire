#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(ALIGNEMENT)
#define ALIGNEMENT 4
#endif

void test_entete(){
  debug("test alignement entete\n");
  mem_init();
  struct head * entete = get_memory_adr(); 
	struct fb* tete_libre = entete->tete_bloc_libre;
  size_t taille = (size_t) tete_libre - (size_t) entete;
  assert(taille%ALIGNEMENT == 0);
}

void test_maillon(){
  debug("test alignement maillon\n");
  mem_init();
  mem_alloc(59);
  void * adr_l = mem_alloc(59);
  mem_alloc(59);
  mem_free(adr_l);
  
  struct head * entete = get_memory_adr(); 
  struct bb* tete_o = entete->tete_bloc_occupe; // 1 er bloc alloue
  struct fb* tete_l = entete->tete_bloc_libre; // 2eme bloc alloue puis liberer
  struct bb* tete_o_suiv = tete_o->suivant;
  
  fprintf(stderr,"%d\n",ALIGNEMENT);
  assert(tete_o->taille%ALIGNEMENT == 0);
  assert(tete_l->taille%ALIGNEMENT == 0);
  
  assert(((size_t)tete_l - (size_t)tete_o) %ALIGNEMENT == 0);
  assert(((size_t)tete_o_suiv - (size_t)tete_l) %ALIGNEMENT == 0);
}


int main(int argc, char *argv[]) {
  debug("test alignement\n");
  test_entete();
  test_maillon();
  debug("test OK\n");
}