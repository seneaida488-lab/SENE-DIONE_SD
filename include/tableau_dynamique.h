#ifndef TABLEAU_DYNAMIQUE_H
#define TABLEAU_DYNAMIQUE_H

#define CAPACITE_INITIALE 4
#include "../include/structure.h"

typedef struct {
    int matricule;
    char nom[50];
    char *prenom;
    float moyenne;
    Date dateNaissance;
    
    // --- LES AJOUTS REQUIS ---
    Date dateInscription;   // Utilise votre structure Date existante
    char filiere[50];       // Chaîne de caractères pour la filière
    char niveau[20];        // Exemple : L1, L2, L3, M1
    char matiere[50];       // Nom de la matière principale
} Etudiant;

typedef struct {
    Etudiant **tab;   /* tableau de pointeurs vers Etudiant */
    int taille; 
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

void afficher_tableau_dyn(TableauDyn *t);

void serialiser_dyn(TableauDyn *t, char *nom_fichier);

TableauDyn* deserialiser_dyn(char *nom_fichier);
#endif /* TABLEAU_DYNAMIQUE_H */