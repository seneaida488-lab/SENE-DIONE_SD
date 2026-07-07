#ifndef TABLEAU_STATIQUE_H
#define TABLEAU_STATIQUE_H

#include "structure.h"
#define TAILLEMAX 100000

int insertionTableau(Etudiant tab[], int *n, Etudiant e);

int RechercheCleprimaire(Etudiant tab[], int n, int matricule);

/* Recherche par préfixe sur le nom. */
void RechercheParPrefixe(Etudiant tab[], int n, char prefixe[]);

/* Recherche par intervalle de moyenne. */
void RechercheParIntervalle(Etudiant tab[], int n,
                             float minmoyenne, float maxmoyenne);

void suppressionCleprimaire(Etudiant tab[], int *n, int matricule);


/* Met à jour le nom d'un étudiant identifié par son matricule. */
void Miseajour(Etudiant tab[], int n, int matricule, char nouveaunom[]);


/* Partitionne le tableau pour le tri rapide.
   Retourne l'index du pivot. */
int partitionnerMoyenne(Etudiant *tab, int premier, int dernier);

/* Trie par moyenne (tri rapide récursif). */
void TriRapide(Etudiant tab[], int premier, int dernier);

/* Trie par moyenne croissante (tri par insertion). */
void Tri_insertion_moyenne(Etudiant *tab, int n);

/* Retourne la plus petite moyenne. */
int minimumDesmoyennes(Etudiant tab[], int n);

/* Retourne la plus grande moyenne. */
int maximumDesmoyennes(Etudiant tab[], int n);

/* Retourne la moyenne générale. */
double MoyenneDesmoyennes(Etudiant tab[], int n);

/* Retourne la médiane des moyennes. */
float MedianeDesmoyennes(Etudiant tab[], int n);

/* Retourne l'écart-type des moyennes. */
float EcartTypeDesmoyennes(Etudiant tab[], int n);

#endif /* TABLEAU_STATIQUE_H */