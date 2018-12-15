#include "analyse_syntaxique.h"

void
analyse_syntaxique(Liste_hach * tab_instruction,
				   file_jeu_instruction file,
				   file_jeu_instruction * file_erreur,
				   file_text * co_text, file_data * co_data,
				   file_bss * co_bss, file_symb * co_symb,
				   file_symb * co_text_attente,
				   file_symb * co_data_attente,
				   file_symb * co_bss_attente, int *nbtext,
				   int *nbdata, int *cptbss)
{
	int             cpt_text,
	cpt_data,
	cpt_bss;
	cpt_text = cpt_data = cpt_bss = 0;
	enum section    etat = INIT;
	file_jeu_instruction g = supprime_commentaire(file);
	file_jeu_instruction h = g;
	g = g->suiv;
	/*
	 * int t=0,b=0,d=0; do{ if(!strcasecmp(g->caractere, ".text")){ t=1; }
	 * else{ if (!strcasecmp(g->caractere, ".data")){ d=1; } else{
	 * if(!strcasecmp(g->caractere, ".bss")){ b=1; } } } g=g->suiv;}
	 * while(g!=h->suiv);
	 *
	 * printf("t %d b %d d%d\n",t,b,d); if(t==1){
	 * *co_symb=ajout_symb(".text", 0,0,".text", *co_symb); } if(d==1){
	 * *co_symb=ajout_symb(".data", 0,0,".data", *co_symb); } if(b==1){
	 * *co_symb=ajout_symb(".bss", 0,0,".bss", *co_symb); }
	 * visualiser_file_symb(*co_symb);
	 */
	do {
		/*
		 * ici on regarde dans quelle section on se trouve avec une
		 * priorité mise en place si on change de section (.qqch avant
		 * l'etat courant)
		 */
		if (strcasecmp(g->caractere, ".set") == 0
			|| strcasecmp(g->identifiant, "Retour à la ligne") == 0
			|| strcasecmp(g->caractere, "noreorder") == 0) {
			g = g->suiv;
		} else {
			
			if (!strcasecmp(g->caractere, ".text")) {
				/*
				 * printf("text\n"); printf("%s\n",g->caractere);
				 * printf("%d\n\n\n",g->ligne);
				 */
				etat = TEXT;
				g = processText(g, co_text, &cpt_text, file_erreur,
								co_symb, co_text_attente, tab_instruction);
			} else {
				if (!strcasecmp(g->caractere, ".data")) {
					/*
					 * printf("data\n"); printf("%s\n",g->caractere);
					 */
					etat = DATA;
					g = processData(g, co_data, &cpt_data, file_erreur,
									co_symb, co_data_attente);
				} else {
					if (!strcasecmp(g->caractere, ".bss")) {
						/*
						 * printf("bss\n"); printf("%s\n",g->caractere);
						 */
						etat = BSS;
						g = processBss(g, co_bss, &cpt_bss, file_erreur,
									   co_symb, co_bss_attente);
					} else {
						if (etat == TEXT) {
							/*
							 * printf("text\n");
							 * printf("%s\n",g->caractere);
							 * printf("%d\n\n\n",g->ligne);
							 */
							etat = TEXT;
							g = processText(g, co_text, &cpt_text,
											file_erreur, co_symb,
											co_text_attente,
											tab_instruction);
						} else {
							if (etat == DATA) {
								/*
								 * printf("data\n");
								 * printf("%s\n",g->caractere);
								 */
								etat = DATA;
								g = processData(g, co_data, &cpt_data,
												file_erreur, co_symb,
												co_data_attente);
							} else {
								if (etat == BSS) {
									/*
									 * printf("bss\n");
									 * printf("%s\n",g->caractere);
									 */
									etat = BSS;
									g = processBss(g, co_bss, &cpt_bss,
												   file_erreur, co_symb,
												   co_bss_attente);
								} else {
									/*
									 * les cas ou il faut mettre d'erreur
									 */
									*file_erreur =
									enfiler
									("Directive attendu à la place de :",
									 g->caractere, g->ligne,
									 *file_erreur);
								}
							}
						}
					}
				}
			}
			g = g->suiv;
		}
	}
	while (g != h->suiv);
	*cptbss = cpt_bss;
	*nbdata = cpt_data;
	*nbtext = cpt_text;
}




/*
 * Les fonctions suivantes renvoie une file qui se situe au debut de la
 * ligne suivant celle qui a été analysée
 */

/*
 * Fonction qui rempli la liste d'instruction dans le cas ou on est dans
 * .text
 */
