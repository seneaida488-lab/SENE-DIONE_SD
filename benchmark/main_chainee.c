#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/liste_chainee.h"

void vider_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void afficher_menu(void)
{
    printf("\n============================================\n");
    printf("   SYSTEME DE GESTION DE SCOLARITÉ\n");
    printf("   Structure : Liste Doublement Chainée\n");
    printf("============================================\n");
    printf("  1.  Ajouter un etudiant en tete\n");
    printf("  2.  Ajouter un etudiant en queue\n");
    printf("  3.  Afficher la liste (debut -> fin)\n");
    printf("  4.  Afficher la liste (fin -> debut)\n");
    printf("  5.  Rechercher par matricule\n");
    printf("  6.  Rechercher par intervalle de moyenne\n");
    printf("  7.  Rechercher par prefixe du nom\n");
    printf("  8.  Modifier la moyenne d'un etudiant\n");
    printf("  9.  Supprimer un etudiant\n");
    printf("  10. Trier par moyenne (tri insertion)\n");
    printf("  11. Trier par nom (tri bulles)\n");
    printf("  12. Statistiques generales\n");
    printf("  13. Sauvegarder dans un fichier\n");
    printf("  14. Charger depuis un fichier\n");
    printf("  0.  Quitter\n");
    printf("============================================\n");
    printf("Votre choix : ");
}

Etudiant* saisir_etudiant(ListeDC *l)
{
    int   matricule;
    float moyenne;
    char  nom[40];
    char  prenom[100];
    Date  date;

    printf("\n--- Saisie d'un etudiant ---\n");

    printf("Matricule    : ");
    scanf("%d", &matricule);
    vider_buffer();

    /* Vérifier que le matricule n'existe pas dèjas */
    if (rechercher_matricule_liste(l, matricule) != NULL) {
        printf("Erreur : le matricule %d existe deja.\n", matricule);
        return NULL;
    }

    printf("Nom          : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0';

    printf("Prenom       : ");
    fgets(prenom, sizeof(prenom), stdin);
    prenom[strcspn(prenom, "\n")] = '\0';

    printf("Moyenne      : ");
    scanf("%f", &moyenne);
    vider_buffer();

    if (moyenne < 0.0f || moyenne > 20.0f) {
        printf("Erreur : la moyenne doit etre entre 0 et 20.\n");
        return NULL;
    }

    printf("Date de naissance\n");
    printf("  Jour  : "); scanf("%d", &date.jour);
    printf("  Mois  : "); scanf("%d", &date.mois);
    printf("  Annee : "); scanf("%d", &date.annee);
    vider_buffer();

    Etudiant *e = creer_etudiant(matricule, moyenne, nom, prenom, date);
    if (e == NULL) {
        printf("Erreur : impossible de creer l'etudiant.\n");
        return NULL;
    }

    return e;
}

void menu_recherche_matricule(ListeDC *l)
{
    int matricule;
    printf("\n--- Recherche par matricule ---\n");
    printf("Matricule : ");
    scanf("%d", &matricule);
    vider_buffer();

    Etudiant *e = rechercher_matricule_liste(l, matricule);
    if (e != NULL) {
        printf("\nEtudiant trouve :\n");
        printf("  Matricule  : %d\n",   e->matricule);
        printf("  Nom        : %s\n",   e->nom);
        printf("  Prenom     : %s\n",   e->prenom);
        printf("  Moyenne    : %.2f\n", e->moyenne);
        printf("  Naissance  : %02d/%02d/%04d\n",
               e->dateNaissance.jour,
               e->dateNaissance.mois,
               e->dateNaissance.annee);
    } else {
        printf("Aucun etudiant avec le matricule %d.\n", matricule);
    }
}

void menu_recherche_intervalle(ListeDC *l)
{
    float min, max;
    printf("\n--- Recherche par intervalle de moyenne ---\n");
    printf("Moyenne minimale : ");
    scanf("%f", &min);
    printf("Moyenne maximale : ");
    scanf("%f", &max);
    vider_buffer();

    rechercher_intervalle_liste(l, min, max);
}

void menu_recherche_prefixe(ListeDC *l)
{
    char prefixe[40];
    printf("\n--- Recherche par prefixe du nom ---\n");
    printf("Prefixe : ");
    fgets(prefixe, sizeof(prefixe), stdin);
    prefixe[strcspn(prefixe, "\n")] = '\0';

    rechercher_prefixe_liste(l, prefixe);
}

void menu_modifier_moyenne(ListeDC *l)
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
        printf("Erreur : la moyenne doit etre entre 0 et 20.\n");
        return;
    }

    modifier_moyenne_liste(l, matricule, nouvelle_moyenne);
}

