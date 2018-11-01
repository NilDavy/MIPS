#include "analyse_syntaxique.h"

void analyse_syntaxique(Liste_hach*tab_instruction,file_jeu_instruction*file, file_jeu_instruction* file_erreur, file_text *co_text, file_data *co_data, file_bss *co_bss, file_symb *co_symb,file_symb *co_text_attente, file_symb *co_data_attente, file_symb *co_bss_attente)
{
	int cpt_text, cpt_data, cpt_bss; 
	cpt_text = cpt_data = cpt_bss = 0;
	enum section etat = INIT;
	file_jeu_instruction g = supprime_commentaire(*file);
	file_jeu_instruction h=g;
	g = g->suiv;
	h->suiv=NULL;
	while(g!= NULL)
	{
		/*printf("Nouvelle ligne : %s\n",g->caractere);
		getchar();*/
		
		
		
		/*ici on regarde dans quelle section on se trouve avec une priorité mise en place si on change de section (.qqch avant l'etat courant*/
		
		if(!strcmp(g->caractere, ".text")){
			/*printf("text\n");
			printf("%s\n",g->caractere);*/
			etat = TEXT;
			g = processText(g, co_text, &cpt_text, file_erreur, co_symb,co_text_attente,tab_instruction);
		}
		else{
			if (!strcmp(g->caractere, ".data")){
				/*printf("data\n");
				printf("%s\n",g->caractere);*/
				etat = DATA;
				g = processData(g, co_data, &cpt_data, file_erreur, co_symb,co_data_attente);
			}
			else{
				if(!strcmp(g->caractere, ".bss")){
					/*printf("bss\n");
					printf("%s\n",g->caractere);*/
					etat = BSS;
					g = processBss(g, co_bss, &cpt_bss, file_erreur, co_symb,co_bss_attente);
				}
				else{
					if(etat==TEXT){
						/*printf("text\n");
						printf("%s\n",g->caractere);*/
						etat = TEXT;
						g = processText(g, co_text, &cpt_text, file_erreur, co_symb,co_text_attente,tab_instruction);
					}
					else{
						if (etat==DATA){
							/*printf("data\n");
							printf("%s\n",g->caractere);*/
							etat = DATA;
							g = processData(g, co_data, &cpt_data, file_erreur, co_symb,co_data_attente);
						}
						else{
							if(etat==BSS){
								/*printf("bss\n");
								printf("%s\n",g->caractere);*/
								etat = BSS;
								g = processBss(g, co_bss, &cpt_bss, file_erreur, co_symb,co_bss_attente);
							}
							else{
								/*les cas ou il ne faut pas mettre d'erreur*/
								
								if(strcmp(g->caractere, ".set") && strcmp(g->identifiant, "Retour à la ligne")&&strcmp(g->caractere,"noreorder")){
									*file_erreur = enfiler("Directive attendu à la place de :", g->caractere, g->ligne, *file_erreur);
								}
							}
						}
					}
				}
			}
		}
		g=g->suiv;
	}
}




/* Les fonctions suivantes renvoie une file qui se situe au debut de la ligne suivant celle qui a été analysée*/

