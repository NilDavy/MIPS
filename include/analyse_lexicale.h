#ifndef analyse_lexicale_h
#define analyse_lexicale_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "file.h"
#include "hachage.h"
#include "global.h"
#include "notify.h"
#include "lex.h"


char* analyse_lexicale(char*,char*,char*, Liste_hach*tab_registre , Liste_hach*tab_instruction,file_jeu_instruction*file,unsigned int nlines,file_jeu_instruction*file_erreur,int*compteur);
void verif_directive(char*,char*,char*,file_jeu_instruction*file,unsigned int nlines,file_jeu_instruction*file_erreur,int*compteur);
void verif_registre(char*, Liste_hach*tab_registre,file_jeu_instruction*file,unsigned int nlines,file_jeu_instruction*file_erreur);
int verif_delimitateur(char*,char*,char*);
void verif_caractere(char*token,char*mot2,char*current_address,char*delimiteur,Liste_hach*tab_instruction,file_jeu_instruction *file,unsigned int nlines,file_jeu_instruction*file_erreur);
void verif_numero(char*token,file_jeu_instruction *file,unsigned int nlines,file_jeu_instruction*file_erreur);
void verif_chaine_de_caractere(char*token,char*current_address,char*delimiteur,file_jeu_instruction *file,unsigned int nlines,file_jeu_instruction*file_erreur,int*compteur);
#endif /* analyse_lexicale_h */
