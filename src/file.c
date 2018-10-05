#include "file.h"

file_jeu_instruction creer_file(void){
	return NULL;
}

int file_vide(file_jeu_instruction f){
	return !f;
}

void visualiser_file(file_jeu_instruction f){
	file_jeu_instruction g=NULL;
	if (file_vide(f)){
		printf("File vide\n");
		return;
	}
	
	g=f->suiv;
	while (g!=f){
		printf("Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
		g=g->suiv;
	}
	printf("Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
}

void ecrire_file(file_jeu_instruction f,FILE*a){
	file_jeu_instruction g=NULL;
	g=f->suiv;
	while (g!=f){
		fprintf(a, "Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
		g=g->suiv;
	}
	fprintf(a, "Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
	liberer_file(g);
}
file_jeu_instruction enfiler(char*nom,char*carac,int line, file_jeu_instruction f){
	file_jeu_instruction e=calloc(1,sizeof(*e));
	if(e==NULL) {
		return NULL;
	}
	strcpy(e->identifiant, nom);
	strcpy(e->caractere, carac);
	e->ligne=line;
	if(file_vide(f)){
		e->suiv=e;
		return e;
	}
	e->suiv=f->suiv;
	f->suiv=e;
	return e;
}

int defiler(file_jeu_instruction* pf,char*nom,char*carac){
	if (*pf==NULL){
		printf("File vide\n");
		exit(0);
	}
	int c = (*pf)->suiv->ligne;
	if(*pf==(*pf)->suiv){
		(*pf)=NULL;
		return c;
	}
	(*pf)->suiv= (*pf)->suiv->suiv;
	return c;
}

void liberer_file(file_jeu_instruction f){
	file_jeu_instruction g=f->suiv;
	f->suiv=NULL;
	while(g!=NULL){
		g=supprimer_tete(g);
	}
}

file_jeu_instruction supprimer_tete(file_jeu_instruction g){
	file_jeu_instruction c = NULL;
	if (file_vide(g)) {
		return NULL;
	}
	c = g->suiv;
	free(g);
	return c;
}


