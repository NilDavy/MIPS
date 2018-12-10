#ifndef write_binaire_h
#define write_binaire_h


#include <stdlib.h>
#include <string.h>

#include "table_relocation.h"
#include <pelf/pelf.h>
#include <pelf/section.h>
#include <pelf/elf-mips32.h>
#include <pelf/elf.h>
#include "file_data.h"
#include "file.h"
#include "file_text.h"
#include "file_symb.h"



int elf_get_sym_index_from_name(section symtab, section strtab,  char* sym_name);
int elf_get_sym_index_from_name2(section symtab, section shstrtab, section strtab, char* sym_name);
section make_shstrtab_section( void );
section make_strtab_section(file_symb co_symb);
section make_data_section( int data_prog[], int nb_data );
section make_text_section( int instructions[], int nb_inst);
section make_bss_section( int bss_prog);
section make_symtab_section(section shstrtab, section strtab,  file_symb co_symb);
section make_rel32_section(char *relname, table_reloc reloc, section symtab, section shstrtab, section strtab);
void swap(char*s2);
void creer_nom_fichier(char*file,char*name);
void creer_data_value(int*data_value,int*data_type,int nbdata,file_data co_data);
void creer_hexa_data(file_jeu_instruction*f_data_bin,int*data_type,int*data_value,int nbdata);
void nombre_mot_data(int*nbdata);
void ordre_data(file_jeu_instruction f_data_bin,int*data_prog,int nbdata);
void creer_section_data(section*data,int nbdata,file_data co_data);
void creer_section_bss(section*bss,int cptbss);
void creer_section_text(section*text,int nbtext,file_text co_text,Liste_hach*tab_instruction,Liste_hach*tab_registre);

void construction_R(int*compteur,int*text_prog,int nbre,file_text f,int n,Liste_hach*tab_instruction,Liste_hach*tab_registre,int*ligne);
void information(char*oppcode,char*fonction,char*a1,char*a2,char*a3,Liste_hach L,instruction e);
long int convertir_bin_dec(char*mot,int n);
void construction_I(int*compteur,int*text_prog,int nbre,file_text f,int n,Liste_hach*tab_instruction,Liste_hach*tab_registre,int*ligne);
void convertir_dec_bin(long int n,char*mot,int taille);
void construction_J(int*compteur,int*text_prog,int nbre,file_text f,int n,Liste_hach*tab_instruction,Liste_hach*tab_registre,int*ligne);
#endif
