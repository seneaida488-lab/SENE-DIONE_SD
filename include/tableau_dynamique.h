#ifndef TABLEAU_DYNAMIQUE_H
#define TABLEAU_DYNAMIQUE_H

#include "scolarite.h"
#define CAPACITE_INITIALE 4

typedef struct {
    Etudiant **tab;   /* tableau de pointeurs vers Etudiant */
    int taille; 
	     /* nombre d'étudiants stocké#ifndef STRUCTURE_H
#define SCOLARITE_H

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

#endif /* SCOLARITE_H */         */
    int capacite;     /* capacité actuelle du tableau       */
} TableauDyn;

TableauDyn* creer_tableau_dyn(void);

void liberer_tableau_dyn(TableauDyn *t);

void inserer_etudiant_dyn(TableauDyn *t, Etudiant *e);

Etudiant* rechercher_par_matricule_dyn(TableauDyn *t, int matricule);

void rechercher_par_intervalle_dyn(TableauDyn *t, float min, float max);

void rechercher_par_prefixe_dyn(TableauDyn *t, char *prefixe);

void supprimer_par_matricule_dyn(TableauDyn *t, int matricule);

void modifier_moyenne_dyn(TableauDyn *t, int matricule,
                           float nouvelle_moyenne);

void tri_insertion_dyn(TableauDyn *t);

void tri_rapide_dyn(TableauDyn *t, int gauche, int droite);

float moyenne_generale_dyn(TableauDyn *t);

float minimum_dyn(TableauDyn *t);

float maximum_dyn(TableauDyn *t);

float mediane_dyn(TableauDyn *t);

float ecart_type_dyn(TableauDyn *t);

void afficher_tableau(taeauDyn *t);

void serialiser_dyn(TableauDyn *t, char *nom_fichier);

TableauDyn* deserialiser_dyn(char *nom_fichier);

#endif /* TABLEAU_DYNAMIQUE_H */