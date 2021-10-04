
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>

void fusion(struct fb * prec, struct fb * cc, struct fb * suiv);

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {

	struct head * entete = get_memory_adr(); 
	entete->fit = mem_worst_fit; // initialisation de la fonction fit par le first fit
	entete->tete_bloc_occupe = NULL; // chaine vide au debut

	struct fb * adr_libre = get_memory_adr() + get_taille_avec_alignement(sizeof(struct head)); //recuperation de l'adresse du premier maillon avec alignement
	entete->tete_bloc_libre = adr_libre;
	adr_libre->taille = get_memory_size() - sizeof(struct head) - sizeof(struct fb); //premier maillon avec le reste de la zone memoire libre
	adr_libre->suivant = NULL;

}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void *mem_alloc(size_t size) {
	
	size = get_taille_avec_alignement(size); //taille a alloue avec alignement 

	struct head * entete = get_memory_adr();
	mem_fit_function_t * strategie = entete->fit; //recuperation de la strategie d'allocation
	
	struct fb * adr_allocation = strategie(entete->tete_bloc_libre,size);
	
	if(adr_allocation == NULL){ // strategie d'allocation retourne null => fragmentation forte
		return NULL;
	}
	else{
		
		size_t taille_bloc = adr_allocation->taille;
		
		//gestion espace restant entre taille_bloc et taille a alloué
		size_t reste = taille_bloc - size;
		if(reste <= get_taille_avec_alignement(sizeof(struct fb))){// ne pas accepter les bloc trop petit pour un maillon ou alors juste assez pour un maillon, malloc(0) ne passe pas
			size = taille_bloc;
		}
		
		// creation du bb avec gestion des listes
		struct fb * tete_libre = entete->tete_bloc_libre;
		struct bb * tete_occupe = entete->tete_bloc_occupe;
		
		struct fb * prec_l = NULL;
		struct fb * suivant_l = adr_allocation->suivant;
		struct bb * prec_o = NULL;
		struct bb * suivant_o = NULL;
		
		//parcours de nos sequence
		
		while(tete_libre != NULL && (void *)tete_libre < (void*) adr_allocation){ //trouver le bloc libre precedant
			prec_l = tete_libre;
			tete_libre = tete_libre->suivant;
		}
		
		while(tete_occupe != NULL && (void *)tete_occupe < (void*)adr_allocation){ //trouver le bloc occupe precedant et suivant par rapport a adr_allocation car on va l'inserer au milieu
			prec_o = tete_occupe;
			tete_occupe = tete_occupe->suivant;
		}
		if(tete_occupe != NULL){suivant_o = tete_occupe;}
		
		//transformation du maillon du bloc alloue en bloc occupe
		
		struct bb * alloue = (struct bb *) adr_allocation;
		//mise a jour de notre chaine des bloc alloues
		alloue->taille = size;
		alloue->suivant = suivant_o;
		if(prec_o != NULL){
			prec_o->suivant = alloue;
		}else{
			entete->tete_bloc_occupe = alloue;
		}
		//mise a jour de notre chaine des bloc libre
		struct fb * nouveau = NULL;
		if(size != taille_bloc){ //mise a jour si besoin d'un nouveau bloc libre qui est le reste du bloc 
			nouveau = (struct fb *)((void *) adr_allocation + get_taille_avec_alignement(sizeof(struct bb)) + size);
			nouveau->taille = taille_bloc - size - get_taille_avec_alignement(sizeof(struct bb));
			//printf("%p   %p/n",(void *)((void *)adr_allocation - get_memory_adr()),(void *)((void *)nouveau- get_memory_adr()));
		}
		if(prec_l != NULL){
			if(nouveau != NULL){
				prec_l->suivant = nouveau;
				nouveau->suivant = suivant_l;
			}else{
				prec_l->suivant = suivant_l;
			}
		}else{
			if(nouveau != NULL){
				entete->tete_bloc_libre = nouveau;
				nouveau->suivant = suivant_l;
			}else{
				entete->tete_bloc_libre = suivant_l;
			}
		}
		
		
		return (void *) adr_allocation + get_taille_avec_alignement(sizeof(struct bb));
	}
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void *zone) {
	struct head * entete = get_memory_adr();
	struct bb * tete_occupe = entete->tete_bloc_occupe;
	struct bb * prec_o = NULL;
	struct bb * suivant_o = NULL;
	zone = zone - get_taille_avec_alignement(sizeof(struct bb));
	
	//placement sur l'adresse
	while(tete_occupe != NULL && (void *)tete_occupe != (void *)zone ){
		prec_o = tete_occupe;
		tete_occupe = tete_occupe->suivant;
	}
	if(tete_occupe == NULL){
		fprintf(stderr,"La zone adressé n'est pas occupe!!!");
	}
	else{
		suivant_o = tete_occupe->suivant;
	
		//parcours de nos zones libres
		struct fb * tete_libre = entete->tete_bloc_libre;
		struct fb * prec_l = NULL;
		struct fb * suivant_l = NULL;
		while(tete_libre != NULL && (void *) tete_libre < zone ){
			prec_l = tete_libre;
			tete_libre = tete_libre->suivant;
		}
		if(tete_libre != NULL){
			suivant_l = tete_libre;
		}
		
		//remplacement de tete_occupe par une zone libre
		struct fb * nouveau = (struct fb *) tete_occupe;
		//printf("%p   %p/n",(void *)((void *)tete_occupe - get_memory_adr()),(void *)((void *)nouveau- get_memory_adr()));
		
		//mise a jour des blocs occupé
		if(prec_o != NULL){
			prec_o->suivant = suivant_o;
		}
		else{
			entete->tete_bloc_occupe = suivant_o;
		}

		if(prec_l == NULL){
			entete->tete_bloc_libre = nouveau;
		}else{
			prec_l->suivant = nouveau;
		}
		nouveau->suivant = suivant_l;
		//appel a fusion
		fusion(prec_l,nouveau,suivant_l);
	}
}

