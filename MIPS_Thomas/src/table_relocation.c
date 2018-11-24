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
/* Cette fonction construit la table de relocation à partir des différentes collections */
table_reloc remplirTableRelocationText(file_text co_text, file_symb co_symb, Liste_hach* hach_inst, file_jeu_instruction* file_erreur)
{
 	table_reloc table = NULL;
 	/*Parcours de la collection des elements de .text*/
 	file_text ft = co_text;
 	ft = ft->suiv;
 	if(!file_vide_text(ft)){
    	do{
      	    /*On parcours la liste des opérandes de chaque instruction à la recherche d'un renvoie vers une étiquette*/
      	    file_jeu_instruction f = ft->op;
      		do{
        	    /*Si un des opérandes est un renvoie vers une étiquette*/
          		if (!strcmp(f->identifiant, "Renvoie vers une étiquette"))
          		{
                    /* Si c'est une instruction de type J */
                    if(!strcasecmp(rec_hachage_type(ft->nomInst, hach_inst[hachage(ft->nomInst, dim_tab_instruction)]), "J"))
            		{
					    /*2 cas : symbole déclaré dans ce fichier ou non déclaré dans ce fichier */
					    if(!est_dans_file(f->caractere, co_symb)){
                            /* Si non déclaré dans le même fichier */
						    table = ajoutElement(table, ft->decalage, R_MIPS_26, f->caractere, NULL);
					    }else{
                            /* Si le symbole est déclaré dans le fichier on récupère la section à lauqelle il appartient */
                            struct cellulesymb* ptr_symb = recuperer_cellule_symb(f->caractere, co_symb);
                            table = ajoutElement(table, ft->decalage, R_MIPS_26, ptr_symb->section, ptr_symb);
					    }
                    }else{
                        /* On rentre dans cette section si c'est une instruction de type I */
                        file_jeu_instruction f2 = ft->op->suiv;
                        int i = 0;
                        f2 = f2->suiv;
                        /* On identifie si il s'agit du 2eme ou 3eme operande */
                        if(!strcmp(f2->caractere, f->caractere)){
                            i = 2;
                        }
                        else{
                            i = 3;
                        }
                        char mot1[25];
                        char mot2[25];
                        char mot3[25];
                        rec_hachage_type_instruction(ft->nomInst, hach_inst[hachage(ft->nomInst, dim_tab_instruction)], mot1, mot2, mot3);
                        /* On identifie si le symbole correspond à un immediat ou à un relatif */
                        int isRel = 1;
                        if(i == 2){
                            if(!strcmp(mot2, "Immediat"))
                                isRel = 0;
                        }else{
                            if(!strcmp(mot3, "Immediat"))
                                isRel = 0;
                        }
                        /* On traite chaque cas independamment */
                        /* Si c'est un immédiat */
                        if(!isRel){
                            if(!est_dans_file(f->caractere, co_symb)){
                                /* Si non déclaré dans le même fichier */
    						    table = ajoutElement(table, ft->decalage, R_MIPS_LO16, f->caractere, NULL);
    					    }else{
                                /* Si le symbole est déclaré dans le fichier on récupère la section à laquelle il appartient*/
                                struct cellulesymb* ptr_symb = recuperer_cellule_symb(f->caractere, co_symb);
                                table = ajoutElement(table, ft->decalage, R_MIPS_LO16, ptr_symb->section, ptr_symb);
    					    }
                        }else{
                            /*Si on a un relatif */
                            if(!est_dans_file(f->caractere, co_symb)){
                                /* Si non déclaré dans le même fichier */
                                *file_erreur = enfiler("Saut relatif à une étiquette qui n'est pas dans ce fichier", f->caractere, ft->ligne, *file_erreur);
    					    }else{
                                /* Si le symbole est déclaré dans le fichier on récupère la section à lauqelle il appartient */
                                struct cellulesymb* ptr_symb = recuperer_cellule_symb(f->caractere, co_symb);
                                int offset = ptr_symb->decalage - ft->decalage;
                                if(offset < -32768 || offset > 32767){
                                    *file_erreur = enfiler("Saut relatif trop grand", f->caractere, ft->ligne, *file_erreur);
                                }else{
                                    strcpy(f->identifiant, "ValeuDSC02738r Décimale");
                                    sprintf(f->caractere, "%d", offset);
                                }
    					    }
                        }
                    }
				}else if (!strcmp(f->identifiant, "EtiquettePFort")){
                    if(!est_dans_file(f->caractere, co_symb)){
                        /* Si non déclaré dans le même fichier */
                        table = ajoutElement(table, ft->decalage, R_MIPS_HI16, f->caractere, NULL);
                    }else{
                        /* Si le symbole est déclaré dans le fichier on récupère la section à laquelle il appartient*/
                        struct cellulesymb* ptr_symb = recuperer_cellule_symb(f->caractere, co_symb);
                        table = ajoutElement(table, ft->decalage, R_MIPS_HI16, ptr_symb->section, ptr_symb);
                    }
                }else if (!strcmp(f->identifiant, "EtiquettePFaible")){
                    if(!est_dans_file(f->caractere, co_symb)){
                        /* Si non déclaré dans le même fichier */
                        table = ajoutElement(table, ft->decalage, R_MIPS_LO16, f->caractere, NULL);
                    }else{
                        /* Si le symbole est déclaré dans le fichier on récupère la section à laquelle il appartient*/
                        struct cellulesymb* ptr_symb = recuperer_cellule_symb(f->caractere, co_symb);
                        table = ajoutElement(table, ft->decalage, R_MIPS_LO16, ptr_symb->section, ptr_symb);
                    }
                }
      			f = f->suiv;
                printf("Pointeur d'arret : %p    pointeur de la liste parcourue : %p\n",ft->op, f);
  			}while(f != ft->op);
  			ft = ft->suiv;
            printf("Instruction suivante");
		}while(ft != co_text->suiv);
  	}
    return table;
}
table_reloc remplirTableRelocationData(file_data co_data, file_symb co_symb, Liste_hach hach_inst, file_jeu_instruction* file_erreur)
{
    table_reloc table = NULL;
 	/*Parcours de la collection des elements de .text*/
 	file_data fd = co_data;
 	fd = fd->suiv;
    if(!file_vide_data(co_data)){
        do{
            if(fd->etiquette){
                if(!est_dans_file(fd->op.s, co_symb)){
                    /* Si non déclaré dans le même fichier */
                    table = ajoutElement(table, fd->decalage, R_MIPS_32, fd->op.s, NULL);
                }else{
                    /* Si le symbole est déclaré dans le fichier on récupère la section à laquelle il appartient*/
                    struct cellulesymb* ptr_symb = recuperer_cellule_symb(fd->op.s, co_symb);
                    table = ajoutElement(table, fd->decalage, R_MIPS_32, ptr_symb->section, ptr_symb);
                }
            }
            fd = fd->suiv;
        }while(fd != co_data->suiv);
    }
    return table;
}