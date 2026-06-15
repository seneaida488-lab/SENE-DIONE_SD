#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define TAILLEMAX 40
#include<math.h>
typedef struct {
    int matricule;
    float moyenne;
    char nom[50];
    char *prenom;
} Etudiant;
typedef struct {
    Etudiant **tab;   // le vrai tableau pointeur vers des pointeurs d'Etudiant//
    int taille;           //nombre d'étudiants actuellement dans le tableau//
    int capacite;         // nombre maximum d'étudiants que le tableau peut contenir avant de devoir être agrandi//
} TableauDyn;
 void tableau_init(TableauDyn *t, int capacite_initiale) {
    t->capacite = capacite_initiale;       // la capacité initiale du tableau//
    t->taille   = 0;                       // le tableau est vide au départ//
    t->tab = (Etudiant **)malloc(capacite_initiale * sizeof(Etudiant *));
    // On rserve de la mmoire pour 'capacite_initiale' pointeurs//
}
void tableau_agrandir(TableauDyn *t) {
    t->capacite = t->capacite * 2;
    // On double la capacité du tableau

    Etudiant **nouveau_tab = (Etudiant **)realloc(t->tab, t->capacite * sizeof(Etudiant *));
    // On alloue un nouveau tableau plus grand
    t->tab=nouveau_tab;
    }

void tableau_liberer(TableauDyn *t) {
if (t->tab != NULL) {  // Rien librer si le tableau est NULL
    for (int i = 0; i < t->taille; i++) {
        free(t->tab[i]);
        // lib�re chaque �tudiant un par un
        // (la m�moire r�serv�e avec malloc pour chaque Etudiant)
    }
    // apr�s cette boucle, tous les �tudiants sont lib�r�s
    free(t->tab);
    // lib�re le tableau de pointeurs lui-m�me
    // (la m�moire r�serv�e pour tab dans tableau_init)
    t->tab=NULL;
}
t->taille=0;
t->capacite=0;
}
void tableau_inserer(TableauDyn *t, Etudiant *e) {
    if (t->taille == t->capacite)
        tableau_agrandir(t);
    // Si plein ? on double la capacit� AVANT d'ins�re
    t->tab[t->taille] = e;
    // On stocke le POINTEUR vers l'�tudiant, pas une copie
    t->taille++;
}
Etudiant* tableau_rechercher_matricule(TableauDyn *t, int matricule) {
    for (int i = 0; i < t->taille; i++) {
        if (t->tab[i]->matricule == matricule)
            return t->tab[i];
        // Retourne le pointeur d�s que trouv�
    }
    return NULL;  // Pas trouv�
}
void tableau_rechercher_intervalle(TableauDyn *t, float min, float max) {
    printf("Etudiants avec moyenne entre %.2f et %.2f :\n", min, max);
    for (int i = 0; i < t->taille; i++) {
        if (t->tab[i]->moyenne <= max &&
            t->tab[i]->moyenne >= min ) {
            printf("  [%d] %s %s  %.2f\n",
                   t->tab[i]->matricule,
                   t->tab[i]->nom,
                   t->tab[i]->prenom,
                   t->tab[i]->moyenne);
        }
}
}
//Recherche par pr�fixe (champ cha�ne)//
void tableau_rechercher_prefixe(TableauDyn *t, const char *prefixe) {
    int len = strlen(prefixe);
    for (int i = 0; i < t->taille; i++) {
        if (strncmp(t->tab[i]->nom, prefixe, len) == 0)
            // strncmp compare uniquement les 'len' premiers caractres
            printf("%s %s\n", t->tab[i]->nom, t->tab[i]->prenom);
    }
    }
//Suppression par cl�//
int tableau_supprimer(TableauDyn *t, int matricule) {
    for (int i = 0; i < t->taille; i++) {
        if (t->tab[i]->matricule == matricule) {
            free(t->tab[i]);  // Lib�re toute la m�moire de l'�tudiant //
            t->tab[i] = t->tab[t->taille - 1]; // Remplace le trou par le DERNIER �l�ment
           t->taille--;
            return 1;  // trouve//
        }
    }
    return 0;  // non trouve//
}
//Mise a jour//
int tableau_modifier_moyenne(TableauDyn *t, int matricule, float nouvelle_moyenne) {
    Etudiant *e = tableau_rechercher_matricule(t, matricule);
    if (!e) return 0;
    e->moyenne = nouvelle_moyenne;
    // Modification directe via le pointeur
    return 1;
}
//Tri par insertion (crit�re 1 : moyenne)//
void tableau_tri_insertion_moyenne(TableauDyn *t) {
    for (int i = 1; i < t->taille; i++) {
        Etudiant *x = t->tab[i];  // L'�l�ment � placer
        int j = i ;
        while (j > 0 && t->tab[j - 1]->moyenne > x->moyenne) {
            t->tab[j - 1] = t->tab[j]; // D�cale vers la droite
            j--;
        }
		t->tab[j]=x; 
    }
}
// Tri par rapide//
int partition(Etudiant **tab, int premier, int dernier) {
    Etudiant *pivot = tab[premier];
    int i = premier - 1;
    int j = dernier + 1;

    while (1) {
        do {
            i++;
        } while (strcmp(tab[i]->nom, pivot->nom) < 0);

        do {
            j--;
        } while (strcmp(tab[j]->nom, pivot->nom) > 0);

        if (i >= j)
            return j;
        Etudiant *tmp = tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
    }
}
void tableau_tri_rapide(Etudiant **tab, int premier, int dernier) {
    if (premier < dernier) {
        int p = partition(tab, premier, dernier);
        tableau_tri_rapide(tab, premier, p);
        tableau_tri_rapide(tab, p + 1, dernier);
    }
}
//Agr�gations statistiques//
double tableau_minimummoyennes(TableauDyn *t) {
    double min = t->tab[0]->moyenne;
    for (int i = 0; i < t->taille; i++) {
        double m = t->tab[i]->moyenne;
        if (m < min)
		 min = m;
    }
    return min;
}
double tableau_maximummoyennes(TableauDyn *t) {
    double max = t->tab[0]->moyenne;
    for (int i = 0; i < t->taille; i++) {
        double mx = t->tab[i]->moyenne;
        if (mx > max)
		 max = mx;
    }
    return max;
}
double tableauMoyenneDesmoyennes(TableauDyn *t) {
    double s=0; 
    double Moy;
    for (int i = 1; i < t->taille; i++) {
        s+=t->tab[i]->moyenne;
    }
    if(t->taille>0)
    Moy=s/t->taille; 
    return Moy;
}
float tableau_ecart_type(TableauDyn *t) {
    double moy = tableauMoyenneDesmoyennes(t);
    double s = 0;
    for (int i = 0; i < t->taille; i++) {
        double diff = t->tab[i]->moyenne - moy;
        s += diff * diff;
    }
    return sqrt(s / t->taille);
} 
