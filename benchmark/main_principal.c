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
#include "../include/structure.h"
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

/* Lit un entier valide (>= min) depuis stdin, en relisant tant que la saisie
   n'est pas un nombre correct. Contrairement a un simple scanf("%d", &x),
   cette fonction :
     - ne laisse jamais la variable de sortie non initialisee en cas de
       saisie invalide (ex: lettres) ;
     - vide systematiquement le reste de la ligne, evitant les decalages
       de lecture sur les scanf suivants ;
     - protege les boucles for(k=0;k<nbE;k++) contre un nbE indetermine
       issu d'une saisie invalide (source de comportement indefini). */
int lire_entier_valide(const char *prompt, int min)
{
    char ligne[128];
    int  valeur;
    int  valide = 0;

    do {
        if (prompt) printf("%s", prompt);
        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        if (sscanf(ligne, "%d", &valeur) == 1 && valeur >= min) {
            valide = 1;
        } else {
            set_couleur(COULEUR_ERREUR);
            printf("Saisie invalide. Entrez un nombre entier (>= %d) : ", min);
            reset_couleur();
        }
    } while (!valide);

    return valeur;
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
    printf("#               UNIVERSITE IBA DER THIAM DE THIES               #\n");
    printf("#                  UFR Sciences et Technologies                 #\n");
    printf("#            Departement de Mathematiques et Informatique       #\n");
    printf("################################################################\n\n");
    set_couleur(COULEUR_TITRE);
    printf("      S Y S T E M E   D E    G E S T I O N    D E  S C O L A R I T E\n\n");
    reset_couleur();
    printf("     [ Conception, Implementation et Evaluation Comparative ]\n");
    printf("     [             de Structures de Donnees en C            ]\n\n");
    set_couleur(COULEUR_ENTETE);
    printf("##############################################################\n\n");
    reset_couleur();
    printf("    ¤ Projet              : Algorithmique et Structures de Donnees   \n");
    printf("    ¤ Filiere / Niveau    : Licence 2 Mathematiques-Informatique (LMI 2)\n");
    printf("    ¤ Annee universitaire : 2025-2026                            \n\n");
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
    printf("\n");
    printf("   ███╗   ███╗███████╗███╗   ██╗██╗   ██╗    ██████╗ ██████╗ ██╗███╗   ██╗ ██████╗██╗██████╗  ██╗\n");
    printf("   ████╗ ████║██╔════╝████╗  ██║██║   ██║    ██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██║██╔══██╗ ██║\n");
    printf("   ██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║    ██████╔╝██████╔╝██║██╔██╗ ██║██║     ██║██████╔╝ ██║\n");
    printf("   ██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║    ██╔═══╝ ██╔══██╗██║██║╚██╗██║██║     ██║██╔═══╝  ╚═╝\n");
    printf("   ██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝    ██║     ██║  ██║██║██║ ╚████║╚██████╗██║██║      ██╗\n");
    printf("   ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝     ╚═╝     ╚═╝  ╚═══╝╚═╝╚═╝  ╚═══╝ ╚═════╝╚═╝╚═╝      ╚═╝\n");
    printf("   ============================================================================================\n\n\n");
    reset_couleur();

    set_couleur(COULEUR_MENU);
    // Les \t permettent de pousser le texte vers la droite pour centrer le menu dans le noir
    printf("\t\t\t╔══════════════════════════════════════════╗\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t║    [1] ➔  TABLEAU STATIQUE               ║\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t║    [2] ➔  TABLEAU DYNAMIQUE              ║\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t║    [3] ➔  LISTE CHAINEE                  ║\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t║    [4] ➔  BENCHMARK                      ║\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t║    [0] ➔  QUITTER LE PROGRAMME           ║\n");
    printf("\t\t\t║                                          ║\n");
    printf("\t\t\t╚══════════════════════════════════════════╝\n\n\n");
    reset_couleur();

    set_couleur(COULEUR_TITRE);
    printf("   ============================================================================================\n");
    printf("\t\t\t👉 VOTRE CHOIX : ");
    reset_couleur();
}
void afficher_sous_menu(const char *nom_structure)
{
    (void)nom_structure;
    set_couleur(COULEUR_TITRE);
    printf("\n");
    printf("   ╔══════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("   ║                      📊  OPTIONS DE GESTION DU TABLEAU ET DU LISTE CHAINEE       ║\n");
    printf("   ╚══════════════════════════════════════════════════════════════════════════════════╝\n\n");
    reset_couleur();

    // 3. Corps du menu élargi et aéré verticalement (\n) et horizontalement (\t\t)
    set_couleur(COULEUR_MENU);
    printf("\t\t╔══════════════════════════════════════════════════════════╗\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 01 ] ➔  INSERER UN NOUVEL ETUDIANT                   ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 02 ] ➔  AFFICHER UN ETUDIANT                         ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 03 ] ➔  AFFICHER TOUT LE TABLEAU                     ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 04 ] ➔  RECHERCHER PAR MATRICULE                     ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 05 ] ➔  RECHERCHER PAR INTERVALLE DE NOTES           ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 06 ] ➔  RECHERCHER PAR PREFIXE DE NOM                ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 07 ] ➔  SUPPRIMER UN ENREGISTREMENT                  ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 08 ] ➔  MISE A JOUR D'UN ETUDIANT                    ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 09 ] ➔  TRIER PAR L'ALGORITHME D'INSERTION           ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 10 ] ➔  TRIER PAR L'ALGORITHME RAPIDE (QUICKSORT)    ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t╠══════════════════════════════════════════════════════════╣\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t║    [ 00 ] ➔  RETOUR AU MENU PRINCIPAL                     ║\n");
    printf("\t\t║                                                          ║\n");
    printf("\t\t╚══════════════════════════════════════════════════════════╝\n\n");
    reset_couleur();

    // 4. Ligne de fermeture inférieure et invite de commande (Prompt)
    set_couleur(COULEUR_TITRE);
    printf("   ====================================================================================\n");
    reset_couleur();

    set_couleur(COULEUR_PROMPT);
    printf("\t\t👉 ENTRER VOTRE CHOIX : ");
    reset_couleur();
}

/* ============================================================
   MENU TABLEAU STATIQUE
   ============================================================ */
void menu_tableau_statique(void)
{
    static Etudiant tab[TAILLEMAX];
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
                int nbE = lire_entier_valide("  Combien d'etudiants a ajouter ? ", 0);

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);
                    Etudiant e;
                    printf("Jour Mois Annee Inscription : ");
                    scanf("%d %d %d", &e.dateInscription.jour, &e.dateInscription.mois, &e.dateInscription.annee);
                    vider_buffer();

                    printf("Filiere : ");
                    fgets(e.filiere, sizeof(e.filiere), stdin);
                    e.filiere[strcspn(e.filiere, "\n")] = '\0';

                    printf("Niveau : ");
                    fgets(e.niveau, sizeof(e.niveau), stdin);
                    e.niveau[strcspn(e.niveau, "\n")] = '\0';

                    printf("Matiere : ");
                    fgets(e.matiere, sizeof(e.matiere), stdin);
                    e.matiere[strcspn(e.matiere, "\n")] = '\0';

                    printf("Nom : ");
                    fgets(e.nom, sizeof(e.nom), stdin);
                    e.nom[strcspn(e.nom, "\n")] = '\0';

                    /* BUG CORRIGE : le prenom n'etait jamais demande, alors
                       qu'il est affiche (case 2/3).
                       BUG CORRIGE (suite a l'unification des headers) :
                       "prenom" est maintenant un "char *" alloue
                       dynamiquement dans le Etudiant unique de structure.h
                       (et non plus un tableau char[100] fixe comme dans
                       l'ancienne definition dupliquee de ce header). On lit
                       donc la saisie dans un buffer temporaire de pile, puis
                       on alloue exactement la taille necessaire pour
                       e.prenom avant de copier -- sans quoi fgets ecrirait
                       dans un pointeur non initialise (comportement indefini,
                       plantage quasi certain). */
                    char bufPrenom[100];
                    printf("Prenom : ");
                    fgets(bufPrenom, sizeof(bufPrenom), stdin);
                    bufPrenom[strcspn(bufPrenom, "\n")] = '\0';
                    e.prenom = malloc(strlen(bufPrenom) + 1);
                    strcpy(e.prenom, bufPrenom);

                    printf("Matricule : ");
                    scanf("%d", &e.matricule);
                    vider_buffer();

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
            }   /* ferme le case 1 */

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

                char prompt[64];
                snprintf(prompt, sizeof(prompt), "Index de l'etudiant (0 a %d) : ", n - 1);
                int id = lire_entier_valide(prompt, 0);
                nettoyerEcran();

                if (id < n) {
                    set_couleur(COULEUR_DONNEE);
                    printf("[%d] matricule:%d | Inscription:%02d/%02d/%d | Filiere:%s | Niveau:%s | Matiere:%s\n",
                           id, tab[id].matricule,
                           tab[id].dateInscription.jour, tab[id].dateInscription.mois, tab[id].dateInscription.annee,
                           tab[id].filiere, tab[id].niveau, tab[id].matiere);
                    printf("     naissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           tab[id].dateNaissance.jour, tab[id].dateNaissance.mois,
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
                    for (i = 0; i < n; i++) {
                        printf("  [%d] matricule:%d | Inscription:%02d/%02d/%d | Filiere:%s | Niveau:%s | Matiere:%s\n",
                               i, tab[i].matricule,
                               tab[i].dateInscription.jour, tab[i].dateInscription.mois, tab[i].dateInscription.annee,
                               tab[i].filiere, tab[i].niveau, tab[i].matiere);
                        printf("       naissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                               tab[i].dateNaissance.jour, tab[i].dateNaissance.mois,
                               tab[i].dateNaissance.annee,
                               tab[i].nom, tab[i].prenom, tab[i].moyenne);
                    }
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                int trouve = 0;
                printf("Prefixe : ");
                scanf("%99s", prefixe);
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                int n_avant = n;
                suppressionCleprimaire(tab, &n, matricule);
                /* BUG CORRIGE : aucune confirmation ni affichage ne suivait
                   la suppression -> impossible de savoir si l'etudiant a
                   bien ete retire. On compare n avant/apres pour deduire le
                   succes (suppressionCleprimaire ne renvoie pas de code de
                   retour), et on reaffiche le tableau resultant. */
                if (n < n_avant) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Etudiant matricule %d supprime.\n", matricule);
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant avec le matricule %d.\n", matricule);
                }
                reset_couleur();
                printf("\n--- Tableau apres suppression (%d etudiant(s)) ---\n", n);
                for (i = 0; i < n; i++)
                    printf("  [%d] matricule:%d | %s %s | moyenne:%.2f\n",
                           i, tab[i].matricule, tab[i].prenom, tab[i].nom, tab[i].moyenne);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
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
                    /* BUG CORRIGE (suite a l'unification des headers) :
                       prenom est maintenant un "char *" alloue dynamiquement
                       (voir structure.h). Toute mise a jour doit donc :
                       1) lire la nouvelle valeur dans un buffer temporaire ;
                       2) liberer l'ancienne allocation pour eviter une fuite
                          memoire ;
                       3) allouer exactement la taille necessaire pour la
                          nouvelle valeur avant de copier. */
                    char bufPrenom[100];
                    printf("Nouveau prenom : ");
                    fgets(bufPrenom, sizeof(bufPrenom), stdin);
                    bufPrenom[strcspn(bufPrenom, "\n")] = '\0';
                    free(tab[pos].prenom);
                    tab[pos].prenom = malloc(strlen(bufPrenom) + 1);
                    strcpy(tab[pos].prenom, bufPrenom);
                }
                if (sousChoix == 3 || sousChoix == 4) {
                    printf("Nouvelle moyenne : ");
                    scanf("%f", &tab[pos].moyenne);
                    vider_buffer();
                }

                set_couleur(COULEUR_SUCCES);
                printf("Etudiant mis a jour avec succes !\n");
                reset_couleur();
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
    /* BUG CORRIGE : t etait local et non-static -> le tableau dynamique
       etait recree vide (creer_tableau_dyn) a CHAQUE entree dans ce sous-menu
       et entierement libere (liberer_tableau_dyn) a CHAQUE sortie. Resultat :
       tous les etudiants ajoutes etaient perdus des qu'on revenait au menu
       principal puis qu'on rentrait de nouveau dans "TABLEAU DYNAMIQUE".
       En le rendant static et en ne le creant qu'une seule fois (premier
       appel), les donnees persistent pendant toute la duree du programme,
       exactement comme pour le tableau statique. */
    static TableauDyn *t = NULL;
    if (t == NULL) t = creer_tableau_dyn();

    int choix = -1;

    while (choix != 0) {
        afficher_sous_menu("TABLEAU DYNAMIQUE");
        choix = lire_choix_valide(0, 10);
        nettoyerEcran();

        switch (choix) {

            /* --- 1. INSERER (plusieurs etudiants) --- */
            case 1: {
                int nbE = lire_entier_valide("  Combien d'etudiants a ajouter ? ", 0);

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);

                    int matricule;
                    float moyenne;
                    char nom[100], prenom[100];
                    Date date;

                    /* --- Champs ajoutes : dateInscription / filiere / niveau / matiere --- */
                    Date dateInscription;
                    char filiere[100], niveau[100], matiere[100];

                    printf("Jour Mois Annee Inscription : ");
                    scanf("%d %d %d", &dateInscription.jour, &dateInscription.mois, &dateInscription.annee);
                    vider_buffer();

                    printf("Filiere : ");
                    fgets(filiere, sizeof(filiere), stdin);
                    filiere[strcspn(filiere, "\n")] = '\0';

                    printf("Niveau : ");
                    fgets(niveau, sizeof(niveau), stdin);
                    niveau[strcspn(niveau, "\n")] = '\0';

                    printf("Matiere : ");
                    fgets(matiere, sizeof(matiere), stdin);
                    matiere[strcspn(matiere, "\n")] = '\0';

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
                    /* Champs supplementaires reportes directement sur la structure
                       (memes champs que dans Etudiant du tableau statique).
                       NOTE : necessite que ces champs existent dans la struct
                       Etudiant partagee -- adapter si filiere/niveau/matiere
                       sont des char* alloues plutot que des tableaux fixes. */
                    e->dateInscription = dateInscription;
                    strcpy(e->filiere, filiere);
                    strcpy(e->niveau, niveau);
                    strcpy(e->matiere, matiere);

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

                char prompt[64];
                snprintf(prompt, sizeof(prompt), "Index de l'etudiant (0 a %d) : ", t->taille - 1);
                int id = lire_entier_valide(prompt, 0);
                nettoyerEcran();

                if (id < t->taille) {
                    Etudiant *e = t->tab[id];
                    set_couleur(COULEUR_DONNEE);
                    printf("[%d] matricule:%d | Inscription:%02d/%02d/%d | Filiere:%s | Niveau:%s | Matiere:%s\n",
                           id, e->matricule,
                           e->dateInscription.jour, e->dateInscription.mois, e->dateInscription.annee,
                           e->filiere, e->niveau, e->matiere);
                    printf("     naissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           e->dateNaissance.jour, e->dateNaissance.mois,
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                printf("Prefixe : ");
                scanf("%99s", prefixe);
                vider_buffer();
                nettoyerEcran();
                rechercher_par_prefixe_dyn(t, prefixe);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                int taille_avant = t->taille;
                supprimer_par_matricule_dyn(t, matricule);
                /* BUG CORRIGE : meme correction que pour le tableau
                   statique -- confirmation + reaffichage apres suppression. */
                if (t->taille < taille_avant) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Etudiant matricule %d supprime.\n", matricule);
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant avec le matricule %d.\n", matricule);
                }
                reset_couleur();
                printf("\n--- Tableau dynamique apres suppression (%d etudiant(s)) ---\n", t->taille);
                afficher_tableau_dyn(t);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 9. TRIER PAR INSERTION --- */
            case 9:
                tri_insertion_dyn(t);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par insertion.\n");
                reset_couleur();
                /* BUG CORRIGE : contrairement au tableau statique, aucun
                   affichage ne suivait le tri -> impossible de verifier
                   visuellement que l'ordre avait bien change. On reaffiche
                   desormais le tableau trie. */
                afficher_tableau_dyn(t);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            /* --- 10. TRIER PAR RAPIDE --- */
            case 10:
                if (t->taille > 1) tri_rapide_dyn(t, 0, t->taille - 1);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par tri rapide.\n");
                reset_couleur();
                /* BUG CORRIGE : meme correction que pour le tri par
                   insertion ci-dessus -- on affiche le resultat. */
                afficher_tableau_dyn(t);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 0:
                break;
        }
    }

    /* BUG CORRIGE : liberer_tableau_dyn(t) a ete retire d'ici. t est
       maintenant static et doit rester vivant pour la prochaine visite
       de ce sous-menu (voir commentaire au debut de la fonction). Sa
       liberation est reportee a la fin de main(). */
}

