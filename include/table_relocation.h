#include "analyse_syntaxique.h"
#ifndef table_relocation_h
#define table_relocation_h
enum type_reloc {R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16};


struct cel_reloc{
    int offset;
    enum type_reloc type;
    char value[200]; /*On a limité à 200 la longueur d'une ligne*/
    struct cellulesymb* symb;
    struct cel_reloc* suiv;
};
typedef struct cel_reloc* table_reloc;
/*La table de relocation sera stocker sous la forme d'une file :
elle pointera vers le dernier élément et le suivant sera le premier*/
table_reloc ajoutElement(table_reloc r, int offset, enum type_reloc tr, char* value, struct cellulesymb* symbole);
int table_vide(table_reloc r);
void visualiser_table(table_reloc r);
void ecrire_table(table_reloc r, FILE* a);
void liberer_table(table_reloc r);

table_reloc remplirTableRelocationText(file_text co_text, file_data co_data, file_bss co_bss, file_symb co_symb, Liste_hach hach_inst);

#endif
