#ifndef file_bss_h
#define file_bss_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#define longueur_str 30


struct cellulebss{
	char nomInst[longueur_str];
	int ligne;
	unsigned int decalage;
	int type;
	union
	{
		char c;
		unsigned char uc;
		int i;
		unsigned int ui;
		char *s;
		
	}op;
	struct cellulebss * suiv;};
typedef struct cellulebss*file_bss;



file_bss creerfile_bss();
int file_vide_bss(file_bss l);

void visualiser_file_bss(file_bss l);

file_bss ajout_bss(char *nom, int line, unsigned int dec, char*op, int type, file_bss l);

void liberer_bss(file_bss l);

file_bss supprimer_tete_bss(file_bss g);
void ecrire_file_bss(file_bss f,FILE*a);
#endif
