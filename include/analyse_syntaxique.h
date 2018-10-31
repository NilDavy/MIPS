#ifndef analyse_syntaxique_h
#define analyse_syntaxique_h


#include "analyse_lexicale.h"
#include "collection.h"


void analyse_syntaxique(Liste_hach*tab_instruction,file_jeu_instruction file, file_jeu_instruction* file_erreur, liste *co_text, liste *co_data, liste *co_bss, liste *co_symb);

file_jeu_instruction processText(file_jeu_instruction file, liste *co_text, int *cpt_text, file_jeu_instruction* file_erreur, liste *co_symb);

file_jeu_instruction processData(file_jeu_instruction file, liste *co_data, int* cpt_data, file_jeu_instruction *file_erreur, liste *co_symb);

file_jeu_instruction processBss(file_jeu_instruction file, liste *co_bss, int* cpt_bss, file_jeu_instruction *file_erreur, liste *co_symb);


#endif
