#ifndef hachage_h
#define hachage_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>


#define dim_tab_registre 32
#define dim_tab_instruction 15
#define nbre_registre 64
#define nbre_instruction 29

/* Structure de liste pour le hachage */
typedef char instruction[25];
struct cellule {
	instruction val;
	int nombreOperande;
	char typeInst[8];
	char type1[25];
	char type2[25];
	char type3[25];
	struct cellule * suiv;};
typedef struct cellule * Liste_hach;
Liste_hach creer_liste_hachage(void);
int est_vide_hachage(Liste_hach);
Liste_hach ajout_tete_hachage(instruction e,int nbop, char*car,char*typeinstruction1,char*typeinstruction2,char*typeinstruction3, Liste_hach L);
unsigned int hachage(char* , int );
int rech_hachage(instruction, Liste_hach);
int rec_hachage_nbparam(instruction, Liste_hach);
Liste_hach supprimer_tete_hachage(Liste_hach);
void liberer_tab_hachage(Liste_hach*, int );
void creation_liste_registre(Liste_hach*tab_registre,int dim);
void creation_liste_instruction(Liste_hach*tab_instruction,int dim);
void visualiser_liste_hachage(Liste_hach l);
void visualiser_tab_hachage(Liste_hach*tab,int n);
char*rec_hachage_type(instruction e, Liste_hach L);
void rec_hachage_type_instruction(instruction e, Liste_hach L,char*mot1,char*mot2,char*mot3);
#endif /* hachage_h */
