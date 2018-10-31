#include "hachage.h"

/* Creation d'une liste de hachage */
Liste_hach creer_liste_hachage(void) {return NULL;}

/* Verification si la liste de hachage est vide
 Renvoie 1 si la liste est vide, 0 sinon */
int est_vide_hachage(Liste_hach L) {return !L;}

/* Ajout d'un element en tete de la liste de hachage */
Liste_hach ajout_tete_hachage(instruction e,int nbop, char car, Liste_hach L){
	Liste_hach p=(Liste_hach) calloc(1,sizeof(*p));
	if (p==NULL) return NULL;
	p->val=malloc(strlen(e));
	p->val= (p->val? strcpy(p->val,e) : NULL);   /*evite les warning de compilation*/
	p->nombreOperande = nbop; 
	p->typeInst = car;
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
	while(!est_vide_hachage(p)&&(strcasecmp(p->val,e)!=0)){
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
	while(!est_vide_hachage(p) && (strcasecmp(p->val, e) != 0))
	{
		p = p->suiv;
	}
	return p->nombreOperande; 
}
/* Suppression d'un element en tete de la liste de hachage */
Liste_hach supprimer_tete_hachage(Liste_hach l) {
	Liste_hach c = NULL;
	if (est_vide_hachage(l)) {
		return NULL;
	}
	free(l->val);
	c = l->suiv;
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
	free(l);
}


Liste_hach*creation_liste_registre(){
	FILE* fregistre = NULL;
	int i;
	int n;
	char mot[8];
	Liste_hach*tab_registre=NULL;
	tab_registre = calloc(dim_tab_registre, sizeof(*tab_registre));

	if (tab_registre == NULL) {
		fprintf( stderr, "Memory error : tab_registre\n" );
		exit(EXIT_FAILURE);
	}
	fregistre=fopen("doc_tab_hachage/registres.txt", "rt");
	if (fregistre == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier registre\n" );
		exit(EXIT_FAILURE);
	}

	for(i=0;i<64;i++){
		fscanf(fregistre,"%s", mot);
		n=hachage(mot, dim_tab_registre);
		tab_registre[n]=ajout_tete_hachage(mot,0,'a',tab_registre[n]);
	}
	/*visualiser_tab_hachage(tab_registre, dim_tab_registre);*/


	fclose(fregistre);
	return tab_registre;
}

Liste_hach*creation_liste_instruction(){
	FILE* finstruction = NULL;
	int i;
	int n;
	char mot[8];
	char car;
	int nbOp;
	Liste_hach*tab_instruction=NULL;
	tab_instruction = calloc(dim_tab_instruction, sizeof(*tab_instruction));

	if (tab_instruction == NULL) {
		fprintf( stderr, "Memory error : tab_instruction\n" );
		exit(EXIT_FAILURE);

	}
	finstruction=fopen("doc_tab_hachage/instructions.txt", "rt");
	if (finstruction == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier instruction\n" );
		exit(EXIT_FAILURE);
	}

	for(i=0;i<29;i++){

		fscanf(finstruction,"%s %d %c", mot, &nbOp, &car);
		n=hachage(mot, dim_tab_instruction);
		tab_instruction[n]=ajout_tete_hachage(mot, nbOp, car,tab_instruction[n]);
	}
	/*visualiser_tab_hachage(tab_instruction, dim_tab_instruction);*/

	fclose(finstruction);
	return tab_instruction;
}

void visualiser_tab_hachage(Liste_hach*tab,int n){
	int i;
	for(i=0;i<n;i++){
		visualiser_liste_hachage(tab[i]);
	}
}

void visualiser_liste_hachage(Liste_hach l) {
	Liste_hach c = NULL;
	if (est_vide_hachage(l)) {
		puts("liste vide\n");
		return;
	}
	for (c = l; !est_vide_hachage(c); c = c->suiv) {
		printf("%s ",c->val);
	}
	printf("\n");
}
