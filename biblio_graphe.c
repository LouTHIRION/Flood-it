/* biblio_graphe.c */

#include"biblio_graphe.h"

/* Un sommet-zone ou sommet (dans le contexte du projet) : c'est un objet (une structure pour parler en langage C) 
utilisé pour représenter une zone (voir définition de zone dans le 1er paragraphe d'introduction).  */

/* Un sommet n'étant pas doté d'un champ suivant, pour stocker 2 ou plusieurs sommets dans une sda quelconque 
(ici FILE, LISTE SIMPLEMENT CHAÎNÉE OU DOUBLEMENT CHAÎNÉE), on se sert d'une sorte de capsule qui contiendra notre donnée
(le sommet) et qui elle, est dotée de champs (suiv, prec) qui permettront d'aller d'un sommet à l'autre dans la sda considérée */


/* ajoute_liste_sommet : Avec cette fonction, on ajoute un sommet (la donnée) dans une collection de type liste simplement chaînée.
Seul le champ suiv sert donc ici. La différence avec les traitements que vous faisiez en L1, c'est que la fonction ne 
retourne aucun résultat. L'adresse-mémoire de la première capsule au lieu d'être retournée, est stockée par adresse d'où le
Cellule_som ** . */
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



/* La finalité de cette fonction est d'allouer de la mémoire pour une structure de type Graphe_zone puis
   d'instancier l'ensemble de ces champs à partir de la matrice M de dimension dim et contenant potentiellement 
   nbcl couleurs (des entiers) différents. Concrètement, à la sortie de cette fonction, les variables (champs) de la structure
   seront initialisées de la façon suivante :
   #) g_z->dim avec le paramètre dim 
   #) g_z->nbcl avec le paramètre nbcl
   #) g_z->Lzsg à NULL vu qu'ici on n'exécute pas d'algorithme de calcul de séquence.
   #) g_z->nb_som_zsg à 0 logiquement;
   #) g_z->B à l'adresse d'un tableau à une dimension de taille nbcl dont les cases sont initialisées à NULL.
   #) g_z->tailleB à l'adresse d'un tableau à une dimension de taille nbcl aussi et dont les cases sont initialisées à 0 naturellement;
   #) g_z->mat à l'adresse d'un tableau à double dimension de taille dim*dim et contenant à l'intersection de la ligne i et
      de la colonne j, l'adresse du Sommet-zone auquel appartient la case (i,j) d'où le type Sommet ***.
   #) g_z->som à l'adresse d'une liste chaînée contenant tous les sommets-zones instanciés dans la grille.
   #) g_z->nbsom le nombre total de sommets instanciés.
   #) g_z->marque à l'adresse d'un tableau à une dimension de taille g_z->nbsom dont les cases sont initialisés à 2 (non-visités).
      Les numéros de sommets ne serviront donc pas uniquement pour l'affichage. Ils permettront également de connaître (en O(1)) l'état 
      (dans la zsg, dans la bordure ou non-visité) d'un sommet lors du calcul d'une séquence de couleurs.
   
   Remarques : 
     Le principe d'initialisation des cases de mat est très bien décrit à la question 4.2 de même que celui de création des arêtes entre sommets.
     Ce dernier principe obéit à la définition suivante : il existe une arête entre deux sommets-zones s1 et s2 s’l existe une case dans la zone s1 qui est adjacente 
     à une case de la zone s2. Le traitement à réaliser ici est très similaire à celui des fonctions trouve_zone_rec et trouve_zone_imp :
     à partir de la case (i, j) on regarde (lorsqu'elles existent) ses 4 voisines (i-1, j), (i+1, j), (i, j-1) et (i, j+1).
     Attention toutefois à ne pas stocker la même information 2 fois. On utilisera donc les fonctions ajoute_voisin pour créer les arêtes et 
     adjacent pour s'assurer que l'arête n'a pas encore déja été créée et donc éviter un double stockage.

     */
