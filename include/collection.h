#ifndef collection_h
#define collection_h

#include "file.h"
#define longueur_str 30

enum section{TEXT, DATA, BSS, INIT};

struct generic{
	void* val;
	struct generic *suiv;
};

typedef struct generic* liste;
/* Cellule pour la collection de .text */
struct celluleInst {
	char nomInst[longueur_str];
  	int nbOp;
  	int ligne;
  	unsigned int decalage;
 	file_jeu_instruction op;
};

typedef struct celluleInst celluleInst;
/* Cellule pour les collections de data et bss */
/* On stock un opérande par chainon de la liste de data*/
struct celluleData{
	char nomInst[longueur_str];
	int ligne;
	unsigned int decalage;
	union
	{
		char c;
		unsigned char uc;
		int i;
		unsigned int ui;
		char *s;

	}op;	

};
typedef struct celluleData celluleData;



/* Cellule pour la collection des symboles */
struct celluleSymb{
	char nom[50]; /* Limite volontaire de la taille du nom des symboles a 50 carac*/
	int ligne;
	enum section section;
	unsigned int decalage;
};
typedef struct celluleSymb celluleSymb;

liste creer_liste();
int liste_vide(liste l);
void visualiser_liste_inst(liste l);
void visualiser_liste_data(liste l);
void visualiser_liste_symb(liste l);
liste ajout_inst(char* nom, int op, int line, unsigned int dec, file_jeu_instruction operande, liste l);
liste ajout_data(char *nom, int line, unsigned int dec, void* op, int type, liste l);
liste ajout_symb(char *nom, int line, enum section S, unsigned int dec, liste l);
void liberer(liste l, int bool_inst); /*boolInst vaut 1 si c'est une liste d'instruction, 0 sinon */
#endif
