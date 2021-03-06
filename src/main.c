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
#include "table_relocation.h"
#include "ecrire_binaire.h"
#include "pelf/section.h"

/**
 * @ param exec Name of executable.
 * @ return Nothing.
 * @ brief Print usage.
 *
 */
void
print_usage(char *exec)
{
    fprintf(stderr, "Usage: %s file.s\n", exec);
}



/**
 * @ param argc Number of arguments on the command line.
 * @ param argv Value of arguments on the command line.
 * @ return Whether this was a success or not.
 * @ brief Main entry point for MIPS assembler.
 *
 */
int
main(int argc, char *argv[])
{

    unsigned int    nlines = 0;
    char           *file = NULL;
    int             i;

    /*
     * exemples d'utilisation des macros du fichier notify.h 
     */
    /*
     * WARNING_MSG : sera toujours affiche 
     */
    /*
     * WARNING_MSG("Un message WARNING_MSG !"); 
     */

    /*
     * macro INFO_MSG : uniquement si compilé avec -DVERBOSE. Cf.
     * Makefile 
     */
    /*
     * INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]); 
     */

    /*
     * macro DEBUG_MSG : uniquement si compilé avec -DDEBUG (ie :
     * compilation avec make debug). Cf. Makefile 
     */
    /*
     * DEBUG_MSG("Un message DEBUG_MSG !"); 
     */

    /*
     * La macro suivante provoquerait l'affichage du message puis la
     * sortie du programme avec un code erreur non nul (EXIT_FAILURE) 
     */
    /*
     * ERROR_MSG("Erreur. Arret du programme"); 
     */


    if (argc < 2) {
	print_usage(argv[0]);
	exit(EXIT_FAILURE);
    }


    file = argv[argc - 1];

    if (NULL == file) {
	fprintf(stderr, "Missing ASM source file, aborting.\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le code instancié **/

    FILE           *fp = NULL;
    fp = fopen("recapitulatif/Analyse_Lexicale.txt", "w+t");
    if (fp == NULL) {
	fprintf(stderr,
		"Erreur sur l'ouverture du fichier Analyse_lexicale\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le rapport des erreurs **/
    FILE           *f_erreur = NULL;
    f_erreur = fopen("recapitulatif/Recaputilatif_Erreur.txt", "wt");
    if (f_erreur == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier Erreur\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le rapport de la section BSS**/
    FILE           *f_bss = NULL;
    f_bss = fopen("recapitulatif/Recaputilatif_BSS.txt", "wt");
    if (f_bss == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier BSS\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le rapport de la section DATA**/
    FILE           *f_data = NULL;
    f_data = fopen("recapitulatif/Recaputilatif_DATA.txt", "wt");
    if (f_data == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier DATA\n");
	exit(EXIT_FAILURE);
    }


	/** ouverture ou creation du fichier contenant le rapport de la section TEXT**/
    FILE           *f_text = NULL;
    f_text = fopen("recapitulatif/Recaputilatif_TEXT.txt", "wt");
    if (f_text == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier TEXT\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le rapport de la section SYMB**/
    FILE           *f_symb = NULL;
    f_symb = fopen("recapitulatif/Recaputilatif_SYMB.txt", "wt");
    if (f_symb == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier SYMB\n");
	exit(EXIT_FAILURE);
    }

	/** ouverture ou creation du fichier contenant le rapport de la table de relocation**/
    FILE           *f_reloc = NULL;
    f_reloc = fopen("recapitulatif/Recaputilatif_Relocation.txt", "wt");
    if (f_reloc == NULL) {
	fprintf(stderr, "Erreur sur l'ouverture du fichier Relocation\n");
	exit(EXIT_FAILURE);
    }

	/** creation des tableaux pour les tables de hachages des instructions et des registres **/

    Liste_hach      tab_registre[dim_tab_registre];
    for (i = 0; i < dim_tab_registre; i++) {
	tab_registre[i] = creer_liste_hachage();
    }
    creation_liste_registre(tab_registre, dim_tab_registre);

    Liste_hach      tab_instruction[dim_tab_instruction];

    for (i = 0; i < dim_tab_instruction; i++) {
	tab_instruction[i] = creer_liste_hachage();
    }
    creation_liste_instruction(tab_instruction, dim_tab_instruction);

	
	/*visualiser_tab_hachage(tab_registre, dim_tab_registre);*/
	/*visualiser_tab_hachage(tab_instruction, dim_tab_instruction);*/

	/** variable interne contenant le code instancié **/
    file_jeu_instruction file_lexeme = creer_file();
    file_jeu_instruction file_erreur = creer_file();

    file_text       co_text = creerfile_text();
    file_bss        co_bss = creerfile_bss();
    file_data       co_data = creerfile_data();
    file_symb       co_symb = creerfile_symb();
    file_symb       co_text_attente = creerfile_symb();
    file_symb       co_data_attente = creerfile_symb();
    file_symb       co_bss_attente = creerfile_symb();
    table_reloc     reloc_text = NULL;
    table_reloc     reloc_data = NULL;

    int             nbtext;
    int             nbdata;
    int             cptbss;

    /*
     * ---------------- do the lexical analysis ------------------- 
     */
    lex_load_file(file, &nlines, tab_registre, tab_instruction,
		  &file_lexeme, &file_erreur);

    printf("\n");
    INFO_MSG("Le code source contient %d lignes", nlines);
    /*visualiser_file(file_lexeme); */

    if (!(file_vide(file_lexeme))) {
	file_lexeme = modifie_instruction(file_lexeme);
	/*
	 * verif_renvoie_vers_etiquette(&file_lexeme,&file_erreur); 
	 */
	file_lexeme = verif_delimiteur_suite(file_lexeme, &file_erreur);
	file_lexeme =
	    verif_remplacement_ecriture_registre(file_lexeme, &file_erreur,
						 tab_registre);
	file_lexeme = modif_chaine_caractere(file_lexeme);
    }

	/*visualiser_file(file_lexeme);*/

	/** Ecriture du code instancié dans le fichier **/
    if (!(file_vide(file_lexeme))) {
	ecrire_file(file_lexeme, fp);
    }

	/** Vérification si présence d'erreurs **/

    if (!(file_vide(file_erreur))) {
	WARNING_MSG("Il y a des erreurs de lexique dans le code source !");
	printf
	    ("************************  ERREUR  ************************\n \n");

	visualiser_file(file_erreur);
	ecrire_file(file_erreur, f_erreur);
	printf
	    ("**********************************************************\n\n");
    } else {
	INFO_MSG("Il n'y a pas d'erreur de lexique dans le code source !");

	/*
	 * analyse syntaxique 
	 */


	if (!(file_vide(file_lexeme))) {
	    analyse_syntaxique(tab_instruction, file_lexeme, &file_erreur,
			       &co_text, &co_data, &co_bss, &co_symb,
			       &co_text_attente, &co_data_attente,
			       &co_bss_attente, &nbtext, &nbdata, &cptbss);
	}
	/*
	 * printf("text %d data %d bss %d\n",nbtext,nbdata,cptbss); 
	 */

	/*
	 * Remplissage table de relocation 
	 */

	if (!file_vide_data(co_data)) {
	    reloc_data =
		remplirTableRelocationData(co_data, &co_symb,
					   tab_instruction, &file_erreur);
	}

	if (!file_vide_text(co_text)) {
	    reloc_text =
		remplirTableRelocationText(co_text,&co_symb,
					   tab_instruction, &file_erreur);
	    verif_operande(co_text, &file_erreur, tab_instruction,
			   tab_registre);
	}

	/*
	 * visualiser_tab_hachage(tab_registre, dim_tab_registre);
	 * visualiser_tab_hachage(tab_instruction, dim_tab_instruction); 
	 */


	ecrire_file_bss(co_bss, f_bss);
	ecrire_file_data(co_data, f_data);
	ecrire_file_text(co_text, f_text);

	ecrire_file_symb(co_symb, f_symb);
	fprintf(f_reloc, "[.rel.text]\nOffset\t Type\t Value\n");
	ecrire_table(reloc_text, f_reloc);
	fprintf(f_reloc, "\n[.rel.data]\nOffset\t Type\t Value\n");
	ecrire_table(reloc_data, f_reloc);

	if (!(file_vide_symb(co_symb))) {
	    verif_etiquette(co_symb, &file_erreur);
	}
	if (!(file_vide(file_erreur))) {
	    WARNING_MSG
		("Il y a des erreurs de syntaxe dans le code source !");
	    printf
		("************************  ERREUR  ************************\n \n");

	    visualiser_file(file_erreur);
	    ecrire_file(file_erreur, f_erreur);
	    printf
		("**********************************************************\n\n");
	} else {
	    INFO_MSG
		("Il n'y a pas d'erreur de syntaxe dans le code source !");

	    /*
	     * visualiser_file_text(co_text); 
	     */
		/*visualiser_file_symb(co_symb); */
	    /*
	     * visualiser_file_data(co_data); visualiser_file_bss(co_bss); 
	     */

	    /*
	     * co_text= modif_etiquette(co_text); 
	     */

	    /*
	     * il reste à generer les codes binaires 
	     */
	    /*
	     * il faut creer les tables shstrtab : table des noms de
	     * section text, data : données (PROGBITS) bss : espace à
	     * alloué au lancement du programme strtab : autres
	     * chaînes de caractères (symboles) Symtab : symboles
	     * rel.text, rel.data : sections de relocation 
	     */

	    /*
	     * visualiser_table(reloc_text); visualiser_table(reloc_data); 
	     */

	    /*
	     * visualiser_file_text(co_text); 
	     */

	    char           *machine = "mips";
	    int             noreorder = 1;
	    char           *name = calloc(strlen(file) + 1, sizeof(char));
	    strncat(name, file, strlen(file) - 2);
	    strcat(name, ".o");

	    /*
	     * prepare sections 
	     */
	    section         text = NULL;
	    section         data = NULL;
	    section         bss = NULL;
	    section         shstrtab = NULL;
	    section         strtab = NULL;
	    section         symtab = NULL;
	    section         reltext = NULL;
	    section         reldata = NULL;

	    /*
	     * section shestrtab 
	     */
	    shstrtab = make_shstrtab_section();
	    /*
	     * printf("shstrtab fini\n"); 
	     */


	    /*
	     * section strtab 
	     */
	    strtab = make_strtab_section(co_symb);
	    /*print_section(strtab); */
	    /*
	     * printf("strtab fini\n"); 
	     */

	    /*
	     * section symtab 
	     */
	    symtab =
		make_symtab_section(shstrtab, strtab, co_symb, nlines);
	    /*print_section(symtab);*/
	    /*
	     * printf("symtab fini\n"); 
	     */

	    reltext =
		make_rel32_section(".rel.text", reloc_text, symtab,
				   shstrtab, strtab);
	    reldata =
		make_rel32_section(".rel.data", reloc_data, symtab,
				   shstrtab, strtab);
	    /*
	     * printf("reloc table fini\n"); 
	     */

	    /*
	     * section bss 
	     */
	    if (!file_vide_bss(co_bss)) {
		/*
		 * printf("debut creation bss\n"); 
		 */
		creer_section_bss(&bss, cptbss);
		/*print_section(bss);*/
		/*
		 * printf("bss fini\n"); 
		 */
	    }
		else{
			bss=new_section(".bss",SECTION_CHUNK_SZ);
			bss->sz=0;
		}

	    /*
	     * section data 
	     */
		if (!file_vide_data(co_data)) {
		creer_section_data(&data, nbdata, co_data, co_symb);
		/*print_section(data);*/
		/*
		 * printf("data fini\n"); 
		 */
	    }
		else{
			data=new_section(".data",SECTION_CHUNK_SZ);
			data->sz=0;
		}
		
	    /*
	     * section text 
	     */
	    if (!file_vide_text(co_text)) {
		creer_section_text(&text, nbtext, co_text, tab_instruction,
				   tab_registre, co_symb);
				/*print_section(text);*/
		/*
		 * printf("text fini\n");
		 */
	    }
		else{
			text=new_section(".text",SECTION_CHUNK_SZ);
			text->sz=0;
		}

		elf_write_relocatable(name, machine, noreorder,
				      text->start, text->sz,
				      data->start, data->sz,
				      bss->start, bss->sz,
				      shstrtab->start, shstrtab->sz,
				      strtab->start, strtab->sz,
				      symtab->start, symtab->sz,
				      reltext->start, reltext->sz,
				      reldata->start, reldata->sz);
		INFO_MSG("Programme Terminé !");
		printf
		    ("\n**********************************************************\n\n");
	    del_section(text);
	    del_section(data);
	    del_section(bss);
	    del_section(shstrtab);
	    del_section(strtab);
	    del_section(symtab);
	    del_section(reltext);
	    del_section(reldata);

	}
    }
    /*
     * ---------------- Free memory and terminate ------------------- 
     */

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
    liberer_table(reloc_data);
    liberer_table(reloc_text);
    liberer_tab_hachage(tab_registre, dim_tab_registre);
    liberer_tab_hachage(tab_instruction, dim_tab_instruction);
    fclose(fp);
    fclose(f_erreur);
    fclose(f_reloc);
    fclose(f_bss);
    fclose(f_data);
    fclose(f_text);
    fclose(f_symb);

    exit(EXIT_SUCCESS);
}
