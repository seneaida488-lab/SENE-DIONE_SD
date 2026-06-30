#ifndef GENERATEUR_DONNEE_H
#define GENERATEUR_DONNEE_H

#include "../include/structure.h"

#define N1      100
#define N2     1000
#define N3    10000
#define N4   100000

typedef enum {
    UNIFORME,
    GAUSSIENNE,
    TRIEE,
    INVERSE,
} Distribution;

Etudiant** generer_etudiants(int n, Distribution dist);

void liberer_etudiants(Etudiant **tab, int n);


const char* nom_distribution(Distribution dist);

#endif /* GENERATEUR_DONNEE_H */