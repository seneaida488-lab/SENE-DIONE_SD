#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/tableau_dynamique.h"
#include "../include/structure.h"

void liberer_etudiant(Etudiant *e);

TableauDyn* creer_tableau_dyn(void)
{
    TableauDyn *t = malloc(sizeof(TableauDyn));
    if (t == NULL) { fprintf(stderr, "Erreur malloc TableauDyn\n"); return NULL; }

    t->tab = malloc(CAPACITE_INITIALE * sizeof(Etudiant *));
    if (t->tab == NULL) {
        fprintf(stderr, "Erreur malloc tab\n");
        free(t); return NULL;
    }
    t->taille   = 0;
    t->capacite = CAPACITE_INITIALE;
    return t;
}

void liberer_tableau_dyn(TableauDyn * t)
{
    if (t == NULL) return;
    for (int i = 0; i < t->taille; i++) {
        liberer_etudiant(t->tab[i]);
        t->tab[i] = NULL;
    }
    free(t->tab);
    t->tab = NULL;
    free(t);
}

/* ============================================================
   2. INSERTION
   ============================================================ */
void inserer_etudiant_dyn(TableauDyn *t, Etudiant *e)
{
    if (t == NULL || e == NULL) return;

    if (t->taille == t->capacite) {
        int nouvelle_capacite = t->capacite * 2;
        Etudiant **nouveau = realloc(t->tab,
                             nouvelle_capacite * sizeof(Etudiant *));
        if (nouveau == NULL) {
            fprintf(stderr, "Erreur realloc\n"); return;
        }
        t->tab      = nouveau;
        t->capacite = nouvelle_capacite;
    }
    t->tab[t->taille] = e;
    t->taille++;
}

/* ============================================================
   3. RECHERCHE
   ============================================================ */
Etudiant* rechercher_par_matricule_dyn(TableauDyn *t, int matricule)
{
    if (t == NULL) return NULL;
    for (int i = 0; i < t->taille; i++) {
        if (t->tab[i]->matricule == matricule)
            return t->tab[i];
    }
    return NULL;
}

void rechercher_par_intervalle_dyn(TableauDyn *t, float min, float max)
{
    if (t == NULL) { printf("Erreur : tableau NULL\n"); return; }
    if (min > max) { printf("Erreur : min > max\n"); return; }
    if (t->taille == 0) { printf("Tableau vide.\n"); return; }

    printf("=== Recherche intervalle [%.2f , %.2f] ===\n", min, max);
    int nb = 0;
    for (int i = 0; i < t->taille; i++) {
        float m = t->tab[i]->moyenne;
        if (m >= min && m <= max) {
            nb++;
            printf("  %d. Matricule : %d\n", nb, t->tab[i]->matricule);
            printf("     Nom       : %s %s\n", t->tab[i]->nom, t->tab[i]->prenom);
            printf("     Moyenne   : %.2f\n", m);
            printf("     Naissance : %02d/%02d/%04d\n",
                   t->tab[i]->dateNaissance.jour,
                   t->tab[i]->dateNaissance.mois,
                   t->tab[i]->dateNaissance.annee);
            printf("     -----\n");
        }
    }
    if (nb == 0) printf("  Aucun etudiant trouve.\n");
    else printf("  Total : %d etudiant(s).\n", nb);
    printf("==========================================\n");
}

void rechercher_par_prefixe_dyn(TableauDyn *t, char *prefixe)
{
    if (t == NULL) { printf("Erreur : tableau NULL\n"); return; }
    if (prefixe == NULL || strlen(prefixe) == 0) {
        printf("Erreur : prefixe vide\n"); return;
    }
    if (t->taille == 0) { printf("Tableau vide.\n"); return; }

    int len = strlen(prefixe);
    printf("=== Recherche prefixe \"%s\" ===\n", prefixe);
    int nb = 0;
    for (int i = 0; i < t->taille; i++) {
        if (strncmp(t->tab[i]->nom, prefixe, len) == 0) {
            nb++;
            printf("  %d. Matricule : %d\n", nb, t->tab[i]->matricule);
            printf("     Nom       : %s %s\n", t->tab[i]->nom, t->tab[i]->prenom);
            printf("     Moyenne   : %.2f\n", t->tab[i]->moyenne);
            printf("     -----\n");
        }
    }
    if (nb == 0) printf("  Aucun etudiant trouve.\n");
    else printf("  Total : %d etudiant(s).\n", nb);
    printf("===================================\n");
}