Graphe_zone *cree_graphe_zone(int **M, int dim, int nbcl) {
	int i, j, num = 0, taille, cl;
	Graphe_zone *G = (Graphe_zone *)malloc(sizeof(Graphe_zone));
	G->dim = dim;
	G->nbsom = 0;
	G->nbcl = nbcl;
	G->som = NULL;
	G->Lzsg = NULL;
	G->nb_som_zsg = 0;
	G->B = (Cellule_som **)malloc(nbcl * sizeof(Cellule_som *));
	G->tailleB = (int *)malloc(nbcl * sizeof(int));
	for(i = 0; i<nbcl; i++) {
		G->B[i] = NULL;
		G->tailleB[i] = 0;
	}
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
				s->distance = 1000000;
				s->pere = NULL;
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
	/* Initailisation du tabeau marque */
	G->marque = (int*)malloc(G->nbsom * sizeof(int));
	for(i = 0; i<G->nbsom; i++) {
		G->marque[i] = 2;
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



/* Detruit entierement le graphe g_z en liberant toutes les zones memoires occupees */
void liberer_graphe_zone(Graphe_zone *g_z) {
	int i;
	if(g_z->marque)
		free(g_z->marque);
	detruit_liste_sommet(&g_z->Lzsg);
	for(i = 0; i<g_z->nbcl; i++) {
		if(g_z->B[i])
			detruit_liste_sommet(&g_z->B[i]);
	}
	if(g_z->B)
		free(g_z->B);
	if(g_z->tailleB)
		free(g_z->tailleB);
	Cellule_som *prec;
	while(g_z->som != NULL) {
		prec = g_z->som;
		g_z->som = g_z->som->suiv;
		detruit_liste(&prec->sommet->cases);
		detruit_liste_sommet(&prec->sommet->sommet_adj);
		free(prec);
	}
	for(i = 0; i<g_z->dim; i++) {
		if(g_z->mat[i])
			free(g_z->mat[i]);
	}
	if(g_z->mat)
		free(g_z->mat);
	free(g_z);
}



/* affichage graphe */
void afficher_graphe_zone(Graphe_zone *g_z) {
	int i;
	printf("Nombre de sommet-zone : %d\n", g_z->nb_som_zsg);
	printf("Zsg : \n");
	Cellule_som *Lzsg = g_z->Lzsg;
	while(Lzsg != NULL) {
		afficher_sommet(Lzsg->sommet);
		Lzsg = Lzsg->suiv;
	}
	/*printf("Bordure : \n");
	for(i = 0; i<g_z->nbcl; i++) {
		printf("tailleB[%d] : %d\n", i, g_z->tailleB[i]);
		Cellule_som *B = g_z->B[i];
		while(B != NULL) {
			afficher_sommet(B->sommet);
			B = B->suiv;
		}
	}*/
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



/* Cette fonction a pour but de mettre à jour les champs Lzsg et B de g_z (utilisation de 
la fonction ajoute_liste_sommet) lorsque le sommet-zone s qui est dans la bordure doit basculer
dans Lzsg. Pour chacun des sommets adjacents à s : 
- soit il est déjà dans Lzsg; auquel cas il n'y a rien à faire
- soit il n'y est pas :
    on distinguera 2 cas ici : 
        - soit il est dans la bordure; auquel cas il n'y a rien à faire non plus
        - soit il n'a pas encore visité et dans ce cas, il faut le rajouter à la 
        bordure sans oublier de le marquer comme tel et de mettre à jour la valeur de 
        la case de tailleB correspondante.
*/
void agrandit_Zsg(Graphe_zone *g_z, Sommet *s) {
	g_z->marque[s->num] = 0;
	ajoute_liste_sommet(&g_z->Lzsg, s);
	g_z->nb_som_zsg += 1;
	Cellule_som *som_adj = s->sommet_adj;
	while(som_adj != NULL) {
		if(g_z->marque[som_adj->sommet->num] == 2) {
			ajoute_liste_sommet(&g_z->B[som_adj->sommet->cl], som_adj->sommet);
			g_z->tailleB[som_adj->sommet->cl] += som_adj->sommet->nbcase_som;
			g_z->marque[som_adj->sommet->num] = 1;
		}
		som_adj = som_adj->suiv;
	}
}

/* Cette fonction met en oeuvre le calcul d'une séquence de couleurs suivant la stratégie max-bordure.
 De même signature que la fonction sequence_aleatoire_rapide, son fonctionnement lui est assez similaire :
    - créer et instancier un graphe_zone g_z en utilisant la fonction cree_graphe_zone
    - Utiliser l'appel à la fonction agrandit_zone à partir du sommet g_z->mat[0][0]
    - À chaque tour de jeu, déterminer la couleur cl de la bordure la plus large (avec le plus grand nombre de cases)
    de la zsg ( détermination à partir des cases de g_z->tailleB) ; utiliser agrandit_zone à partir de chaque sommet-zone appartenant à g_z->B[cl];
    ne pas oublier de détruire g_z->B[cl] et de réinitialiser g_z->tailleB[cl] à 0.
    - Si aff est égal à 1, parcourir les cases de g_z->Lzsg et attribuer aux cases correspondantes de la grille G
    la couleur cl
    - on s'arrête quand le nombre de sommets de zsg est égal aux nombres de sommets-zones de g_z
    - retourner le nombre de changements de couleurs  qui furent nécessaires pour inonder totalement la grille*/

int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff) {
	int i, cpt = 0, cl, plus_grd_nbcase ;
	ListeCase cases;
	Cellule_som *Lzsg;
	Cellule_som * new_sommetsZsg;
	Graphe_zone *g_z = cree_graphe_zone(M, dim, nbcl);
	agrandit_Zsg(g_z, g_z->mat[0][0]);
	while(g_z->nb_som_zsg < g_z->nbsom) {
		/* choix de la couleur */
		plus_grd_nbcase = 0;
		for(i = 0; i<nbcl; i++) {
			if(g_z->tailleB[i] > plus_grd_nbcase) {
				plus_grd_nbcase = g_z->tailleB[i];
				cl = i;
			}
		}
		/* mis a jour des valeurs */
		new_sommetsZsg = g_z->B[cl];
		while(new_sommetsZsg != NULL) {
			agrandit_Zsg(g_z, new_sommetsZsg->sommet);
			new_sommetsZsg = new_sommetsZsg->suiv;
		}
		detruit_liste_sommet(&g_z->B[cl]);
		g_z->tailleB[cl] =  0;
		/* affichage */
		if(aff == 1) {
			Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				cases = Lzsg->sommet->cases;
				while(cases != NULL) {
					Grille_attribue_couleur_case(G, cases->i, cases->j, cl);
					cases = cases->suiv;
				}
				Lzsg = Lzsg->suiv;
			}
			Grille_redessine_Grille();
			SDL_Delay(100);
		}
		cpt += 1;
	}
	liberer_graphe_zone(g_z);
	return cpt;
}



/* Fonction qui fait une copie g_z_a_copier du graphe g_z 
   sachant que les deux graphes sont initialisés en amont avec les memes valeurs,
   on ne met a jour que les elements de la Zsg et de la bordures*/
void copie_graphe_zone(Graphe_zone *g_z, Graphe_zone *g_z_a_copier) {
	int i;
	for(i = 0; i<g_z->nbsom; i++) {
		g_z_a_copier->marque[i] = g_z->marque[i];
	}
	g_z_a_copier->tailleB[g_z_a_copier->Lzsg->sommet->cl] = g_z->tailleB[g_z_a_copier->Lzsg->sommet->cl];
	g_z_a_copier->nb_som_zsg = g_z->nb_som_zsg;
	/* mis a jour de la liste Lzsg */
	Cellule_som *Lzsg = g_z->Lzsg;
	Cellule_som *Lzsg_a_copier = g_z_a_copier->Lzsg;
	Cellule_som *tmp;
	while(Lzsg != NULL && Lzsg_a_copier != NULL) {
		if(Lzsg->sommet->num != Lzsg_a_copier->sommet->num) {
			ajoute_liste_sommet(&g_z_a_copier->B[Lzsg_a_copier->sommet->cl], Lzsg_a_copier->sommet);
			tmp = Lzsg_a_copier;
			g_z_a_copier->Lzsg = Lzsg_a_copier->suiv;
			Lzsg_a_copier = Lzsg_a_copier->suiv;
			free(tmp);
		}
		else {
			Lzsg = Lzsg->suiv;
			Lzsg_a_copier = Lzsg_a_copier->suiv;
		}
	}
	/* mis a jour du tableau de liste B */
	for(i = 0; i<g_z->nbcl; i++) {
		detruit_liste_sommet(&g_z_a_copier->B[i]);
		if(g_z->tailleB[i] == 0) {
			g_z_a_copier->tailleB[i] = 0;
			
		}
		else {
			g_z_a_copier->tailleB[i] = g_z->tailleB[i];
			Cellule_som *B = g_z->B[i];
			while(B != NULL) {
				ajoute_liste_sommet(&g_z_a_copier->B[i], B->sommet);
				B = B->suiv;
			}
		}
	}
}
	
/* Fonction qui realise le calcul de la sequence de couleur en appliquant 
   la strategie de test en profondeur qui ressemble a la strategie de max-bordure 
   mais qui s'efonce d'une profondeur en plus pour tester plus de possibilite.
   Cette strategie est un peu plus efficace que la strategie max-bordure 
   mais elle est beaucoup plus gourmande en temps de calcul et memoire. */
int sequence_max_bord_test_en_profondeur(int **M, Grille *G, int dim, int nbcl, int aff) {
	int i, j,k,  cpt = 0, cl, plus_grd_nbcase, nb_cases, profondeur = 2;
	int *cls = (int *)malloc(profondeur * sizeof(int)); //suite de couleurs a jouer
	ListeCase cases;
	Cellule_som *Lzsg;
	Cellule_som * new_sommetsZsg;
	Cellule_som * new_sommetsZsg_copie;
	Graphe_zone *g_z = cree_graphe_zone(M, dim, nbcl);
	Graphe_zone *copie_g_z = cree_graphe_zone(M, dim, nbcl);
	agrandit_Zsg(copie_g_z, copie_g_z->mat[0][0]);
	agrandit_Zsg(g_z, g_z->mat[0][0]);
	
	while(g_z->nb_som_zsg < g_z->nbsom) {
	
		/* choix de la couleur */
		plus_grd_nbcase = 0;
		for(i = 0; i<nbcl; i++) {
			if(copie_g_z->B[i] != NULL) {
				nb_cases = copie_g_z->tailleB[i];
				new_sommetsZsg_copie = copie_g_z->B[i];
				while(new_sommetsZsg_copie != NULL) {
					agrandit_Zsg(copie_g_z, new_sommetsZsg_copie->sommet);
					new_sommetsZsg_copie = new_sommetsZsg_copie->suiv;
				}
				detruit_liste_sommet(&copie_g_z->B[i]);
				copie_g_z->tailleB[i] =  0;
				for(j = 0; j<nbcl; j++) {
					if(nb_cases+copie_g_z->tailleB[j] > plus_grd_nbcase) {
						plus_grd_nbcase = nb_cases+copie_g_z->tailleB[j];
						cls[0] = i;
						cls[1] = j;
					}
				}
				copie_graphe_zone(g_z, copie_g_z);
			}
		}
		
		/* maj des valeurs avec */
		for(i = 0; i<profondeur; i++) {
			new_sommetsZsg = g_z->B[cls[i]];
			new_sommetsZsg_copie = copie_g_z->B[cls[i]];
			while(new_sommetsZsg != NULL) {
				agrandit_Zsg(g_z, new_sommetsZsg->sommet);
				new_sommetsZsg = new_sommetsZsg->suiv;
				agrandit_Zsg(copie_g_z, new_sommetsZsg_copie->sommet);
				new_sommetsZsg_copie = new_sommetsZsg_copie->suiv;
			}
			detruit_liste_sommet(&g_z->B[cls[i]]);
			detruit_liste_sommet(&copie_g_z->B[cls[i]]);
			g_z->tailleB[cls[i]] =  0;
			copie_g_z->tailleB[cls[i]] = 0;
		
			/* affichage */
			if(aff == 1) {
				Lzsg = g_z->Lzsg;
				while(Lzsg != NULL) {
					cases = Lzsg->sommet->cases;
					while(cases != NULL) {
						Grille_attribue_couleur_case(G, cases->i, cases->j, cls[i]);
						cases = cases->suiv;
					}
					Lzsg = Lzsg->suiv;
				}
				Grille_redessine_Grille();
				SDL_Delay(100);
			}
			cpt += 1;
		}
	}
	liberer_graphe_zone(g_z);
	liberer_graphe_zone(copie_g_z);
	return cpt;
}



/* Cette fonction initialise une file de sommet-zone
   le champ tete pointe sur la capsule contenant le plus ancien sommet dans la file
   tandis que le champ queue pointe sur la capsule contenant le sommet rajouté dernièrement;
   le champ nb contient la taille de la file;
   La file est implémentée sous la forme d'une liste doublement chaînée (le champ prec de Cellule_som 
   servira dans l'implémentation des fonctions enfiler et défiler) */
void initFile(File_Sommet **F) {
	//*F = (File_Sommet *)malloc(sizeof(File_Sommet));
	(*F)->tete = NULL;
	(*F)->queue = NULL;
	(*F)->nb = 0;
}

/* Enfile le sommet s dans la file F (voir cours 2 si nécessaire). Complexité : 0(1) */
void enfiler(File_Sommet *F, Sommet *s) {
	Cellule_som * new_elem = (Cellule_som *)malloc(sizeof(Cellule_som));
	new_elem->sommet = s;
	new_elem->suiv = F->queue;
	new_elem->prec = NULL;
	if(F->nb == 0) {
		F->tete = new_elem;
	}
	else {
		F->queue->prec = new_elem;
	}
	F->queue = new_elem;
	F->nb += 1;
}

/* Defile la file F (voir cours 2 si nécessaire) . retourne NULL si F vide . Complexité : 0(1)*/
Sommet *defiler(File_Sommet *F) {
	if(F->nb == 0) {
		return NULL;
	}
	Sommet *s = F->tete->sommet;
	Cellule_som *tmp = F->tete;
	F->tete = F->tete->prec;
	free(tmp);
	F->nb -= 1;
	return s;
}

/* Réalise le parcours en largeur depuis le sommet-zone racine dans le graphe g_z
(voir cours 7 Parcours en largeur si nécessaire)
Les champs distance et pere de tous les sommets-zones de g_z sont alors modifiés pour 
contenir respectivement la distance minimale en nombre d'arcs de racine à chacun d'eux et l'adresse
de leur pere dans l'arborescence du parcours en largeur (Graphe de liaison dans le cours d'algorithmique 1)
Le pere de racine est NULL par définition*/
void parcours_en_largeur(Graphe_zone *g_z, Sommet *racine) {
	Cellule_som *cour;
	Sommet *pere;
	int i;
	File_Sommet *F = (File_Sommet *)malloc(sizeof(File_Sommet));
	initFile(&F);
	
	racine->distance = 0;
	
	
	enfiler(F, racine);
	while(F->nb > 0) {
		pere = defiler(F);
		cour = pere->sommet_adj;
		while(cour != NULL) {
			if(cour->sommet->pere == NULL) {
				cour->sommet->pere = pere;
				cour->sommet->distance = pere->distance + 1;
				enfiler(F, cour->sommet);
			}
			cour = cour->suiv;
		}
	}
	free(F);
}

/* Cette fonction réalise le calcul de la séquence de couleurs suivant la stratégie mixte
parcours en largeur + max-bordure : tout d’abord effectuer les choix de couleurs correspondant
au chemin déterminer par l’algorithme parcours_en_largeur. Ensuite, terminer le jeu en utilisant
la stratégie max-bordure
*/
int sequence_parcours_largeur_puis_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff) {
	Graphe_zone *g_z = cree_graphe_zone(M, dim, nbcl);
	int i, cpt = 0, plus_grd_nbcase, distance, cl;
	ListeCase cases;
	Cellule_som *Lzsg;
	Cellule_som * new_sommetsZsg;
	Sommet *cour = g_z->mat[dim-1][dim-1];
	parcours_en_largeur(g_z, g_z->mat[0][0]);
	distance = cour->distance;
	int *couleurs = (int*)malloc(distance * sizeof(int));
	i = distance-1;
	
	/* parcours en largeur */
	/* determine les couleurs a choisir 
	   pour aller vers le bord en bas droite */
	while(cour != g_z->mat[0][0]) {
		couleurs[i] = cour->cl;
		cour = cour->pere;
		i -= 1;
	}
	/* agrandit la zone jusqu'au bord en bas a droite 
	   a partir du tableau des couleurs */
	agrandit_Zsg(g_z, g_z->mat[0][0]);
	for(i = 0; i<distance; i++) {
		cl = couleurs[i];
		new_sommetsZsg = g_z->B[cl];
		while(new_sommetsZsg != NULL) {
			agrandit_Zsg(g_z, new_sommetsZsg->sommet);
			new_sommetsZsg = new_sommetsZsg->suiv;
		}
		detruit_liste_sommet(&g_z->B[cl]);
		g_z->tailleB[cl] =  0;
		if(aff == 1) {
			Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				cases = Lzsg->sommet->cases;
				while(cases != NULL) {
					Grille_attribue_couleur_case(G, cases->i, cases->j, cl);
					cases = cases->suiv;
				}
				Lzsg = Lzsg->suiv;
			}
			Grille_redessine_Grille();
			SDL_Delay(100);
		}
	}
	cpt = distance;
	
	/* max-bordure */
	while(g_z->nb_som_zsg < g_z->nbsom) {
		plus_grd_nbcase = 0;
		for(i = 0; i<nbcl; i++) { // choix de la couleur 
			if(g_z->tailleB[i] > plus_grd_nbcase) {
				plus_grd_nbcase = g_z->tailleB[i];
				cl = i;
			}
		}
		new_sommetsZsg = g_z->B[cl];
		while(new_sommetsZsg != NULL) {
			agrandit_Zsg(g_z, new_sommetsZsg->sommet);
			new_sommetsZsg = new_sommetsZsg->suiv;
		}
		detruit_liste_sommet(&g_z->B[cl]);
		g_z->tailleB[cl] =  0;
		if(aff == 1) {
			Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				cases = Lzsg->sommet->cases;
				while(cases != NULL) {
					Grille_attribue_couleur_case(G, cases->i, cases->j, cl);
					cases = cases->suiv;
				}
				Lzsg = Lzsg->suiv;
			}
			Grille_redessine_Grille();
			SDL_Delay(100);
		}
		cpt += 1;
	}
	if(couleurs)
		free(couleurs);
	liberer_graphe_zone(g_z);
	return cpt;	
}



