/* biblio_graphe.h */
#ifndef _BIBLIO_GRAPHE_
#define _BIBLIO_GRAPHE_

#include "Liste_case.h"
#include "Entete_Fonctions.h"
#include "API_Grille.h"
#include "API_Gene_instance.h"

typedef struct sommet Sommet;

typedef struct cellule_som
{
    Sommet * sommet;
    struct cellule_som * suiv;

    struct cellule_som * prec; /* sert uniquement dans l'exo 5 : pour l'implementation d'une file (parcours en largeur) */

} Cellule_som;

struct sommet
{
   int num;                    /* Numero du sommet */
   int cl;                     /* Couleur d'origine du sommet-zone : ne sera pas modifie */
   ListeCase cases;            /* Liste des cases du sommet-zone */
   int nbcase_som;             /* Nombre de cases de la liste cases */
   Cellule_som * sommet_adj;   /* Liste des aretes et pointeurs sur les sommets adjacents */


   int distance;                /* distance au noeud racine : sert uniquement dans l'exo 5 */
   Sommet * pere;               /* Pere du sommet dans l'arborescence du parcours en largeur : exo5 uniquement */
};


typedef struct graphe_zone
{
    int nbsom;         /* Nombre de sommets dans le graphe */
    int nbcl;          /* nombre de couleurs */
    int *marque;        /* Tableau de marquage des sommets-zones : 0(Lzsg), 1(Bordure) ou 2(non-visite) */

    Cellule_som * Lzsg; /* Liste des sommets-zones appartenant a la zone superieure gauche*/
    int nb_som_zsg;     /* Nombre sommets-zones dans la Zsg : a mettre a jour*/

    Cellule_som ** B;  /* Tableau de listes de sommets-zones composant la bordure de la Zonz superieure gauche indicé sur les couleurs*/
    int *tailleB;      /* Taille de chacune des listes des sommets-zones composant la bordure : a maintenir a jour */

    Cellule_som * som;  /* Liste chainee des sommets-zones du graphe */

    Sommet *** mat;    /* Matrice de pointeurs sur les sommets indiquant a quel sommet appartient ue case (i, j) de la grille */
}Graphe_zone;


/* Struct de File (liste doublement chainee) destine a stocker des sommets (Exo 5) */

typedef struct _fileSommet {
    Cellule_som * tete;
    Cellule_som * queue;
    int nb;
} File_Sommet;

/* initialise la File */
void initFile(File_Sommet **F);
/* realise l'operation enfiler */
void enfiler(File_Sommet *F, Sommet *s);
/* realise l'operation defiler */
Sommet *defiler(File_Sommet *F);

/* Ajoute le sommet sommet_a_ajouter à la liste dont l'adresse est pListe*/
void ajoute_liste_sommet(Cellule_som ** pListe, Sommet * sommet_a_ajouter);
/* Detruit la liste de *pListe et reinitialise *pListe à NULL. Attention on ne detruit pas les sommets contenus dans les Cellule_som */
void detruit_liste_sommet(Cellule_som **pliste);
/* Met à jour s1 et s2 en indiquant qu'ils sont adjacents l'un de l'autre */
void ajoute_voisin(Sommet *s1, Sommet *s2);
/* Retourne 1 ssi s1 et s2 sont adjacents */
int adjacent(Sommet *s1, Sommet *s2);

/* Cree un graphe zone tout entier a partir de la matrice M de dimension dim et contenant des cases de nbcl couleurs différentes*/
Graphe_zone * cree_graphe_zone(int **M, int dim, int nbcl);


/* Affiche le graphe_zone sous le format suivant :
 ** Graphe Zone **
Nombre de sommets-zone : 4
Zsg :
Sommet n°3 . Couleur : 0 . Nb de cases : 2
Sommets adjacents : n°2 n°1
Liste des cases : (2, 2) (1, 2)
******
Sommet n°2 . Couleur : 2 . Nb de cases : 3
Sommets adjacents : n°3 n°0
Liste des cases : (2, 0) (2, 1) (1, 1)
******
Sommet n°1 . Couleur : 2 . Nb de cases : 1
Sommets adjacents : n°3 n°0
Liste des cases : (0, 2)
******
Sommet n°0 . Couleur : 1 . Nb de cases : 3
Sommets adjacents : n°1 n°2
Liste des cases : (1, 0) (0, 1) (0, 0)
******
 */

void afficher_graphe_zone(Graphe_zone *g_z);

/* Fonction d'affichage d'un sommet : utilisee dans afficher_graphe_zone */
void afficher_sommet(Sommet *s);

/* Met a jour les champs du graphe_zone g_z en ajoutant s a la Zsg : suivant le meme principe que la fonction 3.2 (mais adapte pour un sommet-zone)*/
void agrandit_Zsg(Graphe_zone *g_z, Sommet *s);
/* Calcule une sequence de couleurs en appliquant la strategie max-bordure : similaire a la fonction 3.3*/
int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff);
/* Realise un parcours en largeur dans le graphe g_z a partir du sommet racine : mise a jour des champs distance et pere de tous les sommets-zones de g_z */
void parcours_en_largeur(Graphe_zone *g_z, Sommet *racine);
/* Calcule  une sequence de couleurs en appliquant la strategie parcours en largeur puis max-bordure*/
int sequence_parcours_largeur_puis_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff);

/* Detruit entierement le graphe g_z en liberant toutes la zone memoire occupee */
void liberer_graphe_zone(Graphe_zone *g_z);

#endif
