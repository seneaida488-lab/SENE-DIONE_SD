#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

#include "../include/structure.h"

typedef struct Noeud {
    Etudiant      *etudiant;  /* pointeur vers les donnees        */
    struct Noeud  *prec;      /* pointeur vers le noeud précédent */
    struct Noeud  *suiv;      /* pointeur vers le noeud suivant   */
} Noeud;

typedef struct {
    Noeud *tete;    /* sentinelle de début */
    Noeud *queue;   /* sentinelle de fin   */
    int    taille;  /* nombre d'étudiants  */
} ListeDC;

ListeDC* creer_liste(void);

void liberer_liste(ListeDC *l);

void inserer_en_tete(ListeDC *l, Etudiant *e);

void inserer_en_queue(ListeDC *l, Etudiant *e);

Etudiant* rechercher_matricule_liste(ListeDC *l, int matricule);

void rechercher_intervalle_liste(ListeDC *l, float min, float max);

void rechercher_prefixe_liste(ListeDC *l, char *prefixe);

void supprimer_matricule_liste(ListeDC *l, int matricule);

void modifier_moyenne_liste(ListeDC *l, int matricule,
                             float nouvelle_moyenne);

void tri_insertion_liste(ListeDC *l);

void tri_bulles_liste(ListeDC *l);

float moyenne_generale_liste(ListeDC *l);

float minimum_liste(ListeDC *l);

float maximum_liste(ListeDC *l);

float mediane_liste(ListeDC *l);

float ecart_type_liste(ListeDC *l);

void afficher_liste(ListeDC *l);

void afficher_liste_inverse(ListeDC *l);

void serialiser_liste(ListeDC *l, char *nom_fichier);

ListeDC* deserialiser_liste(char *nom_fichier);

#endif /* LISTE_CHAINEE_H */