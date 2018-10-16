#ifndef file_h
#define file_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#define longueur_max 200

struct cellule1 {
	char identifiant[longueur_max];
	char caractere[longueur_max];
	int ligne;
	struct cellule1 * suiv;};

typedef struct cellule1* file_jeu_instruction;


file_jeu_instruction creer_file(void);
int file_vide(file_jeu_instruction f);
void visualiser_file(file_jeu_instruction f);


file_jeu_instruction enfiler(char*nom,char*carac,int line, file_jeu_instruction f);
void ecrire_file(file_jeu_instruction f,FILE*a);

/* PRECONDITION : file non vide
 (signifie "INTERDIT D'APPELLER SI FILE VIDE")*/
int defiler(file_jeu_instruction* pf,char*nom,char*carac);
void liberer_file(file_jeu_instruction f);
file_jeu_instruction supprimer_tete(file_jeu_instruction g);

file_jeu_instruction supprime_commentaire(file_jeu_instruction f);
file_jeu_instruction modifie_instruction(file_jeu_instruction f);
void verif_renvoie_vers_etiquette(file_jeu_instruction*f,file_jeu_instruction*file_erreur);
#endif /* file_h */
