/* biblio_graphe.c */

#include"biblio_graphe.h"

/* ajoute le sommet sommet_a_ajouter a la liste dont l'adresse est pListe */
void ajoute_liste_sommet(Cellule_som **pListe, Sommet *sommet_a_ajouter) {
	Cellule_som *nouv_elem = (Cellule_som *)malloc(sizeof(Cellule_som));
	nouv_elem->sommet = sommet_a_ajouter;
	nouv_elem->suiv = *pListe;
	(*pListe) = nouv_elem;
}

/* Detruit la liste de *pListe et reinitialiste *pListe a NULL. ne detruit pas les ommets */
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

/* met a jour s1 et s2 en indiquant qu'ils sont adjacents l'un a l'autre */
void ajoute_voisin(Sommet *s1, Sommet *s2) {
	ajoute_liste_sommet(&s2->sommet_adj, s1);
	ajoute_liste_sommet(&s1->sommet_adj, s2);
}

/* retourne 1 ssi s1 et s2 sont adjacents */
int adjacent(Sommet *s1, Sommet *s2) {
	Cellule_som *som_adj_s1 = s1->sommet_adj;
	while(som_adj_s1 != NULL) {
		if(som_adj_s1->sommet == s2) {
			return 1;
		}
		som_adj_s1 = som_adj_s1->suiv;
	}
	return 0;
}

/* cree un graphe zone tout entier a partir de la matrice M de dimension dim et contenant des cases de nbcl couleurs differentes */
Graphe_zone *cree_graphe_zone(int **M, int dim, int nbcl) {
	int i, j, num = 0, taille, cl;
	Graphe_zone *G = (Graphe_zone *)malloc(sizeof(Graphe_zone));
	G->nbsom = 0;
	G->nbcl = nbcl;
	G->som = NULL;
	G->mat =(Sommet ***)malloc(sizeof(Sommet **) * dim);
	/* Creation de la matrice de pointeurs de sommets */
	for(i = 0; i<dim; i++) {
		G->mat[i] = (Sommet **)malloc(sizeof(Sommet *) * dim);
		if(M[i] == 0) {
			printf("Pas assez d'espace mémoire disponible.\n");
		}
		for(j = 0; j<dim; j++) {
			G->mat[i][j] = NULL;
		}
	}
	/* Creation/Initialisation des sommets-zone */
	for(i = 0; i<dim; i++) {
		for(j = 0; j<dim; j++) {
			if(G->mat[i][j] == NULL) {
				G->nbsom += 1;
				taille = 0;
				cl = M[i][j];
				Sommet *s = (Sommet *)malloc(sizeof(Sommet));
				s->num = num;
				s->cl = cl;
				s->cases = NULL;
				s->sommet_adj = NULL;
				// cherche toutes les cases correspondants au sommet
				trouve_zone_imp(M, dim, i, j, &taille, &s->cases);
				s->nbcase_som = taille;
				ajoute_liste_sommet(&G->som, s);
				ListeCase Lcases = s->cases;
				while(Lcases != NULL) { // mis a jour matrice
					G->mat[Lcases->i][Lcases->j] = s;
					Lcases = Lcases->suiv;
				}
				num += 1;
			}
		}
	}
	/* mis a jour de chaque sommet avec ses sommets adjacents */
	for(i = 0; i<dim; i++) {
		for(j = 0; j<dim; j++) {
			// test pour la cases juste a droite
			if(i<dim-1 && M[i][j] != M[i+1][j] && !adjacent(G->mat[i][j], G->mat[i+1][j])) {
				ajoute_voisin(G->mat[i][j], G->mat[i+1][j]);
			}
			// puis pour celle juste a gauche
			if(j<dim-1 && M[i][j] != M[i][j+1] && !adjacent(G->mat[i][j], G->mat[i][j+1])) {
				ajoute_voisin(G->mat[i][j], G->mat[i][j+1]);
			}
		}
	}
	return G;
}

/* affichage graphe */
void afficher_graphe_zone(Graphe_zone *g_z) {
	printf("Nombre de sommet-zone : %d\n", g_z->nbsom);
	printf("Zsg : \n");
	Cellule_som *ListeSom = g_z->som;
	while(ListeSom != NULL) {
		afficher_sommet(ListeSom->sommet);
		ListeSom = ListeSom->suiv;
	}
}

/* affichage sommet (utilisee dans le fonction affichage graphe) */
void afficher_sommet(Sommet *s) {
	printf("Sommet n°%d . Couleur : %d . Nb de cases : %d\n", s->num, s->cl, s->nbcase_som);
	Cellule_som *som_adj = s->sommet_adj;
	printf("Sommets adjacents : ");
	while(som_adj != NULL) {
		printf("n°%d ", som_adj->sommet->num);
		som_adj = som_adj->suiv;
	}
	printf("\n");
	printf("Liste des cases : ");
	affiche_liste(&s->cases);
	printf("\n******\n");
}