/* Fonction qui realise le calcul de la sequence de couleur suivant les strategie de parcour en largeur et test en profondeur */
int sequence_ultime(int **M, Grille *G, int dim, int nbcl, int aff) {
	int i, j,k,  cpt = 0, cl, plus_grd_nbcase, nb_cases, profondeur = 2;
	int *cls = (int *)malloc(profondeur * sizeof(int)); //suite de couleurs a jouer
	ListeCase cases;
	Cellule_som *Lzsg;
	Cellule_som * new_sommetsZsg;
	Cellule_som * new_sommetsZsg_copie;
	Graphe_zone *g_z = cree_graphe_zone(M, dim, nbcl);
	Graphe_zone *copie_g_z = cree_graphe_zone(M, dim, nbcl);
	agrandit_Zsg(copie_g_z, copie_g_z->mat[0][0]);
	agrandit_Zsg(g_z, g_z->mat[0][0]);
	
	int distance;
	Sommet *cour = g_z->mat[dim-1][dim-1];
	parcours_en_largeur(g_z, g_z->mat[0][0]);
	distance = cour->distance;
	int *couleurs = (int*)malloc(distance * sizeof(int));
	i = distance-1;
	/* parcours en largeur */
	/* determine les couleurs a choisir 
	   pour aller vers le bord en bas droite */
	while(cour != g_z->mat[0][0]) {
		couleurs[i] = cour->cl;
		cour = cour->pere;
		i -= 1;
	}
	/* agrandit la zone jusqu'au bord en bas a droite 
	   a partir du tableau des couleurs */
	for(i = 0; i<distance; i++) {
		cl = couleurs[i];
		new_sommetsZsg = g_z->B[cl];
		new_sommetsZsg_copie = copie_g_z->B[cl];
		while(new_sommetsZsg != NULL) {
			agrandit_Zsg(g_z, new_sommetsZsg->sommet);
			new_sommetsZsg = new_sommetsZsg->suiv;
			agrandit_Zsg(copie_g_z, new_sommetsZsg_copie->sommet);
			new_sommetsZsg_copie = new_sommetsZsg_copie->suiv;
		}
		detruit_liste_sommet(&g_z->B[cl]);
		detruit_liste_sommet(&copie_g_z->B[cl]);
		copie_g_z->tailleB[cl] =  0;
		g_z->tailleB[cl] =  0;
		if(aff == 1) {
			Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				cases = Lzsg->sommet->cases;
				while(cases != NULL) {
					Grille_attribue_couleur_case(G, cases->i, cases->j, cl);
					cases = cases->suiv;
				}
				Lzsg = Lzsg->suiv;
			}
			Grille_redessine_Grille();
			SDL_Delay(100);
		}
	}
	cpt = distance;
	
	/* test en profondeur */
	while(g_z->nb_som_zsg < g_z->nbsom) {
	
		/* choix de la couleur */
		plus_grd_nbcase = 0;
		for(i = 0; i<nbcl; i++) {
			if(copie_g_z->B[i] != NULL) {
				nb_cases = copie_g_z->tailleB[i];
				new_sommetsZsg_copie = copie_g_z->B[i];
				while(new_sommetsZsg_copie != NULL) {
					agrandit_Zsg(copie_g_z, new_sommetsZsg_copie->sommet);
					new_sommetsZsg_copie = new_sommetsZsg_copie->suiv;
				}
				detruit_liste_sommet(&copie_g_z->B[i]);
				copie_g_z->tailleB[i] =  0;
				for(j = 0; j<nbcl; j++) {
					if(nb_cases+copie_g_z->tailleB[j] > plus_grd_nbcase) {
						plus_grd_nbcase = nb_cases+copie_g_z->tailleB[j];
						cls[0] = i;
						cls[1] = j;
					}
				}
				copie_graphe_zone(g_z, copie_g_z);
			}
		}
		
		/* maj des valeurs avec */
		for(i = 0; i<profondeur; i++) {
			new_sommetsZsg = g_z->B[cls[i]];
			new_sommetsZsg_copie = copie_g_z->B[cls[i]];
			while(new_sommetsZsg != NULL) {
				agrandit_Zsg(g_z, new_sommetsZsg->sommet);
				new_sommetsZsg = new_sommetsZsg->suiv;
				agrandit_Zsg(copie_g_z, new_sommetsZsg_copie->sommet);
				new_sommetsZsg_copie = new_sommetsZsg_copie->suiv;
			}
			detruit_liste_sommet(&g_z->B[cls[i]]);
			detruit_liste_sommet(&copie_g_z->B[cls[i]]);
			g_z->tailleB[cls[i]] =  0;
			copie_g_z->tailleB[cls[i]] = 0;
			
		/* affichage */
		if(aff == 1) {
			Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				cases = Lzsg->sommet->cases;
				while(cases != NULL) {
					Grille_attribue_couleur_case(G, cases->i, cases->j, cls[i]);
					cases = cases->suiv;
				}
				Lzsg = Lzsg->suiv;
			}
			Grille_redessine_Grille();
			SDL_Delay(100);
		}
		cpt += 1;
		}
	}
	liberer_graphe_zone(g_z);
	liberer_graphe_zone(copie_g_z);
	return cpt;
}


