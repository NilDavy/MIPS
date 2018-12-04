#include "file_data.h"

/** Creer file **/
file_data creerfile_data(void){
	return NULL;
}

/** Verif file vide **/
int file_vide_data(file_data f){
	return !f;
}

/** visaliser une file **/
void visualiser_file_data(file_data f){
	file_data g=NULL;
	if (file_vide_data(f)){
		printf("File vide\n");
		printf("\n");
		return;
	}
	
	g=f->suiv;
	while (g!=f){
		printf("Nom instruction : %s\t",g->nomInst);
		printf("Ligne : %d\t",g->ligne);
		printf("Decalage : %d\t",g->decalage);
		printf("Etiquette : %d\t",g->etiquette);
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
	printf("Etiquette : %d\t",g->etiquette);
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
file_data ajout_data(char *nom, int line, unsigned int dec, char* op, int type, file_data f,int etiq){
	file_data e=calloc(1,sizeof(*e));
	if(e==NULL) {
		return NULL;
	}
	strcpy(e->nomInst, nom);
	e->ligne=line;
	e->decalage=dec;
	e->type=type;
	e->etiquette=etiq;
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
			strcpy((e->op).s,op);
			break;
	}
	if(file_vide_data(f)){
		e->suiv=e;
		return e;
	}
	e->suiv=f->suiv;
	f->suiv=e;
	return e;
}

/** liberation de la memoire de la file **/

void liberer_data(file_data f){
	if(file_vide_data(f)){
		return;
	}
	file_data g= f->suiv	;
	f->suiv = NULL;
	while(g!=NULL){
		g=supprimer_tete_data(g);
	}
	
}

/** liberation de la memoire d'un maillon de la file **/
file_data supprimer_tete_data(file_data g){
	file_data c;
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

void ecrire_file_data(file_data f ,FILE*a){
	if(file_vide_data(f)){
		return;
	}
	file_data g=NULL;
	g=f->suiv;
	while (g!=f){
		fprintf(a, "Nom instruction : %s\t Ligne : %d\t Decalage : %d\t Etiquette : %d\t" ,g->nomInst,g->ligne,g->decalage,g->etiquette);
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
	fprintf(a, "Nom instruction : %s\t Ligne : %d\t Decalage : %d\t Etiquette : %d\t" ,g->nomInst,g->ligne,g->decalage,g->etiquette);
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
