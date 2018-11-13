#include "hachage.h"

/* Creation d'une liste de hachage */
Liste_hach creer_liste_hachage(void) {return NULL;}

/* Verification si la liste de hachage est vide
 Renvoie 1 si la liste est vide, 0 sinon */
int est_vide_hachage(Liste_hach L) {return !L;}

/* Ajout d'un element en tete de la liste de hachage */
Liste_hach ajout_tete_hachage(instruction e,int nbop, char*car,char*typeinstruction1,char*typeinstruction2,char*typeinstruction3, Liste_hach L){
	Liste_hach p=(Liste_hach) calloc(1,sizeof(*p));
	if (p==NULL) return NULL;
	strcat(p->val,e);
	p->nombreOperande = nbop;
	strcpy(p->typeInst,car);
	strcat(p->type1,typeinstruction1);
	strcat(p->type2,typeinstruction2);
	strcat(p->type3,typeinstruction3);
	/*printf("LLLL ==== %s\n",L);*/
	p->suiv=L;
	return p;
}

/* Hachage du nom de l'instruction */
unsigned int hachage(char* mot, int dim_tab_hach){
	int i=0;
	unsigned long h=0,a=1;
	while(mot[i]!='\0'){
		h=(h+mot[i]*a)%dim_tab_hach;
		a=a*31;
		i=i+1;
	}
	return h;
}

/* Recherche d'un element dans la table de hachage*/
int rech_hachage(instruction e, Liste_hach L){
	Liste_hach p=L;
	while(!est_vide_hachage(p)&&(strcmp(p->val,e)!=0)){
		p=p->suiv;
	}
	if(p==NULL){
		return -1;
	}
	return 1;
}
/* Retroune le nombre de paramètre que prend l'instrucution */
int rec_hachage_nbparam(instruction e, Liste_hach L)
{
	Liste_hach p = L;
	while(!est_vide_hachage(p) && (strcmp(p->val, e) != 0))
	{
		p = p->suiv;
	}
	return p->nombreOperande;
}

/* Retroune le type de l'instruction*/
char*rec_hachage_type(instruction e, Liste_hach L)
{
	Liste_hach p = L;
	while(!est_vide_hachage(p) && (strcmp(p->val, e) != 0))
	{
		p = p->suiv;
	}
	return p->typeInst;
}

/* Retroune le type des opérandes*/
void rec_hachage_type_instruction(instruction e, Liste_hach L,char*mot1,char*mot2,char*mot3)
{
	Liste_hach p = L;
	while(!est_vide_hachage(p) && (strcmp(p->val, e) != 0))
	{
		p = p->suiv;
	}
	strcpy(mot1,p->type1);
	strcpy(mot2,p->type2);
	strcpy(mot3,p->type3);
}

/* Suppression d'un element en tete de la liste de hachage */
Liste_hach supprimer_tete_hachage(Liste_hach l) {
	Liste_hach c = NULL;
	if (est_vide_hachage(l)) {
		return NULL;
	}
	c = l->suiv;
	l->suiv=NULL;
	free(l);
	return c;
}


void liberer_tab_hachage(Liste_hach l[], int len) {
	int i;
	for (i=0; i<len;i++) {
		Liste_hach c = NULL;
		c = l[i];
		while (c != NULL) {
			c = supprimer_tete_hachage(c);
		}
	}
}


void creation_liste_registre(Liste_hach*tab_registre,int dim){
	FILE* fregistre = NULL;
	int i;
	int n;
	int autorisation;
	char reg1[8]=" ";
	char reg2[8]=" ";
	fregistre=fopen("doc_tab_hachage/registres.txt", "rt");
	if (fregistre == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier registre\n" );
		exit(EXIT_FAILURE);
	}
	for(i=0;i<nbre_registre;i++){
		fscanf(fregistre,"%s %s %d", reg1,reg2,&autorisation);
		/*printf("%s %s %d\n",reg1,reg2,autorisation);*/
		/*getchar();*/
		n=hachage(reg1, dim);
		tab_registre[n]=ajout_tete_hachage(reg1,autorisation,reg2,"","","",tab_registre[n]);
	}
	/*visualiser_tab_hachage(tab_registre, dim_tab_registre);*/


	fclose(fregistre);
}

