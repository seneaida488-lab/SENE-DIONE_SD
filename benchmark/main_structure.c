#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structure.h"

int main(void) {

    /* -- Initialisation d'une Note -- */
    Note tabnote;
    tabnote.matricule = 1213;
    tabnote.valeur    = 17.15f;
    strcpy(tabnote.matiere, "physique");     /* matiere est char[50] : strcpy direct */

    /* appreciation est char* : il FAUT malloc avant strcpy */
    tabnote.appreciation = malloc(strlen("Excellent travail") + 1);
    strcpy(tabnote.appreciation, "Excellent travail");

    tabnote.cours = NULL;   /* pas de cours associe pour ce test */
    for (int k = 0; k < 10; k++) {
        tabnote.tab[k] = k;
    }

    /* -- Initialisation d'une Inscription -- */
    Inscription i;
    i.matricule = 123;
    i.montant   = 26324.45f;
    strcpy(i.etat, "valide");                /* etat est char[30] : strcpy direct */

    /* filliere est char* : il FAUT malloc avant strcpy */
    i.filliere = malloc(strlen("Informatique") + 1);
    strcpy(i.filliere, "Informatique");

    i.niveau = NULL;
    for (int k = 0; k < 20; k++) {
        i.tab[k] = k;
    }

    /* -- Initialisation de l'Etudiant -- */
    Etudiant e;
    e.matricule = 1001;
    e.moyenne   = 14.75f;

    /* nom est char[40] : strcpy direct, PAS de malloc */
    strcpy(e.nom, "Ndiaye");

    /* prenom est char* : il FAUT malloc avant strcpy */
    e.prenom = malloc(strlen("Moussa") + 1);
    strcpy(e.prenom, "Moussa");

    e.dateNaissance.jour  = 5;
    e.dateNaissance.mois  = 3;
    e.dateNaissance.annee = 2004;

    /* tabnote est un tableau de 10 pointeurs vers Note : on remplit l'element 0 */
    e.tabnote[0] = &tabnote;
    for (int j = 1; j < 10; j++) {
        e.tabnote[j] = NULL;   /* les autres emplacements ne sont pas utilises ici */
    }

    /* inscription est un simple pointeur vers Inscription */
    e.inscription = &i;

    /* -- Affichage -- */
    printf("Etudiant : %s %s\n", e.prenom, e.nom);
    printf("Matricule : %d | Moyenne : %.2f\n", e.matricule, e.moyenne);
    printf("Naissance : %02d/%02d/%d\n",
           e.dateNaissance.jour,
           e.dateNaissance.mois,
           e.dateNaissance.annee);

    printf("Note en %s : %.2f (%s)\n",
           e.tabnote[0]->matiere,
           e.tabnote[0]->valeur,
           e.tabnote[0]->appreciation);

    printf("Filiere : %s\n", e.inscription->filliere);

    /* -- Liberation memoire -- */
    free(e.prenom);
    free(tabnote.appreciation);
    free(i.filliere);

    return 0;
}