void fusion(struct fb * prec, struct fb * cc, struct fb * suiv){
	if(prec == NULL){
		if(suiv != NULL && (void *) cc + get_taille_avec_alignement(sizeof(struct fb)) + cc->taille == (void *) suiv){ // les adresses se suivent => fusion
			cc->taille = cc->taille + suiv->taille + get_taille_avec_alignement(sizeof(struct fb));
			cc->suivant = suiv->suivant;
		}
	}
	else{
		struct fb * courant = prec;
		struct fb * suivant = cc;
		while(suivant != NULL){ // 2 iterations au max
			if((void *) courant + get_taille_avec_alignement(sizeof(struct fb)) + courant->taille == (void *) suivant){//fusion
				courant->taille = courant->taille + suivant->taille + get_taille_avec_alignement(sizeof(struct fb));
				courant->suivant = suivant->suivant;
				suivant = courant->suivant;
			}
			else{
				courant = suivant;
				suivant = suivant->suivant;
			}
		}
	}
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
	struct head * entete = get_memory_adr();
	struct fb * libre = entete->tete_bloc_libre;
	struct bb * occupe = entete->tete_bloc_occupe;

	//Parcours des listes chainees, affichage dans l'ordre de la memoire
	while(!(libre == NULL && occupe == NULL)){
		if(libre == NULL){
			print((void *)((void *)occupe - get_memory_adr()), occupe->taille, 0);
			occupe = occupe->suivant;
		}
		else if(occupe == NULL){
			print((void *)((void *) libre - get_memory_adr()), libre->taille, 1);
			libre = libre->suivant;
		}
		else{  // cas de comparaisons des adresses
			if((void *)occupe < (void *)libre){
				print((void *)((void *)occupe - get_memory_adr()), occupe->taille, 0);
				occupe = occupe->suivant;
			}else{
				print((void *)((void *) libre - get_memory_adr()), libre->taille, 1);
				libre = libre->suivant;
			}
		}
	}
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t *mff) {
	struct head * entete = get_memory_adr(); 
	mff = entete->fit;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb *mem_first_fit(struct fb *head, size_t size) {
    struct fb *cc = head;	
    while(cc != NULL && cc->taille < size){
    	cc = cc->suivant;
    }
    return cc;
}
//-------------------------------------------------------------
struct fb *mem_best_fit(struct fb *head, size_t size) {
	size_t min = -1; 
	struct fb *cc_min = NULL;
	struct fb *cc = head;	
	while(cc != NULL){
		if( cc->taille >= size && (min == -1 || min > cc->taille)){
			min = cc->taille;
			cc_min = cc;
		}
    		cc = cc->suivant;
	}
	return cc_min;
}
//-------------------------------------------------------------
struct fb *mem_worst_fit(struct fb *head, size_t size) {
	size_t max = -1; 
	struct fb *cc_max = NULL;
	struct fb *cc = head;	
	while(cc != NULL){
		if(cc->taille >= size && ( max == -1 || max < cc->taille ) ){
			max = cc->taille;
			cc_max = cc;
		}
    		cc = cc->suivant;
	}
	return cc_max;
}
