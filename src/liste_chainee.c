#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/liste_chainee.h"

 Noeud* creer_noeud(Etudiant *e)
{
    Noeud *n = malloc(sizeof(Noeud));
    if (n == NULL) {
        fprintf(stderr, "Erreur malloc Noeud\n");
        return NULL;
    }
    n->etudiant = e;
    n->prec     = NULL;
    n->suiv     = NULL;
    return n;
}

ListeDC* creer_liste(void)
{
    /* Allouer la structure principale */
    ListeDC *l = malloc(sizeof(ListeDC));
    if (l == NULL) {
        fprintf(stderr, "Erreur malloc ListeDC\n");
        return NULL;
    }

    l->tete = creer_noeud(NULL);
    if (l->tete == NULL) {
        free(l);
        return NULL;
    }

    l->queue = creer_noeud(NULL);
    if (l->queue == NULL) {
        free(l->tete);
        free(l);
        return NULL;
    }

    /* Relier les deux sentinelles entre elles */
    l->tete->suiv  = l->queue;  /* tete pointe vers queue */
    l->queue->prec = l->tete;   /* queue pointe vers tete */
    l->taille      = 0;

    return l;
}

void liberer_liste(ListeDC *l)
{
    if (l == NULL) return;

    /* Parcourir tous les noeuds sauf les sentinelles */
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        Noeud *suivant = courant->suiv; /* sauvegarder le suivant */

        /* Libérer l'étudiant contenu dans ce noeud */
        if (courant->etudiant != NULL) {
            liberer_etudiant(courant->etudiant);
        }

        /* Libérer le noeud lui-mÃªme */
        free(courant);
        courant = suivant;
    }

    /* Libérer les deux sentinelles */
    free(l->tete);
    free(l->queue);

    /* Libérer la structure principale */
    free(l);
}
void inserer_en_tete(ListeDC *l, Etudiant *e)
{
    if (l == NULL || e == NULL) return;

    Noeud *nouveau = creer_noeud(e);
    if (nouveau == NULL) return;

    /* Le noeud qui était juste après la téte */
    Noeud *ancien_premier = l->tete->suiv;

    /* Relier le nouveau noeud */
    nouveau->prec = l->tete;        /* nouveau pointe vers tete  */
    nouveau->suiv = ancien_premier; /* nouveau pointe vers l'ancien premier */

    /* Mettre Ã  jour les liens des voisins */
    l->tete->suiv         = nouveau; /* tete pointe vers nouveau */
    ancien_premier->prec  = nouveau; /* ancien premier pointe vers nouveau */

    l->taille++;
}

void inserer_en_queue(ListeDC *l, Etudiant *e)
{
    if (l == NULL || e == NULL) return;

    Noeud *nouveau = creer_noeud(e);
    if (nouveau == NULL) return;

    /* Le noeud qui était juste avant la queue */
    Noeud *ancien_dernier = l->queue->prec;

    /* Relier le nouveau noeud */
    nouveau->suiv = l->queue;       /* nouveau pointe vers queue         */
    nouveau->prec = ancien_dernier; /* nouveau pointe vers l'ancien dernier */

    /* Mettre a  jour les liens des voisins */
    ancien_dernier->suiv = nouveau; /* ancien dernier pointe vers nouveau */
    l->queue->prec       = nouveau; /* queue pointe vers nouveau          */

    l->taille++;
}

Etudiant* rechercher_matricule_liste(ListeDC *l, int matricule)
{
    if (l == NULL) return NULL;

    /* On commence après la sentinelle tete */
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        if (courant->etudiant->matricule == matricule) {
            return courant->etudiant;
        }
        courant = courant->suiv;
    }

    return NULL;
	}
	 /* non trouve */
void rechercher_intervalle_liste(ListeDC *l, float min, float max)
{
    if (l == NULL) {
        printf("Erreur : liste NULL\n");
        return;
    }
    if (min > max) {
        printf("Erreur : min (%.2f) superieur a max (%.2f)\n", min, max);
        return;
    }
    if (l->taille == 0) {
        printf("Liste vide.\n");
        return;
    }

    printf("=== Recherche par intervalle [%.2f , %.2f] ===\n", min, max);

    int nb_trouve  = 0;
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        float m = courant->etudiant->moyenne;

        if (m >= min && m <= max) {
            nb_trouve++;
            printf("  %d. Matricule : %d\n", nb_trouve,
                   courant->etudiant->matricule);
            printf("     Nom       : %s %s\n",
                   courant->etudiant->nom,
                   courant->etudiant->prenom);
            printf("     Moyenne   : %.2f\n", m);
            printf("     Naissance : %02d/%02d/%04d\n",
                   courant->etudiant->dateNaissance.jour,
                   courant->etudiant->dateNaissance.mois,
                   courant->etudiant->dateNaissance.annee);
            printf("     -----\n");
        }
        courant = courant->suiv;
    }

    if (nb_trouve == 0)
        printf("  Aucun etudiant trouve dans cet intervalle.\n");
    else
        printf("  Total : %d etudiant(s) trouve(s).\n", nb_trouve);

    printf("==============================================\n");
}

