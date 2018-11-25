#ifndef file_data_h
#define file_data_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#define longueur_str 30


struct celluledata{
	char nomInst[longueur_str];
	int ligne;
	unsigned int decalage;
	int type;
	int etiquette;
	union
	{
		char c;
		unsigned char uc;
		int i;
		unsigned int ui;
		char *s;
		
	}op;
	struct celluledata * suiv;};
typedef struct celluledata*file_data;



file_data creerfile_data();
int file_vide_data(file_data l);

void visualiser_file_data(file_data l);

file_data ajout_data(char *nom, int line, unsigned int dec, char* op, int type, file_data l,int etiq);

void liberer_data(file_data l);

file_data supprimer_tete_data(file_data g);
void ecrire_file_data(file_data f,FILE*a);
#endif
