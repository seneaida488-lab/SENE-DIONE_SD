#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../include/tableau_statique.h"
#include "../include/tableau_dynamique.h"
#include "../include/liste_chainee.h"
#include "generateur_donnee.h"

#define NB_REPETITIONS  10
#define NB_TAILLES       4
#define NB_DISTRIBUTIONS 3

void ecrire_csv(FILE *csv, const char *structure,
                const char *dist, int n,
                const char *op, double *temps, int rep) {
    double moy = 0, var = 0;
    int cnt = 0;
    int r;

    for ( r = 0; r < rep; r++){
        if (temps[r] >= 0) {
             moy += temps[r]; cnt++;
            cnt++;
         }
        }
    if (cnt == 0) return;
    moy /= cnt;
    for (r = 0; r < rep; r++){
        if (temps[r] >= 0){
         var += pow(temps[r] - moy, 2);
        }
    }
    double ecar = sqrt(var / cnt);
    fprintf(csv, "%s,%s,%d,%s,%.4f,%.4f\n",
            structure, dist, n, op, moy, ecar);
}
void liberer_etudiant(Etudiant *e);

Etudiant* creer_etudiant(int matricule, float moyenne, char *nom, char *prenom, Date d);

 int TAILLES[NB_TAILLES] = {N1, N2, N3, N4};

 Distribution DISTRIBUTIONS[NB_DISTRIBUTIONS] = {
    UNIFORME, GAUSSIENNE, TRIEE
};

/* ============================================================
   MESURE DU TEMPS
   ============================================================ */

    double get_temps(void)
{
    return (double)clock() / CLOCKS_PER_SEC;
}

 double calcul_moyenne(double *temps, int n)
{
    double somme = 0.0;
    int i;
    for (i = 0; i < n; i++) somme += temps[i];
    return somme / n;
}

 double calcul_ecart_type(double *temps, int n, double moy)
{
    double somme = 0.0;
    int i;
    for (i = 0; i < n; i++) {
        double diff = temps[i] - moy;
        somme += diff * diff;
    }
    return sqrt(somme / n);
}

/* ============================================================
   BENCHMARK TABLEAU STATIQUE
   ============================================================ */
void bench_tableau_statique(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;
    int r, i, taille;
    static Etudiant T[100000];

    /* --- INSERTION --- */
    printf("    [TAB_STAT] debut insertion (n=%d)\n", n); fflush(stdout);
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **tab = generer_etudiants(n, dist);
        if (tab == NULL) continue;

        taille = 0;
        t_debut = get_temps();
        for (i = 0; i < n; i++)
            insertionTableau(T, &taille, *tab[i]);
        t_fin = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        for (i = 0; i < n; i++) free(tab[i]);
        free(tab);
    }
    printf("    [TAB_STAT] fin insertion\n"); fflush(stdout);
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    printf("    [TAB_STAT] debut Recherche\n");fflush(stdout);
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **tab = generer_etudiants(n, dist);
        if (tab == NULL) continue;

        taille = 0;
        for (i = 0; i < n; i++)
            insertionTableau(T, &taille, *tab[i]);
        for (i = 0; i < n; i++) free(tab[i]);
        free(tab);

        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        RechercheCleprimaire(T, taille, matricule);
        t_fin = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;
    }
    printf("    [TAB_STAT] fin Recherche\n");fflush(stdout);
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    printf("    [TAB_STAT] debut Suppression\n");fflush(stdout);
    for (r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % taille);
        t_debut = get_temps();
        suppressionCleprimaire(T, &taille, matricule);
        t_fin = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        Etudiant e;
        e.matricule = matricule;
        e.moyenne   = 10.0f;
        strcpy(e.nom, "X");
        insertionTableau(T, &taille, e);
    }
    printf("    [TAB_STAT] fin Suppression\n");fflush(stdout);
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI INSERTION --- */
    printf("    [TAB_STAT] debut Tri Insertion\n");fflush(stdout);
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        static Etudiant T2[100000];
        int t2 = 0;
        for (i = 0; i < n; i++)
            insertionTableau(T2, &t2, *d2[i]);

        t_debut = get_temps();
        Tri_insertion_moyenne(T2, t2);
        t_fin = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_etudiants(d2, n);
    }
    printf("    [TAB_STAT] fin Tri Insertion\n");fflush(stdout);
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI RAPIDE --- */
    printf("    [TAB_STAT] debut Tri Rapide\n");fflush(stdout);
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        static Etudiant T2[100000];
        int t2 = 0;
        for (i = 0; i < n; i++)
            insertionTableau(T2, &t2, *d2[i]);

        t_debut = get_temps();
        TriRapide(T2, 0, t2 - 1);
        t_fin = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_etudiants(d2, n);
    }
    printf("    [TAB_STAT] fin Tri Rapide\n");fflush(stdout);
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_STAT;tri_rapide;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);
    }

