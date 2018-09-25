#ifndef liste_h
#define liste_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>


#define longueur_max 5096

struct cellule1 {
	char identifiant[longueur_max];
	char caractere[longueur_max];
	int ligne;
	struct cellule1 * suiv;};

typedef struct cellule1* Liste_jeu_instruction;

Liste_jeu_instruction creer_liste(void);
int liste_vide(Liste_jeu_instruction l);
void visualiser_liste(Liste_jeu_instruction l);
Liste_jeu_instruction ajout_tete(char*nom,char*carac,int line, Liste_jeu_instruction l);
Liste_jeu_instruction supprimer_tete(Liste_jeu_instruction l);
void liberer(Liste_jeu_instruction  l);
Liste_jeu_instruction ajout_queue(char*nom,char*carac,int line, Liste_jeu_instruction l);

#endif /* liste_h */