void rechercher_prefixe_liste(ListeDC *l, char *prefixe)
{
    if (l == NULL) {
        printf("Erreur : liste NULL\n");
        return;
    }
    if (prefixe == NULL || strlen(prefixe) == 0) {
        printf("Erreur : prefixe vide\n");
        return;
    }
    if (l->taille == 0) {
        printf("Liste vide.\n");
        return;
    }

    int len = strlen(prefixe);
    printf("=== Recherche par prefixe \"%s\" ===\n", prefixe);

    int nb_trouve  = 0;
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        if (strncmp(courant->etudiant->nom, prefixe, len) == 0) {
            nb_trouve++;
            printf("  %d. Matricule : %d\n", nb_trouve,
                   courant->etudiant->matricule);
            printf("     Nom       : %s %s\n",
                   courant->etudiant->nom,
                   courant->etudiant->prenom);
            printf("     Moyenne   : %.2f\n",
                   courant->etudiant->moyenne);
            printf("     -----\n");
        }
        courant = courant->suiv;
    }

    if (nb_trouve == 0)
        printf("  Aucun etudiant dont le nom commence par \"%s\".\n",
               prefixe);
    else
        printf("  Total : %d etudiant(s) trouve(s).\n", nb_trouve);

    printf("===================================\n");
}

void supprimer_matricule_liste(ListeDC *l, int matricule)
{
    if (l == NULL) return;

    /* Chercher le noeud a  supprimer */
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        if (courant->etudiant->matricule == matricule) {

            /* Relier les voisins entre eux */
            courant->prec->suiv = courant->suiv;
            courant->suiv->prec = courant->prec;

            /* Libérer l'étudiant et le noeud */
            liberer_etudiant(courant->etudiant);
            free(courant);

            l->taille--;
            printf("Etudiant %d supprime avec succes.\n", matricule);
            return;
        }
        courant = courant->suiv;
    }

    printf("Matricule %d non trouve.\n", matricule);
}

void modifier_moyenne_liste(ListeDC *l, int matricule,
                             float nouvelle_moyenne)
{
    if (l == NULL) return;

    Etudiant *e = rechercher_matricule_liste(l, matricule);
    if (e == NULL) {
        printf("Matricule %d non trouve.\n", matricule);
        return;
    }

    float ancienne = e->moyenne;
    e->moyenne = nouvelle_moyenne;
    printf("Matricule %d : moyenne mise a jour %.2f -> %.2f\n",
           matricule, ancienne, nouvelle_moyenne);
}

void tri_insertion_liste(ListeDC *l)
{
    if (l == NULL || l->taille <= 1) return;
    Noeud *i = l->tete->suiv->suiv; /* on commence au 2e élément */
    while (i != l->queue) {
        Etudiant *cle = i->etudiant; /* élément a  insérer */
        Noeud    *j   = i->prec;
        while (j != l->tete &&
               j->etudiant->moyenne > cle->moyenne) {
            j->suiv->etudiant = j->etudiant;
            j = j->prec;
        }
        j->suiv->etudiant = cle;
        i = i->suiv;
    }
void tri_bulles_liste(ListeDC *l)
{
    if (l == NULL || l->taille <= 1) return;

    int echange;

    do {
        echange = 0;
        Noeud *courant = l->tete->suiv;

        while (courant->suiv != l->queue) {
            /* Comparer les noms des noeuds adjacents */
            if (strcmp(courant->etudiant->nom,
                       courant->suiv->etudiant->nom) > 0) {

                /* échanger les pointeurs Etudiant */
                Etudiant *tmp         = courant->etudiant;
                courant->etudiant     = courant->suiv->etudiant;
                courant->suiv->etudiant = tmp;

                echange = 1;
            }
            courant = courant->suiv;
        }
    } while (echange);

    printf("Liste triee par nom (tri bulles).\n");
}

float moyenne_generale_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;

    float  somme   = 0.0f;
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        somme  += courant->etudiant->moyenne;
        courant = courant->suiv;
    }

    return somme / l->taille;
}

float minimum_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;

    float  min     = l->tete->suiv->etudiant->moyenne;
    Noeud *courant = l->tete->suiv->suiv;

    while (courant != l->queue) {
        if (courant->etudiant->moyenne < min)
            min = courant->etudiant->moyenne;
        courant = courant->suiv;
    }

    return min;
}

