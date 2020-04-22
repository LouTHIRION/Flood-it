#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"

 
 
int main(int argc,char**argv){

  int dim, nbcl, nivdif, graine;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=5){
    printf("usage: %s <dimension nb_de_couleurs niveau_difficulte graine>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);


  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);

  /* Generation de l'instance */

  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);

  /* Affichage de la grille */

  Grille_init(dim,nbcl, 500,&G);

  Grille_ouvre_fenetre(G);

  for (i=0;i<dim;i++)
    for (j=0;j<dim;j++){
      Grille_attribue_couleur_case(G,i,j,M[i][j]);
    }

  Grille_redessine_Grille();
 
  /* 
     A VOUS DE JOUER

  */


  Grille_attente_touche();
 
  Grille_ferme_fenetre();

  Grille_free(&G);

  return 0;
}
