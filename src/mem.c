
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {

	struct head * entete = get_memory_adr(); 
	entete->fit = mem_first_fit; // initialisation de la fonction fit par le first fit
	entete->espace_occupe = NULL; // chaine vide au debut

	struct fb * adr_libre = get_memory_adr() + sizeof(struct head);
	entete->espace_libre = adr_libre;
	adr_libre->taille = get_memory_size() - sizeof(struct head) - sizeof(struct fb); //premier maillon avec le reste de la zone memoire libre
	adr_libre->suivant = NULL;

}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void *mem_alloc(size_t size) {
    /* A COMPLETER */
    return NULL;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void *zone) {
    /* A COMPLETER */
    return;
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
	struct head * entete = get_memory_adr();
	struct fb * libre = entete-> espace_libre;
	struct bb * occupe = entete-> espace_occupe;

	//Parcours des listes chainees, affichage dans l'ordre de la memoire
	while(!(libre == NULL && occupe == NULL)){
		if(libre == NULL){
			print(occupe, occupe->taille, 0);
			occupe = occupe->suivant;
		}
		else if(occupe == NULL){
			print(libre, libre->taille, 1);
			libre = libre->suivant;
		}
		else{  // cas de comparaisons des adresses
			if((void *)occupe < (void *)libre){
				print(occupe, occupe->taille, 0);
				occupe = occupe->suivant;
			}else{
				print(libre, libre->taille, 1);
				libre = libre->suivant;
			}
		}
	}
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t *mff) {
    /* A COMPLETER */
    return;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb *mem_first_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
//-------------------------------------------------------------
struct fb *mem_best_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
//-------------------------------------------------------------
struct fb *mem_worst_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
