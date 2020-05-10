EXEC=Flood-It_Partie1 Flood-It

all: $(EXEC)

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1.c
	
biblio_S_Zsg.o: biblio_S_Zsg.c biblio_S_Zsg.h Liste_case.h
	gcc -c biblio_S_Zsg.c
	
biblio_graphe.o: biblio_graphe.c biblio_graphe.h Liste_case.h Entete_Fonctions.h
	gcc -c biblio_graphe.c
	
Flood-It_Partie1.o: Flood-It_Partie1.c
	gcc -c Flood-It_Partie1.c 

Flood-It_Partie1: Flood-It_Partie1.o Liste_case.o  API_Grille.o API_Gene_instance.o Fonctions_exo1.o biblio_S_Zsg.o
	gcc -o Flood-It_Partie1 Flood-It_Partie1.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o biblio_S_Zsg.o -lSDL

Flood-It: Flood-It.c
	gcc -c Flood-It.c

Flood-It: Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o 
	gcc -o Flood-It Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o -lSDL

clean:
	rm -f *.o $(EXEC)
