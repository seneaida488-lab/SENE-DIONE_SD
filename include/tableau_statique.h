#ifndef TABLEAU_STATIQUE_H
#define TABLEAU_STATIQUE_H

#include "structure.h"
#define TAILLEMAX 100000

typedef struct {
    int matricule;
    char nom[50];
    char prenom[100];
    float moyenne;
    Date dateNaissance;

    // --- LES AJOUTS REQUIS ---
    Date dateInscription;   // Utilise votre structure Date existante
    char filiere[50];       // Chaîne de caractères pour la filière
    char niveau[20];        // Exemple : L1, L2, L3, M1
    char matiere[50];       // Nom de la matière principale
} Etudiant;

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

/* Retourne la plus petite moyenne.
   BUG CORRIGE : declaree "int" alors que "moyenne" est un float (ex: 17.50).
   Un retour "int" tronque toute partie decimale (17.50 -> 17) et surtout
   provoque un comportement indefini partout ou le resultat est affiche
   avec un format "%.2f" (qui attend un double dans les arguments
   variadiques d'un printf, pas un int). Retype en "float" pour rester
   coherent avec MedianeDesmoyennes/EcartTypeDesmoyennes qui renvoient
   deja des float. -> Pense a mettre a jour la definition (.c) en
   consequence : "float minimumDesmoyennes(...)". */
float minimumDesmoyennes(Etudiant tab[], int n);

/* Retourne la plus grande moyenne. Meme correction que ci-dessus. */
float maximumDesmoyennes(Etudiant tab[], int n);

/* Retourne la moyenne générale. */
double MoyenneDesmoyennes(Etudiant tab[], int n);

/* Retourne la médiane des moyennes. */
float MedianeDesmoyennes(Etudiant tab[], int n);

/* Retourne l'écart-type des moyennes. */
float EcartTypeDesmoyennes(Etudiant tab[], int n);

#endif /* TABLEAU_STATIQUE_H */