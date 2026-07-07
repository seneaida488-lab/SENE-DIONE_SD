#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structure.h"
int main(void) {

    /* -- Initialisation d'une Note -- */
    Note tabnote;
    tabnote.matricule   = 1213;
    tabnote.valeur      = 17.15f;
    strcpy(tabnote.matiere, "physique");
    strcpy(tabnote.appreciation, "tres bien");
    /* commentaire : longueur variable ? malloc */
    tabnote.appreciation = malloc(strlen("Excellent travail") + 1);
    strcpy(tabnote.appreciation, "Excellent travail");

    /* -- Initialisation d'une Inscription -- */
    Inscription i;
    i.matricule = 123;
    i.montant   = 26324.45f;
    strcpy(i.etat, "valide");
    /* filiere : longueur variable ? malloc */
    i.filliere = malloc(strlen("Informatique") + 1);
    strcpy(i.filliere, "Informatique");

    /* -- Initialisation de l'Etudiant -- */
    Etudiant e;
    e.matricule = 1001;
    e.moyenne   = 14.75f;
    strcpy(e.prenom, "Moussa");
    /* nom : longueur variable ? malloc */
    e.nom = malloc(strlen("Ndiaye") + 1);
    strcpy(e.nom, "Ndiaye");

    e.dateNaissance.jour  = 5;
    e.dateNaissance.mois  = 3;
    e.dateNaissance.annee = 2004;

    e.tabnote    = &tabnote;   /* pointeur vers Note */
    e.inscription = &i;         /* pointeur vers Inscription */

    /* -- Remplissage du tableau interne -- */
    for (int j = 0; j < 40; j++) {
        e.tab[j] = j + 1;
    }

    /* -- Affichage -- */
    printf("Etudiant : %s %s\n", e.prenom, e.nom);
    printf("Matricule : %d | Moyenne : %.2f\n", e.matricule, e.moyenne);
    printf("Naissance : %02d/%02d/%d\n",
           e.dateNaissance.jour,
           e.dateNaissance.mois,
           e.dateNaissance.annee);
    printf("Note en %s : %.2f (%s)\n",
           e.tabnote->matiere,
           e.tabnote->valeur,
           e.tabnote->appreciation);
    printf("Filiere : %s\n", e.inscription->filliere);

    free(e.nom);
    free(tabnote.appreciation);
    free(i.filliere);

    return 0;
}