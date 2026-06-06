#include<stdio.h>
#include"structure.h"
int main(){
	Cours c={12864,"math"};
	Note tabnote={1213,17.15,"physique","trés bien",12,6,2026,&c,10};
	Niveau nv={1967,"Master"};
	Inscription i={123,26324.45,"valide",&Informatique,19,5,2026,&nv,20};
	Etusiant e={128,15.65,"Ndiaye",&Diethie,29,8,2006,&tabnote,&i,40};
	printf("%d",e.matricule);
	printf("%.2f",e.moyenne);
	puts(e.&prenom);
	puts(e.nom);
	printf("%d",e.dateNaissance);
	for(int i=0;i<40;i++){
		printf("%d",e.tab[i]);
	}
	return 0;
}