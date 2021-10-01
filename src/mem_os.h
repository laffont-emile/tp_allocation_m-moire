#if !defined(mem_os_h)
#define mem_os_h

/* -----------------------------------------------*/
/* Interface de gestion de votre allocateur       */
/* -----------------------------------------------*/
// Initialisation
void mem_init(void);

// Définition du type mem_fit_function_t
// type des fonctions d'allocation
typedef struct fb *(mem_fit_function_t)(struct fb *, size_t);

// Choix de la fonction d'allocation
// = choix de la stratégie de l'allocation
void mem_fit(mem_fit_function_t *);

// Stratégies de base (fonctions) d'allocation
mem_fit_function_t mem_first_fit;
mem_fit_function_t mem_worst_fit;
mem_fit_function_t mem_best_fit;

struct head{
	mem_fit_function_t * fit;
	struct fb * tete_bloc_libre;
	struct bb * tete_bloc_occupe;
};

struct fb{
	size_t taille;
	struct fb * suivant;
};

struct bb{
	size_t taille;
	struct bb * suivant;
};

#endif /* mem_os_h */
