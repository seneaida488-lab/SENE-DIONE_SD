#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/liste_chainee.h"
#include "../include/structure.h"
void liberer_etudiant(Etudiant *e);

 Noeud* creer_noeud(Etudiant *e)
{
    Noeud *n = malloc(sizeof(Noeud));
    if (n == NULL) return NULL;
    n->etudiant = e; n->prec = NULL; n->suiv = NULL;
    return n;
}

ListeDC* creer_liste(void)
{
    ListeDC *l = malloc(sizeof(ListeDC));
    if (l == NULL) return NULL;
    l->tete  = creer_noeud(NULL);
    l->queue = creer_noeud(NULL);
    if (l->tete == NULL || l->queue == NULL) { free(l->tete); free(l->queue); free(l); return NULL; }
    l->tete->suiv  = l->queue;
    l->queue->prec = l->tete;
    l->taille = 0;
    return l;
}

void liberer_liste(ListeDC *l)
{
    if (l == NULL) return;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        Noeud *suivant = courant->suiv;
        if (courant->etudiant != NULL) 
        {
            liberer_etudiant(courant->etudiant);
        }
        free(courant);
        courant = suivant;
    }
    free(l->tete); free(l->queue); free(l);
}

void inserer_en_tete(ListeDC *l, Etudiant *e)
{
    if (l == NULL || e == NULL) return;
    Noeud *nouveau = creer_noeud(e);
    if (nouveau == NULL) return;
    Noeud *ancien = l->tete->suiv;
    nouveau->prec = l->tete; nouveau->suiv = ancien;
    l->tete->suiv = nouveau; ancien->prec = nouveau;
    l->taille++;
}

void inserer_en_queue(ListeDC *l, Etudiant *e)
{
    if (l == NULL || e == NULL) return;
    Noeud *nouveau = creer_noeud(e);
    if (nouveau == NULL) return;
    Noeud *ancien = l->queue->prec;
    nouveau->suiv = l->queue; nouveau->prec = ancien;
    ancien->suiv = nouveau; l->queue->prec = nouveau;
    l->taille++;
}

Etudiant* rechercher_matricule_liste(ListeDC *l, int matricule)
{
    if (l == NULL) return NULL;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        if (courant->etudiant->matricule == matricule) return courant->etudiant;
        courant = courant->suiv;
    }
    return NULL;
}

void rechercher_intervalle_liste(ListeDC *l, float min, float max)
{
    if (l == NULL || l->taille == 0) { printf("Liste vide.\n"); return; }
    printf("=== Intervalle [%.2f , %.2f] ===\n", min, max);
    int nb = 0;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        float m = courant->etudiant->moyenne;
        if (m >= min && m <= max) {
            nb++;
            printf("  %d. [%d] %s %s — %.2f\n", nb,
                   courant->etudiant->matricule,
                   courant->etudiant->nom,
                   courant->etudiant->prenom, m);
        }
        courant = courant->suiv;
    }
    if (nb == 0) printf("  Aucun resultat.\n");
    else printf("  Total : %d\n", nb);
}

void rechercher_prefixe_liste(ListeDC *l, char *prefixe)
{
    if (l == NULL || prefixe == NULL) return;
    int len = strlen(prefixe), nb = 0;
    printf("=== Prefixe \"%s\" ===\n", prefixe);
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        if (strncmp(courant->etudiant->nom, prefixe, len) == 0) {
            nb++;
            printf("  %d. [%d] %s %s — %.2f\n", nb,
                   courant->etudiant->matricule,
                   courant->etudiant->nom,
                   courant->etudiant->prenom,
                   courant->etudiant->moyenne);
        }
        courant = courant->suiv;
    }
    if (nb == 0) printf("  Aucun resultat.\n");
}

void supprimer_matricule_liste(ListeDC *l, int matricule)
{
    if (l == NULL) return;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        if (courant->etudiant->matricule == matricule) {
            courant->prec->suiv = courant->suiv;
            courant->suiv->prec = courant->prec;
            liberer_etudiant(courant->etudiant);
            free(courant);
            l->taille--;
            printf("Etudiant %d supprime.\n", matricule);
            return;
        }
        courant = courant->suiv;
    }
    printf("Matricule %d non trouve.\n", matricule);
}

void modifier_moyenne_liste(ListeDC *l, int matricule, float nouvelle_moyenne)
{
    Etudiant *e = rechercher_matricule_liste(l, matricule);
    if (e == NULL) { printf("Matricule %d non trouve.\n", matricule); return; }
    float ancienne = e->moyenne;
    e->moyenne = nouvelle_moyenne;
    printf("Matricule %d : %.2f -> %.2f\n", matricule, ancienne, nouvelle_moyenne);
}

void tri_insertion_liste(ListeDC *l)
{
    if (l == NULL || l->taille <= 1) return;
    Noeud *i = l->tete->suiv->suiv;
    while (i != l->queue) {
        Etudiant *cle = i->etudiant;
        Noeud *j = i->prec;
        while (j != l->tete && j->etudiant->moyenne > cle->moyenne) {
            j->suiv->etudiant = j->etudiant; j = j->prec;
        }
        j->suiv->etudiant = cle;
        i = i->suiv;
    }
    printf("Liste triee par insertion.\n");
}

