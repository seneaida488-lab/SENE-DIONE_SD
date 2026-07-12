#include <stdio.h>
#include "liste_chainee.h"

/* Petite fonction utilitaire pour separer visuellement */
void titre(const char *texte) {
    printf("\n=== %s ===\n", texte);
}

int main(void) {

    /* --------------------------------------------------------------
     * 1. Creation de la liste et de quelques etudiants
     * ------------------------------------------------------------ */
    titre("Creation de la liste");
    ListeC *l = liste_creer();

    Etudiant *e1 = etudiant_creation(101, 14.5f, "Diop",   "Awa");
    Etudiant *e2 = etudiant_creation(102, 9.0f,  "Ndiaye", "Moussa");
    Etudiant *e3 = etudiant_creation(103, 16.2f, "Fall",   "Aissatou");
    Etudiant *e4 = etudiant_creation(104, 11.8f, "Diallo", "Ibrahima");
    Etudiant *e5 = etudiant_creation(105, 7.5f,  "Sene",   "Fatou");

    /* --------------------------------------------------------------
     * 2. Insertion en tete et en queue
     * ------------------------------------------------------------ */
    titre("Insertion des etudiants");
    inserer_en_queue(l, e1);   /* Diop      */
    inserer_en_queue(l, e2);   /* Ndiaye    */
    inserer_en_tete(l, e3);    /* Fall (en tete) */
    inserer_en_queue(l, e4);   /* Diallo    */
    inserer_en_tete(l, e5);    /* Sene (en tete) */

    afficher_liste(l);

    /* --------------------------------------------------------------
     * 3. Recherche
     * ------------------------------------------------------------ */
    titre("Recherche par matricule (102)");
    Etudiant *trouve = rechercher_matricule_liste(l, 102);
    if (trouve != NULL) {
        printf("  Trouve : %s %s (moyenne=%.2f)\n",
               trouve->nom, trouve->prenom, trouve->moyenne);
    } else {
        printf("  Aucun etudiant avec ce matricule.\n");
    }

    titre("Recherche par intervalle de moyenne [10 ; 15]");
    rechercher_intervalle_liste(l, 10.0f, 15.0f);

    titre("Recherche par prefixe de nom \"D\"");
    rechercher_liste_prefixes(l, "D");

    /* --------------------------------------------------------------
     * 4. Modification
     * ------------------------------------------------------------ */
    titre("Modification de la moyenne du matricule 105");
    liste_modifier_moyenne(l, 105, 12.0f);
    afficher_liste(l);

    /* --------------------------------------------------------------
     * 5. Statistiques (avant tri, l'ordre n'a pas d'importance ici)
     * ------------------------------------------------------------ */
    titre("Statistiques");
    printf("  Moyenne generale : %.2f\n", moyenne_generale_liste(l));
    printf("  Minimum          : %.2f\n", liste_minimum(l));
    printf("  Maximum          : %.2f\n", liste_maximum(l));
    printf("  Mediane          : %.2f\n", liste_mediane(l));
    printf("  Ecart-type       : %.2f\n", liste_ecart_type(l));

    /* --------------------------------------------------------------
     * 6. Tri
     * ------------------------------------------------------------ */
    titre("Tri par insertion (moyenne croissante)");
    liste_tri_insertion(l);
    afficher_liste(l);

    titre("Affichage inverse (apres tri)");
    afficher_liste_inverse(l);

    titre("Tri a bulles (juste pour verifier qu'il redonne le meme resultat)");
    liste_tri_bulles(l);
    afficher_liste(l);

    /* --------------------------------------------------------------
     * 7. Suppression
     * ------------------------------------------------------------ */
    titre("Suppression du matricule 102");
    supprimer_liste_matricule(l, 102);
    afficher_liste(l);

    /* --------------------------------------------------------------
     * 8. Persistance : serialisation puis desialisation
     * ------------------------------------------------------------ */
    titre("Serialisation dans data.bin");
    liste_serialiser(l, "data.bin");
    printf("  Ecrit dans data.bin\n");

    titre("Deserialisation depuis data.bin");
    ListeC *l2 = liste_deserialiser("data.bin");
    afficher_liste(l2);

    /* --------------------------------------------------------------
     * 9. Liberation memoire
     * ------------------------------------------------------------ */
    titre("Liberation de la memoire");
    liste_liberer(l);
    liste_liberer(l2);
    printf("  OK, listes liberees.\n");

    return 0;
}