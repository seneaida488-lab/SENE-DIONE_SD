#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tableau_statique.h"
#include "../include/tableau_dynamique.h"
#include "../include/liste_chainee.h"
#include "generateur_donnee.h"

void vider_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void afficher_menu_principal(void)
{
    printf("\n============================================\n");
    printf("   SYSTÈME DE GESTION DE SCOLARITÉ\n");
    printf("   Projet LMI ASD 2026\n");
    printf("============================================\n");
    printf("  --- Démonstrations automatiques ---\n");
    printf("  1. Démo Tableau Statique\n");
    printf("  2. Démo Tableau Dynamique\n");
    printf("  3. Démo Liste Doublement Chaînée\n");
    printf("  --- Menus interactifs (saisie clavier) ---\n");
    printf("  4. Interface Tableau Statique\n");
    printf("  5. Interface Tableau Dynamique\n");
    printf("  6. Interface Liste Doublement Chaînée\n");
    printf("  --- Outils ---\n");
    printf("  7. Lancer le Benchmark complet\n");
    printf("  0. Quitter\n");
    printf("============================================\n");
    printf("Votre choix : ");
}


void demo_tableau_statique(void)
{
    printf("\n========== TABLEAU STATIQUE ==========\n\n");

    Etudiant tab[100000];
    int n = 0, i;

    Etudiant e1, e2, e3, e4, e5;
    e1.matricule = 1001; e1.moyenne = 14.50; strcpy(e1.nom, "Ndiaye");
    e2.matricule = 1002; e2.moyenne = 12.00; strcpy(e2.nom, "Sène");
    e3.matricule = 1003; e3.moyenne = 17.50; strcpy(e3.nom, "Thiam");
    e4.matricule = 1004; e4.moyenne =  9.00; strcpy(e4.nom, "Kane");
    e5.matricule = 1005; e5.moyenne = 15.75; strcpy(e5.nom, "Diop");

    insertionTableau(tab, &n, e1);
    insertionTableau(tab, &n, e2);
    insertionTableau(tab, &n, e3);
    insertionTableau(tab, &n, e4);
    insertionTableau(tab, &n, e5);

    printf("--- Affichage ---\n");
    for (i = 0; i < n; i++)
        printf("  [%d] matricule:%d | %s | moyenne:%.2f\n",
               i, tab[i].matricule, tab[i].nom, tab[i].moyenne);

    printf("\n--- Recherche matricule 1003 ---\n");
    int pos = RechercheCleprimaire(tab, n, 1003);
    if (pos > -1)
        printf("  Trouve : %s — %.2f\n", tab[pos].nom, tab[pos].moyenne);

    printf("\n--- Recherche intervalle [10, 15] ---\n");
    RechercheParIntervalle(tab, n, 10.0, 15.0);
    printf("\n");

    printf("\n--- Tri par insertion ---\n");
    Tri_insertion_moyenne(tab, n);
    for (i = 0; i < n; i++)
        printf("  [%d] %s — %.2f\n", i, tab[i].nom, tab[i].moyenne);

    printf("\n--- Statistiques ---\n");
    printf("  Moyenne generale : %.2f\n", MoyenneDesmoyennes(tab, n));
    printf("  Min : %d  Max : %d\n",
           minimumDesmoyennes(tab, n), maximumDesmoyennes(tab, n));
    printf("  Mediane : %.2f\n", MedianeDesmoyennes(tab, n));
    printf("  Ecart-type : %.2f\n", EcartTypeDesmoyennes(tab, n));

    printf("\n--- Suppression matricule 1004 ---\n");
    suppressionCleprimaire(tab, &n, 1004);
    printf("Taille finale : %d\n", n);
}

void demo_tableau_dynamique(void)
{
    printf("\n========== TABLEAU DYNAMIQUE ==========\n\n");

    TableauDyn *t = creer_tableau_dyn();
    Date d = {1, 1, 2006};

    char *noms[]    = {"Ndiaye","Sène","Thiam","Kane","Ndione","Diop"};
    char *prenoms[] = {"Déthié","Aida","Aninata","Mouhamed","Coura","Baba"};
    float moys[]    = {14.5, 15.0, 17.5, 19.0, 16.0, 18.5};

    for (int i = 0; i < 6; i++) {
        Etudiant *e = creer_etudiant(1000 + i, moys[i], noms[i], prenoms[i], d);
        inserer_etudiant_dyn(t, e);
    }

    afficher_tableau_dyn(t);

    printf("\n--- Recherche matricule 1002 ---\n");
    Etudiant *e = rechercher_par_matricule_dyn(t, 1002);
    if (e) printf("Trouve : %s %s\n", e->nom, e->prenom);

    printf("\n");
    rechercher_par_intervalle_dyn(t, 10.0f, 15.0f);
    printf("\n");
    rechercher_par_prefixe_dyn(t, "Di");

    printf("\n--- Statistiques ---\n");
    printf("  Moyenne    : %.2f\n", moyenne_generale_dyn(t));
    printf("  Min        : %.2f\n", minimum_dyn(t));
    printf("  Max        : %.2f\n", maximum_dyn(t));
    printf("  Mediane    : %.2f\n", mediane_dyn(t));
    printf("  Ecart-type : %.2f\n", ecart_type_dyn(t));

    printf("\n--- Tri rapide ---\n");
    tri_rapide_dyn(t, 0, t->taille - 1);
    afficher_tableau_dyn(t);

    liberer_tableau_dyn(t);
    printf("\nMemoire liberee.\n");
}