file_jeu_instruction
processText(file_jeu_instruction file,
			file_text * co_text, int *cpt_text,
			file_jeu_instruction * file_erreur,
			file_symb * co_symb,
			file_symb * co_text_attente, Liste_hach * tab_instruction)
{
	file_jeu_instruction f = file;
	file_jeu_instruction g = creer_file();
	char            copie[200];
	char           *instruction = NULL;
	int             nbop;
	int             verif = 0;
	int             concatenation;
	/*
	 * les cas ou on ne fait rien .text, : et retour ligne
	 */
	
	if (!strcasecmp(f->caractere, ".text")
		|| !strcasecmp(f->caractere, ":")
		|| !strcasecmp(f->identifiant, "Retour à la ligne")) {
		return f;
	} else {
		/*
		 * cas d'un etiquette, on la stocke dans une file d'attente pour
		 * eviter les cas ou etiquette puis changement de section
		 */
		if (!strcasecmp(f->identifiant, "Etiquette")) {
			/*
			 * ajout dans la table des symboles d'attente
			 */
			*co_text_attente =
			ajout_symb(f->caractere, f->ligne, *cpt_text, "TEXT",
					   *co_text_attente);
			return f;
		}
		
		else {
			/*
			 * dans la section .text il y a que les instructions
			 */
			
			if (!strcasecmp(f->identifiant, "Instruction")) {
				while (!file_vide_symb(*co_text_attente)) {
					char            mot[200];
					/*
					 * ajout dans la table des symboles
					 */
					defiler_symb(co_text_attente, mot);
					strcpy(copie, mot);
					*co_symb =
					ajout_symb(copie, f->ligne, *cpt_text, "TEXT",
							   *co_symb);
				}
				
				instruction = f->caractere;
				
				/*
				 * recherche nombre d'opération
				 */
				nbop =
				rec_hachage_nbparam(instruction,
									tab_instruction[hachage
													(instruction,
													 dim_tab_instruction)]);
				verif = 0;
				while (strcasecmp(f->identifiant, "Retour à la ligne")) {
					f = f->suiv;
					char           *mot1 = calloc(200, sizeof(char));
					char           *identifiant =
					calloc(200, sizeof(char));
					concatenation = 0;
					file_jeu_instruction verif_file = creer_file();
					
					/*
					 * on concatene si base offset
					 */
					while (strcasecmp(f->identifiant, "Retour à la ligne")
						   != 0 && strcasecmp(f->caractere, ",") != 0) {
						strcat(mot1, f->caractere);
						strcat(identifiant, f->identifiant);
						concatenation = concatenation + 1;
						verif_file =
						enfiler(f->identifiant, f->caractere, f->ligne,
								verif_file);
						f = f->suiv;
					}
					
					/*
					 * ajout a la table .text
					 */
					if (concatenation > 1) {
						if ((strcasecmp
							 (verif_file->suiv->identifiant,
							  "Valeur Décimale") == 0
							 || strcasecmp(verif_file->suiv->identifiant,
										   "Valeur Hexadécimale") == 0)
							&& strcasecmp(verif_file->suiv->suiv->
										  caractere, "(") == 0
							&& strcasecmp(verif_file->suiv->suiv->
										  suiv->identifiant,
										  "Registre") == 0
							&& strcasecmp(verif_file->suiv->suiv->suiv->
										  suiv->caractere, ")") == 0) {
								g = enfiler("Baseoffset", mot1, f->ligne, g);
								verif = verif + 1;
							} else {
								if ((strcasecmp
									 (verif_file->suiv->caractere, "+") == 0
									 || strcasecmp(verif_file->suiv->caractere,
												   "-") == 0)
									&&
									(strcasecmp
									 (verif_file->suiv->suiv->identifiant,
									  "Valeur Décimale") == 0
									 || strcasecmp(verif_file->suiv->
												   suiv->identifiant,
												   "Valeur Hexadécimale") ==
									 0)
									&& strcasecmp(verif_file->suiv->suiv->
												  suiv->caractere, "(") == 0
									&& strcasecmp(verif_file->suiv->suiv->
												  suiv->suiv->identifiant,
												  "Registre") == 0
									&& strcasecmp(verif_file->suiv->suiv->
												  suiv->suiv->suiv->caractere,
												  ")") == 0) {
										g = enfiler("Baseoffset", mot1, f->ligne,
													g);
										verif = verif + 1;
									} else {
										if ((strcasecmp
											 (verif_file->suiv->caractere,
											  "+") == 0
											 || strcasecmp(verif_file->suiv->
														   caractere, "-") == 0)
											&&
											(strcasecmp
											 (verif_file->suiv->suiv->identifiant,
											  "Valeur Décimale") == 0
											 || strcasecmp(verif_file->suiv->suiv->
														   identifiant,
														   "Valeur Hexadécimale")
											 == 0)) {
												g = enfiler("Valeur Décimale", mot1,
															f->ligne, g);
												verif = verif + 1;
											} else {
												*file_erreur =
												enfiler("Erreur Base Offset", mot1,
														f->ligne, *file_erreur);
											}
									}
							}
						
					} else {
						g = enfiler(identifiant, mot1, f->ligne, g);
						verif = verif + 1;
					}
					liberer_file(verif_file);
					free(identifiant);
					free(mot1);
				}
				/*
				 * verif pseudo-instruction
				 */
				if ((strcasecmp(instruction, "NOP") == 0
					 && strlen(g->caractere) == 0)
					|| (strcasecmp(instruction, "LW") == 0 && verif == 2)
					|| (strcasecmp(instruction, "SW") == 0 && verif == 2)
					|| (strcasecmp(instruction, "MOVE") == 0 && verif == 2)
					|| (strcasecmp(instruction, "NEG") == 0 && verif == 2)
					|| (strcasecmp(instruction, "LI") == 0 && verif == 2)
					|| (strcasecmp(instruction, "BLT") == 0 && verif == 3)) {
					*co_text =pseudo_instruction(cpt_text, instruction, co_text,
									   file_erreur, g, f->ligne);
				} else {
					/*
					 * verif si erreur avec le nombre d'operation
					 */
					if (verif == nbop) {
						*co_text =
						ajout_text(instruction, nbop, f->ligne,
								   *cpt_text, *co_text, g);
						*cpt_text = *cpt_text + 4;
					} else {
						*file_erreur =
						enfiler("Erreur sur le nombre d'opérande",
								f->caractere, f->ligne, *file_erreur);
					}
				}
			} else {
				*file_erreur = enfiler("Mauvaise opérande dans la section .text",f->caractere, f->ligne, *file_erreur);
				f = f->suiv;
			}
			
		}
	}
	liberer_file(g);
	return f;
}


