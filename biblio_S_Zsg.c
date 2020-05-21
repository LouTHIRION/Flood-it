/* biblio_S_Zsg.c */

#include "biblio_S_Zsg.h"


/* initialistation S_Zsg, renvoie 0 si tout est bon et 1 sinon */
int init_Zsg(S_Zsg *S, int dim, int nbcl) {
	int i, j;
	S->dim = dim;
	S->nbcl = nbcl;
	S->Lzsg = NULL;
	S->B = (ListeCase *)malloc(nbcl * sizeof(ListeCase));
	for(i = 0; i<nbcl; i++) {
		S->B[i] = NULL;
	}
	S->App =(int **) malloc(sizeof(int*)*dim);
	for(i = 0; i<dim; i++) {
		S->App[i] = (int *)malloc(dim * sizeof(int));
		if(S->App[i] == 0) {
			printf("Pas assez d'espace mémoire disponible\n");
			return 1;
		}
		for(j=0; j<dim; j++) {
			S->App[i][j] = -2;
		}
	}
	return 0;
}

/* ajoute une case a Lzsg */
void ajoute_Zsg(S_Zsg *S, int i, int j) {
	ajoute_en_tete(&S->Lzsg, i, j);
	S->App[i][j] = -1;
}

/* ajoute une case dans la bordure, d'une couleur cl donnee */
void ajoute_Bordure(S_Zsg *S, int i, int j, int cl) {
	ajoute_en_tete(&S->B[cl], i, j);
	S->App[i][j] = cl;
}

/* renvoie vrai si une case est dans LZsg */
int appartient_Zsg(int i, int j, S_Zsg *S) {
	return S->App[i][j] == -1;
}

/* renvoie vrai si une case est dans la bordure de couleur cl donnee */
int appartient_Bordure(int i, int j, int cl, S_Zsg *S) {
	return S->App[i][j] != -1 && S->App[i][j] != -2;
}

/* detruit S en librerant toutes les cases memoires */
void detruit_S_Zsg(S_Zsg *S) {
	int i;
	detruit_liste(&S->Lzsg);
	for(i = 0; i<S->nbcl; i++) {
		detruit_liste(&S->B[i]);
	}
	free(S->App);
	free(S);
}

/* met a jour les champs Lzsg et B de Z lorsqu'une case k,l de couleur cl 
   doit basculer dans Lzsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l) {
	int dim = Z->dim;
	int nbCases = 0;
	int i,j;
	ListeCase *p = (ListeCase *) malloc(sizeof(ListeCase));
	init_liste(p);
	if(!appartient_Zsg(k, l, Z)) {
		ajoute_en_tete(p, k, l);
	}
	/* parcour la grille pour mettre a jour la Zsg */
	while(!test_liste_vide(p)) {
		enleve_en_tete(p, &i, &j);
		ajoute_Zsg(Z, i, j);
		nbCases += 1;
		
		if(i<dim-1 && !test_est_dans(i+1, j, p)) {// pour chacune des cases qui n'ont pas encore ete visitee
			if(cl == M[i+1][j] && !appartient_Zsg(i+1, j, Z)) { // soit elle est de couleur cl
				ajoute_en_tete(p, i+1, j); // alors on l'ajoute a la Zsg
			}
			else if(cl != M[i+1][j] && !appartient_Bordure(i+1, j, M[i+1][j], Z)) { // soit elle est d'une autre couleur
				ajoute_Bordure(Z, i+1, j, M[i+1][j]); // alors on l'ajoute a la bordure
			}
		}
		
		if(i>0 && !test_est_dans(i-1, j, p)) {
			if(cl == M[i-1][j] && !appartient_Zsg(i-1, j, Z)) {
				ajoute_en_tete(p, i-1, j);
			}
			else if(cl != M[i-1][j] && !appartient_Bordure(i-1, j, M[i-1][j], Z)) {
				ajoute_Bordure(Z, i-1, j, M[i-1][j]);
			}
		}
		
		if(j<dim-1 && !test_est_dans(i, j+1, p)) {
			if(cl == M[i][j+1] && !appartient_Zsg(i, j+1, Z)) {
				ajoute_en_tete(p, i, j+1);
			}
			else if(cl != M[i][j+1] && !appartient_Bordure(i, j+1, M[i][j+1], Z)) {
				ajoute_Bordure(Z, i, j+1, M[i][j+1]);
			}
		}
		
		if(j>0 && !test_est_dans(i, j-1, p)) {
			if(cl == M[i][j-1] && !appartient_Zsg(i, j-1, Z)) {
				ajoute_en_tete(p, i, j-1);
			}
			else if(cl != M[i][j-1] && !appartient_Bordure(i, j-1, M[i][j-1], Z)) {
				ajoute_Bordure(Z, i, j-1, M[i][j-1]);
			}
		}
	}
	return nbCases;
}
	
/* Algo tirant au sort une couleur : 
   il utilise la fonction agrandit_zone pour determiner la Zsg */
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff) {
	int taille_Zsg;
	int cl = rand() % nbcl;
	int i;
	int cpt = 0;
	ListeCase Case_bordure;
	ListeCase Case_zone;
	S_Zsg *S = (S_Zsg *)malloc(sizeof(S_Zsg));
	init_Zsg(S, dim, nbcl);
	taille_Zsg = agrandit_Zsg(M, S, M[0][0], 0, 0);
	
	while(taille_Zsg < dim*dim) {
	
		/* tirage de la couleur */
		while(cl == M[0][0]) {
  			cl = rand() % nbcl;
  		}
  		
  		/* mis a jour de la Zsg et de la grille G */
  		Case_zone = S->Lzsg;
  		while(Case_zone != NULL) {
  			M[Case_zone->i][Case_zone->j] = cl;
  			if(aff == 1) {
  				Grille_attribue_couleur_case(G, Case_zone->i, Case_zone->j, cl);
  			}
  			Case_zone = Case_zone->suiv;
  		}
  		
  		/* appel de la fonction agrandit_zone pour chaque element de la bordure */
  		Case_bordure = S->B[cl]; 
  		while(Case_bordure != NULL) {
	 		taille_Zsg += agrandit_Zsg(M, S, cl, Case_bordure->i, Case_bordure->j);
	 		Case_bordure = Case_bordure->suiv;
	 	}
	 	
	 	detruit_liste(&S->B[cl]); // mis a jour de la bordure
	 	
	 	/* affichage */
	 	if(aff == 1) {
	 		Grille_redessine_Grille();
	 		SDL_Delay(100);
	 	}
	 	
	 	cpt += 1;
	}
	detruit_S_Zsg(S);
	return cpt;		
}