void demo_liste_chainee(void)
{
    printf("\n========== LISTE DOUBLEMENT CHAÎNÉE ==========\n\n");

    ListeDC *l = creer_liste();
    Date d = {1, 1, 2006};

    inserer_en_queue(l, creer_etudiant(1001, 14.5, "Ndiaye", "Déthié",  d));
    inserer_en_queue(l, creer_etudiant(1002, 12.0, "Sène", "Aida",   d));
    inserer_en_queue(l, creer_etudiant(1003, 17.5, "Thiam",    "Aminata",    d));
    inserer_en_tete (l, creer_etudiant(1004,  9.0, "Kane",   "Mouhamed",     d));
    inserer_en_queue(l, creer_etudiant(1005, 15.0, "Diop",   "Baba", d));

    afficher_liste(l);
    printf("\n--- Affichage inverse ---\n");
    afficher_liste_inverse(l);

    printf("\n");
    rechercher_intervalle_liste(l, 10.0, 15.0);
    printf("\n");
    rechercher_prefixe_liste(l, "Di");

    printf("\n--- Statistiques ---\n");
    printf("  Moyenne    : %.2f\n", moyenne_generale_liste(l));
    printf("  Min        : %.2f\n", minimum_liste(l));
    printf("  Max        : %.2f\n", maximum_liste(l));
    printf("  Mediane    : %.2f\n", mediane_liste(l));
    printf("  Ecart-type : %.2f\n", ecart_type_liste(l));

    printf("\n--- Tri par insertion ---\n");
    tri_insertion_liste(l);
    afficher_liste(l);

    liberer_liste(l);
    printf("\nMemoire liberee.\n");
}

