#ifndef analyse_syntaxique_h
#define analyse_syntaxique_h

#include "file.h"
#define longueur_str 30
struct cellule2 {
	char nomInst[longueur_str];
  int nbOp;
  int ligne;
  unsigned int decalage;
  file_jeu_instruction op;
	struct cellule2 * suiv;};

typedef struct cellule2* collection;

collection creer_collection(void);
int collection_vide(collection f);
void visualiser_collection(collection f);

collection enfiler_c(char*nomInstruction,int nbOperande,int line, file_jeu_instruction operande, collection f);

void liberer_collection(collection f);
void retire_maillon_c(collection* pf,int a);

#endif