/* Fonction qui rempli la liste d'instruction dans le cas ou on est dans .text */
file_jeu_instruction processText(file_jeu_instruction file, file_text *co_text, int *cpt_text, file_jeu_instruction* file_erreur, file_symb *co_symb, file_symb *co_text_attente,Liste_hach*tab_instruction)
{
	file_jeu_instruction f = file;
	file_jeu_instruction g=creer_file();
	char*mot;
	char copie[200];
	char*instruction=NULL;
	char*identifiant="";
	int nbop;
	int verif=0;
	int concatenation;
	
	
	/*les cas ou on ne fait rien .text, : et retour ligne*/
	
	if (!strcmp(f->caractere, ".text")||!strcmp(f->caractere, ":")||!strcmp(f->identifiant, "Retour à la ligne")){
		return f;
	}
	else{
		/*cas d'un etiquette, on la stocke dans une file d'attente pour eviter les cas ou etiquette puis changement de section*/
		if (!strcmp(f->identifiant, "Etiquette")){
			/*ajout dans la table des symboles d'attente*/
			*co_text_attente=ajout_symb(f->caractere, f->ligne, *cpt_text,"TEXT", *co_text_attente);
			return f;
		}
		else{
			/* dans la section .text il y a que les instructions*/
			
			if(!strcmp(f->identifiant, "Instruction")){
				while(!file_vide_symb(*co_text_attente)){
					/*ajout dans la table des symboles*/
					mot=defiler_symb(co_text_attente);
					strcpy(copie,mot);
					*co_symb=ajout_symb(copie, f->ligne, *cpt_text,"TEXT", *co_symb);
				}
				instruction=f->caractere;
				
				/*recherche nombre d'opération*/
				nbop=rec_hachage_nbparam(instruction, tab_instruction[hachage(instruction, dim_tab_instruction)]);
				
				while(strcmp(f->identifiant,"Retour à la ligne")){
					f=f->suiv;
					char*mot1=calloc(200,sizeof(char));
					concatenation=0;
					
					/*on concatene si base offset*/
					while(strcmp(f->identifiant, "Retour à la ligne") && strcmp(f->caractere, ",")){
						strcat(mot1,f->caractere);
						concatenation=concatenation+1;
						identifiant=f->identifiant;
						f=f->suiv;
					}
					/*ajout a la table .text*/
					if(concatenation>1){
						g=enfiler("Base offset",mot1,f->ligne,g);
						verif=verif+1;
					}
					else{
						g=enfiler(identifiant,mot1,f->ligne,g);
						verif=verif+1;
					}
					free(mot1);
				}
				
				/*verif si erreur avec le nombre d'operation*/
				if(verif==nbop){
					*co_text=ajout_text(instruction,nbop,f->ligne, *cpt_text, *co_text,g);
					*cpt_text=*cpt_text+4;
				}
				else{
					*file_erreur = enfiler("Erreur sur le nombre d'opérande", f->caractere, f->ligne, *file_erreur);
				}
			}
			else{
				*file_erreur = enfiler("Mauvaise commande dans la section .text", f->caractere, f->ligne, *file_erreur);
				f=f->suiv;
			}
	
		}
	}
	return f;
}




