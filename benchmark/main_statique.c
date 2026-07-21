#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tableau_statique.h"

int main(void)
{
    printf("============================================\n");
    printf("   SYSTÈME DE GESTION DE SCOLARITÉ\n");
    printf("   Structure : Tableau Statique\n");
    printf("============================================\n\n");

    Etudiant tab[TAILLEMAX];
    int n = 0;
    int i;

    printf("Tableau statique cree : capacite max = %d places\n\n",
           TAILLEMAX);

    /* --------------------------------------------------
       ÉTAPE 2 : Créer et insérer des étudiants
       (donnees inchangees)
    -------------------------------------------------- */

    Etudiant e1, e2, e3, e4, e5;

    e1.matricule = 1001;
    e1.moyenne = 14.50;
    e1.dateNaissance.jour = 5;
    e1.dateNaissance.mois = 1;
    e1.dateNaissance.annee = 2007;
    strcpy(e1.nom, "Ndiaye");

    e2.matricule = 1002;
    e2.moyenne   = 14.00;
    e2.dateNaissance.jour = 10;
    e2.dateNaissance.mois = 5;
    e2.dateNaissance.annee = 2006;
    strcpy(e2.nom, "Sène");

    e3.matricule = 1003;
    e3.moyenne   = 17.50;
    e3.dateNaissance.jour = 15;
    e3.dateNaissance.mois = 3;
    e3.dateNaissance.annee = 2004;
    strcpy(e3.nom, "Ka");

    e4.matricule = 1004;
    e4.moyenne   = 10.00;
    e4.dateNaissance.jour = 20;
    e4.dateNaissance.mois = 7;
    e4.dateNaissance.annee = 2005;
    strcpy(e4.nom, "Guèye");

    e5.matricule = 1005;
    e5.moyenne   = 15.75;
    e5.dateNaissance.jour = 10;
    e5.dateNaissance.mois = 11;
    e5.dateNaissance.annee = 2007;
    strcpy(e5.nom, "Dia");

    insertionTableau(tab, &n, e1);
    insertionTableau(tab, &n, e2);
    insertionTableau(tab, &n, e3);
    insertionTableau(tab, &n, e4);
    insertionTableau(tab, &n, e5);

    printf("%d etudiants inseres.\n\n", n);

    /* --------------------------------------------------
       ÉTAPE 3 : Afficher tous les étudiants
    -------------------------------------------------- */
    printf("--- Affichage initial ---\n");
    for (i = 0; i < n; i++) {
        printf("  [%d] matricule:%d dateNaissance:%02d/%02d/%d | %s | moyenne:%.2f\n",
               i, tab[i].matricule, tab[i].dateNaissance.jour, tab[i].dateNaissance.mois, tab[i].dateNaissance.annee, tab[i].nom, tab[i].moyenne);
    }
    printf("\n");

    /* --------------------------------------------------
       ÉTAPE 4 : Recherche par matricule (clé primaire)
       CORRIGE : le matricule est maintenant saisi par
       l'utilisateur au lieu d'etre code en dur (1003).
    -------------------------------------------------- */
    int matriculeRecherche;
    printf("--- Recherche par matricule ---\n");
    printf("Entrez le matricule a rechercher : ");
    scanf("%d", &matriculeRecherche);
    vider_buffer();

    int pos = RechercheCleprimaire(tab, n, matriculeRecherche);
    if (pos > -1) {
        printf("  Trouve a l'index %d : %s - moyenne : %.2f\n",
               pos, tab[pos].nom, tab[pos].moyenne);
    } else {
        printf("  Etudiant non trouve.\n");
    }
    printf("\n");

    /* --------------------------------------------------
       ÉTAPE 5 : Recherche par intervalle de moyenne
       CORRIGE : min et max sont maintenant saisis par
       l'utilisateur au lieu d'etre codes en dur.
    -------------------------------------------------- */
    double minRecherche, maxRecherche;
    printf("--- Recherche par intervalle de moyenne ---\n");
    printf("Moyenne minimale : ");
    scanf("%lf", &minRecherche);
    vider_buffer();
    printf("Moyenne maximale : ");
    scanf("%lf", &maxRecherche);
    vider_buffer();

    RechercheParIntervalle(tab, n, minRecherche, maxRecherche);
    printf("\n\n");

    /* --------------------------------------------------
       ÉTAPE 6 : Recherche par préfixe sur le nom
       (inchangee)
    -------------------------------------------------- */
    printf("--- Recherche par préfixe \"Nd\" ---\n");
    RechercheParPrefixe(tab, n, "Nd");
    printf("\n\n");

    /* --------------------------------------------------
       ÉTAPE 7 : Mise à jour (nom)
    -------------------------------------------------- */
    printf("--- Mise à jour du nom de l'étudiant 1002 ---\n");
    Miseajour(tab, n, 1002, "NdiayeModifie");
    printf("\n\n");

    /* --------------------------------------------------
       ÉTAPE 8 : Tri par insertion (par moyenne croissante)
    -------------------------------------------------- */
    printf("--- Tri par insertion (moyenne croissante) ---\n");
    Tri_insertion_moyenne(tab, n);
    for (i = 0; i < n; i++) {
        printf("  [%d] matricule:%d dateNaissance:%02d/%02d/%d | %s | moyenne:%.2f\n",
               i, tab[i].matricule, tab[i].dateNaissance.jour, tab[i].dateNaissance.mois, tab[i].dateNaissance.annee, tab[i].nom, tab[i].moyenne);
    }
    printf("\n");

    /* --------------------------------------------------
       ÉTAPE 9 : Tri rapide (par moyenne croissante)
    -------------------------------------------------- */
    printf("--- Tri rapide (moyenne croissante) ---\n");
    TriRapide(tab, 0, n - 1);
    for (i = 0; i < n; i++) {
        printf("  [%d] matricule:%d dateNaissance:%02d/%02d/%d | %s | moyenne:%.2f\n",
               i, tab[i].matricule, tab[i].dateNaissance.jour, tab[i].dateNaissance.mois, tab[i].dateNaissance.annee, tab[i].nom, tab[i].moyenne);
    }
    printf("\n");

    /* --------------------------------------------------
       ÉTAPE 10 : Agrégations statistiques
    -------------------------------------------------- */
    printf("--- Statistiques sur les moyennes ---\n");
    printf("  Moyenne générale : %.2f\n", MoyenneDesmoyennes(tab, n));
    printf("  Minimum          : %.2f\n",   minimumDesmoyennes(tab, n));
    printf("  Maximum          : %.2f\n",   maximumDesmoyennes(tab, n));
    printf("  Médiane          : %.2f\n", MedianeDesmoyennes(tab, n));
    printf("  Écart-type       : %.2f\n", EcartTypeDesmoyennes(tab, n));
    printf("\n");

    /* --------------------------------------------------
       ÉTAPE 11 : Suppression d'un étudiant
    -------------------------------------------------- */
    printf("--- Suppression de l'étudiant 1004 ---\n");
    suppressionCleprimaire(tab, &n, 1004);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("  [%d] matricule:%d dateNaissance:%02d/%02d/%d | %s | moyenne:%.2f\n",
               i, tab[i].matricule, tab[i].dateNaissance.jour, tab[i].dateNaissance.mois, tab[i].dateNaissance.annee, tab[i].nom, tab[i].moyenne);
    }
    printf("\n");

    printf("============================================\n");
    printf("   FIN DU PROGRAMME\n");
    printf("============================================\n");

    return 0;
}
