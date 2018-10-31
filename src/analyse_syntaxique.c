#include "analyse_syntaxique.h"

void analyse_syntaxique(Liste_hach*tab_instruction,file_jeu_instruction file, file_jeu_instruction* file_erreur, liste *co_text, liste *co_data, liste *co_bss, liste *co_symb)
{
	int cpt_text, cpt_data, cpt_bss; 
	cpt_text = cpt_data = cpt_bss = 0;
	enum section etat = INIT;
	file_jeu_instruction g = supprime_commentaire(file);
	g = g->suiv;
	while(g->suiv != file)
	{
		if(!strcmp(g->identifiant, "Directive")|| etat != INIT)
		{
			if(!strcmp(g->caractere, ".text")|| etat == TEXT)
			{
				etat = TEXT;
				g = processText(g->suiv, co_text, &cpt_text, file_erreur, co_symb);
			}
			else if (!strcmp(g->caractere, ".data")|| etat == DATA)
			{
				etat = DATA;
				g = processData(g->suiv, co_data, &cpt_data, file_erreur, co_symb);
			}	
			else if(!strcmp(g->caractere, ".bss") || etat ==BSS)
			{
				etat = BSS;
				g = processBss(g->suiv, co_bss, &cpt_bss, file_erreur, co_symb);
			}
		}
		else
		{
			if(etat == INIT)
			{
				*file_erreur = enfiler("DIrective attendu à la place de :", g->identifiant, g->ligne, *file_erreur);
			}
		}
		g = g->suiv;
	}
}

/* Les fonctions suivantes renvoie une file qui se situe au debut de la ligne suivant celle qui a été analysée*/

/* Fonction qui rempli la liste d'instruction dans le cas ou on est dans .text */
file_jeu_instruction processText(file_jeu_instruction file, liste *co_text, int *cpt_text, file_jeu_instruction* file_erreur, liste *co_symb)
{
	file_jeu_instruction f = file;
	int cpt_arg = 0;
	char *instruction = calloc(1, sizeof(*instruction));
	/*On effectue seulement l'analyse d'une ligne a chaque passage dans cette fonction*/
	while(!strcmp(f->identifiant, "Retour à la ligne"))
	{
		/* a completer */
	}
	return file;
}
/* Fonction qui rempli la liste de data dans le cas ou on est dans .data */
file_jeu_instruction processData(file_jeu_instruction file, liste *co_data, int* cpt_data, file_jeu_instruction *file_erreur, liste *co_symb)
{
	file_jeu_instruction f = file;
	/* Pas encore codé*/	
	return file;
}
/* Fonction qui rempli la liste bss dans le cas ou on est dans .bss */
file_jeu_instruction processBss(file_jeu_instruction file, liste *co_bss, int* cpt_bss, file_jeu_instruction *file_erreur, liste *co_symb)
{
	file_jeu_instruction f = file;

	return file;
}


