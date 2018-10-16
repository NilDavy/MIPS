#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*unsigned long hex_to_dec(char*hex,int a){
	unsigned long res=0;
	int i;
	for (i=a;i<strlen(hex);i++){
//		printf("res=%lu\n",res);
//		printf("hex = %c\n",hex[i]);
//		printf("pow = %d\n",strlen(hex)-1-i);
//		printf("str = %d, i=  %d\n",strlen(hex),i);
		switch (hex[i]) {
			case '1':
				res=res+pow(16, strlen(hex)-1-i);
				break;
			case '2':
				res=res+2*pow(16, strlen(hex)-1-i);
				break;
			case '3':
				res=res+3*pow(16, strlen(hex)-1-i);
				break;
			case '4':
				res=res+4*pow(16, strlen(hex)-1-i);
				break;
			case '5':
				res=res+5*pow(16, strlen(hex)-1-i);
				break;
			case '6':
				res=res+6*pow(16, strlen(hex)-1-i);
				break;
			case '7':
				res=res+7*pow(16, strlen(hex)-1-i);
				break;
			case '8':
				res=res+8*powl(16, strlen(hex)-1-i);
				break;
			case '9':
				res=res+9*pow(16, strlen(hex)-1-i);
				break;
			case 'a':
				res=res+10*pow(16, strlen(hex)-1-i);
				break;
			case 'A':
				res=res+10*pow(16, strlen(hex)-1-i);
				break;
			case 'b':
				res=res+11*pow(16, strlen(hex)-1-i);
				break;
			case 'B':
				res=res+11*pow(16, strlen(hex)-1-i);
				break;
			case 'c':
				res=res+12*pow(16, strlen(hex)-1-i);
				break;
			case 'C':
				res=res+12*pow(16, strlen(hex)-1-i);
				break;
			case 'd':
				res=res+13*pow(16, strlen(hex)-1-i);
				break;
			case 'D':
				res=res+13*pow(16, strlen(hex)-1-i);
				break;
			case 'e':
				res=res+14*pow(16, strlen(hex)-1-i);
				break;
			case 'E':
				res=res+14*pow(16, strlen(hex)-1-i);
				break;
			case 'f':
				res=res+15*pow(16, strlen(hex)-1-i);
				break;
			case 'F':
				res=res+15*pow(16, strlen(hex)-1-i);
				break;
			default:
				break;
		}
	}
	return res;
}*/


file_jeu_instruction*supprime_commentaire(file_jeu_instruction*f){
	int i;
	int a=0;
	file_jeu_instruction g;
	for(g=(*f)->suiv;g!=(*f);g=g->suiv){
		if(strcmp(g->identifiant,"Commentaire"){
			retire_maillon(&g, i-a);
			a=a+1;
		}
	}
	return f;
}
/*retire un maillon specifique*/
void retire_maillon(file_jeu_instruction* pf,int a){
	if (*pf==NULL){
		exit(0);
	}
	int i;
	file_jeu_instruction g = (*pf)->suiv;
	file_jeu_instruction h = (*pf);
	if(a==0){
		(*pf)->suiv=g->suiv;
		g->suiv=NULL;
		free(g);
		return;
	}
	else{
		for(i=0;i<a;i++){
			g=g->suiv;
			h=h->suiv;
		}
		h->suiv=g->suiv;
		g->suiv;
		free(g);
		return;
	}
}

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