file_text
pseudo_instruction(int *cpt_text, char *instruction,
				   file_text * co_text,
				   file_jeu_instruction * file_erreur,
				   file_jeu_instruction g, int ligne)
{
	/*
	 * printf("ligne %d\n",ligne);
	 */
	if (strcasecmp(instruction, "NOP") == 0) {
		/*
		 * printf("NOP\n");
		 */
		file_jeu_instruction h = creer_file();
		h = enfiler("Registre", "$0", ligne, h);
		h = enfiler("Registre", "$0", ligne, h);
		h = enfiler("Valeur Décimale", "0", ligne, h);
		*co_text = ajout_text("SLL", 3, ligne, *cpt_text, *co_text, h);
		*cpt_text = *cpt_text + 4;
		liberer_file(h);
	} else {
		if (strcasecmp(instruction, "LW") == 0
			&& strcasecmp(g->suiv->identifiant, "Registre") == 0) {
			/*
			 * printf("LW\n");
			 */
			
			if (strcasecmp(g->identifiant, "Baseoffset") == 0) {
				*co_text =
				ajout_text("LW", 2, ligne, *cpt_text, *co_text, g);
				*cpt_text = *cpt_text + 4;
			} else {
				if (strcasecmp
					(g->identifiant, "Renvoie vers une étiquette")
					== 0) {
					
					file_jeu_instruction h = creer_file();
					file_jeu_instruction i = creer_file();
					/*
					 * unsigned octet_pfort=(atoi(g->caractere)
					 * &0xFFFF0000)>>16; unsigned
					 * octet_pfaible=atoi(g->caractere)&0xFFFF; char
					 * o_p_fort[30],o_p_faible[30]; sprintf(o_p_fort,
					 * "%d",octet_pfort); sprintf(o_p_faible,
					 * "%d",octet_pfaible); printf("poids faible
					 * %s\n",o_p_faible); printf("poids fort
					 * %s\n",o_p_fort); strcat(o_p_faible,g->caractere);
					 * strcat(o_p_faible,"(");
					 * strcat(o_p_faible,g->suiv->caractere);
					 * strcat(o_p_faible,")");
					 */
					h = enfiler(g->suiv->identifiant, g->suiv->caractere,
								ligne, h);
					h = enfiler("EtiquettePFort", g->caractere, ligne, h);
					*co_text =
					ajout_text("LUI", 2, ligne, *cpt_text, *co_text,
							   h);
					*cpt_text = *cpt_text + 4;
					i = enfiler(g->suiv->identifiant, g->suiv->caractere,
								ligne, i);
					i = enfiler("EtiquettePFaible", g->caractere, ligne,
								i);
					*co_text =
					ajout_text("LW", 2, ligne, *cpt_text, *co_text, i);
					*cpt_text = *cpt_text + 4;
					liberer_file(h);
					liberer_file(i);
				} else {
					*file_erreur =
					enfiler("Mauvaise opérande après un LW",
							g->identifiant, ligne, *file_erreur);
				}
			}
		} else {
			if (strcasecmp(instruction, "SW") == 0) {
				/*
				 * printf("SW\n");
				 */
				
				if (strcasecmp(g->identifiant, "Baseoffset") == 0) {
					
					*co_text =
					ajout_text("SW", 2, ligne, *cpt_text, *co_text, g);
					*cpt_text = *cpt_text + 4;
				} else {
					if (strcasecmp
						(g->identifiant,
						 "Renvoie vers une étiquette") == 0) {
							file_jeu_instruction h = creer_file();
							file_jeu_instruction i = creer_file();
							h = enfiler(g->suiv->identifiant,
										g->suiv->caractere, ligne, h);
							h = enfiler("EtiquettePFort", g->caractere, ligne,
										h);
							*co_text =
							ajout_text("LUI", 2, ligne, *cpt_text,
									   *co_text, h);
							*cpt_text = *cpt_text + 4;
							
							i = enfiler(g->suiv->identifiant,
										g->suiv->caractere, ligne, i);
							i = enfiler("EtiquettePFaible", g->caractere,
										ligne, i);
							*co_text =
							ajout_text("SW", 2, ligne, *cpt_text, *co_text,
									   i);
							*cpt_text = *cpt_text + 4;
							liberer_file(h);
							liberer_file(i);
						} else {
							*file_erreur =
							enfiler("Mauvaise opérande après un SW",
									g->identifiant, ligne, *file_erreur);
						}
				}
			} else {
				if (strcasecmp(instruction, "MOVE") == 0) {
					/*
					 * printf("MOVE\n");
					 */
					
					g = enfiler("Registre", "$0", ligne, g);
					*co_text =
					ajout_text("ADD", 3, ligne, *cpt_text, *co_text,
							   g);
					*cpt_text = *cpt_text + 4;
				} else {
					if (strcasecmp(instruction, "NEG") == 0) {
						/*
						 * printf("NEG\n");
						 */
						
						file_jeu_instruction h = creer_file();
						h = enfiler(g->suiv->identifiant,
									g->suiv->caractere, ligne, h);
						h = enfiler("Registre", "$0", ligne, h);
						h = enfiler(g->identifiant, g->caractere, ligne,
									h);
						*co_text =
						ajout_text("SUB", 3, ligne, *cpt_text,
								   *co_text, h);
						*cpt_text = *cpt_text + 4;
						liberer_file(h);
					} else {
						if (strcasecmp(instruction, "LI") == 0) {
							/*
							 * printf("LI\n");
							 */
							
							file_jeu_instruction h = creer_file();
							h = enfiler(g->suiv->identifiant,
										g->suiv->caractere, ligne, h);
							h = enfiler("Registre", "$0", ligne, h);
							h = enfiler(g->identifiant, g->caractere,
										ligne, h);
							*co_text =
							ajout_text("ADDI", 3, ligne, *cpt_text,
									   *co_text, h);
							*cpt_text = *cpt_text + 4;
							liberer_file(h);
						} else {
							/*
							 * instruction BLT
							 */
							/*
							 * printf("BLT\n");
							 */
							
							file_jeu_instruction h = creer_file();
							file_jeu_instruction i = creer_file();
							h = enfiler("Registre", "$1", ligne, h);
							h = enfiler(g->suiv->identifiant,
										g->suiv->caractere, ligne, h);
							h = enfiler(g->suiv->suiv->identifiant,
										g->suiv->suiv->caractere, ligne,
										h);
							i = enfiler("Registre", "$1", ligne, i);
							i = enfiler("Registre", "$0", ligne, i);
							i = enfiler(g->suiv->suiv->suiv->identifiant,
										g->suiv->suiv->suiv->caractere,
										ligne, i);
							*co_text =
							ajout_text("SLT", 3, ligne, *cpt_text,
									   *co_text, h);
							*cpt_text = *cpt_text + 4;
							*co_text =
							ajout_text("BNE", 3, ligne, *cpt_text,
									   *co_text, i);
							*cpt_text = *cpt_text + 4;
							liberer_file(h);
							liberer_file(i);
						}
					}
				}
			}
		}
	}
	return *co_text;
}


