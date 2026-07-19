#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <wchar.h>

#include "../include/tableau_statique.h"
#include "../include/tableau_dynamique.h"
#include "../include/liste_chainee.h"
#include "generateur_donnee.h"

#define NB_REPETITIONS  10
#define NB_TAILLES       4
#define NB_DISTRIBUTIONS 3

/* ============================================================
   COULEURS CONSOLE (Windows)
   ============================================================ */
#define COULEUR_DEFAUT   7   /* gris clair (defaut) */
#define COULEUR_ENTETE   11  /* cyan clair          */
#define COULEUR_TITRE    14  /* jaune                */
#define COULEUR_MENU     10  /* vert clair           */
#define COULEUR_PROMPT   13  /* magenta clair        */
#define COULEUR_SUCCES   10  /* vert clair           */
#define COULEUR_ERREUR   12  /* rouge clair          */
#define COULEUR_DONNEE   15  /* blanc vif            */

void set_couleur(int couleur)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)couleur);
}

void reset_couleur(void)
{
    set_couleur(COULEUR_DEFAUT);
}

/* Agrandit la police de la console Windows pour une meilleure lisibilite
   des menus et sous-menus. A appeler une seule fois, au debut de main(). */
void agrandir_police_console(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize       = sizeof(cfi);
    cfi.nFont        = 0;
    cfi.dwFontSize.X = 0;      /* largeur automatique */
    cfi.dwFontSize.Y = 20;     /* hauteur de police agrandie (par defaut ~16) */
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

/* ============================================================
   ANIMATIONS CONSOLE
   ============================================================ */
void animer_texte(const char *texte, int delai_ms)
{
    for (int i = 0; texte[i] != '\0'; i++) {
        putchar(texte[i]);
        fflush(stdout);
        Sleep(delai_ms);
    }
}

void animer_chargement(const char *message, int duree_totale_ms)
{
    int largeur        = 30;
    int etapes          = 30;
    int delai_par_etape = duree_totale_ms / etapes;

    set_couleur(COULEUR_PROMPT);
    printf("%s\n", message);
    reset_couleur();

    printf("[");
    for (int i = 0; i < largeur; i++) printf(" ");
    printf("]");
    printf("\r[");

    set_couleur(COULEUR_SUCCES);
    for (int i = 0; i < etapes; i++) {
        printf("#");
        fflush(stdout);
        Sleep(delai_par_etape);
    }
    reset_couleur();
    printf("] 100%%\n");
}

/* ============================================================
   UTILITAIRES
   ============================================================ */
void nettoyerEcran(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void vider_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lire_choix_valide(int min, int max)
{
    char ligne[128];
    int  choix;
    int  valide = 0;

    do {
        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        if (sscanf(ligne, "%d", &choix) == 1 && choix >= min && choix <= max) {
            valide = 1;
        } else {
            set_couleur(COULEUR_ERREUR);
            printf("Choix invalide. Entrez un nombre entre %d et %d : ", min, max);
            reset_couleur();
        }
    } while (!valide);

    return choix;
}

char* lire_chaine_dynamique(void)
{
    char buffer[256];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    char *resultat = malloc(strlen(buffer) + 1);
    strcpy(resultat, buffer);
    return resultat;
}

/* ============================================================
   EN-TETE
   ============================================================ */
void afficherHeader(void)
{
    set_couleur(COULEUR_ENTETE);
    printf("################################################################\n");
    printf("               UNIVERSITE IBA DER THIAM DE THIES                \n");
    printf("                  UFR Sciences et Technologies                  \n");
    printf("            Departement de Mathematiques et Informatique        \n");
    printf("################################################################\n\n");
    set_couleur(COULEUR_TITRE);
    printf("                  Systeme de Gestion de Scolarite               \n\n");
    reset_couleur();
    printf("      Conception, Implementation et Evaluation Comparative      \n");
    printf("                  de Structures de Donnees en C                 \n\n");
    set_couleur(COULEUR_ENTETE);
    printf("##############################################################\n\n");
    reset_couleur();
    printf("     Projet  Algorithmique et Structures de Donnees             \n");
    printf("     Filiere : Licence 2 Mathematiques-Informatique (LMI 2)     \n");
    printf("     Annee universitaire : 2025-2026                            \n\n");
    printf("     Binome                               Enseignant :         \n");
    printf("     AIDA SENE                    Dr Abdoulaye DIALLO          \n");
    printf("     COURA NDIONE                                              \n\n");
    printf("     Rendu: JUILLET 2026                                       \n");
    set_couleur(COULEUR_ENTETE);
    printf("##############################################################\n\n");
    reset_couleur();
}

/* ============================================================
   MENU PRINCIPAL
   ============================================================ */
void afficher_menu_principal(void)
{
    set_couleur(COULEUR_TITRE);
    printf("\n===================================================\n");
    printf("                  MENU PRINCIPAL                    \n");
    printf("===================================================\n");
    reset_couleur();

    set_couleur(COULEUR_MENU);
    printf("  1. TABLEAU STATIQUE\n");
    printf("  2. TABLEAU DYNAMIQUE\n");
    printf("  3. LISTE CHAINEE\n");
    printf("  4. BENCHMARK\n");
    printf("  0. QUITTER\n");
    reset_couleur();

    set_couleur(COULEUR_TITRE);
    printf("===================================================\n");
    reset_couleur();

    set_couleur(COULEUR_PROMPT);
    printf("Votre choix : ");
    reset_couleur();
}

void afficher_sous_menu(const char *nom_structure)
{
    set_couleur(COULEUR_TITRE);
    printf("\n===================================================\n");
    printf("   %s\n", nom_structure);
    printf("===================================================\n");
    reset_couleur();

    set_couleur(COULEUR_MENU);
    printf("\n");
    printf("   1.  INSERER\n");
    printf("   2.  AFFICHER UN ETUDIANT\n");
    printf("   3.  AFFICHER TOUT\n");
    printf("   4.  RECHERCHER PAR MATRICULE\n");
    printf("   5.  RECHERCHER PAR INTERVALLE\n");
    printf("   6.  RECHERCHER PAR PREFIXE\n");
    printf("   7.  SUPPRIMER\n");
    printf("   8.  MISE A JOUR\n");
    printf("   9.  TRIER PAR INSERTION\n");
    printf("   10. TRIER PAR RAPIDE\n");
    printf("\n");
    printf("   0.  RETOUR\n");
    printf("\n");
    reset_couleur();

    set_couleur(COULEUR_TITRE);
    printf("===================================================\n");
    reset_couleur();

    set_couleur(COULEUR_PROMPT);
    printf("Choix : ");
    reset_couleur();
}

/* ============================================================
   MENU TABLEAU STATIQUE
   ============================================================ */
void menu_tableau_statique(void)
{
    static Etudiant tab[100000];
    static int n = 0;
    int i;
    int choix = -1;

    while (choix != 0) {
        afficher_sous_menu("TABLEAU STATIQUE");
        choix = lire_choix_valide(0, 10);
        nettoyerEcran();

        switch (choix) {

            /* --- 1. INSERER (plusieurs etudiants) --- */
            case 1: {
                printf("--- Insertion des etudiants ---\n");
                int nbE;
                printf("  Combien d'etudiants a ajouter ? ");
                scanf("%d", &nbE);
                while (getchar() != '\n');

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);
                    Etudiant e;

                    printf("Matricule : ");
                    scanf("%d", &e.matricule);
                    vider_buffer();

                    printf("Nom : ");
                    fgets(e.nom, sizeof(e.nom), stdin);
                    e.nom[strcspn(e.nom, "\n")] = '\0';

                    printf("Moyenne : ");
                    scanf("%f", &e.moyenne);
                    vider_buffer();

                    printf("Jour Mois Annee : ");
                    scanf("%d %d %d", &e.dateNaissance.jour,
                          &e.dateNaissance.mois, &e.dateNaissance.annee);
                    vider_buffer();

                    if (insertionTableau(tab, &n, e)) {
                        set_couleur(COULEUR_SUCCES);
                        printf("Etudiant ajoute.\n");
                        reset_couleur();
                    } else {
                        set_couleur(COULEUR_ERREUR);
                        printf("Echec de l'insertion (tableau plein ?).\n");
                        reset_couleur();
                    }
                }   /* ferme le for : tous les etudiants sont inseres */

                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }   /* ferme le case 1 (accolade qui manquait avant) */

            /* --- 2. AFFICHER UN ETUDIANT (par index) --- */
            case 2: {
                if (n == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                    break;
                }

                int id;
                printf("Index de l'etudiant (0 a %d) : ", n - 1);
                scanf("%d", &id);
                vider_buffer();
                nettoyerEcran();

                if (id >= 0 && id < n) {
                    set_couleur(COULEUR_DONNEE);
                    printf("[%d] matricule:%d dateNaissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           id, tab[id].matricule, tab[id].dateNaissance.jour, tab[id].dateNaissance.mois,
                           tab[id].dateNaissance.annee, tab[id].nom, tab[id].prenom, tab[id].moyenne);
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Index invalide.\n");
                    reset_couleur();
                }

                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 3. AFFICHER TOUT --- */
            case 3: {
                if (n == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                } else {
                    printf("Affichage de tous les etudiants (%d au total) :\n", n);
                    for (i = 0; i < n; i++)
                        printf("  [%d] matricule:%d dateNaissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                               i, tab[i].matricule,
                               tab[i].dateNaissance.jour, tab[i].dateNaissance.mois,
                               tab[i].dateNaissance.annee,
                               tab[i].nom, tab[i].prenom, tab[i].moyenne);
                }

                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 4. RECHERCHER UN ETUDIANT (par matricule) --- */
            case 4: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();

                int pos = RechercheCleprimaire(tab, n, matricule);
                if (pos > -1) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Trouve : %s -- %.2f\n", tab[pos].nom, tab[pos].moyenne);
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 5. RECHERCHER PAR INTERVALLE --- */
            case 5: {
                double maxmoyenne = 0, minmoyenne = 0;
                int trouve = 0;
                printf("moyenne min : ");
                scanf("%lf", &minmoyenne);
                printf("moyenne max : ");
                scanf("%lf", &maxmoyenne);
                vider_buffer();
                nettoyerEcran();

                for (i = 0; i < n; i++) {
                    if (tab[i].moyenne >= minmoyenne && tab[i].moyenne <= maxmoyenne) {
                        trouve++;
                        set_couleur(COULEUR_SUCCES);
                        printf("Trouve : %s -- %.2f\n", tab[i].nom, tab[i].moyenne);
                        reset_couleur();
                    }
                }
                if (!trouve) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                int trouve = 0;
                printf("Prefixe : ");
                scanf("%s", prefixe);
                vider_buffer();
                nettoyerEcran();

                for (i = 0; i < n; i++) {
                    if (strncmp(tab[i].nom, prefixe, strlen(prefixe)) == 0) {
                        trouve++;
                        set_couleur(COULEUR_SUCCES);
                        printf("Trouve : %s %s -- matricule:%d -- moyenne:%.2f\n",
                               tab[i].prenom, tab[i].nom, tab[i].matricule, tab[i].moyenne);
                        reset_couleur();
                    }
                }
                if (!trouve) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                suppressionCleprimaire(tab, &n, matricule);
                break;
            }

            /* --- 8. MISE A JOUR --- */
            case 8: {
                int matricule, pos = -1, sousChoix;
                printf("Matricule de l'etudiant a mettre a jour : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();

                for (i = 0; i < n; i++) {
                    if (tab[i].matricule == matricule) { pos = i; break; }
                }
                if (pos == -1) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Etudiant non trouve.\n");
                    reset_couleur();
                    break;
                }

                printf("Etudiant trouve : %s %s -- %.2f\n",
                       tab[pos].nom, tab[pos].prenom, tab[pos].moyenne);
                printf("1. Nom  2. Prenom  3. Moyenne  4. Tout mettre a jour\n");
                printf("Choix : ");
                sousChoix = lire_choix_valide(1, 4);
                nettoyerEcran();

                if (sousChoix == 1 || sousChoix == 4) {
                    printf("Nouveau nom : ");
                    fgets(tab[pos].nom, sizeof(tab[pos].nom), stdin);
                    tab[pos].nom[strcspn(tab[pos].nom, "\n")] = '\0';
                }
                if (sousChoix == 2 || sousChoix == 4) {
                    printf("Nouveau prenom : ");
                    free(tab[pos].prenom);
                    tab[pos].prenom = lire_chaine_dynamique();
                }
                if (sousChoix == 3 || sousChoix == 4) {
                    printf("Nouvelle moyenne : ");
                    scanf("%f", &tab[pos].moyenne);
                    vider_buffer();
                }

                set_couleur(COULEUR_SUCCES);
                printf("Etudiant mis a jour avec succes !\n");
                reset_couleur();
                break;
            }

            /* --- 9. TRIER PAR INSERTION --- */
            case 9: {
                Tri_insertion_moyenne(tab, n);
                set_couleur(COULEUR_SUCCES);
                printf("Tri par insertion effectue (par moyenne).\n");
                reset_couleur();
                for (i = 0; i < n; i++)
                    printf("  [%d] matricule:%d | %s | moyenne:%.2f\n",
                           i, tab[i].matricule, tab[i].nom, tab[i].moyenne);
                break;
            }

            /* --- 10. TRIER PAR RAPIDE --- */
            case 10: {
                if (n > 1) TriRapide(tab, 0, n - 1);
                set_couleur(COULEUR_SUCCES);
                printf("Tri rapide effectue (par moyenne).\n");
                reset_couleur();
                for (i = 0; i < n; i++)
                    printf("  [%d] matricule:%d | %s | moyenne:%.2f\n",
                           i, tab[i].matricule, tab[i].nom, tab[i].moyenne);
                break;
            }

            case 0:
                break;
        }
    }
}

/* ============================================================
   MENU TABLEAU DYNAMIQUE
   ============================================================ */
void menu_tableau_dynamique(void)
{
    TableauDyn *t = creer_tableau_dyn();
    int choix = -1;

    while (choix != 0) {
        afficher_sous_menu("TABLEAU DYNAMIQUE");
        choix = lire_choix_valide(0, 10);
        nettoyerEcran();

        switch (choix) {

            /* --- 1. INSERER (plusieurs etudiants) --- */
            case 1: {
                int nbE;
                printf("  Combien d'etudiants a ajouter ? ");
                scanf("%d", &nbE);
                while (getchar() != '\n');

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);

                    int matricule;
                    float moyenne;
                    char nom[100], prenom[100];
                    Date date;

                    printf("Matricule : ");
                    scanf("%d", &matricule);
                    vider_buffer();

                    printf("Nom : ");
                    fgets(nom, sizeof(nom), stdin);
                    nom[strcspn(nom, "\n")] = '\0';

                    printf("Prenom : ");
                    fgets(prenom, sizeof(prenom), stdin);
                    prenom[strcspn(prenom, "\n")] = '\0';

                    printf("Moyenne : ");
                    scanf("%f", &moyenne);
                    vider_buffer();

                    printf("Jour Mois Annee : ");
                    scanf("%d %d %d", &date.jour, &date.mois, &date.annee);
                    vider_buffer();

                    Etudiant *e = creer_etudiant(matricule, moyenne, nom, prenom, date);
                    inserer_etudiant_dyn(t, e);
                }

                nettoyerEcran();
                set_couleur(COULEUR_SUCCES);
                printf("%d etudiant(s) ajoute(s).\n", nbE);
                reset_couleur();
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 2. AFFICHER UN ETUDIANT (par index) --- */
            case 2: {
                if (t->taille == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                    break;
                }

                int id;
                printf("Index de l'etudiant (0 a %d) : ", t->taille - 1);
                scanf("%d", &id);
                vider_buffer();
                nettoyerEcran();

                if (id >= 0 && id < t->taille) {
                    Etudiant *e = t->tab[id];
                    set_couleur(COULEUR_DONNEE);
                    printf("[%d] matricule:%d dateNaissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           id, e->matricule, e->dateNaissance.jour, e->dateNaissance.mois,
                           e->dateNaissance.annee, e->nom, e->prenom, e->moyenne);
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Index invalide.\n");
                    reset_couleur();
                }

                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 3. AFFICHER TOUT --- */
            case 3: {
                if (t->taille == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                } else {
                    afficher_tableau_dyn(t);
                }
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 4. RECHERCHER UN ETUDIANT --- */
            case 4: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();

                Etudiant *e = rechercher_par_matricule_dyn(t, matricule);
                if (e) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Trouve : %s %s -- %.2f\n", e->nom, e->prenom, e->moyenne);
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 5. RECHERCHER PAR INTERVALLE --- */
            case 5: {
                float min, max;
                printf("Min : "); scanf("%f", &min);
                printf("Max : "); scanf("%f", &max);
                vider_buffer();
                nettoyerEcran();
                rechercher_par_intervalle_dyn(t, min, max);
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                printf("Prefixe : ");
                scanf("%s", prefixe);
                vider_buffer();
                nettoyerEcran();
                rechercher_par_prefixe_dyn(t, prefixe);
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                supprimer_par_matricule_dyn(t, matricule);
                break;
            }

            /* --- 8. MISE A JOUR (moyenne) --- */
            case 8: {
                int matricule;
                float nouvelle_moyenne;
                printf("Matricule : "); scanf("%d", &matricule);
                printf("Nouvelle moyenne : "); scanf("%f", &nouvelle_moyenne);
                vider_buffer();
                nettoyerEcran();
                modifier_moyenne_dyn(t, matricule, nouvelle_moyenne);
                set_couleur(COULEUR_SUCCES);
                printf("Mise a jour effectuee.\n");
                reset_couleur();
                break;
            }

            /* --- 9. TRIER PAR INSERTION --- */
            case 9:
                tri_insertion_dyn(t);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par insertion.\n");
                reset_couleur();
                break;

            /* --- 10. TRIER PAR RAPIDE --- */
            case 10:
                if (t->taille > 1) tri_rapide_dyn(t, 0, t->taille - 1);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par tri rapide.\n");
                reset_couleur();
                break;

            case 0:
                break;
        }
    }

    liberer_tableau_dyn(t);
}

/* ============================================================
   MENU LISTE CHAINEE
   ============================================================ */
void menu_liste_chainee(void)
{
    ListeC *l = liste_creer();
    int choix = -1;

    while (choix != 0) {
        afficher_sous_menu("LISTE CHAINEE");
        choix = lire_choix_valide(0, 10);
        nettoyerEcran();

        switch (choix) {

            /* --- 1. INSERER (plusieurs etudiants) --- */
            case 1: {
                int nbE;
                printf("  Combien d'etudiants a ajouter ? ");
                scanf("%d", &nbE);
                while (getchar() != '\n');

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);

                    int matricule;
                    float moyenne;
                    char nom[100], prenom[100];
                    Date date;

                    printf("Matricule : ");
                    scanf("%d", &matricule);
                    vider_buffer();

                    printf("Nom : ");
                    fgets(nom, sizeof(nom), stdin);
                    nom[strcspn(nom, "\n")] = '\0';

                    printf("Prenom : ");
                    fgets(prenom, sizeof(prenom), stdin);
                    prenom[strcspn(prenom, "\n")] = '\0';

                    printf("Moyenne : ");
                    scanf("%f", &moyenne);
                    vider_buffer();

                    printf("Jour Mois Annee : ");
                    scanf("%d %d %d", &date.jour, &date.mois, &date.annee);
                    vider_buffer();

                    Etudiant *e = creer_etudiant(matricule, moyenne, nom, prenom, date);
                    inserer_en_queue(l, e);
                }

                nettoyerEcran();
                set_couleur(COULEUR_SUCCES);
                printf("%d etudiant(s) ajoute(s).\n", nbE);
                reset_couleur();
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 2. AFFICHER UN ETUDIANT (par index) --- */
            case 2: {
                if (l->taille == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                    break;
                }

                int id;
                printf("Index de l'etudiant (0 a %d) : ", l->taille - 1);
                scanf("%d", &id);
                vider_buffer();
                nettoyerEcran();

                if (id < 0 || id >= l->taille) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Index invalide.\n");
                    reset_couleur();
                } else {
                    Noeud *courant = l->tete;
                    int k;
                    for (k = 0; k < id; k++) {
                        courant = courant->suivant;
                    }
                    Etudiant *e = courant->suivant->etudiant;
                    set_couleur(COULEUR_DONNEE);
                    printf("[%d] matricule:%d dateNaissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           id, e->matricule, e->dateNaissance.jour, e->dateNaissance.mois,
                           e->dateNaissance.annee, e->nom, e->prenom, e->moyenne);
                    reset_couleur();
                }

                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 3. AFFICHER TOUT --- */
            case 3: {
                if (l->taille == 0) {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant enregistre pour le moment.\n");
                    reset_couleur();
                } else {
                    afficher_liste(l);
                }
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 4. RECHERCHER UN ETUDIANT --- */
            case 4: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();

                Etudiant *e = rechercher_matricule_liste(l, matricule);
                if (e) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Trouve : %s %s -- %.2f\n", e->nom, e->prenom, e->moyenne);
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 5. RECHERCHER PAR INTERVALLE --- */
            case 5: {
                float min, max;
                printf("Min : "); scanf("%f", &min);
                printf("Max : "); scanf("%f", &max);
                vider_buffer();
                nettoyerEcran();
                /* NOTE : verifie que cette fonction existe bien dans ton
                   liste_chainee.h actuel (nom exact a confirmer). */
                rechercher_intervalle_liste(l, min, max);
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                printf("Prefixe : ");
                scanf("%s", prefixe);
                vider_buffer();
                nettoyerEcran();
                /* NOTE : verifie que cette fonction existe bien dans ton
                   liste_chainee.h actuel (nom exact a confirmer). */
                rechercher_liste_prefixes(l, prefixe);
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                supprimer_liste_matricule(l, matricule);
                break;
            }

            /* --- 8. MISE A JOUR (moyenne) --- */
            case 8: {
                int matricule;
                float nouvelle_moyenne;
                printf("Matricule : "); scanf("%d", &matricule);
                printf("Nouvelle moyenne : "); scanf("%f", &nouvelle_moyenne);
                vider_buffer();
                nettoyerEcran();

                Etudiant *e = rechercher_matricule_liste(l, matricule);
                if (e) {
                    e->moyenne = nouvelle_moyenne;
                    set_couleur(COULEUR_SUCCES);
                    printf("Mise a jour effectuee.\n");
                    reset_couleur();
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Etudiant non trouve.\n");
                    reset_couleur();
                }
                break;
            }

            /* --- 9. TRIER PAR INSERTION --- */
            case 9:
                liste_tri_insertion(l);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par insertion.\n");
                reset_couleur();
                break;

            /* --- 10. TRIER PAR RAPIDE --- */
            case 10:
                set_couleur(COULEUR_ERREUR);
                printf("Tri rapide non disponible pour la liste chainee.\n");
                printf("(structure sequentielle : utilisez le tri par insertion)\n");
                reset_couleur();
                break;

            case 0:
                break;
        }
    }

    liste_liberer(l);
}

/* ============================================================
   BENCHMARK
   ============================================================ */
void lancer_benchmark(void)
{
    set_couleur(COULEUR_TITRE);
    printf("\n========== BENCHMARK ==========\n");
    reset_couleur();
    printf("Cette fonctionnalite necessite l'executable 'bench'.\n");
    printf("Compilez avec : make main_bench  puis lancez : ./bench\n");
}

/* ============================================================
   PROGRAMME PRINCIPAL
   ============================================================ */
int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    agrandir_police_console();

    nettoyerEcran();
    afficherHeader();

    printf("\n");
    animer_texte("Bienvenue AIDA dans le Systeme de Gestion de Scolarite...\n", 25);
    animer_chargement("Initialisation du systeme", 1500);
    printf("\n");

    set_couleur(COULEUR_PROMPT);
    printf("Appuyez sur Entree pour initialiser le systeme...");
    reset_couleur();
    getchar();
    nettoyerEcran();

    int choix = -1;

    while (choix != 0) {
        afficher_menu_principal();
        choix = lire_choix_valide(0, 4);
        nettoyerEcran();

        switch (choix) {
            case 1: menu_tableau_statique();  break;
            case 2: menu_tableau_dynamique(); break;
            case 3: menu_liste_chainee();     break;
            case 4: lancer_benchmark();       break;
            case 0:
                set_couleur(COULEUR_SUCCES);
                printf("\nAu revoir AIDA !\n");
                reset_couleur();
                break;
        }
    }

    return 0;
}