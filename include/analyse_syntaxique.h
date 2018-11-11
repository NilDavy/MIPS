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

file_text pseudo_instruction(int *cpt_text,char*instruction,file_text *co_text,file_jeu_instruction*file_erreur,int nbop,file_jeu_instruction g,int ligne);
void verif_operande(file_text co_text,file_jeu_instruction*file_erreur,Liste_hach*tab_instruction);
void verif_registre_ope(file_jeu_instruction*file_erreur,file_jeu_instruction f,file_text a);
void verif_baseoffset_ope(file_jeu_instruction*file_erreur,file_jeu_instruction f,file_text a);
void verif_immediate_ope(file_jeu_instruction*file_erreur,file_jeu_instruction f,file_text a);
void verif_shiftamount_ope(file_jeu_instruction*file_erreur,file_jeu_instruction f,file_text a);
void verif_relatif_ope(file_jeu_instruction*file_erreur,file_jeu_instruction f,file_text a);

#endif
