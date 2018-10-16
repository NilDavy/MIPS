#include "analyse_syntaxique.h"


collection creer_collection(void){
  return NULL;
}
int collection_vide(collection f){
  return !f;
}
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

collection enfiler_c(char*nomInstruction,int nbOperande,int line, file_jeu_instruction operande, collection f){
  collection e = calloc(1,sizeof(*e));
  if(e == NULL) {
    return NULL;
  }
  strcpy(e->nomInst, nomInstruction);
  e->nbOp = nbOperande;
  e->ligne = line;
  e->op = creer_file();
  while(!file_vide(operande))
  {
      char* nom;
      char* carac;
      int ligne = defiler(&operande, nom, carac);
      e->op = enfiler(nom, carac, ligne, e->op);

  }
  if(file_vide(f)){
    e->suiv=e;
    return e;
  }
  e->suiv=f->suiv;
  f->suiv=e;
  return e;
}
void liberer_collection(collection f){

}
void retire_maillon_c(collection* pf,int a){

}
collection supprimer_tete_c(collection c){

}
