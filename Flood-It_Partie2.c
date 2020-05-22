#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "biblio_graphe.h"
 
 
int main(int argc,char**argv){

  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=7){
    printf("usage: %s <dimension nb_de_couleurs niveau_difficulte graine exo(5 6 7) aff(0 1)>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);


  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Generation de l'instance */

  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);

  /* Affichage de la grille */
	if(aff == 1) {
		Grille_init(dim,nbcl, 500,&G);

		Grille_ouvre_fenetre(G);
	

		for (i=0;i<dim;i++)
			for (j=0;j<dim;j++){
				Grille_attribue_couleur_case(G,i,j,M[i][j]);
		}

		Grille_redessine_Grille();
	}
	
	/* calcule sequences */
 	switch(exo) {
 		case(5):
 			//printf(" %d essais.\n", sequence_max_bordure(M, G, dim, nbcl, aff));
 			printf("%d ", sequence_max_bordure(M, G, dim, nbcl, aff));
 			break;
 		case(6):
 			//printf(" %d essais.\n", sequence_parcours_largeur_puis_max_bordure(M, G, dim, nbcl, aff));
 			printf("%d ", sequence_parcours_largeur_puis_max_bordure(M, G, dim, nbcl, aff));
 			break;
 		case(7):
 			//printf(" %d essais.\n", sequence_max_bord_test_en_profondeur(M, G, dim, nbcl, 1));
 			//printf(" %d essais.\n", sequence_ultime(M, G, dim, nbcl, aff));
 			printf("%d ", sequence_ultime(M, G, dim, nbcl, aff));
 			break;
 		default:
 			printf("Vous devez selectionner l'exercice (5, 6 ou 7)\n");
 			break;
 	}
	
  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
  //printf("temps cpu : %f\n",temps_cpu);
  printf("%f\n",temps_cpu);
	
	/* Desalocation de la matrice */
	for(i = 0; i< dim; i++) {
		if (M[i])
		free(M[i]);
	}
	if (M) free(M);
	
	/* Fermeture et desalocation de la grille */
	if(aff == 1) {
		Grille_attente_touche();

		Grille_ferme_fenetre();

		Grille_free(&G);
	}
	
	
  return 0;
}
