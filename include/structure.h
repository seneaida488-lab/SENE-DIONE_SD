#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
typedef struct {
int jour; 
int mois; 
int annee;
}Date;
typedef struct {
 int id;
 char nom[20];   
}Cours;
typedef struct {
int matricule;     
float valeur;
char matiere[50]; 
char *appreciation;
Date dateDeliberation;
Cours *cours;;
int tab[10];
}Note;
typedef struct{
int id;
char nom[16];
}Niveau;
typedef struct {
int matricule;
float montant;
char etat[30];
char *filliere;
Date dateInscription;
Niveau *niveau;
int tab[20];
}Inscription;
typedef struct{
int matricule;
float moyenne;
char *nom;
char prenom[40];
Date dateNaissance;
Note *tabnote;
Inscription *inscription;
int tab[20];	
}Etudiant;

	
	
	
	
	
	
	
	
	
	
	
	
