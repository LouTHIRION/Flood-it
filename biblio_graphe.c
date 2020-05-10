/* biblio_graphe.c */

#include"biblio_graphe.h"

void ajoute_liste_sommet(Sommet *S, Cellule_som *L) {
	Cellule_som *nouv_elem = (Cellule_som *)malloc(sizeof(Cellule_som));
	nouv_elem->sommet = S;
	/* version 1 */
	if(L != NULL) {
		nouv_elem->suiv = L->suiv;
		L->suiv = nouv_elem;
	}
	else {
		L = nouv_elem;
	}	
	/* version 2 a tester */
	/*
	Cellule_som *prec = L;
	while(L != NULL) {
		prec = L;
		L = L->suiv;
	}
	prec->suiv = nouv_elem;
	*/
}

void detruit_liste_sommet(Cellule_som *L) {
	Cellule_som *prec = L;
	while(L != NULL) {
		prec = L;
		L = L->suiv;
		free(prec);
	}
}

void ajoute_voisin(Sommet *s1, Sommet *s2) {
	ajoute_liste_sommet(s1, s2->sommet_adj);
	ajoute_liste_sommet(s2, s1->sommet_adj);
}

int adjacent(Sommet *s1, Sommet *s2) {
	Cellule_som *som_adj_s1 = s1->sommet_adj;
	while(som_adj_s1 != NULL) {
		if(som_adj_s1->sommet == s2) {
			return 1;
		}
	}
	return 0;
}

int cree_graphe_zone(int **M, int dim) {
	int i, j, num = 0, taille, cl;
	G->nbsom = 0;
	G->som = NULL;
	G->mat =(sommet ***)malloc(sizeof(sommet **) * dim);
	for(i = 0; i++ ; i<dim) {
		G->mat[i] = (sommet **)malloc(sizeof(sommet *) * dim);
		if(M[i] == 0) {
			printf("Pas assez d'espace mémoire disponible.\n");
			return 1;
		}
		for(j = 0; j++; j<dim) {
			G->mat[i}[j] = NULL;
		}
	}
	for(i = 0; i++; i<dim) {
		for(j = 0; j++; j<dim) {
			if(G->mat[i}[j] == NULL) {
				taille = 0;
				sommet *s = (sommet *)malloc(sizeof(sommet));
				s->num = num;
				num += 1;
				cl = M[i][j];
				s->cl = cl;
				trouve_zone_imp(M, dim, i, j, &taille, s->cases);
				s->nbcase_som = taille;
				s->sommet_adj = NULL;
				ajoute_liste_sommet(s, som);
				ListeCases *Lcases = s->cases;
				while(Lcases != NULL) {
					G->mat[Lcases->i][Lcases->j] = s;
					Lcases = Lcases->suiv;
				}
			}
		}
	}
	for(i = 0; i++; i<dim) {
		for(j = 0; j++; j<dim) {
			if(i<dim-1 && M[i][j] != M[i+1][j]) {
				ajoute_voisin(G->mat[i][j], G->mat[i+1][j]);
			}
			if(j<dim-1 && M[i][j] != M[i][j+1]) {
				ajoute_voisin(G->mat[i][j], G->mat[i][j+1]);
			}
		}
	}
}







