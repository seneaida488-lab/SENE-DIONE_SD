#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/structure.h"
int main(){
Note* creer_note(int matricule, float valeur, char *matiere,
                 char *appreciation, Date date, Cours *cours)
{
    Note *n = malloc(sizeof(Note));
    if (n == NULL) {
        fprintf(stderr, "Erreur malloc Note\n");
        return NULL;
    }
    n->matricule = matricule;
    n->valeur    = valeur;
    strncpy(n->matiere, matiere, sizeof(n->matiere) - 1);
    n->matiere[sizeof(n->matiere) - 1] = '\0'; 
    n->appreciation = malloc(strlen(appreciation) + 1);
    if (n->appreciation == NULL) {
        fprintf(stderr, "Erreur malloc appreciation\n");
        free(n);
        return NULL;
    }
    strcpy(n->appreciation, appreciation);
    n->dateDeliberation = date;
    n->cours = cours;
    for (int k = 0; k < 10; k++) {
        n->tab[k] = 0;
    }

    return n;
}
void afficher_note(Note *n)
{
    if (n == NULL) {
        printf("Note NULL\n");
        return;
    }
    printf("--- Note ---\n");
    printf("  Matricule    : %d\n",   n->matricule);
    printf("  Valeur       : %.2f\n", n->valeur);
    printf("  Matiere      : %s\n",   n->matiere);
    printf("  Appreciation : %s\n",   n->appreciation);
    printf("  Date         : %02d/%02d/%04d\n",
           n->dateDeliberation.jour,
           n->dateDeliberation.mois,
           n->dateDeliberation.annee);
    if (n->cours != NULL) {
        printf("  Cours        : [%d] %s\n", n->cours->id, n->cours->nom);
}
}
void liberer_note(Note *n)
{
    if (n == NULL) return;
    free(n->appreciation);
    n->appreciation = NULL;
    free(n);
}
Inscription* creer_inscription(int matricule, float montant,
                                char *etat, char *filliere,
                                Date date, Niveau *niveau)
{
    Inscription *i = malloc(sizeof(Inscription));
    if (i == NULL) {
        fprintf(stderr, "Erreur malloc Inscription\n");
        return NULL;
    }

    i->matricule = matricule;
    i->montant   = montant;
    strncpy(i->etat, etat, sizeof(i->etat) - 1);
    i->etat[sizeof(i->etat) - 1] = '\0';
    i->filliere = malloc(strlen(filliere) + 1);
    if (i->filliere == NULL) {
        fprintf(stderr, "Erreur malloc filliere\n");
        free(i);
        return NULL;
    }
    strcpy(i->filliere, filliere);

    i->dateInscription = date;
    i->niveau          = niveau;

    for (int k = 0; k < 20; k++) {
        i->tab[k] = 0;
    }

    return i;
}
void afficher_inscription(Inscription *i)
{
    if (i == NULL) {
        printf("Inscription NULL\n");
        return;
    }
    printf("--- Inscription ---\n");
    printf("  Matricule : %d\n",     i->matricule);
    printf("  Montant   : %.2f\n",   i->montant);
    printf("  Etat      : %s\n",     i->etat);
    printf("  Filiere   : %s\n",     i->filliere);
    printf("  Date      : %02d/%02d/%04d\n",
           i->dateInscription.jour,
           i->dateInscription.mois,
           i->dateInscription.annee);
    if (i->niveau != NULL) {
        printf("  Niveau    : %s\n", i->niveau->nom);
    }
}
void liberer_inscription(Inscription *i)
{
    if (i == NULL) return;
    free(i->filliere);
    i->filliere = NULL;
    free(i);
}
Etudiant* creer_etudiant(int matricule, float moyenne,
                          char *nom, char *prenom, Date dateNaissance)
{
    Etudiant *e = malloc(sizeof(Etudiant));
    if (e == NULL) {
        fprintf(stderr, "Erreur malloc Etudiant\n");
        return NULL;
    }

    e->matricule = matricule;
    e->moyenne   = moyenne;
    strncpy(e->nom, nom, sizeof(e->nom) - 1);
    e->nom[sizeof(e->nom) - 1] = '\0';
    e->prenom = malloc(strlen(prenom) + 1);
    if (e->prenom == NULL) {
        fprintf(stderr, "Erreur malloc prenom\n");
        free(e);
        return NULL;
    }
    strcpy(e->prenom, prenom);

    e->dateNaissance = dateNaissance;
    for (int k = 0; k < 10; k++) {
        e->tabnote[k] = NULL;
    }
    e->inscription = NULL;

    return e;
}
void ajouter_note_etudiant(Etudiant *e, Note *n, int index)
{
    if (e == NULL || n == NULL) return;
    if (index < 0 || index >= 10) {
        fprintf(stderr, "Index note invalide : %d\n", index);
        return;
    }
    e->tabnote[index] = n;
}
void afficher_etudiant(Etudiant *e)
{
    if (e == NULL) {
        printf("Etudiant NULL\n");
        return;
    }
    printf("========== Etudiant ==========\n");
    printf("  Matricule      : %d\n",     e->matricule);
    printf("  Nom            : %s\n",     e->nom);
    printf("  Prenom         : %s\n",     e->prenom);
    printf("  Moyenne        : %.2f\n",   e->moyenne);
    printf("  Date naissance : %02d/%02d/%04d\n",
           e->dateNaissance.jour,
           e->dateNaissance.mois,
           e->dateNaissance.annee);
    printf("  Notes :\n");
    int aucune = 1;
    for (int k = 0; k < 10; k++) {
        if (e->tabnote[k] != NULL) {
            printf("    [%d] %.2f en %s\n",
                   k,
                   e->tabnote[k]->valeur,
                   e->tabnote[k]->matiere);
            aucune = 0;
        }
    }
    if (aucune) printf("    (aucune note)\n");
    if (e->inscription != NULL) {
        afficher_inscription(e->inscription);
    }
    printf("==============================\n");
}
void liberer_etudiant(Etudiant *e)
{
    if (e == NULL) return;
    for (int k = 0; k < 10; k++) {
        if (e->tabnote[k] != NULL) {
            liberer_note(e->tabnote[k]);
            e->tabnote[k] = NULL;
        }
    }
    if (e->inscription != NULL) {
        liberer_inscription(e->inscription);
        e->inscription = NULL;
    }
    free(e->prenom);
    e->prenom = NULL;
    free(e);
}
return 0;
}