void interface_tableau_statique(void)
{
    static Etudiant tab[100000];
    static int n = 0;
    int choix = -1;

    while (choix != 0) {
        printf("\n--- Interface Tableau Statique (%d/%d) ---\n", n, 100000);
        printf("1. Ajouter   2. Afficher   3. Rechercher matricule\n");
        printf("4. Rechercher intervalle   5. Rechercher prefixe\n");
        printf("6. Supprimer 7. Trier (insertion) 8. Trier (rapide)\n");
        printf("9. Statistiques            0. Retour\n");
        printf("Choix : ");
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {
            case 1: {
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
                if (insertionTableau(tab, &n, e))
                    printf("Etudiant ajoute.\n");
                break;
            }
            case 2:
                for (int i = 0; i < n; i++)
                    printf("  [%d] matricule:%d | %s | moyenne:%.2f\n",
                           i, tab[i].matricule, tab[i].nom, tab[i].moyenne);
                break;
            case 3: {
                int matricule;
                printf("Matricule : "); 
                scanf("%d", &matricule); 
                vider_buffer();
                int pos = RechercheCleprimaire(tab, n, matricule);
                if (pos > -1)
                    printf("Trouve : %s — %.2f\n", tab[pos].nom, tab[pos].moyenne);
                else
                    printf("Non trouve.\n");
                break;
            }
            case 4: {
                float min, max;
                printf("Min : ");
                scanf("%f", &min);
                printf("Max : "); 
                scanf("%f", &max); 
                vider_buffer();
                RechercheParIntervalle(tab, n, min, max);
                printf("\n");
                break;
            }
            case 5: {
                char prefixe[40];
                printf("Prefixe : ");
                fgets(prefixe, sizeof(prefixe), stdin);
                prefixe[strcspn(prefixe, "\n")] = '\0';
                RechercheParPrefixe(tab, n, prefixe);
                printf("\n");
                break;
            }
            case 6: {
                int matricule;
                printf("Matricule : "); 
                scanf("%d", &matricule); 
                vider_buffer();
                suppressionCleprimaire(tab, &n, matricule);
                break;
            }
            case 7:
                Tri_insertion_moyenne(tab, n);
                printf("Trie par insertion.\n");
                break;
            case 8:
                if (n > 1) TriRapide(tab, 0, n - 1);
                printf("Trie par tri rapide.\n");
                break;
            case 9:
                if (n > 0) {
                    printf("Moyenne generale : %.2f\n", MoyenneDesmoyennes(tab, n));
                    printf("Min : %d  Max : %d\n",
                           minimumDesmoyennes(tab, n), maximumDesmoyennes(tab, n));
                    printf("Mediane : %.2f\n", MedianeDesmoyennes(tab, n));
                    printf("Ecart-type : %.2f\n", EcartTypeDesmoyennes(tab, n));
                } else {
                    printf("Tableau vide.\n");
                }
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    }
}

void interface_tableau_dynamique(void)
{
    TableauDyn *t = creer_tableau_dyn();
    int choix = -1;

    while (choix != 0) {
        printf("\n--- Interface Tableau Dynamique (%d/%d) ---\n",
               t->taille, t->capacite);
        printf("1. Ajouter   2. Afficher   3. Rechercher matricule\n");
        printf("4. Rechercher intervalle   5. Rechercher prefixe\n");
        printf("6. Supprimer 7. Modifier moyenne\n");
        printf("8. Trier (insertion) 9. Trier (rapide)\n");
        printf("10. Statistiques  11. Sauvegarder  12. Charger\n");
        printf("0. Retour\n");
        printf("Choix : ");
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {
            case 1: {
                int matricule; 
                float moyenne; 
                char nom[40], prenom[100];
                Date date;
                printf("Matricule : "); 
                scanf("%d", &matricule); 
                vider_buffer();
                printf("Nom : "); fgets(nom, sizeof(nom), stdin);
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
                printf("Etudiant ajoute.\n");
                break;
            }
            case 2:
                afficher_tableau_dyn(t);
                break;
            case 3: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule); vider_buffer();
                Etudiant *e = rechercher_par_matricule_dyn(t, matricule);
                if (e) printf("Trouve : %s %s — %.2f\n", e->nom, e->prenom, e->moyenne);
                else printf("Non trouve.\n");
                break;
            }
            case 4: {
                float min, max;
                printf("Min : "); scanf("%f", &min);
                printf("Max : "); scanf("%f", &max); vider_buffer();
                rechercher_par_intervalle_dyn(t, min, max);
                break;
            }
            case 5: {
                char prefixe[40];
                printf("Prefixe : "); 
                fgets(prefixe, sizeof(prefixe), stdin);
                prefixe[strcspn(prefixe, "\n")] = '\0';
                rechercher_par_prefixe_dyn(t, prefixe);
                break;
            }
            case 6: {
                int matricule;
                printf("Matricule : ");
                scanf("%d", &matricule); 
                vider_buffer();
                supprimer_par_matricule_dyn(t, matricule);
                break;
            }
            case 7: {
                int matricule; 
                float nouvelle_moyenne;
                printf("Matricule : "); scanf("%d", &matricule);
                printf("Nouvelle moyenne : "); scanf("%f", &nouvelle_moyenne);
                vider_buffer();
                modifier_moyenne_dyn(t, matricule, nouvelle_moyenne);
                break;
            }
            case 8:
                tri_insertion_dyn(t);
                printf("Trie par insertion.\n");
                break;
            case 9:
                if (t->taille > 1) tri_rapide_dyn(t, 0, t->taille - 1);
                printf("Trie par tri rapide.\n");
                break;
            case 10:
                if (t->taille > 0) {
                    printf("Moyenne generale : %.2f\n", moyenne_generale_dyn(t));
                    printf("Min : %.2f  Max : %.2f\n",
                           minimum_dyn(t), maximum_dyn(t));
                    printf("Mediane : %.2f\n", mediane_dyn(t));
                    printf("Ecart-type : %.2f\n", ecart_type_dyn(t));
                } else {
                    printf("Tableau vide.\n");
                }
                break;
            case 11: {
                char nom_fichier[100];
                printf("Nom du fichier : "); fgets(nom_fichier, sizeof(nom_fichier), stdin);
                nom_fichier[strcspn(nom_fichier, "\n")] = '\0';
                serialiser_dyn(t, nom_fichier);
                break;
            }
            case 12: {
                char nom_fichier[100];
                printf("Nom du fichier : "); fgets(nom_fichier, sizeof(nom_fichier), stdin);
                nom_fichier[strcspn(nom_fichier, "\n")] = '\0';
                TableauDyn *nouveau = deserialiser_dyn(nom_fichier);
                if (nouveau) { liberer_tableau_dyn(t); t = nouveau; }
                break;
            }
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    }

    liberer_tableau_dyn(t);
}