/*
 * Fonction qui rempli la liste bss dans le cas ou on est dans .bss
 */
file_jeu_instruction
processBss(file_jeu_instruction file,
		   file_bss * co_bss, int *cpt_bss,
		   file_jeu_instruction * file_erreur,
		   file_symb * co_symb, file_symb * co_bss_attente)
{
	file_jeu_instruction f = file;
	char            copie[200];
	
	/*
	 * les cas ou on ne fait rien .bss, : et retour ligne
	 */
	if (!(strcasecmp(f->caractere, ".bss"))
		|| !(strcasecmp(f->caractere, ":"))
		|| !(strcasecmp(f->identifiant, "Retour à la ligne"))) {
		return f;
	} else {
		if (!strcasecmp(f->identifiant, "Etiquette")) {
			/*
			 * ajout dans la table des symboles d'attente
			 */
			*co_bss_attente =
			ajout_symb(f->caractere, f->ligne, *cpt_bss, "BSS",
					   *co_bss_attente);
			return f;
		} else {
			
			/*
			 * dans la section .bss il peut y avoir que .space
			 */
			
			if (!strcasecmp(f->caractere, ".space")) {
				while (!file_vide_symb(*co_bss_attente)) {
					char            mot[200];
					/*
					 * ajout dans la table des symboles
					 */
					defiler_symb(co_bss_attente, mot);
					strcpy(copie, mot);
					*co_symb =
					ajout_symb(copie, f->ligne, *cpt_bss, "BSS",
							   *co_symb);
					
				}
				
				f = f->suiv;
				while (strcasecmp(f->identifiant, "Retour à la ligne")) {
					if (!strcasecmp(f->identifiant, "Valeur Décimale")
						|| !strcasecmp(f->identifiant,
									   "Valeur Hexadécimale")) {
							/*
							 * Creer maillon bss, ici il peut y avoir apres
							 * space soit un hex qui est deja transformee en
							 * dec ou un dec
							 */
							/*
							 * au final c'est un dec quoi qu'il arrive et
							 * positif car on veut une taille
							 */
							*co_bss =
							ajout_bss(".space", f->ligne, *cpt_bss,
									  f->caractere, 2, *co_bss);
							*cpt_bss = (*cpt_bss) + atoi(f->caractere);
						} else {
							if (strcasecmp(f->caractere, ",")) {
								*file_erreur =
								enfiler
								("Mauvaise commande après la directive .space",
								 f->identifiant, f->ligne, *file_erreur);
							}
						}
					f = f->suiv;
				}
			} else {
				*file_erreur =
				enfiler("Directive .space attendu à la place de :",
						f->caractere, f->ligne, *file_erreur);
				f = f->suiv;
			}
			
		}
	}
	return f;
}


