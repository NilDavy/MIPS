#define _POSIX_C_SOURCE 200112L

#include "lex.h"

/* ATTENTION: getNextToken est juste un exemple -> il est à recoder completement !!! */
/**
 * @ param current_adress
 * @ param delimiteur
 * @ return the address at which the analysis stopped or NULL if the analysis is terminated.
 * @ brief This function get an input line, extracts a token from it and return the pointeur to the next place where the analysis should continue.
 */

/* note that MIPS assembly supports distinctions between lower and upper case*/
char* getNextToken(char* current_address,char* delimiteur) {
		return strtok(NULL,delimiteur);
}



/**
 * @ param line String of the line of source code to be analysed.
 * @ param nline the line number in the source code.
 * @ return should return the collection of lexemes that represent the input line of source code.
 * @ brief This function performs lexical analysis of one standardized line.
 *
 */
void lex_read_line( char *line, int nline,Liste_hach*tab_registre,Liste_hach*tab_instruction,file_jeu_instruction*file,unsigned int nlines,file_jeu_instruction*file_erreur) {
    char* token = NULL;
    char* current_address=line;
	char* delimiteur = " ";

	token=strtok(current_address, delimiteur);
	
    /* TODO : faire l'analyse lexical de chaque token ici et les ajouter dans une collection*/
    /* ATTENTION: getNextToken est à recoder completement*/
    while( token != NULL ){
		token=analyse_lexicale(token, current_address,delimiteur,tab_registre,tab_instruction,file,nlines,file_erreur);
    }
	*file=enfiler("Retour à la ligne", " ", nlines, *file);
	return;
}


/**
 * @ param file Assembly source code file name.
 * @ param nlines Pointer to the number of lines in the file.
 * @ return should return the collection of lexemes
 * @ brief This function loads an assembly code from a file into memory.
 *
 */
void lex_load_file( char *file, unsigned int *nlines,Liste_hach*tab_registre,Liste_hach*tab_instruction,file_jeu_instruction*files,file_jeu_instruction*file_erreur) {

    FILE        *fp   = NULL;
    char         line[STRLEN]; /* original source line */



    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) ) {
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;

            if ( 0 != strlen(line) ) {
                lex_read_line(line,*nlines,tab_registre,tab_instruction,files,*nlines,file_erreur);
            }
        }
    }

    fclose(fp);
    return;
}
