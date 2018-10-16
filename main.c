#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


void analyse_syntaxique(file_jeu_instruction*f){
	int i;
	
	/*Collection des symboles ?????????*/
	
	file_jeu_instruction g;
	for(g=(*f)->suiv;g!=(*f);g=g->suiv){
		if(strcmp(g->identifiant, "Directive")){
			switch (g->caractere){
				case ".text":
					analyse_text(f);
					break;
				case ".bss":
					analyse_bss(f);
					break;
				case ".data":
					analyse_data(f);
					break;
					
				default:
					break;
			}
		}
	}
}
		   
file_jeu_instruction*modifie_instruction(file_jeu_instruction*f){
	int i;
	int a=0;
	file_jeu_instruction g;
	file_jeu_instruction h = (*f)->suiv;
	for(g=(*f)->suiv->suiv;g!=(*f);g=g->suiv){
		if(strcmp(h->identifiant,"Instruction") && strcmp(g->caractere,":")){
			h->identifiant="Etiquette";
		}
	}
	return f;
}

void verif_renvoie_vers_etiquette(file_jeu_instruction*f,file_jeu_instruction*file_erreur){
	int i;
	int a=0;
	file_jeu_instruction*file=creer_file();
	file_jeu_instruction g;
	file_jeu_instruction h;
	for(g=(*f)->suiv;g!=(*f);g=g->suiv){
		*file=enfiler(g->identifiant, g->caractere,g->ligne, *file);
	}
	for(g=(*f)->suiv;g!=(*f);g=g->suiv){
		if(strcmp(g->identifiant,"Renvoie vers une etiquette")){
			for(h=(*file)->suiv;h!=(*file);h=h->suiv){
				if(!strcmp(file->caractere,g->caractere)){
					*file_erreur=enfiler("Erreur sur renvoie vers une etiquette", "Aucune correspondance", file->ligne, *file_erreur);
				}
			}
		}
	}
}
		   
int 
