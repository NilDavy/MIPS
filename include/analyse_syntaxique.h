#ifndef analyse_syntaxique_h
#define analyse_syntaxique_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "analyse_lexicale.h"
#include "hachage.h"
#include "file_bss.h"
#include "file_data.h"
#include "file_text.h"
#include "file_symb.h"

enum section{TEXT, DATA, BSS, INIT};

void analyse_syntaxique(Liste_hach*tab_instruction,file_jeu_instruction*file, file_jeu_instruction* file_erreur, file_text *co_text, file_data *co_data, file_bss *co_bss, file_symb *co_symb,file_symb *co_text_attente, file_symb *co_data_attente, file_symb*co_bss_attente);

file_jeu_instruction processText(file_jeu_instruction file, file_text *co_text, int *cpt_text, file_jeu_instruction* file_erreur, file_symb *co_symb,file_symb *co_text_attente,Liste_hach*tab_instruction);

file_jeu_instruction processData(file_jeu_instruction file, file_data *co_data, int* cpt_data, file_jeu_instruction *file_erreur, file_symb *co_symb, file_symb *co_data_attente);

file_jeu_instruction processBss(file_jeu_instruction file, file_bss *co_bss, int* cpt_bss, file_jeu_instruction *file_erreur, file_symb *co_symb, file_symb *co_bss_attente);


#endif
