#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../include/tableau_statique.h"
#include "../include/tableau_dynamique.h"
#include "../include/liste_chainee.h"
#include "generateur_donnee.h"

/* ============================================================
   CONSTANTES
   ============================================================ */
#define NB_REPETITIONS  10   /* chaque mesure répétée 10 fois    */
#define NB_TAILLES       4   /* 100, 1000, 10000, 100000         */
#define NB_DISTRIBUTIONS 3   /* uniforme, gaussienne, triée      */

/* Tailles des jeux de données */
 int TAILLES[NB_TAILLES] = {N1, N2, N3, N4};

/* Distributions testées */
 Distribution DISTRIBUTIONS[NB_DISTRIBUTIONS] = {
    UNIFORME, GAUSSIENNE, TRIEE
};

 double get_temps(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

 double calcul_moyenne(double *temps, int n)
{
    double somme = 0.0;
    for (int i = 0; i < n; i++) somme += temps[i];
    return somme / n;
}

 double calcul_ecart_type(double *temps, int n, double moy)
{
    double somme = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = temps[i] - moy;
        somme += diff * diff;
    }
    return sqrt(somme / n);
}

 void bench_tableau_statique(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;

    /* --- INSERTION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **data = generer_etudiants(n, dist);
        static Etudiant tab[100000];
        int taille = 0;

        t_debut = get_temps();
        for (int i = 0; i < n; i++) {
            insertionTableau(tab, &taille, *data[i]);
        }
        t_fin    = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_etudiants(data, n);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    Etudiant **data = generer_etudiants(n, dist);
    Etudiant tab[100000];
    int taille = 0;
    for (int i = 0; i < n; i++)
        insertionTableau(tab, &taille, *data[i]);

    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut  = get_temps();
        RechercheCleprimaire(tab, taille, matricule);
        t_fin    = get_temps();
        temps[r] = t_fin - t_debut;
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % taille);
        t_debut  = get_temps();
        suppressionCleprimaire(tab, &taille, matricule);
        t_fin    = get_temps();
        temps[r] = t_fin - t_debut;
        /* Rinsérer pour garder la taille stable */
        Etudiant e;
        e.matricule = matricule;
        e.moyenne   = 10.0f;
        strcpy(e.nom, "X");
        insertionTableau(tab, &taille, e);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI INSERTION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        static Etudiant tab2[100000];
        int t2 = 0;
        for (int i = 0; i < n; i++)
            insertionTableau(tab2, &t2, *d2[i]);

        t_debut  = get_temps();
        Tri_insertion_moyenne(tab2, t2);
        t_fin    = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_etudiants(d2, n);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI RAPIDE --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
         Etudiant tab2[100000];
        int t2 = 0;
        for (int i = 0; i < n; i++)
            insertionTableau(tab2, &t2, *d2[i]);

        t_debut  = get_temps();
        TriRapide(tab2, 0, t2 - 1);
        t_fin    = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_etudiants(d2, n);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;tri_rapide;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    liberer_etudiants(data, n);
}

 void bench_tableau_dynamique(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;

    /* --- INSERTION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **data = generer_etudiants(n, dist);
        TableauDyn *t   = creer_tableau_dyn();

        t_debut = get_temps();
        for (int i = 0; i < n; i++)
            inserer_etudiant_dyn(t, data[i]);
        t_fin = get_temps();

        temps[r] = t_fin - t_debut;

        /* Ne pas libÃ©rer les étudiants ici â€” le tableau les gÃ¨re */
        liberer_tableau_dyn(t);
        free(data); /* libérer seulement le tableau de pointeurs */
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    Etudiant **data = generer_etudiants(n, dist);
    TableauDyn *t   = creer_tableau_dyn();
    for (int i = 0; i < n; i++)
        inserer_etudiant_dyn(t, data[i]);

    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        rechercher_par_matricule_dyn(t, matricule);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        supprimer_par_matricule_dyn(t, matricule);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;
        /* Rinsrer pour garder la taille stable */
        Etudiant *e = creer_etudiant(matricule, 10.0f,
                                      "X", "X",
                                      (Date){1,1,2000});
        inserer_etudiant_dyn(t, e);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI INSERTION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        /* Recréer un tableau non trié pour chaque répétition */
        Etudiant **d2 = generer_etudiants(n, dist);
        TableauDyn *t2 = creer_tableau_dyn();
        for (int i = 0; i < n; i++)
            inserer_etudiant_dyn(t2, d2[i]);

        t_debut = get_temps();
        tri_insertion_dyn(t2);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_tableau_dyn(t2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI RAPIDE --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        TableauDyn *t2 = creer_tableau_dyn();
        for (int i = 0; i < n; i++)
            inserer_etudiant_dyn(t2, d2[i]);

        t_debut = get_temps();
        tri_rapide_dyn(t2, 0, t2->taille - 1);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_tableau_dyn(t2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;tri_rapide;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    liberer_tableau_dyn(t);
    free(data);
}

void bench_liste(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;

    /* --- INSERTION EN QUEUE --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **data = generer_etudiants(n, dist);
        ListeDC *l      = creer_liste();

        t_debut = get_temps();
        for (int i = 0; i < n; i++)
            inserer_en_queue(l, data[i]);
        t_fin = get_temps();

        temps[r] = t_fin - t_debut;
        liberer_liste(l);
        free(data);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    Etudiant **data = generer_etudiants(n, dist);
    ListeDC *l      = creer_liste();
    for (int i = 0; i < n; i++)
        inserer_en_queue(l, data[i]);

    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        rechercher_matricule_liste(l, matricule);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        supprimer_matricule_liste(l, matricule);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;
        /* RÃ©insÃ©rer pour garder la taille stable */
        Etudiant *e = creer_etudiant(matricule, 10.0f,
                                      "X", "X",
                                      (Date){1,1,2000});
        inserer_en_queue(l, e);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI INSERTION --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        ListeDC *l2   = creer_liste();
        for (int i = 0; i < n; i++)
            inserer_en_queue(l2, d2[i]);

        t_debut = get_temps();
        tri_insertion_liste(l2);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_liste(l2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI BULLES --- */
    for (int r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        ListeDC *l2   = creer_liste();
        for (int i = 0; i < n; i++)
            inserer_en_queue(l2, d2[i]);

        t_debut = get_temps();
        tri_bulles_liste(l2);
        t_fin = get_temps();
        temps[r] = t_fin - t_debut;

        liberer_liste(l2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;tri_bulles;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    liberer_liste(l);
    free(data);
}

int main(void)
{
    printf("============================================\n");
    printf("   BENCHMARK â€” LMI ASD 2026\n");
    printf("   Structures : TabDyn + Liste Chainee\n");
    printf("============================================\n\n");

    /* Ouvrir le fichier de résultats CSV */
    FILE *f = fopen("resultats_benchmark.csv", "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible de crÃ©er resultats_benchmark.csv\n");
        return 1;
    }

    /* En-tete CSV */
    fprintf(f, "structure;operation;n;distribution;temps_moy(s);ecart_type(s)\n");

    /* Boucle sur toutes les combinaisons */
    for (int d = 0; d < NB_DISTRIBUTIONS; d++) {
        for (int t = 0; t < NB_TAILLES; t++) {
            int n = TAILLES[t];
            Distribution dist = DISTRIBUTIONS[d];

            printf("Benchmark n=%6d | distribution=%-12s ...",
                   n, nom_distribution(dist));
            fflush(stdout);

            bench_tableau_statique(n, dist, f);
            bench_tableau_dynamique(n, dist, f);
            bench_liste(n, dist, f);

            printf(" OK\n");
        }
    }

    fclose(f);

    printf("\n============================================\n");
    printf("Résultats sauvegardés dans resultats_benchmark.csv\n");
    printf("============================================\n");

    return 0;
}