/*
 * Fonction qui rempli la liste de data dans le cas ou on est dans .data
 */
file_jeu_instruction
processData(file_jeu_instruction file,
			file_data * co_data, int *cpt_data,
			file_jeu_instruction * file_erreur,
			file_symb * co_symb, file_symb * co_data_attente)
{
	file_jeu_instruction f = file;
	char            copie[200];
	int             i;
	/*
	 * les cas ou on ne fait rien .data, : et retour ligne
	 */
	if (!strcasecmp(f->caractere, ".data")
		|| !strcasecmp(f->caractere, ":")
		|| !strcasecmp(f->identifiant, "Retour à la ligne")) {
		return f;
	} else {
		if (!strcasecmp(f->identifiant, "Etiquette")) {
			/*
			 * ajout dans la table des symboles d'attente
			 */
			*co_data_attente =
			ajout_symb(f->caractere, f->ligne, *cpt_data, "DATA",
					   *co_data_attente);
			return f;
		} else {
			
			/*
			 * dans la section .data il peut y avoir .space, .word, .byte
			 * ou .asciiz
			 */
			
			if (!strcasecmp(f->caractere, ".space")) {
				while (!file_vide_symb(*co_data_attente)) {
					char            mot[200];
					/*
					 * ajout dans la table des symboles
					 */
					defiler_symb(co_data_attente, mot);
					strcpy(copie, mot);
					*co_symb =
					ajout_symb(copie, f->ligne, *cpt_data, "DATA",
							   *co_symb);
				}
				f = f->suiv;
				while (strcasecmp(f->identifiant, "Retour à la ligne")) {
					if (!strcasecmp(f->identifiant, "Valeur Décimale")
						|| !strcasecmp(f->identifiant,
									   "Valeur Hexadécimale")) {
							/*
							 * Creer maillon data, ici il peut y avoir apres
							 * space soit un hex qui est deja transformee en
							 * dec ou un dec
							 */
							/*
							 * au final c'est un dec quoi qu'il arrive et
							 * positif car on veut une taille
							 */
							*co_data =
							ajout_data(".space", f->ligne, *cpt_data,
									   f->caractere, 2, *co_data, 0);
							*cpt_data = (*cpt_data) + atoi(f->caractere);
						} else {
							if (strcasecmp(f->caractere, ",")) {
								*file_erreur =
								enfiler
								("Mauvaise commande après la directive .space",
								 f->identifiant, f->ligne, *file_erreur);
							}
						}
					f = f->suiv;
				}
			}
			
			else {
				if (!strcasecmp(f->caractere, ".byte")) {
					while (!file_vide_symb(*co_data_attente)) {
						char            mot[200];
						/*
						 * ajout dans la table des symboles
						 */
						defiler_symb(co_data_attente, mot);
						strcpy(copie, mot);
						*co_symb =
						ajout_symb(copie, f->ligne, *cpt_data, "DATA",
								   *co_symb);
					}
					f = f->suiv;
					while (strcasecmp
						   (f->identifiant, "Retour à la ligne")) {
						/*
						 * si inf a 127
						 */
						if (!strcasecmp(f->identifiant, "Valeur Décimale")
							&& atoi(f->caractere) < 127) {
							*co_data =
							ajout_data(".byte", f->ligne, *cpt_data,
									   f->caractere, 2, *co_data, 0);
							*cpt_data = (*cpt_data) + 1;
						}
						/*
						 * si sup à -128
						 */
						else {
							if (!strcasecmp(f->identifiant, "Délimiteur")
								&& (!strcasecmp(f->caractere, "-"))
								&& atoi(f->suiv->caractere) < 128) {
								char           *mot2 =
								calloc(200, sizeof(char));
								strcat(mot2, f->caractere);
								strcat(mot2, f->suiv->caractere);
								*co_data =
								ajout_data(".byte", f->ligne,
										   *cpt_data, mot2, 2,
										   *co_data, 0);
								*cpt_data = (*cpt_data) + 1;
								free(mot2);
							} else {
								/*
								 * hexa compris entre 0x0 et 0xff
								 */
								if (!strcasecmp
									(f->identifiant,
									 "Valeur Hexadécimale")
									&& atoi(f->caractere) < 256) {
									*co_data =
									ajout_data(".byte", f->ligne,
											   *cpt_data, f->caractere,
											   2, *co_data, 0);
									*cpt_data = (*cpt_data) + 1;
								} else {
									if (strcasecmp(f->caractere, ",")) {
										*file_erreur =
										enfiler
										("Mauvaise commande après la directive .byte",
										 f->caractere, f->ligne,
										 *file_erreur);
									}
								}
								
							}
						}
						
						f = f->suiv;
					}
				}
				
				else {
					if (!strcasecmp(f->caractere, ".word")) {
						if (*cpt_data && (*cpt_data) % 4 != 0) {
							*cpt_data += 4 - ((*cpt_data) % 4);
						}
						while (!file_vide_symb(*co_data_attente)) {
							char            mot[200];
							/*
							 * ajout dans la table des symboles
							 */
							defiler_symb(co_data_attente, mot);
							strcpy(copie, mot);
							*co_symb =
							ajout_symb(copie, f->ligne, *cpt_data,
									   "DATA", *co_symb);
						}
						f = f->suiv;
						
						
						/*
						 * les verifs des 32 bits
						 */
						while (strcasecmp(f->identifiant, "Retour à la ligne")) {
							if(!strcasecmp(f->identifiant, "Valeur Hexadécimale")){
								*co_data =ajout_data(".word", f->ligne,*cpt_data, f->caractere, 5,*co_data, 0);
							}
							else if (!strcasecmp(f->identifiant, "Valeur Décimale")&& (unsigned long) atoi(f->caractere) <2147483647) {
								*co_data =ajout_data(".word", f->ligne,*cpt_data, f->caractere, 5,*co_data, 0);
								*cpt_data = (*cpt_data) + 4;
							} else {
								/*
								 * si nombre negatif
								 */
								if (!strcasecmp(f->identifiant, "Délimiteur")&& (!strcasecmp(f->caractere, "-"))
									&& !strcasecmp(f->suiv->identifiant,"Valeur Décimale")
									&& (unsigned long) atoi(f->caractere) <2147483648){
									char*mot2 =calloc(200, sizeof(char));
									strcat(mot2, f->caractere);
									strcat(mot2, f->suiv->caractere);
									*co_data =
									ajout_data(".word", f->ligne,
											   *cpt_data, mot2, 5,
											   *co_data, 0);
									*cpt_data = (*cpt_data) + 4;
									free(mot2);
								} else {
									if (!strcasecmp
										(f->identifiant,
										 "Renvoie vers une étiquette")) {
											*co_data = ajout_data(".word", f->ligne,*cpt_data,f->caractere, 4,*co_data, 1);
											*cpt_data = (*cpt_data) + 4;
										} else {
											if (strcasecmp(f->caractere, ",")) {
												*file_erreur =
												enfiler
												("Mauvaise commande après la directive .word",
												 f->identifiant, f->ligne,
												 *file_erreur);
											}
										}
								}
							}
							f = f->suiv;
						}
					} else {
						if (!strcasecmp(f->caractere, ".asciiz")) {
							while (!file_vide_symb(*co_data_attente)) {
								char            mot[200];
								/*
								 * ajout dans la table des symboles
								 */
								defiler_symb(co_data_attente, mot);
								strcpy(copie, mot);
								*co_symb =
								ajout_symb(copie, f->ligne, *cpt_data,
										   "DATA", *co_symb);
							}
							f = f->suiv;
							int             z = 1;
							/*
							 * concat chaine de caractère
							 */
							while (z == 1) {
								char           *mot1 =
								calloc(200, sizeof(char));
								while (strcasecmp
									   (f->identifiant,
										"Retour à la ligne")
									   && (strcasecmp(f->caractere, ",")
										   && strcasecmp(f->identifiant,
														 "Délimiteur"))) {
											   if (!strcasecmp
												   (f->identifiant,
													"Chaine de caractère")) {
													   strcat(mot1, f->caractere);
													   /*printf("%s %s %s\n",mot1,f->suiv->identifiant,f->suiv->caractere);*/
												   } else {
													   *file_erreur =
													   enfiler
													   ("Mauvaise commande après la directive .asciiz",
														f->identifiant, f->ligne,
														*file_erreur);
												   }
											   f = f->suiv;
											   if (strcasecmp(f->identifiant,"Retour à la ligne") == 0) {
												   /*printf("Fin Ligne\n\n\n");*/
												   z = 0;
												   *co_data =ajout_data(".asciiz", f->ligne,*cpt_data, mot1, 4,	*co_data, 0);
												   for (i = 0; i < (strlen(mot1) - 1);i++) {
													   if (mot1[i] == '\\'
														   && (mot1[i + 1] == 'n'
															   || mot1[i + 1] == 't'))
													   {
														   *cpt_data =
														   (*cpt_data) - 1;
													   }
												   }
												   *cpt_data =
												   (*cpt_data) + strlen(mot1) + 1;
												   
											   } else {
												   if (strcasecmp
													   (f->identifiant,
														"Délimiteur") == 0
													   && strcasecmp(f->caractere,
																	 ",") == 0) {
														   f = f->suiv;
														   *co_data =
														   ajout_data(".asciiz",
																	  f->ligne,
																	  *cpt_data, mot1,
																	  4, *co_data, 0);
														   for (i = 0;
																i < (strlen(mot1) - 1);
																i++) {
															   if (mot1[i] == '\\'
																   && (mot1[i + 1] == 'n'
																	   || mot1[i + 1] ==
																	   't')) {
																	   *cpt_data =
																	   (*cpt_data) - 1;
																   }
														   }
														   *cpt_data =
														   (*cpt_data) +
														   strlen(mot1) + 1;
														   strcpy(mot1, "");
													   } else {
														   strcat(mot1, " ");
													   }
											   }
										   }
								free(mot1);
							}
						} else {
							*file_erreur =
							enfiler
							("Mauvaise directive dans la section .data",
							 " ", f->ligne, *file_erreur);
							f = f->suiv;
						}
					}
				}
			}
		}
	}
	return f;
}

