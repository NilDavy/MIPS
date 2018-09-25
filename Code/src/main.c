#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "global.h"
#include "notify.h"
#include "lex.h"
#include "analyse_lexicale.h"
#include "hachage.h"
#include "liste.h"

/**
 * @ param exec Name of executable.
 * @ return Nothing.
 * @ brief Print usage.
 *
 */
void print_usage( char *exec ) {
	fprintf(stderr, "Usage: %s file.s\n",
			exec);
}



/**
 * @ param argc Number of arguments on the command line.
 * @ param argv Value of arguments on the command line.
 * @ return Whether this was a success or not.
 * @ brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {
	
	unsigned int 	nlines 	= 0;
	char         	 *file 	= NULL;
	
	/* exemples d'utilisation des macros du fichier notify.h */
	/* WARNING_MSG : sera toujours affiche */
	/*WARNING_MSG("Un message WARNING_MSG !");*/
	
	/* macro INFO_MSG : uniquement si compilé avec -DVERBOSE. Cf. Makefile*/
	INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);
	
	/* macro DEBUG_MSG : uniquement si compilé avec -DDEBUG (ie : compilation avec make debug). Cf. Makefile */
	/*DEBUG_MSG("Un message DEBUG_MSG !");*/
	
	/* La macro suivante provoquerait l'affichage du message
	 puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
	/* ERROR_MSG("Erreur. Arret du programme"); */
	
	
	if ( argc <2 ) {
		print_usage(argv[0]);
		exit( EXIT_FAILURE );
	}
	
	
	file  	= argv[argc-1];
	
	
	if ( NULL == file ) {
		fprintf( stderr, "Missing ASM source file, aborting.\n" );
		exit( EXIT_FAILURE );
	}
	
	
	
	/** ouverture du fichier contenant le code instancié **/

	FILE*fp=NULL;
	fp=fopen("Analyse_Lexicale.txt", "w+t");
	if (fp == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier Analyse_lexicale\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture du fichier contenant le rapport des erreurs **/
	FILE*f_erreur=NULL;
	f_erreur=fopen("Recaputilatif_Erreur.txt", "wt");
	if (f_erreur == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier Erreur\n" );
		exit(EXIT_FAILURE);
	}
	
	/** creation des tableaux pour les tables de hachages des instructions et des registres **/
	Liste_hach* tab_registre=NULL;
	tab_registre=calloc(dim_tab_registre, sizeof(*tab_registre));
	tab_registre=creation_liste_registre();
	
	Liste_hach* tab_instruction=NULL;
	tab_instruction=calloc(dim_tab_instruction, sizeof(*tab_instruction));
	tab_instruction=creation_liste_instruction();
	
	
	/** variable interne contenant le code instancié **/
	Liste_jeu_instruction Liste_lexeme=creer_liste();
	Liste_jeu_instruction Liste_erreur=creer_liste();
	Liste_jeu_instruction i=creer_liste();
	
	
	
	/* ---------------- do the lexical analysis -------------------*/
	lex_load_file( file, &nlines,tab_registre,tab_instruction,&Liste_lexeme,&Liste_erreur);
	DEBUG_MSG("Le code source contient %d lignes",nlines);
	

	
	/** Ecriture du code instancié dans le fichier **/
	for(i=Liste_lexeme;!liste_vide(i);i=i->suiv){
		fprintf(fp, "%s\t %s\t %d\n\n" ,i->identifiant,i->caractere,i->ligne);
	}

	
	/** Vérification si présence d'erreurs **/

	
	if(!(liste_vide(Liste_erreur))){
		for(i=Liste_erreur;!liste_vide(i);i=i->suiv){
			fprintf(f_erreur, "Ligne : %d\t %s\t %s\n\n" ,i->ligne, i->identifiant,i->caractere);
		}
		
		
		WARNING_MSG("Il y a des erreurs de lexique dans le code source !");
		printf("************************  ERREUR  ************************\n \n");
	}
	else{
		DEBUG_MSG("Il n'y a pas d'erreur de lexique dans le code source !");
	}
	
	for(i=Liste_erreur;!liste_vide(i);i=i->suiv){
			printf("Ligne : %d\t %s\t %s\n\n" ,i->ligne, i->identifiant,i->caractere);
		}
	printf("\n**********************************************************\n");
	/* ---------------- Free memory and terminate -------------------*/
	
	/** Libération des mémoires **/
	liberer(Liste_lexeme);
	liberer(Liste_erreur);
	liberer_tab_hachage(tab_registre, dim_tab_registre);
	liberer_tab_hachage(tab_instruction, dim_tab_instruction);
	fclose(fp);
	fclose(f_erreur);
	exit( EXIT_SUCCESS );
}