void creation_liste_instruction(Liste_hach*tab_instruction,int dim){
	FILE* finstruction = NULL;
	int i;
	int n;
<<<<<<< HEAD
	char*mot=calloc(8,sizeof(char));
=======
	char* mot= calloc(5, sizeof(char));
>>>>>>> 496bb7d920b30c88de63130ff489b92f07918045
	char type[1];
	int nbOp;

	finstruction=fopen("doc_tab_hachage/instructions2.txt", "rt");
	if (finstruction == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier instruction\n" );
		exit(EXIT_FAILURE);
	}

	for(i=0;i<nbre_instruction;i++){
		fscanf(finstruction,"%s %d %s", mot, &nbOp, type);
<<<<<<< HEAD
		/*printf("instruc: %s, nbop: %d, type: %s\n",mot,nbOp,type);*/
		/*printf("%c\n",mot[0]);*/
		/*getchar();*/
		
=======
		/*printf("instruc: %s, nbop: %d, type: %s\n",mot,nbOp,type);
		getchar();*/

>>>>>>> 496bb7d920b30c88de63130ff489b92f07918045
		n=hachage(mot, dim);
		if(nbOp==1){
			char*mot1=calloc(25,sizeof(char));
			fscanf(finstruction,"%s",mot1);
			tab_instruction[n]=ajout_tete_hachage(mot, nbOp, type,mot1,"","",tab_instruction[n]);
			/*printf("mot %s\n",mot1);*/
			free(mot1);
		}
		else{
			if(nbOp==2){
				char*mot1=calloc(25,sizeof(char));
				char*mot2=calloc(25,sizeof(char));
				fscanf(finstruction,"%s %s",mot1,mot2);
				tab_instruction[n]=ajout_tete_hachage(mot, nbOp, type,mot1,mot2,"",tab_instruction[n]);
				/*printf("mot %s %s\n",mot1, mot2);*/
				free(mot1);
				free(mot2);
			}
			else{
				if(nbOp==3){
					char*mot1=calloc(25,sizeof(char));
					char*mot2=calloc(25,sizeof(char));
					char*mot3=calloc(25,sizeof(char));
					fscanf(finstruction,"%s %s %s",mot1,mot2,mot3);
					tab_instruction[n]=ajout_tete_hachage(mot, nbOp, type,mot1,mot2,mot3,tab_instruction[n]);
					/*printf("mot %s %s %s\n",mot1, mot2,mot3);*/
					free(mot1);
					free(mot2);
					free(mot3);
				}
				else{
					tab_instruction[n]=ajout_tete_hachage(mot, nbOp, type,"","","",tab_instruction[n]);
				}
			}
		}
	}
	free(mot);
	/*visualiser_tab_hachage(tab_instruction, dim_tab_instruction);*/
	fclose(finstruction);
}

void visualiser_tab_hachage(Liste_hach*tab,int n){
	int i;
	for(i=0;i<n;i++){
		visualiser_liste_hachage(tab[i]);
	}
	printf("\n");
	return;
}

void visualiser_liste_hachage(Liste_hach l) {
	Liste_hach c = NULL;
	if (est_vide_hachage(l)) {
		puts("liste vide");
		return;
	}
	c=l;
	while(c!=NULL) {
		printf("%s\t",c->val);
		printf("%s\t",c->typeInst);
		printf("%d\t",c->nombreOperande);
		printf("%s\t",c->type1);
		printf("%s\t",c->type2);
		printf("%s\n",c->type3);
		c=c->suiv;
	}
	printf("\n");
	return;
}
