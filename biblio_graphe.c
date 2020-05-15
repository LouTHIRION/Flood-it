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
	Graphe_zone *g_z = cree_graphe_zone(M, dim, nbcl);
	agrandit_Zsg(g_z, g_z->mat[0][0]);
	while(g_z->nb_som_zsg < g_z->nbsom) {
		plus_grd_nbcase = 0;
		for(i = 0; i<nbcl; i++) { // choix de la couleur 
			if(g_z->tailleB[i] > plus_grd_nbcase) {
				plus_grd_nbcase = g_z->tailleB[i];
				cl = i;
			}
		}
		Cellule_som * new_sommetsZsg = g_z->B[cl];
		while(new_sommetsZsg != NULL) {
			agrandit_Zsg(g_z, new_sommetsZsg->sommet);
			new_sommetsZsg = new_sommetsZsg->suiv;
		}
		detruit_liste_sommet(&g_z->B[cl]);
		g_z->tailleB[cl] =  0;
		if(aff == 1) {
			Cellule_som *Lzsg = g_z->Lzsg;
			while(Lzsg != NULL) {
				ListeCase cases = Lzsg->sommet->cases;
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
	return cpt;
}

/* Cette fonction initialise une file de sommet-zone
   le champ tete pointe sur la capsule contenant le plus ancien sommet dans la file
   tandis que le champ queue pointe sur la capsule contenant le sommet rajouté dernièrement;
   le champ nb contient la taille de la file;
   La file est implémentée sous la forme d'une liste doublement chaînée (le champ prec de Cellule_som 
   servira dans l'implémentation des fonctions enfiler et défiler) */
void initFile(File_Sommet **F)
{
}

/* Enfile le sommet s dans la file F (voir cours 2 si nécessaire). Complexité : 0(1) */
void enfiler(File_Sommet *F, Sommet *s)
{
}

/* Defile la file F (voir cours 2 si nécessaire) . retourne NULL si F vide . Complexité : 0(1)*/
Sommet *defiler(File_Sommet *F)
{
}

/* Réalise le parcours en largeur depuis le sommet-zone racine dans le graphe g_z
(voir cours 7 Parcours en largeur si nécessaire)
Les champs distance et pere de tous les sommets-zones de g_z sont alors modifiés pour 
contenir respectivement la distance minimale en nombre d'arcs de racine à chacun d'eux et l'adresse
de leur pere dans l'arborescence du parcours en largeur (Graphe de liaison dans le cours d'algorithmique 1)
Le pere de racine est NULL par définition*/
void parcours_en_largeur(Graphe_zone *g_z, Sommet *racine)
{
}

/* Cette fonction réalise le calcul de la séquence de couleurs suivant la stratégie mixte
parcours en largeur + max-bordure : tout d’abord effectuer les choix de couleurs correspondant
au chemin déterminer par l’algorithme parcours_en_largeur. Ensuite, terminer le jeu en utilisant
la stratégie max-bordure
*/
int sequence_parcours_largeur_puis_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff)
{
}