/* Fonction qui rempli la liste de data dans le cas ou on est dans .data */
file_jeu_instruction processData(file_jeu_instruction file, file_data *co_data, int* cpt_data, file_jeu_instruction *file_erreur, file_symb *co_symb,file_symb *co_data_attente)
{
	file_jeu_instruction f = file;
	char*mot;
	int vide =0;
	char copie[200];
	char*mot1=calloc(200,sizeof(char));
	
	/*les cas ou on ne fait rien .data, : et retour ligne*/
	if (!strcmp(f->caractere, ".data")||!strcmp(f->caractere, ":")||!strcmp(f->identifiant, "Retour à la ligne")){
		return f;
	}
	else{
		if (!strcmp(f->identifiant, "Etiquette")){
			/*ajout dans la table des symboles d'attente*/
			*co_data_attente=ajout_symb(f->caractere, f->ligne, *cpt_data,"DATA", *co_data_attente);
			return f;
		}
		else{
			
			/*dans la section .data il peut y avoir .space, .word, .byte ou .asciiz*/
			
			if(!strcmp(f->caractere, ".space")){
				while(!file_vide_symb(*co_data_attente)){
					/*ajout dans la table des symboles*/
					mot=defiler_symb(co_data_attente);
					strcpy(copie,mot);
					*co_symb=ajout_symb(copie, f->ligne, *cpt_data,"DATA", *co_symb);
				}
				f=f->suiv;
				while(strcmp(f->identifiant, "Retour à la ligne")){
					if(!strcmp(f->identifiant, "Valeur Décimale") || !strcmp(f->identifiant, "Valeur Hexadécimale")){
						/*Creer maillon data, ici il peut y avoir apres space soit un hex qui est deja transformee en dec ou un dec*/
						/* au final c'est un dec quoi qu'il arrive et positif car on veut une taille*/
						*co_data=ajout_data(".space", f->ligne, *cpt_data,f->caractere, 2, *co_data);
						*cpt_data=(*cpt_data)+1;
					}
					else{
						if(strcmp(f->caractere, ",")){
							*file_erreur = enfiler("Mauvaise commande après la directive .space", f->identifiant, f->ligne, *file_erreur);
						}
					}
					f=f->suiv;
				}
			}
		
			else{
					if(!strcmp(f->caractere, ".byte")){
						while(!file_vide_symb(*co_data_attente)){
							/*ajout dans la table des symboles*/
							mot=defiler_symb(co_data_attente);
							strcpy(copie,mot);
							*co_symb=ajout_symb(copie, f->ligne, *cpt_data,"DATA", *co_symb);
						}
						f=f->suiv;
						while(strcmp(f->identifiant, "Retour à la ligne")){
							/*si inf a 127*/
							if(!strcmp(f->identifiant, "Valeur Décimale")&& atoi(f->caractere)<127){
								*co_data=ajout_data(".byte", f->ligne, *cpt_data, f->caractere, 2, *co_data);
								*cpt_data=(*cpt_data)+1;
							}
							/*si sup à -128*/
							else{
								if(!strcmp(f->identifiant, "Délimiteur") && (!strcmp(f->caractere, "-")) && atoi(f->suiv->caractere)<128){
									char*mot2=calloc(200,sizeof(char));
									strcat(mot2,f->caractere);
									strcat(mot2,f->suiv->caractere);
									*co_data=ajout_data(".byte", f->ligne, *cpt_data,mot2, 2, *co_data);
									*cpt_data=(*cpt_data)+1;
									free(mot2);
								}
								else{
									/*hexa compris entre 0x0 et 0xff*/
									if(!strcmp(f->identifiant, "Valeur Hexadécimale")&& atoi(f->caractere)<256){
										*co_data=ajout_data(".byte", f->ligne, *cpt_data,f->caractere, 2, *co_data);
										*cpt_data=(*cpt_data)+1;
									}
									else{
										if(strcmp(f->caractere, ",")){
											*file_erreur = enfiler("Mauvaise commande après la directive .byte", f->caractere, f->ligne, *file_erreur);
										}
									}
									
								}
							}
							
							f=f->suiv;
						}
					}
		
					else{
						if(!strcmp(f->caractere, ".word")){
							while(!file_vide_symb(*co_data_attente)){
								/*ajout dans la table des symboles*/
								mot=defiler_symb(co_data_attente);
								strcpy(copie,mot);
								*co_symb=ajout_symb(copie, f->ligne, *cpt_data,"DATA", *co_symb);
							}
							f=f->suiv;
							while(strcmp(f->identifiant, "Retour à la ligne")){
								if(!strcmp(f->identifiant, "Valeur Décimale")|| !strcmp(f->identifiant, "Valeur Hexadécimale")){
									*co_data=ajout_data(".word", f->ligne, *cpt_data, f->caractere, 2, *co_data);
									*cpt_data=4*((*cpt_data)/4+1);
								}
								else{
									/*si nombre negatif*/
									if(!strcmp(f->identifiant, "Délimiteur") && (!strcmp(f->caractere, "-")) && !strcmp(f->suiv->identifiant, "Valeur Décimale")){
										char*mot2=calloc(200,sizeof(char));
										strcat(mot2,f->caractere);
										strcat(mot2,f->suiv->caractere);
										*co_data=ajout_data(".word", f->ligne, *cpt_data,mot2, 2, *co_data);
										*cpt_data=4*((*cpt_data)/4+1);
										free(mot2);
									}
									else{
										if(!strcmp(f->identifiant, "Renvoie vers une étiquette")){
											*co_data=ajout_data(".word", f->ligne, *cpt_data,f->caractere, 4, *co_data);
											*cpt_data=4*((*cpt_data)/4+1);
										}
										else{
											if(strcmp(f->caractere, ",")){
												*file_erreur = enfiler("Mauvaise commande après la directive .word", f->identifiant, f->ligne, *file_erreur);
											}
										}
									}
								}
								f=f->suiv;
							}
						}
						
						else{
							if(!strcmp(f->caractere, ".asciiz")){
								while(!file_vide_symb(*co_data_attente)){
									/*ajout dans la table des symboles*/
									mot=defiler_symb(co_data_attente);
									strcpy(copie,mot);
									*co_symb=ajout_symb(copie, f->ligne, *cpt_data,"DATA", *co_symb);
								}
								f=f->suiv;
								
								/*concat chaine de caractère*/
								while(strcmp(f->identifiant, "Retour à la ligne")){
									if(!strcmp(f->identifiant, "Chaine de caractère")){
										strcat(mot1,f->caractere);
										vide=1;
									}
									else{
											*file_erreur = enfiler("Mauvaise commande après la directive .asciiz", f->identifiant, f->ligne, *file_erreur);
									}
									f=f->suiv;
									if(strcmp(f->identifiant, "Retour à la ligne")){
										strcat(mot1," ");
									}
								}
								if(vide==1){
									*co_data=ajout_data(".asciiz", f->ligne, *cpt_data,mot1, 4, *co_data);
									*cpt_data=(*cpt_data)+strlen(mot1)-1;
								}
							}
							else{
								*file_erreur = enfiler("Mauvaise directive", " ", f->ligne, *file_erreur);
								f=f->suiv;
							}
						}
					}
			}
		}
	}
	free(mot1);
	return f;
}
	
	
	




