#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/tableau_statique.h"

/* 
 * Insère un étudiant à la bonne position (tri par matricule).
 * Retourne 1 si succès, 0 si tableau plein.
 */
int insertionTableau(Etudiant tab[], int *n, Etudiant e)
{
    int i, pos;

    if (*n >= TAILLEMAX) {
        printf("le tableau est plein");
        return 0;
    }

    /* Trouver la position d'insertion */
    for (i = 0; i < *n; i++) {
        if (e.matricule < tab[i].matricule) {
            pos = i;
            break;
        }
    }

    if (i == *n) {
        pos = *n;
    }

    /* Décaler les éléments vers la droite */
    for (i = *n; i > pos; i--) {
        tab[i].matricule = tab[i-1].matricule;
    }

    tab[pos] = e;
    (*n)++;
    return 1;
}

/*
 * RechercheCleprimaire
 * Parcours séquentiel par matricule.
 * Retourne l'index si trouvé, -1 sinon.
 * Complexité : O(n).
 */
int RechercheCleprimaire(Etudiant tab[], int n, int matricule)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].matricule == matricule) {
            return i;
        }
    }
    return -1;
}

/*
 * RechercheParPrefixe
 * Affiche les étudiants dont le nom commence par le préfixe.
 * Utilise strncmp pour comparer les n premiers caractères.
 */
void RechercheParPrefixe(Etudiant tab[], int n, char prefixe[])
{
    int i;
    int len = strlen(prefixe);

    printf("Etudiant dont le nom commence par : %s\n", prefixe);
    for (i = 0; i < n; i++) {
        if (strncmp(tab[i].nom, prefixe, len) == 0) {
            printf("%s", tab[i].nom);
        }
    }
}

/*
 * RechercheParIntervalle
 * Affiche les étudiants dont la moyenne est entre min et max.
 */
void RechercheParIntervalle(Etudiant tab[], int n,
                             float minmoyenne, float maxmoyenne)
{
    int i;
    printf("Etudiant dont la moyenne est entre %.2f et %.2f :\n",
           minmoyenne, maxmoyenne);
    for (i = 0; i < n; i++) {
        if (tab[i].moyenne >= minmoyenne &&
            tab[i].moyenne <= maxmoyenne) {
            printf("%.2f ", tab[i].moyenne);
        }
    }
}

/*
 * suppressionCleprimaire
 * Trouve la position, décale les éléments vers la gauche.
 * Complexité : O(n).
 */
void suppressionCleprimaire(Etudiant tab[], int *n, int matricule)
{
    int i;
    int pos = RechercheCleprimaire(tab, *n, matricule);

    if (pos > -1) {
        for (i = pos; i < *n - 1; i++) {
            tab[i] = tab[i+1];
        }
        (*n)--;
    }
}
/*
 * Miseajour
 * Met à jour le nom d'un étudiant identifié par son matricule.
 */
void Miseajour(Etudiant tab[], int n, int matricule, char nouveaunom[])
{
    int pos = RechercheCleprimaire(tab, n, matricule);
    if (pos > -1) {
        printf("matricule trouver");
        strcpy(tab[pos].nom, nouveaunom);
    }
}

/*
 * partitionnerMoyenne
 * Partitionne le tableau autour d'un pivot (moyenne du premier).
 * Utilisé par TriRapide.
 */
int partitionnerMoyenne(Etudiant *tab, int premier, int dernier)
{
    Etudiant tmp;
    double pivot = tab[(premier + dernier) / 2].moyenne;  /* pivot au milieu, plus robuste */
    int i = premier - 1;
    int j = dernier + 1;

    while (1) {
        do {
            j--;
        } while (tab[j].moyenne > pivot);

        do {
            i++;
        } while (tab[i].moyenne < pivot);

        if (i >= j)
            return j;

        tmp    = tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
    }
}

/*
 * TriRapide
 * Tri rapide récursif par moyenne.
 * Complexité : O(n log n) moyen, O(n²) pire cas.
 */
void TriRapide(Etudiant tab[], int premier, int dernier)
{
    int pivot;
    if (premier < dernier) {
        pivot = partitionnerMoyenne(tab, premier, dernier);
        TriRapide(tab, premier, pivot - 1);
        TriRapide(tab, pivot + 1, dernier);
    }
}

/*
 * Tri_insertion_moyenne
 * Tri par insertion par moyenne croissante.
 * Complexité : O(n²) pire cas, O(n) si déjà trié.
 */
void Tri_insertion_moyenne(Etudiant *tab, int n)
{
    Etudiant x;
    int i, j;

    for (i = 1; i < n; i++) {
        x = tab[i];
        j = i;
        while (j > 0 && tab[j-1].moyenne > x.moyenne) {
            tab[j] = tab[j-1];
            j = j - 1;
        }
        tab[j] = x;
    }
}
/*
 * minimumDesmoyennes
 * Retourne la plus petite moyenne.
 */
int minimumDesmoyennes(Etudiant tab[], int n)
{
    double min = tab[0].moyenne;
    int i;
    for (i = 1; i < n; i++) {
        if (tab[i].moyenne < min) {
            min = tab[i].moyenne;
        }
    }
    return min;
}

/*
 * maximumDesmoyennes
 * Retourne la plus grande moyenne.
 */
int maximumDesmoyennes(Etudiant tab[], int n)
{
    double max = tab[0].moyenne;
    int i;
    for (i = 1; i < n; i++) {
        if (tab[i].moyenne > max) {
            max = tab[i].moyenne;
        }
    }
    return max;
}

/*
 * MoyenneDesmoyennes
 * Retourne la moyenne générale.
 */
double MoyenneDesmoyennes(Etudiant tab[], int n)
{
    double s = 0;
    int i;
    double Moy;
    for (i = 0; i < n; i++)
        s += tab[i].moyenne;
    Moy = s / n;
    return Moy;
}

/*
 * MedianeDesmoyennes
 * Retourne la médiane des moyennes.
 * ATTENTION : suppose que le tableau est déjà trié.
 */
float MedianeDesmoyennes(Etudiant tab[], int n)
{
    if (n % 2 != 0) {
        return tab[n/2].moyenne;       /* impair */
    }
    else {
        return (tab[n/2 - 1].moyenne + tab[n/2].moyenne) / 2; /* pair */
    }
}

/*
 * EcartTypeDesmoyennes
 * Retourne l'écart-type des moyennes.
 * Formule : sqrt( somme((xi - moy)²) / n )
 */
float EcartTypeDesmoyennes(Etudiant tab[], int n)
{
    float moyenne, ecart;
    float somme      = 0;
    float sommecarre = 0;
    int i;

    for (i = 0; i < n; i++) {
        somme += tab[i].moyenne;
    }
    moyenne = somme / n;

    for (i = 0; i < n; i++) {
        ecart       = tab[i].moyenne - moyenne;
        sommecarre += ecart * ecart;
    }
    return sqrt(sommecarre / n);
}