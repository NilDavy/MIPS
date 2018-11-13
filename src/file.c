#include "file.h"

/** Creer file **/
file_jeu_instruction creer_file(void){
	return NULL;
}

/** Verif file vide **/
int file_vide(file_jeu_instruction f){
	return !f;
}

/** visaliser une file **/
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

/** Ecrire une file dans un fichier exterieur **/
void ecrire_file(file_jeu_instruction f,FILE*a){
	file_jeu_instruction g=NULL;
	g=f->suiv;
	while (g!=f){
		fprintf(a, "Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
		g=g->suiv;
	}
	fprintf(a, "Ligne : %d\t %s\t %s\n\n" ,g->ligne, g->identifiant,g->caractere);
	
}

/** ajout d'un maillon a la file **/
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

/** retire d'un maillon a la file **/
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

/** liberation de la memoire de la file **/

void liberer_file(file_jeu_instruction f){
	if(file_vide(f)){
		return;
	}
	file_jeu_instruction g= f->suiv	;
 	f->suiv = NULL;	
	while(g!=NULL){
		g=supprimer_tete(g);
	}
	
}

/** liberation de la memoire d'un maillon de la file **/
file_jeu_instruction supprimer_tete(file_jeu_instruction g){
	file_jeu_instruction c;
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

/*Supprime un commentaire*/
file_jeu_instruction supprime_commentaire(file_jeu_instruction f){
	file_jeu_instruction g=f->suiv;
	file_jeu_instruction h=f;
	while(g!=f){
		file_jeu_instruction i;
		if(!strcmp(g->identifiant,"Commentaire")){
			i=g;
			h->suiv=g->suiv;
			g=g->suiv;
			free(i);
		}
		else{
			g=g->suiv;
			h=h->suiv;
		}
	}

	file_jeu_instruction i;
	if(!strcmp(g->identifiant,"Commentaire")){
		i=g;
		h->suiv=g->suiv;
		g=g->suiv;
		free(i);
	}

	return f;
}

/*Modifie instruction en etiquette si apres il y a un : ou retour ligne sauf nop*/
file_jeu_instruction modifie_instruction(file_jeu_instruction f){
	file_jeu_instruction g=f->suiv->suiv;
	file_jeu_instruction h=f->suiv;
	while(g!=f){
		if(strcmp(g->caractere,":")==0&&strcmp(h->identifiant,"Instruction")==0){
			strcpy(h->identifiant,"Etiquette");
		}
		if(strcmp(g->identifiant,"Retour à la ligne")==0&&strcmp(h->identifiant,"Instruction")==0){
			strcpy(h->identifiant,"Renvoie vers une étiquette");
		}
		g=g->suiv;
		h=h->suiv;
	}
	if(strcmp(g->caractere,":")==0&&strcmp(h->identifiant,"Instruction")==0){
		strcpy(h->identifiant,"Etiquette");
	}
	if(strcmp(g->identifiant,"Retour à la ligne")==0&&strcmp(h->identifiant,"Instruction")==0){
		strcpy(h->identifiant,"Renvoie vers une étiquette");
	}
	return f;
}


/*Verif si le nom des etiquettes colle avec celui des renvoies vers etiquette*/
void verif_renvoie_vers_etiquette(file_jeu_instruction*f,file_jeu_instruction*file_erreur){
	int a;
	file_jeu_instruction file_renvoie=creer_file();
	file_jeu_instruction file_etiquette=creer_file();
	file_jeu_instruction g=(*f)->suiv;
	file_jeu_instruction h;
	while(g!=(*f)){
		if(!strcmp(g->identifiant,"Renvoie vers une étiquette")){
			file_renvoie=enfiler(g->identifiant, g->caractere,g->ligne, file_renvoie);
		}
		if(!strcmp(g->identifiant,"Etiquette")){
			file_etiquette=enfiler(g->identifiant, g->caractere,g->ligne, file_etiquette);
		}
		g=g->suiv;
	}
	if(!strcmp(g->identifiant,"Renvoie vers une étiquette")){
		file_renvoie=enfiler(g->identifiant, g->caractere,g->ligne, file_renvoie);
	}
	if(!strcmp(g->identifiant,"Etiquette")){
		file_etiquette=enfiler(g->identifiant, g->caractere,g->ligne, file_etiquette);
	}
	if(file_vide(file_renvoie)){return;}
	else{
		if(file_vide(file_etiquette)){
			*file_erreur=enfiler("Erreur orthographe renvoie sur etiquette", "Pas d'étiquette présente", 1, *file_erreur);
			return;
			}
		g=file_renvoie->suiv;
		while(g!=file_renvoie){
			a=0;
			h=file_etiquette->suiv;
			while(h!=file_etiquette){
				if(!strcmp(h->caractere,g->caractere)){
					a=1;
				}
				h=h->suiv;
			}
			if(!strcmp(h->caractere,g->caractere)){
					a=1;
				}
			if(a==0){
				*file_erreur=enfiler("Erreur orthographe renvoie sur etiquette", g->caractere, g->ligne, *file_erreur);
			}
			g=g->suiv;
		}
		a=0;
		while(h!=file_etiquette){
			if(!strcmp(h->caractere,g->caractere)){
				a=1;
			}
			h=h->suiv;
		}
		if(!strcmp(h->caractere,g->caractere)){
			a=1;
		}
		if(a==0){
			*file_erreur=enfiler("Erreur orthographe renvoie sur etiquette", g->caractere, g->ligne, *file_erreur);
		}
	}
	liberer_file(file_renvoie);
	liberer_file(file_etiquette);
}


file_jeu_instruction verif_delimiteur_suite(file_jeu_instruction f,file_jeu_instruction*file_erreur){
	file_jeu_instruction g=f->suiv->suiv;
	file_jeu_instruction h=f->suiv;
	while(g!=f){
		if(strcmp(g->identifiant,"Délimiteur")==0&&strcmp(h->identifiant,"Délimiteur")==0&&strcmp(h->caractere,",")!=0){
			*file_erreur=enfiler("Erreur ", "2 délimiteurs à la suite", g->ligne, *file_erreur);
		}
		g=g->suiv;
		h=h->suiv;
	}
	if(strcmp(g->identifiant,"Délimiteur")==0&&strcmp(h->identifiant,"Délimiteur")==0&&strcmp(h->caractere,",")!=0){
		*file_erreur=enfiler("Erreur ", "2 délimiteurs à la suite", g->ligne, *file_erreur);

	}
	return f;
}

file_jeu_instruction verif_remplacement_ecriture_registre(file_jeu_instruction f,file_jeu_instruction*file_erreur,Liste_hach*tab_registre){
	file_jeu_instruction g=f->suiv;
	char*mot=NULL;
	while(g!=f){
		if(strcmp(g->identifiant,"Registre")==0){
			if(rec_hachage_nbparam(g->caractere,tab_registre[hachage(g->caractere, dim_tab_registre)])==1){
				*file_erreur=enfiler("Erreur ", "Interdit d'écrire dans ce registre", g->ligne, *file_erreur);
			}
			else{
				mot=rec_hachage_type(g->caractere,tab_registre[hachage(g->caractere, dim_tab_registre)]);
				strcpy(g->caractere,mot);
			}
		}
		g=g->suiv;
	}
	return f;
}
