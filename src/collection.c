#include "collection.h"


liste creerliste(void){
  return NULL;
}
int liste_vide(liste l){
  return !l;
}
/*
void visualiser_collection(collection f){
  collection g=NULL;
	if (collection_vide(f)){
		printf("File vide\n");
		return;
	}

	g=f->suiv;
	while (g!=f){
		printf("Instruction : %s\n\tLigne : %d\n\tDecalage : %d \n\tNombre d'opérande : %d\n\t Liste des opérandes :\n" ,g->nomInst, g->ligne,g->decalage, g->nbOp);
    visualiser_file(g->op);
		g=g->suiv;
	}
	printf("Instruction : %s\n\tLigne : %d\n\tDecalage : %d \n\tNombre d'opérande : %d\n\t Liste des opérandes :\n" ,g->nomInst, g->ligne,g->decalage, g->nbOp);
  visualiser_file(g->op);
}
*/
liste ajout_inst(char*nom,int op,int line, unsigned int dec, file_jeu_instruction operande, liste l){
  celluleInst *e = calloc(1,sizeof(*e));
  if(e == NULL) {
    return NULL;
  }
  strcpy(e->nomInst, nom);
  e->nbOp = op;
  e->ligne = line;
  e->op = creer_file();
  while(!file_vide(operande))
  {
      char nom[longueur_max];
      char carac[longueur_max];
      int ligne = defiler(&operande, nom, carac);
      e->op = enfiler(nom, carac, ligne, e->op);

  }

  liste a = calloc(1, sizeof(*a));
  a->val = e;
  a->suiv = l;
  return a;

}

liste ajout_data(char *nom, int line, unsigned int dec, void *op, int type, liste l){
	celluleData *e = calloc(1, sizeof(*e));
	if (e == NULL)
		return NULL;
	strcpy(e->nomInst, nom);
	e->ligne = line;
	e->decalage = dec;
	switch(type)
	{
		case 0:
			(e->op).c =*((char *)op) ;
			break;
		case 1:
			(e->op).uc = *((unsigned char *)op);
			break;
		case 2:
			(e->op).i = *((int *)op);
			break;
		case 3:
			(e->op).ui =*((unsigned int *)op);
			break;
		case 4:
			(e->op).s = calloc(strlen(*((char **)op)),sizeof(char)); 
			strcpy((e->op).s, *((char **)op));
			break;
	}
	liste a = calloc (1, sizeof(*a));
	if(a == NULL)
		return NULL;
	a->val = e;
	a->suiv = l;
	return a;	
}

liste ajout_symb(char *nom, int line, enum section S, unsigned int dec, liste l)
{
	celluleSymb *e = calloc(1, sizeof(*e));
	if(e == NULL)
		return NULL;
	strcpy(e->nom, nom);
	e->ligne = line;
	e->section = S;
	e->decalage = dec;

	liste a = calloc(1, sizeof(*a));
	if( a == NULL)
		return NULL;
	a->val = e;
	a->suiv = l;
	return a;
}
void liberer(liste l, int bool_inst)
{
	liste f = l;
	if(liste_vide(l))
		return;
	liste e = f->suiv;
	if(!bool_inst)
	{
		if(e == NULL)
			free(f);
		else
		{
			while(e != NULL)
			{
				free(f);
				f = e;
				e = f->suiv;
			}
			free(f);
		}

	}
	else if(bool_inst == 1)
	{	
		if(e == NULL)
		{
			celluleInst* cel =(celluleInst*)f->val;
			liberer_file(cel->op);
			free(f);
		}
	}
}
