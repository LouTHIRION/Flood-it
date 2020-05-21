/* biblio_S_Zsg.h */
#ifndef _BIBLIO_S_ZSG_
#define _BIBLIO_S_ZSG_


#include <stdlib.h>
#include <stdio.h>
#include "API_Grille.h"
#include "Liste_case.h"

typedef struct {
	int dim;	/* dimensions de la grille */
	int nbcl;	/* nombre de couleur */
	
	ListeCase Lzsg;	/* liste des cases de la zone Zsg */
	ListeCase *B;	/* tableau de liste de case de la bordure de Zsg */
	int **App;	/* tableau a double entree des appartenances */
} S_Zsg;

/* initialistation S_Zsg */
int init_Zsg(S_Zsg *S, int dim, int nbcl);

/* ajoute une case a Lzsg */
void ajoute_Zsg(S_Zsg *S, int i, int j);

/* ajoute une case dans la bordure, d'une couleur cl donnee */
void ajoute_Bordure(S_Zsg *S, int i, int j, int cl);

/* renvoie vrai si une case est dans LZsg */
int appartient_Zsg(int i, int j, S_Zsg *S);

/* renvoie vrai si une case est dans la bordure de couleur cl donnee */
int appartient_Bordure(int i, int j, int cl, S_Zsg *S);

/* Libere la memoire pour la structure et tous ses elements */
void detruit_S_Zsg(S_Zsg *S);

/* met a jour les champs Lzsg et B d'une S_Zsg lorsqu'une case k,l de couleur cl 
   doit basculer dans Lzsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

/* Algorithme tirant au sort une couleur: 
   il utilise la fonction agrandit_zone pour determiner la Zsg */
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);

#endif