/* ============================================================
   MENU LISTE CHAINEE
   ============================================================ */
void menu_liste_chainee(void)
{
    /* BUG CORRIGE : meme probleme que pour le tableau dynamique. l etait
       local et libere (liste_liberer) a chaque sortie de sous-menu, donc
       toute la liste etait perdue en revenant au menu principal. l est
       maintenant static et n'est cree qu'une seule fois. */
    static ListeC *l = NULL;
    if (l == NULL) l = liste_creer();

    int choix = -1;

    while (choix != 0) {
        afficher_sous_menu("LISTE CHAINEE");
        choix = lire_choix_valide(0, 10);
        nettoyerEcran();

        switch (choix) {

            /* --- 1. INSERER (plusieurs etudiants) --- */
            case 1: {
                int nbE = lire_entier_valide("  Combien d'etudiants a ajouter ? ", 0);

                for (int k = 0; k < nbE; k++) {
                    printf("\n--- Etudiant %d/%d ---\n", k + 1, nbE);

                    int matricule;
                    float moyenne;
                    char nom[100], prenom[100];
                    Date date;

                    /* --- Champs ajoutes : dateInscription / filiere / niveau / matiere --- */
                    Date dateInscription;
                    char filiere[100], niveau[100], matiere[100];

                    printf("Jour Mois Annee Inscription : ");
                    scanf("%d %d %d", &dateInscription.jour, &dateInscription.mois, &dateInscription.annee);
                    vider_buffer();

                    printf("Filiere : ");
                    fgets(filiere, sizeof(filiere), stdin);
                    filiere[strcspn(filiere, "\n")] = '\0';

                    printf("Niveau : ");
                    fgets(niveau, sizeof(niveau), stdin);
                    niveau[strcspn(niveau, "\n")] = '\0';

                    printf("Matiere : ");
                    fgets(matiere, sizeof(matiere), stdin);
                    matiere[strcspn(matiere, "\n")] = '\0';

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
                    e->dateInscription = dateInscription;
                    strcpy(e->filiere, filiere);
                    strcpy(e->niveau, niveau);
                    strcpy(e->matiere, matiere);

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

                char prompt[64];
                snprintf(prompt, sizeof(prompt), "Index de l'etudiant (0 a %d) : ", l->taille - 1);
                int id = lire_entier_valide(prompt, 0);
                nettoyerEcran();

                if (id >= l->taille) {
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
                    printf("[%d] matricule:%d | Inscription:%02d/%02d/%d | Filiere:%s | Niveau:%s | Matiere:%s\n",
                           id, e->matricule,
                           e->dateInscription.jour, e->dateInscription.mois, e->dateInscription.annee,
                           e->filiere, e->niveau, e->matiere);
                    printf("     naissance:%02d/%02d/%d | %s %s | moyenne:%.2f\n",
                           e->dateNaissance.jour, e->dateNaissance.mois,
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 6. RECHERCHER PAR PREFIXE --- */
            case 6: {
                char prefixe[100];
                printf("Prefixe : ");
                scanf("%99s", prefixe);
                vider_buffer();
                nettoyerEcran();
                /* NOTE : verifie que cette fonction existe bien dans ton
                   liste_chainee.h actuel (nom exact a confirmer). */
                rechercher_liste_prefixes(l, prefixe);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 7. SUPPRIMER --- */
            case 7: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule);
                vider_buffer();
                nettoyerEcran();
                int taille_avant = l->taille;
                supprimer_liste_matricule(l, matricule);
                /* BUG CORRIGE : meme correction que pour les deux autres
                   structures -- confirmation + reaffichage apres
                   suppression. */
                if (l->taille < taille_avant) {
                    set_couleur(COULEUR_SUCCES);
                    printf("Etudiant matricule %d supprime.\n", matricule);
                } else {
                    set_couleur(COULEUR_ERREUR);
                    printf("Aucun etudiant avec le matricule %d.\n", matricule);
                }
                reset_couleur();
                printf("\n--- Liste apres suppression (%d etudiant(s)) ---\n", l->taille);
                afficher_liste(l);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
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
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

            /* --- 9. TRIER PAR INSERTION --- */
            case 9:
                liste_tri_insertion(l);
                set_couleur(COULEUR_SUCCES);
                printf("Trie par insertion.\n");
                reset_couleur();
                /* BUG CORRIGE : meme correction que pour le tableau
                   dynamique -- sans cet affichage, impossible de verifier
                   que le tri a reellement modifie l'ordre de la liste. */
                afficher_liste(l);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            /* --- 10. TRIER PAR RAPIDE --- */
            case 10:
                set_couleur(COULEUR_ERREUR);
                printf("Tri rapide non disponible pour la liste chainee.\n");
                printf("(structure sequentielle : utilisez le tri par insertion)\n");
                reset_couleur();
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 0:
                break;
        }
    }

    /* BUG CORRIGE : liste_liberer(l) a ete retire d'ici pour la meme
       raison que pour le tableau dynamique (voir plus haut). */
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
    animer_texte("Bienvenue  dans le Systeme de Gestion de Scolarite...\n", 25);
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