/* ============================================================
   BENCHMARK TABLEAU DYNAMIQUE
   ============================================================ */
 void bench_tableau_dynamique(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;

    /* --- INSERTION --- */
    int r;
    for ( r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **tab = generer_etudiants(n, dist);
        TableauDyn *t   = creer_tableau_dyn();

        t_debut = get_temps();
        int i;
        for (i = 0; i < n; i++)
            inserer_etudiant_dyn(t, tab[i]);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_tableau_dyn(t);
        free(tab);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    Etudiant **tab = generer_etudiants(n, dist);
    TableauDyn *t   = creer_tableau_dyn();
    int i;
    for ( i = 0; i < n; i++)
        inserer_etudiant_dyn(t, tab[i]);

    for (r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut  = get_temps();
        rechercher_par_matricule_dyn(t, matricule);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    for (r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        supprimer_par_matricule_dyn(t, matricule);
        t_fin   = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;
        /* Réinsérer pour garder la taille stable */
        Date d_temp = {1, 1, 2000};
        Etudiant *e = creer_etudiant(matricule, 10.0f, "X", "X", d_temp);
        inserer_etudiant_dyn(t, e);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI INSERTION --- */
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2  = generer_etudiants(n, dist);
        TableauDyn *t2 = creer_tableau_dyn();
        int i;
        for ( i = 0; i < n; i++)
            inserer_etudiant_dyn(t2, d2[i]);

        t_debut  = get_temps();
        tri_insertion_dyn(t2);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_tableau_dyn(t2);
        free(d2);
    }
    moy  = calcul_moyenne(temps,    NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- TRI RAPIDE --- */
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2  = generer_etudiants(n, dist);
        TableauDyn *t2 = creer_tableau_dyn();
        int i;
        for ( i = 0; i < n; i++)
            inserer_etudiant_dyn(t2, d2[i]);

        t_debut  = get_temps();
        tri_rapide_dyn(t2, 0, t2->taille - 1);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_tableau_dyn(t2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "TAB_DYN;tri_rapide;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    liberer_tableau_dyn(t);
    free(tab);
}

/* ============================================================
   BENCHMARK LISTE DOUBLEMENT CHAÎNÉE
   ============================================================ */
void bench_liste(int n, Distribution dist, FILE *f)
{
    double temps[NB_REPETITIONS];
    double t_debut, t_fin, moy, ecar;

    /* --- INSERTION EN QUEUE --- */
    int r;
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **tab = generer_etudiants(n, dist);
        ListeDC *l      = creer_liste();

        t_debut = get_temps();
        int i;
        for (i = 0; i < n; i++)
            inserer_en_queue(l, tab[i]);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_liste(l);
        free(tab);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- RECHERCHE --- */
    Etudiant **tab = generer_etudiants(n, dist);
    ListeDC *l      = creer_liste();
    int i;
    for (i = 0; i < n; i++){
        inserer_en_queue(l, tab[i]);
    }
    for (r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut  = get_temps();
        rechercher_matricule_liste(l, matricule);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;recherche;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);

    /* --- SUPPRESSION --- */
    for (r = 0; r < NB_REPETITIONS; r++) {
        int matricule = 1000 + (rand() % n);
        t_debut = get_temps();
        supprimer_matricule_liste(l, matricule);
        t_fin   = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;
        /* Réinsérer pour garder la taille stable */
        Date d_temp = {1, 1, 2000};
        Etudiant *e = creer_etudiant(matricule, 10.0f, "X", "X", d_temp);
        inserer_en_queue(l, e);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;suppression;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);
    
    /* --- TRI INSERTION --- */
    for (r = 0; r < NB_REPETITIONS; r++) {
        Etudiant **d2 = generer_etudiants(n, dist);
        ListeDC *l2   = creer_liste();
        int i;
        for (i = 0; i < n; i++) {
            inserer_en_queue(l2, d2[i]);
        }
        t_debut  = get_temps();
        tri_insertion_liste(l2);
        t_fin    = get_temps();
        temps[r] = (t_fin - t_debut) * 1000.0;

        liberer_liste(l2);
        free(d2);
    }
    moy  = calcul_moyenne(temps, NB_REPETITIONS);
    ecar = calcul_ecart_type(temps, NB_REPETITIONS, moy);
    fprintf(f, "LISTE;tri_insertion;%d;%s;%.9f;%.9f\n",
            n, nom_distribution(dist), moy, ecar);
    }

/* ============================================================
   PROGRAMME PRINCIPAL
   ============================================================ */
int main(void)
{
    printf("============================================\n");
    printf("Démarrer BENCHMARK\n");
    printf("   BENCHMARK — LMI ASD 2026\n");
    printf("   3 structures x 4 tailles x 3 distributions\n");
    printf("============================================\n\n");

    FILE *f = fopen("resultats_benchmark.csv", "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible de créer resultats_benchmark.csv\n");
        return 1;
    }

    /* En-tête CSV */
    fprintf(f, "structure;operation;n;distribution;temps_moy(s);ecart_type(s)\n");
    int d;
    for (d = 0; d < NB_DISTRIBUTIONS; d++) {
        int i;
        for ( i = 0; i < NB_TAILLES; i++) {
            int n = TAILLES[i];
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