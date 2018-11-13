#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "global.h"
#include "notify.h"
#include "lex.h"
#include "analyse_lexicale.h"
#include "hachage.h"
#include "file.h"
#include "file_bss.h"
#include "file_data.h"
#include "file_text.h"
#include "file_symb.h"
#include "analyse_syntaxique.h"


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
	int i;
	
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
	
	
	
	/** ouverture ou creation du fichier contenant le code instancié **/

	FILE*fp=NULL;
	fp=fopen("recapitulatif/Analyse_Lexicale.txt", "w+t");
	if (fp == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier Analyse_lexicale\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture ou creation du fichier contenant le rapport des erreurs **/
	FILE*f_erreur=NULL;
	f_erreur=fopen("recapitulatif/Recaputilatif_Erreur.txt", "wt");
	if (f_erreur == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier Erreur\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture ou creation du fichier contenant le rapport de la section BSS**/
	FILE*f_bss=NULL;
	f_bss=fopen("recapitulatif/Recaputilatif_BSS.txt", "wt");
	if (f_bss == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier BSS\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture ou creation du fichier contenant le rapport de la section DATA**/
	FILE*f_data=NULL;
	f_data=fopen("recapitulatif/Recaputilatif_DATA.txt", "wt");
	if (f_data == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier DATA\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture ou creation du fichier contenant le rapport de la section TEXT**/
	FILE*f_text=NULL;
	f_text=fopen("recapitulatif/Recaputilatif_TEXT.txt", "wt");
	if (f_text == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier TEXT\n" );
		exit(EXIT_FAILURE);
	}
	
	/** ouverture ou creation du fichier contenant le rapport de la section SYMB**/
	FILE*f_symb=NULL;
	f_symb=fopen("recapitulatif/Recaputilatif_SYMB.txt", "wt");
	if (f_symb == NULL) {
		fprintf( stderr, "Erreur sur l'ouverture du fichier SYMB\n" );
		exit(EXIT_FAILURE);
	}
	/** creation des tableaux pour les tables de hachages des instructions et des registres **/

	Liste_hach tab_registre[dim_tab_registre];
	for(i=0;i<dim_tab_registre;i++){
		tab_registre[i]=creer_liste_hachage();
	}
	creation_liste_registre(tab_registre,dim_tab_registre);
	
	
	Liste_hach tab_instruction[dim_tab_instruction];
	for(i=0;i<dim_tab_instruction;i++){
		tab_instruction[i]=creer_liste_hachage();
	}
	creation_liste_instruction(tab_instruction,dim_tab_instruction);
	
	/** variable interne contenant le code instancié **/
	file_jeu_instruction file_lexeme=creer_file();
	file_jeu_instruction file_erreur=creer_file();
	
	file_text co_text=creerfile_text();
	file_bss co_bss=creerfile_bss();
	file_data co_data=creerfile_data();
	file_symb co_symb=creerfile_symb();
	file_symb co_text_attente=creerfile_symb();
	file_symb co_data_attente=creerfile_symb();
	file_symb co_bss_attente=creerfile_symb();
	
	/* ---------------- do the lexical analysis -------------------*/
	lex_load_file( file, &nlines,tab_registre,tab_instruction,&file_lexeme,&file_erreur);
	DEBUG_MSG("Le code source contient %d lignes",nlines);
	
	if(!(file_vide(file_lexeme))){
		file_lexeme=modifie_instruction(file_lexeme);
		/*verif_renvoie_vers_etiquette(&file_lexeme,&file_erreur);*/
		file_lexeme=verif_delimiteur_suite(file_lexeme,&file_erreur);
		file_lexeme=verif_remplacement_ecriture_registre(file_lexeme,&file_erreur,tab_registre);
	}
	
	/*visualiser_file(file_lexeme);*/
	/** Ecriture du code instancié dans le fichier **/

	ecrire_file(file_lexeme, fp);
	fclose(fp);

	/** Vérification si présence d'erreurs **/
	
	if(!(file_vide(file_erreur))){
		WARNING_MSG("Il y a des erreurs de lexique dans le code source !");
		printf("************************  ERREUR  ************************\n \n");
		
		visualiser_file(file_erreur);
		ecrire_file(file_erreur, f_erreur);
	printf("**********************************************************\n");
	}
	else{
		DEBUG_MSG("Il n'y a pas d'erreur de lexique dans le code source !");
		
		/*analyse syntaxique*/
		

		analyse_syntaxique(tab_instruction,&file_lexeme,&file_erreur,&co_text, &co_data, &co_bss, &co_symb,& co_text_attente,&co_data_attente,&co_bss_attente);
		if(!file_vide_text(co_text)){
			verif_operande(co_text,&file_erreur,tab_instruction);
		}
		
		/*visualiser_tab_hachage(tab_registre, dim_tab_registre);
		visualiser_tab_hachage(tab_instruction, dim_tab_instruction);*/
		
		ecrire_file_bss(co_bss,f_bss);
		ecrire_file_data(co_data,f_data);
		ecrire_file_text(co_text,f_text);
		ecrire_file_symb(co_symb,f_symb);

		
		if(!(file_vide(file_erreur))){
			WARNING_MSG("Il y a des erreurs de syntaxe dans le code source !");
			printf("************************  ERREUR  ************************\n \n");
			
			visualiser_file(file_erreur);
			ecrire_file(file_erreur, f_erreur);
			printf("**********************************************************\n");
		}
		else{
			DEBUG_MSG("Il n'y a pas d'erreur de syntaxe dans le code source !");
			printf("\n**********************************************************\n");
		}
	
	}

	/* ---------------- Free memory and terminate -------------------*/

	/** Libération des mémoires **/
	liberer_symb(co_symb);
	liberer_symb(co_bss_attente);
	liberer_symb(co_data_attente);
	liberer_symb(co_text_attente);
	liberer_data(co_data);
	liberer_bss(co_bss);
	liberer_text(co_text);
	liberer_file(file_lexeme);
	liberer_file(file_erreur);
	liberer_tab_hachage(tab_registre, dim_tab_registre);
	liberer_tab_hachage(tab_instruction, dim_tab_instruction);
	fclose(f_erreur);
	fclose(f_bss);
	fclose(f_data);
	fclose(f_text);
	fclose(f_symb);
	exit( EXIT_SUCCESS );
}
