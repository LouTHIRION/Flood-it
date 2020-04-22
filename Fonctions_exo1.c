#include<stdio.h>
#include "Entete_Fonctions.h"

void trouve_zone_rec(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
	int cl = M[i][j];
	ajoute_en_tete(L, i, j);
	*taille += 1;
	if(i<nbcase-1 && cl == M[i+1][j] && test_est_dans(i+1, j, L)==0) {
		trouve_zone_rec(M, nbcase, i+1, j, taille, L);
	}
	if(i>0 && cl == M[i-1][j] && test_est_dans(i-1, j, L)==0) {
		trouve_zone_rec(M, nbcase, i-1, j, taille, L);
	}
	if(j<nbcase-1 && cl == M[i][j+1] && test_est_dans(i, j+1, L)==0) {
		trouve_zone_rec(M, nbcase, i, j+1, taille, L);
	}
	if(j>0 && cl == M[i][j-1] && test_est_dans(i, j-1, L)==0) {
		trouve_zone_rec(M, nbcase, i, j-1, taille, L);
	}
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff){
  int cpt=0;
  int cl = M[0][0];
  int taille_Zsg = 0;
  int k, l;
  ListeCase *L = malloc(sizeof(ListeCase));
  init_liste(L);
  while(taille_Zsg < dim*dim){
  	taille_Zsg = 0;
  	trouve_zone_rec(M, dim, 0, 0, &taille_Zsg, L);
  	while(cl == M[0][0]) { // tirage de la couleur
  		cl = rand() % nbcl;
  	}
  	while(*L != NULL) { // mis a jour de la grille
  		enleve_en_tete(L, &k, &l); // vidage de la liste
  		M[k][l] = cl;
  		
  		if(aff == 1) {
  			Grille_attribue_couleur_case(G,k,l,cl);
  		}
  	}
	if(aff == 1) {
    	Grille_redessine_Grille();
    }
  	cpt += 1;
  	//SDL_Delay(100);
  }
  return cpt;
}


void trouve_zone_imp(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
	int cl = M[i][j];
	ListeCase *p = (ListeCase *) malloc(sizeof(ListeCase));
	init_liste(p);
	ajoute_en_tete(p, i, j);
	while(!test_liste_vide(p)) {
		enleve_en_tete(p, &i, &j);
		ajoute_en_tete(L, i, j);
		*taille += 1;
		if(i<nbcase-1 && cl == M[i+1][j] && !test_est_dans(i+1, j, L) && !test_est_dans(i+1, j, p)) {
			ajoute_en_tete(p, i+1, j);
		}
		if(i>0 && cl == M[i-1][j] && !test_est_dans(i-1, j, L) && !test_est_dans(i-1, j, p)) {
			ajoute_en_tete(p, i-1, j);
		}
		if(j<nbcase-1 && cl == M[i][j+1] && !test_est_dans(i, j+1, L) && !test_est_dans(i, j+1, p)) {
			ajoute_en_tete(p, i, j+1);
		}
		if(j>0 && cl == M[i][j-1] && !test_est_dans(i, j-1, L) && !test_est_dans(i, j-1, p)) {
			ajoute_en_tete(p, i, j-1);
		}
	}
}


int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff){
  int cpt=0;
  int cl = M[0][0];
  int taille_Zsg = 0;
  int k, l;
  ListeCase *L = malloc(sizeof(ListeCase));
  init_liste(L);
  while(taille_Zsg < dim*dim){
  	taille_Zsg = 0;
  	trouve_zone_imp(M, dim, 0, 0, &taille_Zsg, L);
  	while(cl == M[0][0]) { // tirage de la couleur
  		cl = rand() % nbcl;
  	}
  	while(*L != NULL) { //mis a jour grille
  		enleve_en_tete(L, &k, &l); // vidage de la liste
  		M[k][l] = cl;
  		if(aff == 1) {
  			Grille_attribue_couleur_case(G,k,l,cl);
  		}
  	}
	if(aff == 1) {
    	Grille_redessine_Grille();
    }
  	cpt += 1;
  	//SDL_Delay(1000);
  }
  return cpt;
}