void interface_liste_chainee(void)
{
    ListeDC *l = creer_liste();
    int choix = -1;

    while (choix != 0) {
        printf("\n--- Interface Liste Doublement Chaînée (%d elements) ---\n",
               l->taille);
        printf("1. Ajouter en tete  2. Ajouter en queue\n");
        printf("3. Afficher debut->fin  4. Afficher fin->debut\n");
        printf("5. Rechercher matricule  6. Rechercher intervalle\n");
        printf("7. Rechercher prefixe    8. Supprimer\n");
        printf("9. Trier (insertion)    10. Trier (bulles)\n");
        printf("11. Statistiques  12. Sauvegarder  13. Charger\n");
        printf("0. Retour\n");
        printf("Choix : ");
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {
            case 1:
            case 2: {
                int matricule;
                float moyenne; 
                char nom[40], prenom[100];
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
                if (choix == 1) { inserer_en_tete(l, e); printf("Ajoute en tete.\n"); }
                else            { inserer_en_queue(l, e); printf("Ajoute en queue.\n"); }
                break;
            }
            case 3:
                afficher_liste(l);
                break;
            case 4:
                afficher_liste_inverse(l);
                break;
            case 5: {
                int matricule;
                printf("Matricule : "); scanf("%d", &matricule); vider_buffer();
                Etudiant *e = rechercher_matricule_liste(l, matricule);
                if (e) printf("Trouve : %s %s — %.2f\n", e->nom, e->prenom, e->moyenne);
                else printf("Non trouve.\n");
                break;
            }
            case 6: {
                float min, max;
                printf("Min : "); scanf("%f", &min);
                printf("Max : "); scanf("%f", &max); vider_buffer();
                rechercher_intervalle_liste(l, min, max);
                break;
            }
            case 7: {
                char prefixe[40];
                printf("Prefixe : "); fgets(prefixe, sizeof(prefixe), stdin);
                prefixe[strcspn(prefixe, "\n")] = '\0';
                rechercher_prefixe_liste(l, prefixe);
                break;
            }
            case 8: {
                int matricule;
                printf("Matricule : "); scanf("%d", &matricule); vider_buffer();
                supprimer_matricule_liste(l, matricule);
                break;
            }
            case 9:
                tri_insertion_liste(l);
                break;
            case 10:
                tri_bulles_liste(l);
                break;
            case 11:
                if (l->taille > 0) {
                    printf("Moyenne generale : %.2f\n", moyenne_generale_liste(l));
                    printf("Min : %.2f  Max : %.2f\n",
                           minimum_liste(l), maximum_liste(l));
                    printf("Mediane : %.2f\n", mediane_liste(l));
                    printf("Ecart-type : %.2f\n", ecart_type_liste(l));
                } else {
                    printf("Liste vide.\n");
                }
                break;
            case 12: {
                char nom_fichier[100];
                printf("Nom du fichier : "); fgets(nom_fichier, sizeof(nom_fichier), stdin);
                nom_fichier[strcspn(nom_fichier, "\n")] = '\0';
                serialiser_liste(l, nom_fichier);
                break;
            }
            case 13: {
                char nom_fichier[100];
                printf("Nom du fichier : "); fgets(nom_fichier, sizeof(nom_fichier), stdin);
                nom_fichier[strcspn(nom_fichier, "\n")] = '\0';
                ListeDC *nouvelle = deserialiser_liste(nom_fichier);
                if (nouvelle) { liberer_liste(l); l = nouvelle; }
                break;
            }
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    }

    liberer_liste(l);
}

void lancer_benchmark(void)
{
    printf("\n========== BENCHMARK ==========\n");
    printf("Cette fonctionnalite necessite le fichier benchmark/main.c\n");
    printf("complet avec les mesures de temps sur les 3 structures.\n");
    printf("Compilez avec : make bench  puis lancez : ./bench\n");
}


int main(void)
{
    int choix = -1;

    while (choix != 0) {
        afficher_menu_principal();
        scanf("%d", &choix);
        vider_buffer();

        switch (choix) {
            case 1: demo_tableau_statique();          break;
            case 2: demo_tableau_dynamique();         break;
            case 3: demo_liste_chainee();             break;
            case 4: interface_tableau_statique();     break;
            case 5: interface_tableau_dynamique();    break;
            case 6: interface_liste_chainee();        break;
            case 7: lancer_benchmark();               break;
            case 0: printf("\nAu revoir !\n");        break;
            default: printf("Choix invalide.\n");     break;
        }
    }

    return 0;
}