void menu_supprimer(ListeDC *l)
{
    int matricule;
    printf("\n--- Suppression d'un etudiant ---\n");
    printf("Matricule : ");
    scanf("%d", &matricule);
    vider_buffer();

    supprimer_matricule_liste(l, matricule);
}

void menu_statistiques(ListeDC *l)
{
    if (l->taille == 0) {
        printf("Aucun etudiant dans la liste.\n");
        return;
    }

    printf("\n--- Statistiques generales ---\n");
    printf("  Nombre d'etudiants : %d\n",   l->taille);
    printf("  Moyenne generale   : %.2f\n", moyenne_generale_liste(l));
    printf("  Minimum            : %.2f\n", minimum_liste(l));
    printf("  Maximum            : %.2f\n", maximum_liste(l));
    printf("  Mediane            : %.2f\n", mediane_liste(l));
    printf("  Ecart-type         : %.2f\n", ecart_type_liste(l));
}

void menu_sauvegarder(ListeDC *l)
{
    char nom_fichier[100];
    printf("\n--- Sauvegarde ---\n");
    printf("Nom du fichier : ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    serialiser_liste(l, nom_fichier);
}

ListeDC* menu_charger(ListeDC *l)
{
    char nom_fichier[100];
    printf("\n--- Chargement ---\n");
    printf("Nom du fichier : ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    ListeDC *nouvelle = deserialiser_liste(nom_fichier);
    if (nouvelle != NULL) {
        liberer_liste(l);
        return nouvelle;
    }

    printf("Erreur : chargement echoue, donnees actuelles conservees.\n");
    return l;
}

int main(void)
{
    /* Créer la liste doublement chainee vide */
    ListeDC *l = creer_liste();
    if (l == NULL) {
        fprintf(stderr, "Erreur critique : impossible d'initialiser\n");
        return 1;
    }

    int choix = -1;

    while (choix != 0) {
        afficher_menu();
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {

            case 1: {
                Etudiant *e = saisir_etudiant(l);
                if (e != NULL) {
                    inserer_en_tete(l, e);
                    printf("Etudiant ajoute en tete.\n");
                }
                break;
            }

            case 2: {
                Etudiant *e = saisir_etudiant(l);
                if (e != NULL) {
                    inserer_en_queue(l, e);
                    printf("Etudiant ajoute en queue.\n");
                }
                break;
            }

            case 3:
                printf("\n--- Liste debut -> fin ---\n");
                afficher_liste(l);
                break;

            case 4:
                printf("\n--- Liste fin -> debut ---\n");
                afficher_liste_inverse(l);
                break;

            case 5:
                menu_recherche_matricule(l);
                break;

            case 6:
                menu_recherche_intervalle(l);
                break;

            case 7:
                menu_recherche_prefixe(l);
                break;

            case 8:
                menu_modifier_moyenne(l);
                break;

            case 9:
                menu_supprimer(l);
                break;

            case 10:
                tri_insertion_liste(l);
                afficher_liste(l);
                break;

            case 11:
                tri_bulles_liste(l);
                afficher_liste(l);
                break;

            case 12:
                menu_statistiques(l);
                break;

            case 13:
                menu_sauvegarder(l);
                break;

            case 14:
                l = menu_charger(l);
                break;

            case 0:
                printf("\nAu revoir !\n");
                break;

            default:
                printf("Choix invalide. Entrez un nombre entre 0 et 14.\n");
                break;
        }
    }

    /* Libérer toute la mémoire */
    liberer_liste(l);
    return 0;
}