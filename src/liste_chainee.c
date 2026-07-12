#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/liste_chainee.h"
/* ========================================================================
 * CREATION / LIBERATION DE LA LISTE
 * ==================================================================== */

ListeC* liste_creer(void) {
    ListeC *l = malloc(sizeof(ListeC));
    if (l == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire (liste_creer)\n");
        exit(EXIT_FAILURE);
    }
    l->tete = NULL;
    l->queue = NULL;
    l->taille = 0;
    return l;
}

void liste_liberer(ListeC *l) {
    if (l == NULL) return;
    Noeud *courant = l->tete;
    while (courant != NULL) {
        Noeud *suivant = courant->suivant;
        liberer_etudiant(courant->etudiant);
        free(courant);
        courant = suivant;
    }
    free(l);
}

/* ========================================================================
 * INSERTION  (O(1) en tete comme en queue grace au pointeur "queue")
 * ==================================================================== */

void inserer_en_tete(ListeC *l, Etudiant *e) {
    if (l == NULL || e == NULL) return;

    Noeud *nouveau = malloc(sizeof(Noeud));
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire (inserer_en_tete)\n");
        exit(EXIT_FAILURE);
    }
    nouveau->etudiant = e;
    nouveau->suivant = l->tete;

    l->tete = nouveau;
    if (l->queue == NULL) {
        /* la liste etait vide : ce noeud est aussi la queue */
        l->queue = nouveau;
    }
    l->taille++;
}

void inserer_en_queue(ListeC *l, Etudiant *e) {
    if (l == NULL || e == NULL) return;

    Noeud *nouveau = malloc(sizeof(Noeud));
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire (inserer_en_queue)\n");
        exit(EXIT_FAILURE);
    }
    nouveau->etudiant = e;
    nouveau->suivant = NULL;

    if (l->queue == NULL) {
        /* liste vide */
        l->tete = nouveau;
        l->queue = nouveau;
    } else {
        l->queue->suivant = nouveau;
        l->queue = nouveau;
    }
    l->taille++;
}

/* ========================================================================
 * RECHERCHE
 * ==================================================================== */

Etudiant* rechercher_matricule_liste(ListeC *l, int matricule) {
    if (l == NULL) return NULL;
    Noeud *courant = l->tete;
    while (courant != NULL) {
        if (courant->etudiant->matricule == matricule) {
            return courant->etudiant;
        }
        courant = courant->suivant;
    }
    return NULL; /* non trouve */
}

void rechercher_intervalle_liste(ListeC *l, float min, float max) {
    if (l == NULL) return;
    Noeud *courant = l->tete;
    printf("Etudiants avec une moyenne dans [%.2f ; %.2f] :\n", min, max);
    while (courant != NULL) {
        if (courant->etudiant->moyenne >= min && courant->etudiant->moyenne <= max) {
            printf("  Matricule=%d  Nom=%s %s  Moyenne=%.2f\n",
                   courant->etudiant->matricule,
                   courant->etudiant->nom,
                   courant->etudiant->prenom,
                   courant->etudiant->moyenne);
        }
        courant = courant->suivant;
    }
}

void rechercher_liste_prefixes(ListeC *l, char *prefixe) {
    if (l == NULL || prefixe == NULL) return;
    size_t len = strlen(prefixe);
    Noeud *courant = l->tete;
    printf("Etudiants dont le nom commence par \"%s\" :\n", prefixe);
    while (courant != NULL) {
        if (strncmp(courant->etudiant->nom, prefixe, len) == 0) {
            printf("  Matricule=%d  Nom=%s %s  Moyenne=%.2f\n",
                   courant->etudiant->matricule,
                   courant->etudiant->nom,
                   courant->etudiant->prenom,
                   courant->etudiant->moyenne);
        }
        courant = courant->suivant;
    }
}

/* ========================================================================
 * SUPPRESSION / MODIFICATION
 * ==================================================================== */

