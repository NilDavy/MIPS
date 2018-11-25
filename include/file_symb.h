#ifndef file_symb_h
#define file_symb_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#define longueur_str 30


struct cellulesymb{
	char nom[200];
	int ligne;
	char section[5];
	unsigned int decalage;
	struct cellulesymb * suiv;};
typedef struct cellulesymb * file_symb;



file_symb creerfile_symb();
int file_vide_symb(file_symb l);

void visualiser_file_symb(file_symb l);

file_symb ajout_symb(char *name, int line, unsigned int dec,char*sec, file_symb f);

void liberer_symb(file_symb l);

file_symb supprimer_tete_symb(file_symb g);
char*defiler_symb(file_symb*f);
void ecrire_file_symb(file_symb f,FILE*a);
int est_dans_file(char* symb, file_symb f);
/* Ne pas utiliser avant d'avoir vérifier que le symbole est bien dans la collection
de symbole avec la fct precedente */
struct cellulesymb* recuperer_cellule_symb(char* symb, file_symb s);
#endif