/* Fonction qui rempli la liste bss dans le cas ou on est dans .bss */
file_jeu_instruction processBss(file_jeu_instruction file, file_bss *co_bss, int* cpt_bss, file_jeu_instruction *file_erreur, file_symb *co_symb, file_symb *co_bss_attente)
{
	file_jeu_instruction f = file;
	char*mot;
	char copie[200];
	
	/*les cas ou on ne fait rien .bss, : et retour ligne*/
	if (!strcmp(f->caractere, ".bss")||!strcmp(f->caractere, ":")||!strcmp(f->identifiant, "Retour à la ligne")){
		return f;
	}
	else{
		if (!strcmp(f->identifiant, "Etiquette")){
			/*ajout dans la table des symboles d'attente*/
			*co_bss_attente=ajout_symb(f->caractere, f->ligne, *cpt_bss,"BSS", *co_bss_attente);
			return f;
		}
		else {
			
			/*dans la section .bss il peut y avoir que .space*/
			
			if(!strcmp(f->caractere, ".space")){
				while(!file_vide_symb(*co_bss_attente)){
					/*ajout dans la table des symboles*/
					mot=defiler_symb(co_bss_attente);
					strcpy(copie,mot);
					*co_symb=ajout_symb(copie, f->ligne, *cpt_bss,"BSS", *co_symb);

				}
				
				f=f->suiv;
				while(strcmp(f->identifiant, "Retour à la ligne")){
					if(!strcmp(f->identifiant, "Valeur Décimale") || !strcmp(f->identifiant, "Valeur Hexadécimale")){
						/*Creer maillon bss, ici il peut y avoir apres space soit un hex qui est deja transformee en dec ou un dec*/
						/* au final c'est un dec quoi qu'il arrive et positif car on veut une taille*/
						*co_bss=ajout_bss(".space", f->ligne, *cpt_bss,f->caractere, 2, *co_bss);
						*cpt_bss=(*cpt_bss)+1;
					}
					else{
						if(strcmp(f->caractere, ",")){
							*file_erreur = enfiler("Mauvaise commande après la directive .space", f->identifiant, f->ligne, *file_erreur);
						}
					}
					f=f->suiv;
				}
			}
			else{
				*file_erreur = enfiler("Directive .space attendu à la place de :", f->caractere, f->ligne, *file_erreur);
				f=f->suiv;
			}

		}
	}
	return f;
}
