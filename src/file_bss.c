#include "file_bss.h"

/** Creer file **/
file_bss creerfile_bss(void){
	return NULL;
}

/** Verif file vide **/
int file_vide_bss(file_bss f){
	return !f;
}

/** visaliser une file **/
void visualiser_file_bss(file_bss f){
	file_bss g=NULL;
	if (file_vide_bss(f)){
		printf("File vide\n");
		printf("\n");
		return;
	}
	
	g=f->suiv;
	while (g!=f){
		printf("Nom instruction : %s\t",g->nomInst);
		printf("Ligne : %d\t",g->ligne);
		printf("Decalage : %d\t",g->decalage);
		switch(g->type)
		{
			case 0:
				printf("Opérande : %c\n",g->op.c);
				break;
			case 1:
				printf("Opérande : %u\n",g->op.uc);
				break;
			case 2:
				printf("Opérande : %d\n",g->op.i);
				break;
			case 3:
				printf("Opérande : %u\n",g->op.ui);
				break;
			case 4:
				printf("Opérande : %s\n",g->op.s);
				break;
		}
		g=g->suiv;
	}
	printf("Nom instruction : %s\t",g->nomInst);
	printf("Ligne : %d\t",g->ligne);
	printf("Decalage : %d\t",g->decalage);
	switch(g->type)
	{
		case 0:
			printf("Opérande : %c\n",g->op.c);
			break;
		case 1:
			printf("Opérande : %u\n",g->op.uc);
			break;
		case 2:
			printf("Opérande : %d\n",g->op.i);
			break;
		case 3:
			printf("Opérande : %u\n",g->op.ui);
			break;
		case 4:
			printf("Opérande : %s\n",g->op.s);
			break;
	}
	printf("\n");
}


/** ajout d'un maillon a la file **/
file_bss ajout_bss(char *nom, int line, unsigned int dec, char*op, int type, file_bss f){
	file_bss e=calloc(1,sizeof(*e));
	if(e==NULL) {
		return NULL;
	}
	strcpy(e->nomInst, nom);
	e->ligne=line;
	e->decalage=dec;
	e->type=type;
	switch(type)
	{
		case 0:
			(e->op).c =*op;
			break;
		case 1:
			(e->op).uc = (unsigned char)*op;
			break;
		case 2:
			(e->op).i = atoi(op);
			break;
		case 3:
			(e->op).ui =(unsigned int)*op;
			break;
		case 4:
			(e->op).s = calloc(strlen(op),sizeof(char));
			strcpy((e->op).s,op);
			break;
	}
	if(file_vide_bss(f)){
		e->suiv=e;
		return e;
	}
	e->suiv=f->suiv;
	f->suiv=e;
	return e;
}

/** liberation de la memoire de la file **/

void liberer_bss(file_bss f){
	if(file_vide_bss(f)){
		return;
	}
	file_bss g= f->suiv	;
	f->suiv = NULL;
	while(g!=NULL){
		g=supprimer_tete_bss(g);
	}
	
}

/** liberation de la memoire d'un maillon de la file **/
file_bss supprimer_tete_bss(file_bss g){
	file_bss c;
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


/** Ecrire une file dans un fichier exterieur **/
void ecrire_file_bss(file_bss f,FILE*a){
	if(file_vide_bss(f)){
		return;
	}
	file_bss g=NULL;
	g=f->suiv;
	while (g!=f){
		fprintf(a, "Nom instruction : %s\t Ligne : %d\t Decalage : %d\t" ,g->nomInst,g->ligne,g->decalage);
		switch(g->type)
		{
			case 0:
				fprintf(a,"Opérande : %c\n",g->op.c);
				break;
			case 1:
				fprintf(a,"Opérande : %u\n",g->op.uc);
				break;
			case 2:
				fprintf(a,"Opérande : %d\n",g->op.i);
				break;
			case 3:
				fprintf(a,"Opérande : %u\n",g->op.ui);
				break;
			case 4:
				fprintf(a,"Opérande : %s\n",g->op.s);
				break;
		}
		g=g->suiv;
	}
	fprintf(a, "Nom instruction : %s\t Ligne : %d\t Decalage : %d\t" ,g->nomInst,g->ligne,g->decalage);
	switch(g->type)
	{
		case 0:
			fprintf(a,"Opérande : %c\n",g->op.c);
			break;
		case 1:
			fprintf(a,"Opérande : %u\n",g->op.uc);
			break;
		case 2:
			fprintf(a,"Opérande : %d\n",g->op.i);
			break;
		case 3:
			fprintf(a,"Opérande : %u\n",g->op.ui);
			break;
		case 4:
			fprintf(a,"Opérande : %s\n",g->op.s);
			break;
	}
	
}