void tri_bulles_liste(ListeDC *l)
{
    if (l == NULL || l->taille <= 1) return;
    int echange;
    do {
        echange = 0;
        Noeud *courant = l->tete->suiv;
        while (courant->suiv != l->queue) {
            if (strcmp(courant->etudiant->nom, courant->suiv->etudiant->nom) > 0) {
                Etudiant *tmp = courant->etudiant;
                courant->etudiant = courant->suiv->etudiant;
                courant->suiv->etudiant = tmp;
                echange = 1;
            }
            courant = courant->suiv;
        }
    } while (echange);
    printf("Liste triee par bulles.\n");
}

float moyenne_generale_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;
    float s = 0.0f;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) { s += courant->etudiant->moyenne; courant = courant->suiv; }
    return s / l->taille;
}

float minimum_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;
    float min = l->tete->suiv->etudiant->moyenne;
    Noeud *courant = l->tete->suiv->suiv;
    while (courant != l->queue) {
        if (courant->etudiant->moyenne < min) min = courant->etudiant->moyenne;
        courant = courant->suiv;
    }
    return min;
}

float maximum_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;
    float max = l->tete->suiv->etudiant->moyenne;
    Noeud *courant = l->tete->suiv->suiv;
    while (courant != l->queue) {
        if (courant->etudiant->moyenne > max) max = courant->etudiant->moyenne;
        courant = courant->suiv;
    }
    return max;
}

float mediane_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;
    int n = l->taille;
    float *tmp = malloc(n * sizeof(float));
    if (tmp == NULL) return 0.0f;
    Noeud *courant = l->tete->suiv;
    for (int i = 0; i < n; i++) { tmp[i] = courant->etudiant->moyenne; courant = courant->suiv; }
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (tmp[j] > tmp[j+1]) { float t = tmp[j]; tmp[j] = tmp[j+1]; tmp[j+1] = t; }
    float med = (n % 2 == 0) ? (tmp[n/2-1] + tmp[n/2]) / 2.0f : tmp[n/2];
    free(tmp);
    return med;
}

float ecart_type_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;
    float moy = moyenne_generale_liste(l), s = 0.0f;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        float d = courant->etudiant->moyenne - moy;
        s += d * d; courant = courant->suiv;
    }
    return (float)sqrt(s / l->taille);
}

void afficher_liste(ListeDC *l)
{
    if (l == NULL) { printf("Liste NULL\n"); return; }
    printf("=== Liste : %d etudiant(s) ===\n", l->taille);
    if (l->taille == 0) { printf("  (vide)\n"); return; }
    int i = 0;
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
               i++, courant->etudiant->matricule,
               courant->etudiant->nom, courant->etudiant->prenom,
               courant->etudiant->moyenne);
        courant = courant->suiv;
    }
}

void afficher_liste_inverse(ListeDC *l)
{
    if (l == NULL) { printf("Liste NULL\n"); return; }
    printf("=== Liste inverse : %d etudiant(s) ===\n", l->taille);
    int i = l->taille - 1;
    Noeud *courant = l->queue->prec;
    while (courant != l->tete) {
        printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
               i--, courant->etudiant->matricule,
               courant->etudiant->nom, courant->etudiant->prenom,
               courant->etudiant->moyenne);
        courant = courant->prec;
    }
}

void serialiser_liste(ListeDC *l, char *nom_fichier)
{
    if (l == NULL || nom_fichier == NULL) return;
    FILE *f = fopen(nom_fichier, "wb");
    if (f == NULL) { fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier); return; }
    fwrite(&l->taille, sizeof(int), 1, f);
    Noeud *courant = l->tete->suiv;
    while (courant != l->queue) {
        Etudiant *e = courant->etudiant;
        fwrite(&e->matricule,     sizeof(int),   1,  f);
        fwrite(&e->moyenne,       sizeof(float), 1,  f);
        fwrite(e->nom,            sizeof(char),  40, f);
        fwrite(&e->dateNaissance, sizeof(Date),  1,  f);
        int len = strlen(e->prenom) + 1;
        fwrite(&len,      sizeof(int),  1,   f);
        fwrite(e->prenom, sizeof(char), len, f);
        courant = courant->suiv;
    }
    fclose(f);
    printf("Liste serialisee dans %s.\n", nom_fichier);
}

ListeDC* deserialiser_liste(char *nom_fichier)
{
    if (nom_fichier == NULL) return NULL;
    FILE *f = fopen(nom_fichier, "rb");
    if (f == NULL) { fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier); return NULL; }
    int n = 0;
    fread(&n, sizeof(int), 1, f);
    ListeDC *l = creer_liste();
    if (l == NULL) { fclose(f); return NULL; }
    for (int i = 0; i < n; i++) {
        Etudiant *e = malloc(sizeof(Etudiant));
        if (e == NULL) break;
        fread(&e->matricule,     sizeof(int),   1,  f);
        fread(&e->moyenne,       sizeof(float), 1,  f);
        fread(e->nom,            sizeof(char),  40, f);
        fread(&e->dateNaissance, sizeof(Date),  1,  f);
        int len = 0; fread(&len, sizeof(int), 1, f);
        e->prenom = malloc(len * sizeof(char));
        if (e->prenom != NULL) fread(e->prenom, sizeof(char), len, f);
        for (int k = 0; k < 10; k++) e->tabnote[k] = NULL;
        e->inscription = NULL;
        inserer_en_queue(l, e);
    }
    fclose(f);
    printf("Liste deserialisee depuis %s.\n", nom_fichier);
    return l;
}