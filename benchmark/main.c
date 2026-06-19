#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<math.h>

#include "../include/tableau_dynamique.h"
#include "../include/structure.h"
#include"../include/tableau_statique.h"

void afficher_echantillon_jeu_donnees(TableauDyn *t) {
   float tabnote;
    if (t->taille == 0) return;
    
    printf("   [Aperçu du jeu de données (%d étudiants)] :\n", t->taille);
    // Affichage des 3 premiers
    for (int i = 0; i < 3 && i < t->taille; i++) {
        printf("      Index %d : %s | Note : %.2f\n", i, t->tab[i]->matricule, t->tab[i]->tabnote);
    } 
    if (t->taille > 6) {
        printf("      ...\n");  


    }
    // Affichage des 3 derniers
    int depart = (t->taille - 3 > 3) ? t->taille - 3 : 3;
    for (int i = depart; i < t->taille; i++) {
        printf("      Index %d : %s | Note : %.2f\n", i, t->tab[i]->matricule, t->tab[i]->tabnote);
    }
}

// --- 2. ALGORITHME DE TRI RAPIDE (QuickSort par Note Décroissante) ---
void quicksort_note(Etudiant **tab, int gauche, int droite) {
    if (gauche >= droite) return;
    double pivot = tab[(gauche + droite) / 2]->tabnote;
    int i = gauche, j = droite;
    while (i <= j) {
        while (tab[i]->tabnote > pivot) i++; // Tri Décroissant
        while (tab[j]->tabnote < pivot) j--;
        if (i <= j) {
            Etudiant *temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
            i++; j--;
        }
    }
    quicksort_note(tab, gauche, j);
    quicksort_note(tab, i, droite);
}

void generer_uniforme(TableauDyn *t, int n) {
    tableau_init(t, 10);
    for (int i = 0; i < n; i++) {
        Etudiant *e = malloc(sizeof(Etudiant));
        sprintf(e->matricule, "MAT-%06d", i);
        e->tabnote = (double)(rand() % 2001) / 100.0; // Aléatoire entre 0 et 20
        tableau_inserer(t, e);
    }
}

void generer_deja_trie(TableauDyn *t, int n) {
    tableau_init(t, 10);
    for (int i = 0; i < n; i++) {
        Etudiant *e = malloc(sizeof(Etudiant));
        sprintf(e->matricule, "MAT-%06d", i);
        e->tabnote = 20.0 - ((double)i * (20.0 / n)); // Déjà trié (Décroissant)
        tableau_inserer(t, e);
    }
}

void generer_inversement_trie(TableauDyn *t, int n) {
    tableau_init(t, 10);
    for (int i = 0; i < n; i++) {
        Etudiant *e = malloc(sizeof(Etudiant));
        sprintf(e->matricule, "MAT-%06d", i);
        e->tabnote = (double)i * (20.0 / n); // Trié à l'envers (Croissant)
        tableau_inserer(t, e);
    }
}

void executer_etude_experimentale_5_2() {
    int tailles[] = {100, 1000, 10000, 100000}; 
    int nb_tailles = 4;
    int repetitions = 10; 
    srand(time(NULL));

    printf("==================================================\n");
    printf("==================================================\n");

    for (int profil = 1; profil <= 3; profil++) {
        if (profil == 1) printf("\n[PROFIL 1] DISTRIBUTION UNIFORME\n");
        if (profil == 2) printf("\n[PROFIL 2] DONNEES DEJA TRIEES\n");
        if (profil == 3) printf("\n[PROFIL 3] DONNEES INVERSEMENT TRIEES\n");

        for (int i = 0; i < nb_tailles; i++) {
            int n = tailles[i];
            double somme_temps = 0.0;
            
            // Pour le premier essai de chaque taille, on affiche un exemple du jeu de données avant le tri
            TableauDyn exemple_visuel;
            if (profil == 1) generer_uniforme(&exemple_visuel, n);
            if (profil == 2) generer_deja_trie(&exemple_visuel, n);
            if (profil == 3) generer_inversement_trie(&exemple_visuel, n);
            
            // On affiche le jeu de données généré 
            afficher_echantillon_jeu_donnees(&exemple_visuel);
            tableau_liberer(&exemple_visuel);

            // Boucle des 10 répétitions pour le chronomètre
            for (int rep = 0; rep < repetitions; rep++) {
                TableauDyn t;
                
                if (profil == 1) generer_uniforme(&t, n);
                if (profil == 2) generer_deja_trie(&t, n);
                if (profil == 3) generer_inversement_trie(&t, n);

                // CHRONOMÉTRAGE DE L'ALGORITHME
                clock_t debut = clock();
                quicksort_note(t.tab, 0, t.taille - 1); 
                clock_t fin = clock();

                somme_temps += (double)(fin - debut) / CLOCKS_PER_SEC;
                tableau_liberer(&t); 
            }

            double temps_moyen = somme_temps / repetitions;
            printf("   => TOTAL N = %-6d | Temps Moyen : %f secondes\n\n", n, temps_moyen);
        }
    }
}