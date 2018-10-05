#ifndef _LEX_H_
#define _LEX_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "hachage.h"
#include "global.h"
#include "file.h"
#include "notify.h"
#include "analyse_lexicale.h"


void lex_read_line( char *, int, Liste_hach*tab_registre,Liste_hach*tab_instruction,file_jeu_instruction*file,unsigned int nlines,file_jeu_instruction*file_erreur);
void lex_load_file( char *, unsigned int *,Liste_hach*tab_registre,Liste_hach*tab_instruction,file_jeu_instruction*file,file_jeu_instruction*file_erreur);
char* getNextToken( char* ,char*);

#endif /* _LEX_H_ */

