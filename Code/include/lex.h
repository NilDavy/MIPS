#ifndef _LEX_H_
#define _LEX_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "hachage.h"
#include "global.h"
#include "liste.h"
#include "notify.h"
#include "analyse_lexicale.h"


void lex_read_line( char *, int, Liste_hach*tab_registre,Liste_hach*tab_instruction,Liste_jeu_instruction*Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur);
void lex_load_file( char *, unsigned int *,Liste_hach*tab_registre,Liste_hach*tab_instruction,Liste_jeu_instruction*Liste,Liste_jeu_instruction*Liste_erreur);
char* getNextToken( char* ,char*);

#endif /* _LEX_H_ */

