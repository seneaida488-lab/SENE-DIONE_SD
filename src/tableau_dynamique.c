#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../include/tableau_dynamique.h"
int main() {
    TableauDyn t;
    tableau_init(&t, 2);

    Etudiant *e1 = malloc(sizeof(Etudiant));
    e1->matricule = 101;
	 strcpy(e1->nom, "Ndiaye");
	  strcpy(e1->prenom, "Déthié");
	   e1->moyenne = 14.5;

    Etudiant *e2 = malloc(sizeof(Etudiant));
    e2->matricule = 102;
	strcpy(e2->nom, "dione");
	strcpy(e2->prenom, "Coura"); 
	e2->moyenne = 10.0;

    Etudiant *e3 = malloc(sizeof(Etudiant));
    e3->matricule = 103;
	strcpy(e3->nom, "Diagne"); 
	strcpy(e3->prenom, "babacar");
	e3->moyenne = 17.2;

    tableau_inserer(&t, e1);
    tableau_inserer(&t, e2);
    tableau_inserer(&t, e3); // déclenche tableau_agrandir

    // Recherche par matricule
    Etudiant *res = tableau_rechercher_matricule(&t, 102);
    if (res)
        printf("Trouve: %s %s - %.2f\n", res->nom, res->prenom, res->moyenne);

    // Recherche par préfixe
    printf("\nEtudiants commencant par 'D':\n");
    tableau_rechercher_prefixe(&t, "D");

    // Recherche par intervalle (maintenant fonctionnelle)
    printf("\n");
    tableau_rechercher_intervalle(&t, 10.0, 15.0);

    // Modification de moyenne
    tableau_modifier_moyenne(&t, 101, 18.0);

    // Tri par insertion sur la moyenne (maintenant fonctionnel)
    tableau_tri_insertion_moyenne(&t);

    printf("\nListe triee par moyenne:\n");
    for (int i = 0; i < t.taille; i++)
        printf("  [%d] %s %s - %.2f\n", t.tab[i]->matricule, t.tab[i]->nom, t.tab[i]->prenom, t.tab[i]->moyenne);

    // Tri rapide par nom
    tableau_tri_rapide(t.tab, 0, t.taille - 1);

    printf("\nListe triee par nom:\n");
    for (int i = 0; i < t.taille; i++)
        printf("  [%d] %s %s - %.2f\n", t.tab[i]->matricule, t.tab[i]->nom, t.tab[i]->prenom, t.tab[i]->moyenne);

    // Statistiques
    printf("\nMoyenne min: %.2f\n", tableau_minimummoyennes(&t));
    printf("Moyenne max: %.2f\n", tableau_maximummoyennes(&t));
    printf("Moyenne generale: %.2f\n", tableauMoyenneDesmoyennes(&t));
    printf("Ecart type: %.2f\n", tableau_ecart_type(&t));

    // Suppressions
    tableau_supprimer(&t, 102);
    printf("\nApres suppression de 102, taille = %d\n", t.taille);

    // Libération (fonctionne maintenant correctement)
    tableau_liberer(&t);

    return 0;
}