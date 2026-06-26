#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* --- Type Date --- */
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

/* --- Type Cours --- */
typedef struct {
    int id;
    char nom[20];
} Cours;

/* --- Type Note --- */
typedef struct {
    int matricule;
    float valeur;
    char matiere[50];
    char *appreciation;      /* longueur variable -> malloc */
    Date dateDeliberation;
    Cours *cours;            /* pointeur vers un Cours */
    int tab[10];
} Note;

/* --- Type Niveau --- */
typedef struct {
    char nom[16];
} Niveau;

/* --- Type Inscription --- */
typedef struct {
    int matricule;
    float montant;
    char etat[30];
    char *filliere;          /* longueur variable -> malloc */
    Date dateInscription;
    Niveau *niveau;
    int tab[20];
} Inscription;

/* --- Type Etudiant (type principal) --- */
typedef struct {
    int matricule;
    float moyenne;
    char nom[40];
    char *prenom;            /* longueur variable -> malloc */
    Date dateNaissance;
    Note *tabnote[10];       /* tableau de 10 pointeurs vers Note */
    Inscription *inscription;
} Etudiant;

#endif /* STRUCTURE_H */