void
verif_operande(file_text co_text, file_jeu_instruction * file_erreur,
			   Liste_hach * tab_instruction, Liste_hach * tab_registre)
{
	file_text       a = NULL;
	a = co_text->suiv;
	int             n;
	int             i;
	char           *tab[3];
	for (i = 0; i < 3; i++) {
		tab[i] = calloc(25, sizeof(char));
	}
	file_jeu_instruction f = NULL;
	while (a != co_text) {
		char            mot1[25];
		char            mot2[25];
		char            mot3[25];
		n = hachage(a->nomInst, dim_tab_instruction);
		rec_hachage_type_instruction(a->nomInst, tab_instruction[n], mot1,
									 mot2, mot3);
		strcpy(tab[0], mot1);
		strcpy(tab[1], mot2);
		strcpy(tab[2], mot3);
		n = rec_hachage_nbparam(a->nomInst, tab_instruction[n]);
		/*
		 * printf("%s %d %s %s %s\n",a->nomInst,n,tab[0],tab[1],tab[2]);
		 */
		f = a->op->suiv;
		for (i = 0; i < n; i++) {
			if (strcasecmp(tab[i], "Registre") == 0) {
				verif_registre_ope(file_erreur, f, a, tab_registre);
			} else {
				if (strcasecmp(tab[i], "Immediat") == 0) {
					verif_immediat_ope(file_erreur, f, a);
				} else {
					if (strcasecmp(tab[i], "Shiftamount") == 0) {
						verif_shiftamount_ope(file_erreur, f, a);
					} else {
						if (strcasecmp(tab[i], "Relatif") == 0) {
							verif_relatif_ope(file_erreur, f, a);
						} else {
							if (strcasecmp(tab[i], "Absolu") == 0) {
								verif_absolu_ope(file_erreur, f, a);
							} else {
								verif_baseoffset_ope(file_erreur, f, a);
							}
						}
					}
				}
			}
			f = f->suiv;
		}
		a = a->suiv;
	}
	char            mot1[25];
	char            mot2[25];
	char            mot3[25];
	n = hachage(a->nomInst, dim_tab_instruction);
	rec_hachage_type_instruction(a->nomInst, tab_instruction[n], mot1,
								 mot2, mot3);
	strcpy(tab[0], mot1);
	strcpy(tab[1], mot2);
	strcpy(tab[2], mot3);
	n = rec_hachage_nbparam(a->nomInst, tab_instruction[n]);
	/*
	 * printf("%s %d %s %s %s\n",a->nomInst,n,tab[0],tab[1],tab[2]);
	 */
	f = a->op->suiv;
	for (i = 0; i < n; i++) {
		if (strcasecmp(tab[i], "Registre") == 0) {
			verif_registre_ope(file_erreur, f, a, tab_registre);
		} else {
			if (strcasecmp(tab[i], "Immediat") == 0) {
				verif_immediat_ope(file_erreur, f, a);
			} else {
				if (strcasecmp(tab[i], "Shiftamount") == 0) {
					verif_shiftamount_ope(file_erreur, f, a);
				} else {
					if (strcasecmp(tab[i], "Relatif") == 0) {
						verif_relatif_ope(file_erreur, f, a);
					} else {
						if (strcasecmp(tab[i], "Absolu") == 0) {
							verif_absolu_ope(file_erreur, f, a);
						} else {
							verif_baseoffset_ope(file_erreur, f, a);
						}
					}
				}
			}
		}
		f = f->suiv;
	}
	free(tab[0]);
	free(tab[1]);
	free(tab[2]);
}