/* ============================================================
   4. SUPPRESSION
   ============================================================ */
void supprimer_par_matricule_dyn(TableauDyn *t, int matricule)
{
    if (t == NULL) return;
    int index = -1;
    for (int i = 0; i < t->taille; i++) {
        if (t->tab[i]->matricule == matricule) { index = i; break; }
    }
    if (index == -1) { printf("Matricule %d non trouve.\n", matricule); return; }

    liberer_etudiant(t->tab[index]);
    for (int i = index; i < t->taille - 1; i++)
        t->tab[i] = t->tab[i + 1];
    t->tab[t->taille - 1] = NULL;
    t->taille--;
    printf("Etudiant %d supprime.\n", matricule);
}

/* ============================================================
   5. MISE À JOUR
   ============================================================ */
void modifier_moyenne_dyn(TableauDyn *t, int matricule, float nouvelle_moyenne)
{
    if (t == NULL) return;
    Etudiant *e = rechercher_par_matricule_dyn(t, matricule);
    if (e == NULL) { printf("Matricule %d non trouve.\n", matricule); return; }
    float ancienne = e->moyenne;
    e->moyenne = nouvelle_moyenne;
    printf("Matricule %d : %.2f -> %.2f\n", matricule, ancienne, nouvelle_moyenne);
}

/* ============================================================
   6. TRI
   ============================================================ */
void tri_insertion_dyn(TableauDyn *t)
{
    if (t == NULL) return;
    for (int i = 1; i < t->taille; i++) {
        Etudiant *cle = t->tab[i];
        int j = i - 1;
        while (j >= 0 && t->tab[j]->moyenne > cle->moyenne) {
            t->tab[j + 1] = t->tab[j];
            j--;
        }
        t->tab[j + 1] = cle;
    }
    printf("Trie par insertion.\n");
}

static int partition_rapide(TableauDyn *t, int gauche, int droite)
{
    float pivot = t->tab[(gauche + droite) / 2]->moyenne;
    int i = gauche, j = droite;
    while (i <= j) {
        while (t->tab[i]->moyenne < pivot) i++;
        while (t->tab[j]->moyenne > pivot) j--;
        if (i <= j) {
            Etudiant *tmp = t->tab[i];
            t->tab[i]    = t->tab[j];
            t->tab[j]    = tmp;
            i++; j--;
        }
    }
    return i;
}

void tri_rapide_dyn(TableauDyn *t, int gauche, int droite)
{
    if (t == NULL || gauche >= droite) return;
    int p = partition_rapide(t, gauche, droite);
    tri_rapide_dyn(t, gauche, p - 1);
    tri_rapide_dyn(t, p, droite);
}

/* ============================================================
   7. AGRÉGATIONS
   ============================================================ */
float moyenne_generale_dyn(TableauDyn *t)
{
    if (t == NULL || t->taille == 0) return 0.0f;
    float s = 0.0f;
    for (int i = 0; i < t->taille; i++) s += t->tab[i]->moyenne;
    return s / t->taille;
}

float minimum_dyn(TableauDyn *t)
{
    if (t == NULL || t->taille == 0) return 0.0f;
    float min = t->tab[0]->moyenne;
    for (int i = 1; i < t->taille; i++)
        if (t->tab[i]->moyenne < min) min = t->tab[i]->moyenne;
    return min;
}

float maximum_dyn(TableauDyn *t)
{
    if (t == NULL || t->taille == 0) return 0.0f;
    float max = t->tab[0]->moyenne;
    for (int i = 1; i < t->taille; i++)
        if (t->tab[i]->moyenne > max) max = t->tab[i]->moyenne;
    return max;
}

