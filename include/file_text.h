#ifndef file_text_h
#define file_text_h

#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#define longueur_str 30


struct celluletext{
	char nomInst[longueur_str];
	int nbOp;
	int ligne;
	unsigned int decalage;
	file_jeu_instruction op;
	struct celluletext * suiv;};
typedef struct celluletext*file_text;



file_text creerfile_text();
int file_vide_text(file_text l);

void visualiser_file_text(file_text l);

file_text ajout_text(char *nom, int nbop,int line, unsigned int dec, file_text l,file_jeu_instruction g);

void liberer_text(file_text l);

file_text supprimer_tete_text(file_text g);
void ecrire_file_text(file_text f,FILE*a);
file_text modif_etiquette(file_text co_text);
#endif
