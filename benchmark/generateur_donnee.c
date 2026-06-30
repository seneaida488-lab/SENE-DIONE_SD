#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "generateur_donnee.h"

 float rand_float(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

 float rand_gaussien(float moy, float sigma)
{
    float u1 = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
    float u2 = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);

    float z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * M_PI * u2);

    float valeur = moy + sigma * z;

    /* Borner entre 0 et 20 */
    if (valeur < 0.0f)  valeur = 0.0f;
    if (valeur > 20.0f) valeur = 20.0f;

    return valeur;
}

 Etudiant* creer_etudiant_simple(int matricule, float moyenne)
{
    char nom[40];
    char prenom[50];
    Date d = {1, 1, 2000};

    snprintf(nom,    sizeof(nom),    "Etudiant%d", matricule);
    snprintf(prenom, sizeof(prenom), "Prenom%d",   matricule);

    return creer_etudiant(matricule, moyenne, nom, prenom, d);
}

Etudiant** generer_etudiants(int n, Distribution dist)
{
    srand((unsigned int)time(NULL));

    Etudiant **tab = malloc(n * sizeof(Etudiant *));
    if (tab == NULL) {
        fprintf(stderr, "Erreur malloc generateur\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        float moyenne = 0.0f;

        switch (dist) {
            case UNIFORME:
                moyenne = rand_float(0.0f, 20.0f);
                break;

            case GAUSSIENNE:
                moyenne = rand_gaussien(10.0f, 3.0f);
                break;

            case TRIEE:
                moyenne = (float)i * 20.0f / (float)(n - 1);
                break;

            case INVERSE:
                moyenne = 20.0f - (float)i * 20.0f / (float)(n - 1);
                break;
        }

        tab[i] = creer_etudiant_simple(1000 + i, moyenne);

        if (tab[i] == NULL) {
            for (int j = 0; j < i; j++)
                liberer_etudiant(tab[j]);
            free(tab);
            return NULL;
        }
    }

    return tab;
}

void liberer_etudiants(Etudiant **tab, int n)
{
    if (tab == NULL) return;

    for (int i = 0; i < n; i++) {
        if (tab[i] != NULL)
            liberer_etudiant(tab[i]);
    }

    free(tab);
}
const char* nom_distribution(Distribution dist)
{
    switch (dist) {
        case UNIFORME:   return "Uniforme";
        case GAUSSIENNE: return "Gaussienne";
        case TRIEE:      return "Triee";
        case INVERSE:    return "Inverse";
        default:         return "Inconnue";
    }
}