void supprimer_liste_matricule(ListeC *l, int matricule) {
    if (l == NULL || l->tete == NULL) return;

    Noeud *courant = l->tete;
    Noeud *precedent = NULL;

    while (courant != NULL) {
        if (courant->etudiant->matricule == matricule) {
            if (precedent == NULL) {
                /* on supprime la tete */
                l->tete = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            if (courant == l->queue) {
                /* on supprime la queue : il faut la mettre a jour */
                l->queue = precedent;
            }
            liberer_etudiant(courant->etudiant);
            free(courant);
            l->taille--;
            return; /* matricule suppose unique */
        }
        precedent = courant;
        courant = courant->suivant;
    }
}

void liste_modifier_moyenne(ListeC *l, int matricule, float nouvelle_moyenne) {
    Etudiant *e = rechercher_matricule_liste(l, matricule);
    if (e != NULL) {
        e->moyenne = nouvelle_moyenne;
    }
}

/* ========================================================================
 * TRI
 * ==================================================================== */

/* Tri par insertion : on reconstruit la liste en inserant chaque
 * etudiant a sa place dans une nouvelle liste triee (par moyenne croissante) */
void liste_tri_insertion(ListeC *l) {
    if (l == NULL || l->tete == NULL || l->tete->suivant == NULL) return;

    Noeud *trie_tete = NULL;
    Noeud *courant = l->tete;

    while (courant != NULL) {
        Noeud *suivant_original = courant->suivant;

        if (trie_tete == NULL || courant->etudiant->moyenne < trie_tete->etudiant->moyenne) {
            courant->suivant = trie_tete;
            trie_tete = courant;
        } else {
            Noeud *rech = trie_tete;
            while (rech->suivant != NULL &&
                   rech->suivant->etudiant->moyenne <= courant->etudiant->moyenne) {
                rech = rech->suivant;
            }
            courant->suivant = rech->suivant;
            rech->suivant = courant;
        }
        courant = suivant_original;
    }

    l->tete = trie_tete;

    /* mise a jour de la queue */
    Noeud *n = l->tete;
    while (n->suivant != NULL) n = n->suivant;
    l->queue = n;
}

/* Tri a bulles, par moyenne croissante, en echangeant les donnees
 * (pas les noeuds) pour rester simple sur une liste simplement chainee */
void liste_tri_bulles(ListeC *l) {
    if (l == NULL || l->tete == NULL) return;

    int echange;
    do {
        echange = 0;
        Noeud *courant = l->tete;
        while (courant != NULL && courant->suivant != NULL) {
            if (courant->etudiant->moyenne > courant->suivant->etudiant->moyenne) {
                Etudiant *tmp = courant->etudiant;
                courant->etudiant = courant->suivant->etudiant;
                courant->suivant->etudiant = tmp;
                echange = 1;
            }
            courant = courant->suivant;
        }
    } while (echange);
}

/* ========================================================================
 * STATISTIQUES
 * ==================================================================== */

float moyenne_generale_liste(ListeC *l) {
    if (l == NULL || l->taille == 0) return 0.0f;
    float somme = 0.0f;
    Noeud *courant = l->tete;
    while (courant != NULL) {
        somme += courant->etudiant->moyenne;
        courant = courant->suivant;
    }
    return somme / l->taille;
}

float liste_minimum(ListeC *l) {
    if (l == NULL || l->tete == NULL) return 0.0f;
    float min = l->tete->etudiant->moyenne;
    Noeud *courant = l->tete->suivant;
    while (courant != NULL) {
        if (courant->etudiant->moyenne < min) min = courant->etudiant->moyenne;
        courant = courant->suivant;
    }
    return min;
}

float liste_maximum(ListeC *l) {
    if (l == NULL || l->tete == NULL) return 0.0f;
    float max = l->tete->etudiant->moyenne;
    Noeud *courant = l->tete->suivant;
    while (courant != NULL) {
        if (courant->etudiant->moyenne > max) max = courant->etudiant->moyenne;
        courant = courant->suivant;
    }
    return max;
}

/* La mediane necessite un tableau trie temporaire : O(n log n) / O(n) memoire */
static int comparer_float(const void *a, const void *b) {
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

float liste_mediane(ListeC *l) {
    if (l == NULL || l->taille == 0) return 0.0f;

    float *valeurs = malloc(l->taille * sizeof(float));
    if (valeurs == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire (liste_mediane)\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    Noeud *courant = l->tete;
    while (courant != NULL) {
        valeurs[i++] = courant->etudiant->moyenne;
        courant = courant->suivant;
    }

    qsort(valeurs, l->taille, sizeof(float), comparer_float);

    float mediane;
    if (l->taille % 2 == 0) {
        mediane = (valeurs[l->taille / 2 - 1] + valeurs[l->taille / 2]) / 2.0f;
    } else {
        mediane = valeurs[l->taille / 2];
    }

    free(valeurs);
    return mediane;
}

float liste_ecart_type(ListeC *l) {
    if (l == NULL || l->taille == 0) return 0.0f;
    float moyenne = moyenne_generale_liste(l);
    float somme_carres = 0.0f;

    Noeud *courant = l->tete;
    while (courant != NULL) {
        float diff = courant->etudiant->moyenne - moyenne;
        somme_carres += diff * diff;
        courant = courant->suivant;
    }

    return sqrtf(somme_carres / l->taille);
}

/* ========================================================================
 * AFFICHAGE
 * ==================================================================== */

void afficher_liste(ListeC *l) {
    if (l == NULL) return;
    printf("--- Liste (%d etudiant(s)) ---\n", l->taille);
    Noeud *courant = l->tete;
    while (courant != NULL) {
        printf("  Matricule=%d  Nom=%s %s  Moyenne=%.2f\n",
               courant->etudiant->matricule,
               courant->etudiant->nom,
               courant->etudiant->prenom,
               courant->etudiant->moyenne);
        courant = courant->suivant;
    }
}

/* Une liste simplement chainee n'a pas de pointeur "precedent" : on
 * affiche donc a l'envers via la pile d'appels de la recursion, qui
 * "deroule" la liste dans l'ordre inverse a la sortie des appels. */
static void afficher_liste_inverse_rec(Noeud *n) {
    if (n == NULL) return;
    afficher_liste_inverse_rec(n->suivant);
    printf("  Matricule=%d  Nom=%s %s  Moyenne=%.2f\n",
           n->etudiant->matricule,
           n->etudiant->nom,
           n->etudiant->prenom,
           n->etudiant->moyenne);
}

void afficher_liste_inverse(ListeC *l) {
    if (l == NULL) return;
    printf("--- Liste inversee (%d etudiant(s)) ---\n", l->taille);
    afficher_liste_inverse_rec(l->tete);
}

/* ========================================================================
 * PERSISTANCE (serialisation binaire)
 *
 * Format du fichier :
 *   int   taille                  (nombre d'etudiants)
 *   pour chaque etudiant, dans l'ordre de la liste :
 *     int    matricule
 *     float  moyenne
 *     int    longueur_nom      (sans le '\0')
 *     char[] nom                (longueur_nom octets, sans '\0')
 *     int    longueur_prenom   (sans le '\0')
 *     char[] prenom             (longueur_prenom octets, sans '\0')
 *
 * On ne peut pas ecrire directement les pointeurs char* du struct
 * Etudiant (ils ne veulent rien dire d'un processus a l'autre), donc
 * on serialise les chaines explicitement avec leur longueur.
 * ==================================================================== */

void liste_serialiser(ListeC *l, char *nom_fichier) {
    if (l == NULL || nom_fichier == NULL) return;

    FILE *f = fopen(nom_fichier, "wb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s en ecriture\n", nom_fichier);
        return;
    }

    fwrite(&l->taille, sizeof(int), 1, f);

    Noeud *courant = l->tete;
    while (courant != NULL) {
        Etudiant *e = courant->etudiant;

        fwrite(&e->matricule, sizeof(int), 1, f);
        fwrite(&e->moyenne, sizeof(float), 1, f);

        int len_nom = (int) strlen(e->nom);
        fwrite(&len_nom, sizeof(int), 1, f);
        fwrite(e->nom, sizeof(char), len_nom, f);

        int len_prenom = (int) strlen(e->prenom);
        fwrite(&len_prenom, sizeof(int), 1, f);
        fwrite(e->prenom, sizeof(char), len_prenom, f);

        courant = courant->suivant;
    }

    fclose(f);
}

ListeC* liste_deserialiser(char *nom_fichier) {
    if (nom_fichier == NULL) return NULL;

    FILE *f = fopen(nom_fichier, "rb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s en lecture\n", nom_fichier);
        return NULL;
    }

    ListeC *l = liste_creer();

    int taille;
    if (fread(&taille, sizeof(int), 1, f) != 1) {
        fclose(f);
        return l; /* fichier vide ou corrompu -> liste vide */
    }

    for (int i = 0; i < taille; i++) {
        int matricule;
        float moyenne;
        int len_nom, len_prenom;
        Date d;
        fread(&matricule, sizeof(int), 1, f);
        fread(&moyenne, sizeof(float), 1, f);

        fread(&len_nom, sizeof(int), 1, f);
        char *nom = malloc(len_nom + 1);
        fread(nom, sizeof(char), len_nom, f);
        nom[len_nom] = '\0';

        fread(&len_prenom, sizeof(int), 1, f);
        char *prenom = malloc(len_prenom + 1);
        fread(prenom, sizeof(char), len_prenom, f);
        prenom[len_prenom] = '\0';

        Etudiant *e = creer_etudiant(matricule, moyenne, nom, prenom, d);
        inserer_en_queue(l, e);

        free(nom);
        free(prenom);
    }

    fclose(f);
    return l;
}