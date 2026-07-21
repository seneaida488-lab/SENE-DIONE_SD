#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

#include "../include/structure.h"

/* BUG CORRIGE : le typedef "Etudiant" qui existait ici a ete supprime. Un
   seul type Etudiant existe desormais, dans structure.h, importe ci-dessus.
   Voir le commentaire detaille dans structure.h pour l'explication complete
   du bug de types incompatibles entre les quatre definitions dupliquees. */

/* --- STRUCTURES --- */
typedef struct Noeud {
    Etudiant *etudiant;        /* pointeur vers les données               */
    struct Noeud *suivant;     /* pointeur vers le noeud suivant          */
} Noeud;

typedef struct {
    Noeud *tete;    /* premier noeud de la liste (NULL si vide)    */
    Noeud *queue;   /* dernier noeud de la liste (NULL si vide)    */
    int taille;     /* nombre d'étudiants dans la liste            */
} ListeC;

/* --- FONCTIONS ETUDIANT (dépendent de structure.h) --- */

void liberer_etudiant(Etudiant *e);
Etudiant* creer_etudiant(int matricule, float moyenne, char *nom, char *prenom, Date d);
/* --- FONCTIONS LISTE --- */

ListeC* liste_creer(void);
void liste_liberer(ListeC *l);

/* Insertion */
void inserer_en_tete(ListeC *l, Etudiant *e);
void inserer_en_queue(ListeC *l, Etudiant *e);

/* Recherche */
Etudiant* rechercher_matricule_liste(ListeC *l, int matricule);
void rechercher_intervalle_liste(ListeC *l, float min, float max);
void rechercher_liste_prefixes(ListeC *l, char *prefixe);

/* Suppression / Modification */
void supprimer_liste_matricule(ListeC *l, int matricule);
void liste_modifier_moyenne(ListeC *l, int matricule, float nouvelle_moyenne);

/* Tri */
void liste_tri_insertion(ListeC *l);
void liste_tri_bulles(ListeC *l);

/* Statistiques */
float moyenne_generale_liste(ListeC *l);
float liste_minimum(ListeC *l);
float liste_maximum(ListeC *l);
float liste_mediane(ListeC *l);
float liste_ecart_type(ListeC *l);

/* Affichage */
void afficher_liste(ListeC *l);
void afficher_liste_inverse(ListeC *l);

/* Persistance (serialisation binaire) */
void liste_serialiser(ListeC *l, char *nom_fichier);
ListeC* liste_deserialiser(char *nom_fichier);

#endif // LISTE_CHAINEE_H