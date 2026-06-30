#include <stdio.h>
#include <stdlib.h>
#include "../include/tableau_dynamique.h"

int main(void)
{
    printf("============================================\n");
    printf("   SYSTEME DE GESTION DE SCOLARITE\n");
    printf("   Structure : Tableau Dynamique\n");
    printf("============================================\n\n");

    TableauDyn *t = creer_tableau_dyn();
    if (t == NULL) {
        fprintf(stderr, "Erreur : impossible de créer le tableau\n");
        return 1;
    }
    printf("Tableau crée : capacité initiale = %d places\n\n",
           t->capacite);
    printf("--- Insertion des étudiants ---\n");
    
    Date d1 = {5,  3, 2003};
    Date d2 = {12, 7, 2002};
    Date d3 = {20, 1, 2004};
    Date d4 = {8,  9, 2003};
    Date d5 = {15, 5, 2002};

    Etudiant *e1 = creer_etudiant(1001, 14.50f, "Ndiaye", "Moussa",  d1);
    Etudiant *e2 = creer_etudiant(1002, 12.00f, "Diallo", "Fatou",   d2);
    Etudiant *e3 = creer_etudiant(1003, 17.50f, "Sow",    "Ibou",    d3);
    Etudiant *e4 = creer_etudiant(1004,  9.00f, "Fall",   "Awa",     d4);
    Etudiant *e5 = creer_etudiant(1005, 15.75f, "Diop",   "Khadija", d5);

    inserer_etudiant_dyn(t, e1);
    inserer_etudiant_dyn(t, e2);
    inserer_etudiant_dyn(t, e3);
    inserer_etudiant_dyn(t, e4);
    inserer_etudiant_dyn(t, e5);

    printf("\n");

    printf("--- Affichage initial ---\n");
    afficher_tableau_dyn(t);
    printf("\n");

    printf("--- Recherche par matricule (1003) ---\n");
    Etudiant *trouve = rechercher_par_matricule_dyn(t, 1003);
    if (trouve != NULL) {
        printf("  TrouvÃ© : [%d] %s %s â€” moyenne : %.2f\n",
               trouve->matricule,
               trouve->nom,
               trouve->prenom,
               trouve->moyenne);
    } else {
        printf("  Etudiant non trouvÃ©.\n");
    }
    printf("\n",);
    
    printf("--- Recherche par intervalle [10.00 , 15.00] ---\n");
    rechercher_par_intervalle_dyn(t, 10.00f, 15.00f);
    printf("\n");

    printf("--- Recherche par préfixe \"Di\" ---\n");
    rechercher_par_prefixe_dyn(t, "Di");
    printf("\n");

    printf("--- Mise a jour moyenne de l'étudiant 1002 ---\n");
    modifier_moyenne_dyn(t, 1002, 16.00f);
    printf("\n");
    printf("--- Tri par insertion (moyenne croissante) ---\n");
    tri_insertion_dyn(t);
    afficher_tableau_dyn(t);
    printf("\n");

    printf("--- Tri rapide (moyenne croissante) ---\n");
    tri_rapide_dyn(t, 0, t->taille - 1);
    afficher_tableau_dyn(t);
    printf("\n");

    printf("--- Statistiques sur les moyennes ---\n");
    printf("  Moyenne générale : %.2f\n", moyenne_generale_dyn(t));
    printf("  Minimum          : %.2f\n", minimum_dyn(t));
    printf("  Maximum          : %.2f\n", maximum_dyn(t));
    printf("  Médiane          : %.2f\n", mediane_dyn(t));
    printf("  écart-type       : %.2f\n", ecart_type_dyn(t));
    printf("\n");

    printf("--- Suppression de l'étudiant 1004 ---\n");
    supprimer_par_matricule_dyn(t, 1004);
    afficher_tableau_dyn(t);
    printf("\n");

    printf("--- Sérialisation dans etudiants_dyn.bin ---\n");
    serialiser_dyn(t, "etudiants_dyn.bin");

    printf("\n--- Désérialisation depuis etudiants_dyn.bin ---\n");
    TableauDyn *t2 = deserialiser_dyn("etudiants_dyn.bin");
    if (t2 != NULL) {
        afficher_tableau_dyn(t2);
        liberer_tableau_dyn(t2);
    }
    printf("\n");

    liberer_tableau_dyn(t);
    printf("MÃ©moire libérée avec succés.\n");
    printf("\n============================================\n");
    printf("   FIN DU PROGRAMME\n");
    printf("============================================\n");

    return 0;
}