float maximum_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;

    float  max     = l->tete->suiv->etudiant->moyenne;
    Noeud *courant = l->tete->suiv->suiv;

    while (courant != l->queue) {
        if (courant->etudiant->moyenne > max)
            max = courant->etudiant->moyenne;
        courant = courant->suiv;
    }

    return max;
}

float mediane_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;

    int n = l->taille;

    /* Copier les moyennes dans un tableau temporaire */
    float *tmp = malloc(n * sizeof(float));
    if (tmp == NULL) return 0.0f;

    Noeud *courant = l->tete->suiv;
    for (int i = 0; i < n; i++) {
        tmp[i]  = courant->etudiant->moyenne;
        courant = courant->suiv;
    }

    /* Tri a  bulles sur la copie */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (tmp[j] > tmp[j + 1]) {
                float t  = tmp[j];
                tmp[j]   = tmp[j + 1];
                tmp[j+1] = t;
            }
        }
    }

    /* Calcul de la médiane */
    float med;
    if (n % 2 == 0)
        med = (tmp[n/2 - 1] + tmp[n/2]) / 2.0f;
    else
        med = tmp[n/2];

    free(tmp);
    return med;
}

float ecart_type_liste(ListeDC *l)
{
    if (l == NULL || l->taille == 0) return 0.0f;

    float  moy     = moyenne_generale_liste(l);
    float  somme   = 0.0f;
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        float diff = courant->etudiant->moyenne - moy;
        somme     += diff * diff;
        courant    = courant->suiv;
    }

    return (float)sqrt(somme / l->taille);
}

void afficher_liste(ListeDC *l)
{
    if (l == NULL) {
        printf("Liste NULL\n");
        return;
    }

    printf("=== Liste doublement chainee : %d etudiant(s) ===\n",
           l->taille);

    if (l->taille == 0) {
        printf("  (vide)\n");
        printf("=================================================\n");
        return;
    }

    int    index   = 0;
    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
               index,
               courant->etudiant->matricule,
               courant->etudiant->nom,
               courant->etudiant->prenom,
               courant->etudiant->moyenne);
        index++;
        courant = courant->suiv;
    }

    printf("=================================================\n");
}

void afficher_liste_inverse(ListeDC *l)
{
    if (l == NULL) {
        printf("Liste NULL\n");
        return;
    }

    printf("=== Affichage inverse : %d etudiant(s) ===\n",
           l->taille);

    if (l->taille == 0) {
        printf("  (vide)\n");
        printf("==========================================\n");
        return;
    }

    int    index   = l->taille - 1;
    Noeud *courant = l->queue->prec; /* on commence avant la sentinelle queue */

    while (courant != l->tete) {
        printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
               index,
               courant->etudiant->matricule,
               courant->etudiant->nom,
               courant->etudiant->prenom,
               courant->etudiant->moyenne);
        index--;
        courant = courant->prec;
    }

    printf("==========================================\n");
}

void serialiser_liste(ListeDC *l, char *nom_fichier)
{
    if (l == NULL || nom_fichier == NULL) return;

    FILE *f = fopen(nom_fichier, "wb");
    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier);
        return;
    }

    /* écrire le nombre d'étudiants */
    fwrite(&l->taille, sizeof(int), 1, f);

    Noeud *courant = l->tete->suiv;

    while (courant != l->queue) {
        Etudiant *e = courant->etudiant;

        fwrite(&e->matricule,     sizeof(int),   1,  f);
        fwrite(&e->moyenne,       sizeof(float), 1,  f);
        fwrite(e->nom,            sizeof(char),  40, f);
        fwrite(&e->dateNaissance, sizeof(Date),  1,  f);

        /* Prenom : longueur variable */
        int len = strlen(e->prenom) + 1;
        fwrite(&len,      sizeof(int),  1,   f);
        fwrite(e->prenom, sizeof(char), len, f);

        courant = courant->suiv;
    }

    fclose(f);
    printf("Liste serialisee dans %s (%d etudiants).\n",
           nom_fichier, l->taille);
}

ListeDC* deserialiser_liste(char *nom_fichier)
{
    if (nom_fichier == NULL) return NULL;

    FILE *f = fopen(nom_fichier, "rb");
    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvrir %s\n", nom_fichier);
        return NULL;
    }

    int n = 0;
    fread(&n, sizeof(int), 1, f);

    ListeDC *l = creer_liste();
    if (l == NULL) {
        fclose(f);
        return NULL;
    }

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
        if (e->prenom != NULL)
            fread(e->prenom, sizeof(char), len, f);

        /* Initialiser les pointeurs non sérialisés */
        for (int k = 0; k < 10; k++) e->tabnote[k] = NULL;
        e->inscription = NULL;

        /* On insére en queue pour respecter l'ordre original */
        inserer_en_queue(l, e);
    }

    fclose(f);
    printf("Liste deserialisee depuis %s (%d etudiants).\n",
           nom_fichier, l->taille);
    return l;
}