float mediane_dyn(TableauDyn *t)
{
    if (t == NULL || t->taille == 0) return 0.0f;
    int n = t->taille;
    float *tmp = malloc(n * sizeof(float));
    if (tmp == NULL) return 0.0f;
    for (int i = 0; i < n; i++) tmp[i] = t->tab[i]->moyenne;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (tmp[j] > tmp[j+1]) {
                float temp = tmp[j]; tmp[j] = tmp[j+1]; tmp[j+1] = temp;
            }
    float med = (n % 2 == 0) ? (tmp[n/2-1] + tmp[n/2]) / 2.0f : tmp[n/2];
    free(tmp);
    return med;
}

float ecart_type_dyn(TableauDyn *t)
{
    if (t == NULL || t->taille == 0) return 0.0f;
    float moy = moyenne_generale_dyn(t);
    float s   = 0.0f;
    for (int i = 0; i < t->taille; i++) {
        float d = t->tab[i]->moyenne - moy;
        s += d * d;
    }
    return (float)sqrt(s / t->taille);
}

/* ============================================================
   8. AFFICHAGE
   ============================================================ */
void afficher_tableau_dyn(TableauDyn *t)
{
    if (t == NULL) { printf("Tableau NULL\n"); return; }
    printf("=== Tableau dynamique : %d/%d etudiants ===\n",
           t->taille, t->capacite);
    if (t->taille == 0) { printf("  (vide)\n"); return; }
    for (int i = 0; i < t->taille; i++)
        printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
               i, t->tab[i]->matricule,
               t->tab[i]->nom, t->tab[i]->prenom,
               t->tab[i]->moyenne);
    printf("==========================================\n");
}

/* ============================================================
   9. PERSISTANCE
   ============================================================ */
void serialiser_dyn(TableauDyn *t, char *nom_fichier)
{
    if (t == NULL || nom_fichier == NULL) return;
    FILE *f = fopen(nom_fichier, "wb");
    if (f == NULL) { fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier); return; }

    fwrite(&t->taille, sizeof(int), 1, f);
    for (int i = 0; i < t->taille; i++) {
        Etudiant *e = t->tab[i];
        fwrite(&e->matricule,     sizeof(int),   1,  f);
        fwrite(&e->moyenne,       sizeof(float), 1,  f);
        fwrite(e->nom,            sizeof(char),  40, f);
        fwrite(&e->dateNaissance, sizeof(Date),  1,  f);
        int len = strlen(e->prenom) + 1;
        fwrite(&len,      sizeof(int),  1,   f);
        fwrite(e->prenom, sizeof(char), len, f);
    }
    fclose(f);
    printf("Serialise dans %s (%d etudiants).\n", nom_fichier, t->taille);
}

TableauDyn* deserialiser_dyn(char *nom_fichier)
{
    if (nom_fichier == NULL) return NULL;
    FILE *f = fopen(nom_fichier, "rb");
    if (f == NULL) { fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier); return NULL; }

    int n = 0;
    fread(&n, sizeof(int), 1, f);
    TableauDyn *t = creer_tableau_dyn();
    if (t == NULL) { fclose(f); return NULL; }

    for (int i = 0; i < n; i++) {
        Etudiant *e = malloc(sizeof(Etudiant));
        if (e == NULL) break;
        fread(&e->matricule,     sizeof(int),   1,  f);
        fread(&e->moyenne,       sizeof(float), 1,  f);
        fread(e->nom,            sizeof(char),  40, f);
        fread(&e->dateNaissance, sizeof(Date),  1,  f);
        int len = 0;
        fread(&len, sizeof(int), 1, f);
        e->prenom = malloc(len * sizeof(char));
        if (e->prenom != NULL) fread(e->prenom, sizeof(char), len, f);
        for (int k = 0; k < 10; k++) e->tabnote[k] = NULL;
        e->inscription = NULL;
        inserer_etudiant_dyn(t, e);
    }
    fclose(f);
    printf("Deserialise depuis %s (%d etudiants).\n", nom_fichier, t->taille);
    return t;
}