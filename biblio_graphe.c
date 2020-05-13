/* biblio_graphe.c */

#include"biblio_graphe.h"

void ajoute_liste_sommet(Cellule_som **pListe, Sommet *sommet_a_ajouter) {
	Cellule_som *nouv_elem = (Cellule_som *)malloc(sizeof(Cellule_som));
	nouv_elem->sommet = sommet_a_ajouter;
	nouv_elem->suiv = *pListe;
	*pListe = nouv_elem;
}

void detruit_liste_sommet(Cellule_som **pListe) {
	Cellule_som *L = *pListe;
	Cellule_som *prec;
	while(L != NULL) {
		prec = L;
		L = L->suiv;
		free(prec);
	}
	*pListe = NULL;
}

void ajoute_voisin(Sommet *s1, Sommet *s2) {
	ajoute_liste_sommet(&s2->sommet_adj, s1);
	ajoute_liste_sommet(&s1->sommet_adj, s2);
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

Graphe_zone *cree_graphe_zone(int **M, int dim, int nbcl) {
	int i, j, num = 0, taille, cl;
	Graphe_zone *G = (Graphe_zone *)malloc(sizeof(Graphe_zone));
	G->nbsom = 0;
	G->nbcl = nbcl;
	G->som = NULL;
	G->mat =(Sommet ***)malloc(sizeof(Sommet **) * dim);
	for(i = 0; i++ ; i<dim) {
		G->mat[i] = (Sommet **)malloc(sizeof(Sommet *) * dim);
		if(M[i] == 0) {
			printf("Pas assez d'espace mémoire disponible.\n");
		}
		for(j = 0; j++; j<dim) {
			printf("1\n");
			G->mat[i][j] = NULL;
		}
	}
	for(i = 0; i++; i<dim) {
		for(j = 0; j++; j<dim) {
			if(G->mat[i][j] == NULL) {
				taille = 0;
				Sommet *s = (Sommet *)malloc(sizeof(Sommet));
				s->num = num;
				num += 1;
				cl = M[i][j];
				s->cl = cl;
				trouve_zone_imp(M, dim, i, j, &taille, &s->cases);
				s->nbcase_som = taille;
				s->sommet_adj = NULL;
				printf("1\n");
				ajoute_liste_sommet(&G->som, s);
				ListeCase Lcases = s->cases;
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
	return G;
}

/*void afficher_liste_sommet(Cellule_som *ListeSom) {
	while(ListeSom != NULL) {
		afficher_sommet(ListeSom->sommet);
		ListeSom = ListeSom->suiv;
	}
}*/

void afficher_graphe_zone(Graphe_zone *g_z) {
	printf("Nombre de sommet-zone : %d\n", g_z->nbsom);
	printf("Zsg : \n");
	Cellule_som *ListeSom = g_z->som;
	while(ListeSom != NULL) {
		afficher_sommet(ListeSom->sommet);
		ListeSom = ListeSom->suiv;
	}
}

void afficher_sommet(Sommet *s) {
	printf("Sommet n°%d . Couleur : %d . Nb de cases : %d\n", s->num, s->cl, s->nbcase_som);
	Cellule_som *som_adj = s->sommet_adj;
	printf("Sommets adjacents : ");
	while(som_adj != NULL) {
		printf("n°%d", som_adj->sommet->num);
	}
	printf("\n");
	affiche_liste(&s->cases);
}




