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

/* --- Type Etudiant (type principal, UNIQUE dans tout le projet) ---
   CORRIGE : ce type etait auparavant re-defini independamment dans
   tableau_statique.h, tableau_dynamique.h et liste_chainee.h, chacun avec
   un "typedef struct { ... } Etudiant;" SANS nom de tag. En C, une struct
   anonyme cree un type distinct a chaque occurrence -- meme si les champs
   sont identiques -- donc le compilateur voyait quatre types "Etudiant"
   totalement incompatibles entre eux. Resultat concret : creer_etudiant()
   (declaree dans liste_chainee.h) renvoyait un Etudiant* different de
   celui attendu par inserer_etudiant_dyn() (declaree dans
   tableau_dynamique.h), d'ou l'erreur de compilation "l'argument de type
   'Etudiant' est incompatible avec le parametre de type 'E...'".

   Desormais Etudiant n'existe qu'ICI. tableau_statique.h, tableau_dynamique.h
   et liste_chainee.h se contentent d'un #include "structure.h" et n'en
   redefinissent plus de copie.

   Les champs dateInscription/filiere/niveau/matiere ont ete ajoutes ici
   (ils existaient deja, identiques, dans les trois definitions dupliquees)
   pour que le code existant (menus d'insertion/affichage dans
   main_principal.c) continue de fonctionner sans modification.

   Les champs tabnote/inscription (deja utilises par main_structure.c)
   sont conserves : ce sont eux qui repondent a l'exigence du cahier des
   charges "au moins un pointeur vers un enregistrement d'un autre type"
   et "au moins un champ tableau ou liste interne non trivial". */
typedef struct {
    int matricule;
    float moyenne;
    char nom[40];
    char *prenom;            /* longueur variable -> malloc */
    Date dateNaissance;
    Note *tabnote[10];       /* tableau de 10 pointeurs vers Note */
    Inscription *inscription;

    /* --- Champs "a plat" utilises par les menus tableau/liste --- */
    Date dateInscription;
    char filiere[50];
    char niveau[20];
    char matiere[50];
} Etudiant;

#endif /* STRUCTURE_H */