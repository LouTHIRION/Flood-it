/* biblio_graphe.h */
#ifndef _BIBLIO_GRAPHE_
#define _BIBLIO_GRAPHE_

#include<stdio.h>
#include<stdlib.h>
#include"Liste_case.h"
#include"Entete_Fonctions.h"

typedef struct sommet Sommet ;
/* Element d’une liste cha\^in\’ee de pointeurs sur Sommets */
typedef struct cellule_som {
	Sommet * sommet ;
	struct cellule_som * suiv ;
} Cellule_som ;


struct sommet {
	int num ; /* Numero du sommet (sert uniquement a l’affichage) */
	int cl ; /* Couleur d’origine du sommet-zone */
	ListeCase cases ; /* Listes des cases du sommet-zone */
	int nbcase_som ; /* Nombre de cases de cette liste */

	Cellule_som * sommet_adj ; /* Liste des ar\eteses pointeurs sur les sommets adjacents */
};

typedef struct graphe_zone {
	int nbsom ; /* Nombre de sommets dans le graphe */
	Cellule_som * som ; /* Liste chainee des sommets du graphe */
	Sommet *** mat ; /* Matrice de pointeurs sur les sommets indiquant a quel sommet appartient une case (i,j) de la grille */
} Graphe_zone;

/* ajoute un pointeur sur un sommet a une liste chainee de Cellule_som */
void ajoute_liste_sommet(Sommet *S, Cellule_som *L); 

/* detruit une liste chainee de Cellule_som SANS detruire les sommets pointes par cette liste */
void detruit_liste_sommet(Cellule_som *L);

/* met a jour deux sommets s1 et s2 en indiquant qu'ils sont adjacents l'un a l'autre */
void ajoute_voisin(Sommet *s1, Sommet *s2);

/* renvoie vrai si et seulement si deux sommets sont adjacents */
int adjacents(Sommet *s1, Sommet *s2);

/*creer le graphe tout entier */
int cree_graphe_zone(int **M, int dim);

#endif

