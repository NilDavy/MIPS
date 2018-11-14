#include "file_symb.h"

/** Creer file **/
file_symb creerfile_symb(void){
	return NULL;
}

/** Verif file vide **/
int file_vide_symb(file_symb f){
	return !f;
}

/** visaliser une file **/
void visualiser_file_symb(file_symb f){
	file_symb g=NULL;
	if (file_vide_symb(f)){
		printf("File vide\n");
		printf("\n");
		return;
	}
	
	g=f->suiv;
	while (g!=f){
		printf("Nom symbole : %s\t",g->nom);
		printf("Ligne : %d\t",g->ligne);
		printf("Lieu : %s\t",g->section);
		printf("Decalage : %d\n",g->decalage);
		g=g->suiv;
	}
	printf("Nom symbole : %s\t",g->nom);
	printf("Ligne : %d\t",g->ligne);
	printf("Lieu : %s\t",g->section);
	printf("Decalage : %d\n",g->decalage);
	printf("\n");
}


/** ajout d'un maillon a la file **/
file_symb ajout_symb(char *nom, int line, unsigned int dec,char*sec, file_symb f){

	file_symb e=calloc(1,sizeof(*e));
	if(e==NULL) {
		return NULL;
	}
	/*printf("nom %s\n",nom);
	printf("e->nom %s\n",e->nom);*/
	strncpy(e->nom,nom,strlen(nom));
	/*printf("nom1 %s\n",nom);
	printf("e->nom1 %s\n",e->nom);*/
	e->ligne=line;
	e->decalage=dec;
	strcpy(e->section,sec);
	if(file_vide_symb(f)){
		e->suiv=e;
		return e;
	}
	e->suiv=f->suiv;
	f->suiv=e;
	return e;
}

/** liberation de la memoire de la file **/

void liberer_symb(file_symb f){
	if(file_vide_symb(f)){
		return;
	}
	file_symb g= f->suiv	;
	f->suiv = NULL;
	while(g!=NULL){
		g=supprimer_tete_symb(g);
	}
	
}

/** liberation de la memoire d'un maillon de la file **/
file_symb supprimer_tete_symb(file_symb g){
	file_symb c;
	if(g->suiv == g){
		free(g);
		return NULL;
	}
	c = g;
	g=c->suiv;
	c->suiv=NULL;
	free(c);
	return g;
}

char*defiler_symb (file_symb*f){
	char*mot=NULL;
	mot = calloc(strlen((*f)->suiv->nom), sizeof(char));
	strcpy(mot,(*f)->suiv->nom);
	file_symb c=creerfile_symb();
	if((*f)==(*f)->suiv){
		(*f)=NULL;
	}
	else{
		c=(*f)->suiv;
		(*f)->suiv=c->suiv;
		c->suiv=NULL;
		supprimer_tete_symb(c);
	}
	return mot;
}

void ecrire_file_symb(file_symb f,FILE*a){
	if(file_vide_symb(f)){
		return;
	}
	file_symb g=NULL;
	g=f->suiv;
	while (g!=f){
		fprintf(a, "Nom instruction : %s\t Ligne : %d\t Section : %s\t Decalage : %d\n" ,g->nom,g->ligne,g->section,g->decalage);
		g=g->suiv;
	}
	fprintf(a, "Nom instruction : %s\t Ligne : %d\t Section : %s\t Decalage : %d\n" ,g->nom,g->ligne,g->section,g->decalage);
}