void
verif_registre_ope(file_jeu_instruction * file_erreur,
				   file_jeu_instruction f, file_text a,
				   Liste_hach * tab_registre)
{
	if (strcasecmp(f->identifiant, "Registre") != 0) {
		*file_erreur =
		enfiler("Type Registre attendu ", f->identifiant, a->ligne,
				*file_erreur);
	}
}

void
verif_baseoffset_ope(file_jeu_instruction * file_erreur,
					 file_jeu_instruction f, file_text a)
{
	if (strcasecmp(f->identifiant, "Baseoffset") != 0
		&& strcasecmp(f->identifiant, "EtiquettePFort") != 0
		&& strcasecmp(f->identifiant, "EtiquettePFaible") != 0) {
		*file_erreur =
		enfiler("Type Base Offset attendu ", f->identifiant, a->ligne,
				*file_erreur);
	}
}

void
verif_immediat_ope(file_jeu_instruction * file_erreur,
				   file_jeu_instruction f, file_text a)
{
	if (strcasecmp(f->identifiant, "Valeur Hexadécimale") == 0
		|| strcasecmp(f->identifiant, "Valeur Décimale") == 0) {
		if (atoi(f->caractere) < -32768 || atoi(f->caractere) > 32767) {
			*file_erreur =
			enfiler("Nombre sur 16 bit signé attendu", f->caractere,
					a->ligne, *file_erreur);
		} else {
			return;
		}
	} else {
		if (strcasecmp(f->identifiant, "Renvoie vers une étiquette") == 0
			|| strcasecmp(f->identifiant, "EtiquettePFort") == 0) {
			return;
		}
		*file_erreur =
		enfiler("Type Immediat attendu", f->identifiant, a->ligne,
				*file_erreur);
	}
}

