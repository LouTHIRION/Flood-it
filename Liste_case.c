#include<stdlib.h>
#include<stdio.h>
#include "Liste_case.h"

 /* Initialise une liste vide */
void init_liste(ListeCase *L){
  *L=NULL;
}

/* Ajoute un element en tete de liste */
int ajoute_en_tete(ListeCase *L, int i, int j){
  Elnt_liste *elnt;
  elnt=(Elnt_liste*) malloc(sizeof(Elnt_liste));
  if (elnt==NULL) 
    return 0;
  elnt->suiv=*L;
  elnt->i=i;
  elnt->j=j;
  (*L)=elnt;
}

/* teste si une liste est vide */
int test_liste_vide(ListeCase *L){
  return (*L)==NULL;
}

/* Supprime l element de tete et retourne les valeurs en tete */
/* Attention: il faut que la liste soit non vide */
void enleve_en_tete(ListeCase *L, int *i, int *j){
  Elnt_liste *temp;
  *i=(*L)->i;
  *j=(*L)->j;
  temp=*L;
  *L=(*L)->suiv;
  free(temp);
}

/* Detruit tous les elements de la liste */
void detruit_liste(ListeCase *L){
 Elnt_liste *cour,*temp;
  cour=(*L);
  while (cour!=NULL){
    temp=cour;
    cour=cour->suiv;
    free(temp);
  }

  *L=NULL;
}

/*test si le couple i, j est dans la liste */
int test_est_dans(int i, int j, ListeCase *L) {
	Elnt_liste *cour = (*L);
	while(cour != NULL) {
		if(i == cour->i && j == cour->j) {
			return 1;
		}
		cour = cour->suiv;
	}
	return 0;
}

/* Affichage liste */
void affiche_liste(ListeCase *L) {
	Elnt_liste *cour = (*L);
	while(cour != NULL) {
		printf("(%d, %d) ", cour->i, cour->j);
		cour = cour->suiv;
	}
}

/*int main() {
	ListeCase *l = malloc(sizeof(ListeCase));
	init_liste(l);
	ajoute_en_tete(l, 0, 0);
	//ajoute_en_tete(l, 1, 2);
	affiche_liste(l);
	printf("test est dans %d\n", test_est_dans(0, 1, l));
	
	return 0;
}*/
