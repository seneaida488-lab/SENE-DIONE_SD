#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tableau_dynamique.h"


void vider_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void afficher_menu(void)
{
    printf("\n============================================\n");
    printf("   SYSTÈME DE GESTION DE SCOLARITÉ\n");
    printf("============================================\n");
    printf("  1.  Ajouter un etudiant\n");
    printf("  2.  Afficher tous les etudiants\n");
    printf("  3.  Rechercher par matricule\n");
    printf("  4.  Rechercher par intervalle de moyenne\n");
    printf("  5.  Rechercher par prefixe du nom\n");
    printf("  6.  Modifier la moyenne d'un etudiant\n");
    printf("  7.  Supprimer un etudiant\n");
    printf("  8.  Trier par moyenne (tri insertion)\n");
    printf("  9.  Trier par moyenne (tri rapide)\n");
    printf("  10. Statistiques generales\n");
    printf("  11. Sauvegarder dans un fichier\n");
    printf("  12. Charger depuis un fichier\n");
    printf("  0.  Quitter\n");
    printf("============================================\n");
    printf("Votre choix : ");
}

/*
 * saisir_etudiant
 * Demande à l'utilisateur de saisir les informations
 * d'un nouvel étudiant et l'insère dans le tableau.
 */
void saisir_etudiant(TableauDyn *t)
{
    int    matricule;
    float  moyenne;
    char   nom[40];
    char   prenom[100];
    Date   date;

    printf("\n--- Ajout d'un étudiant ---\n");

    printf("Matricule    : ");
    scanf("%d", &matricule);
    vider_buffer();

    /* Vérifier que le matricule n'existe pas déjà */
    if (rechercher_par_matricule_dyn(t, matricule) != NULL) {
        printf("Erreur : le matricule %d existe déjà.\n", matricule);
        return;
    }

    printf("Nom          : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0'; /* supprimer le \n final */

    printf("Prénom       : ");
    fgets(prenom, sizeof(prenom), stdin);
    prenom[strcspn(prenom, "\n")] = '\0';

    printf("Moyenne      : ");
    scanf("%f", &moyenne);
    vider_buffer();

    /* Vérifier que la moyenne est valide */
    if (moyenne < 0.0f || moyenne > 20.0f) {
        printf("Erreur : la moyenne doit être entre 0 et 20.\n");
        return;
    }

    printf("Date de naissance\n");
    printf("  Jour  : ");
    scanf("%d", &date.jour);
    printf("  Mois  : ");
    scanf("%d", &date.mois);
    printf("  Année : ");
    scanf("%d", &date.annee);
    vider_buffer();

    /* Créer et insérer l'étudiant */
    Etudiant *e = creer_etudiant(matricule, moyenne, nom, prenom, date);
    if (e == NULL) {
        printf("Erreur : impossible de créer l'étudiant.\n");
        return;
    }

    inserer_etudiant_dyn(t, e);
    printf("Étudiant %s %s ajouté avec succès.\n", nom, prenom);
}

/*
 * menu_recherche_matricule
 * Demande un matricule et affiche l'étudiant trouvé.
 */
void menu_recherche_matricule(TableauDyn *t)
{
    int matricule;
    printf("\n--- Recherche par matricule ---\n");
    printf("Matricule : ");
    scanf("%d", &matricule);
    vider_buffer();

    Etudiant *e = rechercher_par_matricule_dyn(t, matricule);
    if (e != NULL) {
        printf("\nÉtudiant trouvé :\n");
        printf("  Matricule  : %d\n",     e->matricule);
        printf("  Nom        : %s\n",     e->nom);
        printf("  Prénom     : %s\n",     e->prenom);
        printf("  Moyenne    : %.2f\n",   e->moyenne);
        printf("  Naissance  : %02d/%02d/%04d\n",
               e->dateNaissance.jour,
               e->dateNaissance.mois,
               e->dateNaissance.annee);
    } else {
        printf("Aucun étudiant avec le matricule %d.\n", matricule);
    }
}

/*
 * menu_recherche_intervalle
 * Demande un intervalle [min, max] et affiche les résultats.
 */
void menu_recherche_intervalle(TableauDyn *t)
{
    float min, max;
    printf("\n--- Recherche par intervalle de moyenne ---\n");
    printf("Moyenne minimale : ");
    scanf("%f", &min);
    printf("Moyenne maximale : ");
    scanf("%f", &max);
    vider_buffer();

    rechercher_par_intervalle_dyn(t, min, max);
}

/*
 * menu_recherche_prefixe
 * Demande un préfixe et affiche les étudiants dont le nom commence par ce préfixe.
 */
void menu_recherche_prefixe(TableauDyn *t)
{
    char prefixe[40];
    printf("\n--- Recherche par préfixe du nom ---\n");
    printf("Préfixe : ");
    fgets(prefixe, sizeof(prefixe), stdin);
    prefixe[strcspn(prefixe, "\n")] = '\0';

    rechercher_par_prefixe_dyn(t, prefixe);
}

/*
 * menu_modifier_moyenne
 * Demande un matricule et une nouvelle moyenne, puis met à jour.
 */
void menu_modifier_moyenne(TableauDyn *t)
{
    int   matricule;
    float nouvelle_moyenne;

    printf("\n--- Modification de moyenne ---\n");
    printf("Matricule        : ");
    scanf("%d", &matricule);
    printf("Nouvelle moyenne : ");
    scanf("%f", &nouvelle_moyenne);
    vider_buffer();

    if (nouvelle_moyenne < 0.0f || nouvelle_moyenne > 20.0f) {
        printf("Erreur : la moyenne doit être entre 0 et 20.\n");
        return;
    }

    modifier_moyenne_dyn(t, matricule, nouvelle_moyenne);
}

/*
 * menu_supprimer
 * Demande un matricule et supprime l'étudiant correspondant.
 */
void menu_supprimer(TableauDyn *t)
{
    int matricule;
    printf("\n--- Suppression d'un étudiant ---\n");
    printf("Matricule : ");
    scanf("%d", &matricule);
    vider_buffer();

    supprimer_par_matricule_dyn(t, matricule);
}

/*
 * menu_statistiques
 * Affiche toutes les statistiques sur les moyennes.
 */
void menu_statistiques(TableauDyn *t)
{
    if (t->taille == 0) {
        printf("Aucun étudiant dans le système.\n");
        return;
    }

    printf("\n--- Statistiques générales ---\n");
    printf("  Nombre d'étudiants : %d\n",   t->taille);
    printf("  Moyenne générale   : %.2f\n", moyenne_generale_dyn(t));
    printf("  Minimum            : %.2f\n", minimum_dyn(t));
    printf("  Maximum            : %.2f\n", maximum_dyn(t));
    printf("  Médiane            : %.2f\n", mediane_dyn(t));
    printf("  Écart-type         : %.2f\n", ecart_type_dyn(t));
}

/*
 * menu_sauvegarder
 * Demande un nom de fichier et sérialise le tableau.
 */
void menu_sauvegarder(TableauDyn *t)
{
    char nom_fichier[100];
    printf("\n--- Sauvegarde ---\n");
    printf("Nom du fichier : ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    serialiser_dyn(t, nom_fichier);
}

/*
 * menu_charger
 * Demande un nom de fichier et charge les données.
 * Remplace le tableau actuel.
 */
TableauDyn* menu_charger(TableauDyn *t)
{
    char nom_fichier[100];
    printf("\n--- Chargement ---\n");
    printf("Nom du fichier : ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    TableauDyn *nouveau = deserialiser_dyn(nom_fichier);
    if (nouveau != NULL) {
        liberer_tableau_dyn(t); /* libérer l'ancien tableau */
        return nouveau;
    }

    printf("Erreur : chargement échoué, données actuelles conservées.\n");
    return t;
}

/* ============================================================
   PROGRAMME PRINCIPAL
   ============================================================ */
int main(void)
{
    /* Créer le tableau dynamique vide */
    TableauDyn *t = creer_tableau_dyn();
    if (t == NULL) {
        fprintf(stderr, "Erreur critique : impossible d'initialiser\n");
        return 1;
    }

    int choix = -1;

    /* Boucle principale du menu */
    while (choix != 0) {
        afficher_menu();
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {
            case 1:
                saisir_etudiant(t);
                break;
            case 2:
                printf("\n--- Liste des étudiants ---\n");
                afficher_tableau_dyn(t);
                break;
            case 3:
                menu_recherche_matricule(t);
                break;
            case 4:
                menu_recherche_intervalle(t);
                break;
            case 5:
                menu_recherche_prefixe(t);
                break;
            case 6:
                menu_modifier_moyenne(t);
                break;
            case 7:
                menu_supprimer(t);
                break;
            case 8:
                tri_insertion_dyn(t);
                afficher_tableau_dyn(t);
                break;
            case 9:
                if (t->taille > 1)
                    tri_rapide_dyn(t, 0, t->taille - 1);
                afficher_tableau_dyn(t);
                break;
            case 10:
                menu_statistiques(t);
                break;
            case 11:
                menu_sauvegarder(t);
                break;
            case 12:
                t = menu_charger(t);
                break;
            case 0:
                printf("\nAu revoir !\n");
                break;
            default:
                printf("Choix invalide. Entrez un nombre entre 0 et 12.\n");
                break;
        }
    }

    /* Libérer toute la mémoire avant de quitter */
    liberer_tableau_dyn(t);
    return 0;
}