void
verif_shiftamount_ope(file_jeu_instruction * file_erreur,
					  file_jeu_instruction f, file_text a)
{
	if (strcasecmp(f->identifiant, "Valeur Hexadécimale") == 0
		|| strcasecmp(f->identifiant, "Valeur Décimale") == 0) {
		if (atoi(f->caractere) < 0 || atoi(f->caractere) > 31) {
			*file_erreur =
			enfiler("Nombre entre 0 et 31 attendu", f->caractere,
					a->ligne, *file_erreur);
		} else {
			return;
		}
	} else {
		*file_erreur =
		enfiler("Type Shift Amount attendu", f->identifiant, a->ligne,
				*file_erreur);
	}
}

void
verif_relatif_ope(file_jeu_instruction * file_erreur,
				  file_jeu_instruction f, file_text a)
{
	if (strcasecmp(f->identifiant, "Valeur Hexadécimale") == 0
		|| strcasecmp(f->identifiant, "Valeur Décimale") == 0) {
		if (atoi(f->caractere) < -131072 || atoi(f->caractere) > 131071
			|| atoi(f->caractere) % 4 != 0) {
			*file_erreur =
			enfiler
			("Nombre sur 18 bit signé attendu et divisible par 4",
			 f->caractere, a->ligne, *file_erreur);
		} else {
			int             i = atoi(f->caractere) / 4;
			char            buffer[26];
			sprintf(buffer, "%d", i);
			strcpy(f->caractere, buffer);
			return;
		}
	} else {
		if (strcasecmp(f->identifiant, "Renvoie vers une étiquette") == 0) {
			return;
		}
		*file_erreur =
		enfiler("Type Relatif attendu", f->identifiant, a->ligne,
				*file_erreur);
	}
}

void
verif_absolu_ope(file_jeu_instruction * file_erreur,
				 file_jeu_instruction f, file_text a)
{
	if (strcasecmp(f->identifiant, "Valeur Hexadécimale") == 0
		|| strcasecmp(f->identifiant, "Valeur Décimale") == 0) {
		if ((atoi(f->caractere) < -134217728
			 || atoi(f->caractere) > 134217727)
			|| atoi(f->caractere) % 4 != 0) {
			*file_erreur =
			enfiler
			("Nombre sur 18 bit signé attendu et divisible par 4",
			 f->caractere, a->ligne, *file_erreur);
		} else {
			return;
		}
	} else {
		if (strcasecmp(f->identifiant, "Renvoie vers une étiquette") == 0) {
			return;
		}
		*file_erreur =
		enfiler("Type Absolu attendu", f->identifiant, a->ligne,
				*file_erreur);
	}
}

