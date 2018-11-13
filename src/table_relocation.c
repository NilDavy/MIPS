#include "table_relocation.h"


table_reloc ajoutElement(table_reloc r, int offset, enum type_reloc tr, char* value, struct cellulesymb* symbole){
    if(table_vide(r)){
      r = calloc(1, sizeof(*r));
      if(r == NULL){
        printf("Erreur allocation table ralocation");
        return NULL;
      }
      r->offset = offset;
      r->type = tr;
      strcpy(r->value, value);
      r->suiv = r;
      r->symb = symbole;
      return r;
    }
    else{
      table_reloc a = calloc(1, sizeof(*a));
      if(a == NULL)
      {
        printf("Erreur allocation table ralocation");
        return NULL;
      }
      a->offset = offset;
      a->type = tr;
      strcpy(a->value, value);
      a->symb = symbole;
      a->suiv = r->suiv;
      r->suiv = a;
      return a;
    }
}
int table_vide(table_reloc r){
    return !r;
}
void visualiser_table(table_reloc r){
  if(table_vide(r))
    return;
  if(r->suiv == r)
  {
    if(r->type == R_MIPS_32){
      printf("%d R_MIPS_32 %s\n", r->offset, r->value);
    }else if (r->type == R_MIPS_26){
      printf("%d R_MIPS_26 %s\n", r->offset, r->value);
    }
    else if (r->type == R_MIPS_HI16){
      printf("%d R_MIPS_HI16 %s\n", r->offset, r->value);
    }
    else if (r->type == R_MIPS_LO16){
      printf("%d R_MIPS_LO16 %s\n", r->offset, r->value);
    }
    return;
  }
  table_reloc f = r->suiv;
  do
  {
    if(f->type == R_MIPS_32){
      printf("%d R_MIPS_32 %s\n", f->offset, f->value);
    }else if (f->type == R_MIPS_26){
      printf("%d R_MIPS_26 %s\n", f->offset, f->value);
    }
    else if (f->type == R_MIPS_HI16){
      printf("%d R_MIPS_HI16 %s\n", f->offset, f->value);
    }
    else if (f->type == R_MIPS_LO16){
      printf("%d R_MIPS_LO16 %s\n", f->offset, f->value);
    }
    f = f->suiv;
  }while(f != r->suiv);

}
void ecrire_table(table_reloc r, FILE* a){
  if(table_vide(r))
    return;
  if(r->suiv == r)
  {
    if(r->type == R_MIPS_32){
      fprintf(a, "%d R_MIPS_32 %s\n", r->offset, r->value);
    }else if (r->type == R_MIPS_26){
      fprintf(a, "%d R_MIPS_26 %s\n", r->offset, r->value);
    }
    else if (r->type == R_MIPS_HI16){
      fprintf(a, "%d R_MIPS_HI16 %s\n", r->offset, r->value);
    }
    else if (r->type == R_MIPS_LO16){
      fprintf(a, "%d R_MIPS_LO16 %s\n", r->offset, r->value);
    }
    return;
  }else{
    table_reloc f = r->suiv;
    do
    {
      if(f->type == R_MIPS_32){
        fprintf(a, "%d R_MIPS_32 %s\n", f->offset, f->value);
      }else if (f->type == R_MIPS_26){
        fprintf(a, "%d R_MIPS_26 %s\n", f->offset, f->value);
      }
      else if (f->type == R_MIPS_HI16){
        fprintf(a, "%d R_MIPS_HI16 %s\n", f->offset, f->value);
      }
      else if (f->type == R_MIPS_LO16){
        fprintf(a, "%d R_MIPS_LO16 %s\n", f->offset, f->value);
      }
      f = f->suiv;
    }while(f != r->suiv);
  }
}
void liberer_table(table_reloc r){
    if(table_vide(r))
      return;
    else{
      table_reloc a = r->suiv;
      table_reloc b = r->suiv;
      if(a->suiv == a)
        free(a);
      else{
        while(a != r)
        {
          b = a->suiv;
          free(a);
          a = b;
        }
        free(r);
      }
    }
}
table_reloc remplirTableRelocation(file_text co_text, file_data co_data, file_bss co_bss, file_symb co_symb)
{
  table_reloc table = NULL;
  //Parcours de la collection des elements de .text
  file_text ft = co_text;
  ft = ft->suiv;
  do{
    //On parcours la liste des opérandes de chaque instruction à la recherche d'un renvoie vers une étiquette
    file_jeu_instruction f = ft->op;
    do{
      //Si un des opérandes ets un renvoie vers une étiquette
        if (f->identifiant == "Renvoie vers une étiquette")
        {
          file_symb symb = co_symb;

        }
        f = f->suiv;
    }while(f != ft->op);
    ft = ft->suiv;
  }while(ft!= *co_text->suiv);
  return table;
}
