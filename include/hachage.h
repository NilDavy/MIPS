#ifndef hachage_h
#define hachage_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>


#define dim_tab_registre 32
#define dim_tab_instruction 15

/* Structure de liste pour le hachage */
typedef char* instruction;
struct cellule {
	instruction val;
	int nombreOperande;
	char typeInst;
	struct cellule * suiv;};
typedef struct cellule * Liste_hach;

Liste_hach creer_liste_hachage(void);
int est_vide_hachage(Liste_hach);
Liste_hach ajout_tete_hachage(instruction e,int nbop, char car, Liste_hach L);
unsigned int hachage(char* , int );
int rech_hachage(instruction, Liste_hach);
int rec_hachage_nbparam(instruction, Liste_hach);
Liste_hach supprimer_tete_hachage(Liste_hach);
void liberer_tab_hachage(Liste_hach*, int );
Liste_hach*creation_liste_registre(void);
Liste_hach*creation_liste_instruction(void);
void visualiser_liste_hachage(Liste_hach l);
void visualiser_tab_hachage(Liste_hach*tab,int n);
#endif /* hachage_h */
