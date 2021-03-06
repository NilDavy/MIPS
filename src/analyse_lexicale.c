#include "lex.h"

/** Fonction principale qui distingue les cas **/
char           *
analyse_lexicale(char *token, char *current_address,
		 char *delimiteur, Liste_hach * tab_registre,
		 Liste_hach * tab_instruction,
		 file_jeu_instruction * file, unsigned int nlines,
		 file_jeu_instruction * file_erreur, int *compteur)
{
    char           *mot1 = calloc(strlen(token), sizeof(token));
    if (mot1 == NULL) {
	fprintf(stderr, "Memory error : mot1\n");
	exit(EXIT_FAILURE);
    }
    char           *mot2 = calloc(strlen(token), sizeof(token));
    if (mot2 == NULL) {
	fprintf(stderr, "Memory error : mot2\n");
	exit(EXIT_FAILURE);
    }
    int             j = 0;
	int a=0;
    j = verif_delimitateur(token, mot1, mot2);

	/*printf("mot1 : %s\n",mot1); printf("mot2 : %s\n",mot2);
	printf("j :%d\n",j);*/
    switch (mot1[0]) {

/** Commentaire **/
    case '#':
	while (token != NULL) {
	    *file = enfiler("Commentaire", token, nlines, *file);
	    token = getNextToken(current_address, delimiteur);
/** verif longueur de la ligne **/
	    if (token != NULL) {
		*compteur = *compteur + strlen(token);
		if (*compteur > longeur_ligne) {
		    *file_erreur =
			enfiler("Ligne trop longue",
				"Doit etre inférieure à 200 caractères sans espaces",
				nlines, *file_erreur);
		    token = NULL;
		}
	    }
	}
	break;

/** directive **/
    case '.':
	verif_directive(mot1, current_address, delimiteur, file, nlines,
			file_erreur, compteur);
	break;

/** registre **/
    case '$':
	verif_registre(mot1, tab_registre, file, nlines, file_erreur);
	break;

/** Delimiteur **/
    case '(':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case ')':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case '-':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case '+':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case '/':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case ':':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

    case ',':
	*file = enfiler("Délimiteur", mot1, nlines, *file);
	break;

/** Chaine de caractere **/
    case '"':
	a=1;
	verif_chaine_de_caractere(mot1, current_address, delimiteur, file,
				  nlines, file_erreur, compteur,mot2);
	break;
    case 9:
	break;
    default:
/** Caractere alphabetique **/
	if (isalpha(mot1[0]) || mot1[0] == '_') {
	    int             c;
/** Verif des caractères dans la chaine **/
	    for (c = 1; c < strlen(mot1); c++) {
		if ((mot1[c] != '_') && !isalpha(mot1[c])
		    && !isdigit(mot1[c])) {

		    *file_erreur = enfiler("Mauvais caractere dans la chaine", mot1,
				nlines, *file_erreur);
		    return getNextToken(current_address, delimiteur);
		}

	    }

	    if (strlen(mot2) == 0) {
		mot2 = getNextToken(NULL, delimiteur);
		verif_caractere(mot1, mot2, current_address, delimiteur,
				tab_instruction, file, nlines,
				file_erreur);
		return mot2;
	    } else {
		verif_caractere(mot1, mot2, current_address, delimiteur,
				tab_instruction, file, nlines,
				file_erreur);
	    }

	}

/** caractere numérique **/
	else if (isdigit(mot1[0])) {
	    verif_numero(mot1, file, nlines, file_erreur);
	} else {
	    *file_erreur =
		enfiler("Mauvais caractere au debut de la chaine", mot1,
			nlines, *file_erreur);
	}
    }
    if (j&&a==0) {
		token = analyse_lexicale(mot2, current_address, delimiteur,tab_registre, tab_instruction, file, nlines,file_erreur, compteur);
    } else {
	if (token != NULL) {
	    token = getNextToken(current_address, delimiteur);

/** verif longueur de la ligne **/

	    if (token != NULL) {
		*compteur = *compteur + strlen(token);
		if (*compteur > longeur_ligne) {
		    *file_erreur =
			enfiler("Ligne trop longue",
				"Doit etre inférieure à 200 caractères sans espaces",
				nlines, *file_erreur);
		    token = NULL;
		}
	    }
	}
    }
    free(mot1);
    free(mot2);

    return token;
}

/** Verifie l'orthographe des directives **/
void
verif_directive(char *token, char *current_address, char *delimiteur,
		file_jeu_instruction * file, unsigned int nlines,
		file_jeu_instruction * file_erreur, int *compteur)
{
    int             i;
    int             compteur1 = 0;
    char           *not = "noreorder";
    char           *tab[8] =
	{ ".word", ".bss", ".data", ".text", ".byte", ".asciiz",
	".space"
    };
	/** Verification qu'apres .set il y ait bien noreorder **/
    if (strcmp(token, ".set") == 0) {
	*file = enfiler("Directive", token, nlines, *file);
	token = getNextToken(current_address, delimiteur);
/** verif longueur de la ligne **/
	if (token != NULL) {
	    *compteur = *compteur + strlen(token);
	    if (*compteur > longeur_ligne) {
		*file_erreur =
		    enfiler("Ligne trop longue",
			    "Doit etre inférieure à 200 caractères sans espaces",
			    nlines, *file_erreur);
		token = NULL;
	    }
	}
	if (strcmp(token, not) == 0) {
	    *file =
		enfiler("Complement Directive .set", token, nlines, *file);
	}
	return;
    }

/** verif des autres directives **/

    for (i = 0; i < 7; i++) {
	if (strcmp(token, tab[i]) != 0) {
	    compteur1 = compteur1 + 1;
	}
    }
    if (compteur1 == 7) {
	*file_erreur =
	    enfiler("Erreur directive : ", token, nlines, *file_erreur);
    } else {
	*file = enfiler("Directive", token, nlines, *file);
    }
}

/** Verifie l'orthographe des registres **/
void
verif_registre(char *token, Liste_hach * tab_registre,
	       file_jeu_instruction * file, unsigned int nlines,
	       file_jeu_instruction * file_erreur)
{
    /*
     * int i; for(i=1;i<strlen(token);i++){ token[i]=tolower(token[i]); } 
     */
    int             n = hachage(token, dim_tab_registre);
    n = rech_hachage(token, tab_registre[n]);
    if (n == -1) {
	*file_erreur =
	    enfiler("Erreur nom registre", token, nlines, *file_erreur);
    } else {
	*file = enfiler("Registre", token, nlines, *file);
    }
}

/** Verifie l'orthographe des caractères **/
void
verif_caractere(char *token, char *mot2, char *current_address,
		char *delimiteur, Liste_hach * tab_instruction,
		file_jeu_instruction * file, unsigned int nlines,
		file_jeu_instruction * file_erreur)
{
    int             n;
    int             a;

    /*
     * int i; for(i=0;i<strlen(token);i++){ token[i]=toupper(token[i]); } 
     */
    n = hachage(token, dim_tab_instruction);

    a = rech_hachage(token, tab_instruction[n]);

    /*
     * printf("%s %d\n",token,a); 
     */
    if (a == 1) {
	*file = enfiler("Instruction", token, nlines, *file);
    }

    else {
	if ((strcmp(mot2, "€") != 0 && strcmp(mot2, ":") == 0)
	    || (strcmp(mot2, "€") != 0 && mot2[0] == ':')) {
	    *file = enfiler("Etiquette", token, nlines, *file);
	} else {
	    if (strcmp(mot2, "€") == 0
		|| (strcmp(mot2, "€") != 0
		    && (mot2[0] == '#' || mot2[0] == ','))) {
		*file =
		    enfiler("Renvoie vers une étiquette", token, nlines,
			    *file);
	    } else {
		*file_erreur =
		    enfiler("Erreur chaine de caractère ", token, nlines,
			    *file_erreur);
	    }

	}
    }
}

/** Verifie l'orthographe des numéros **/
void
verif_numero(char *token, file_jeu_instruction * file,
	     unsigned int nlines, file_jeu_instruction * file_erreur)
{
    int             i = 0;

    unsigned long   a;
    while (isdigit(token[i])) {
	i = i + 1;
    }
    a = (unsigned long) atoi(token);
    if (strlen(token) == i && a <= 4294967295) {
	*file = enfiler("Valeur Décimale", token, nlines, *file);
	return;
    } else {
/** verif 32 bits **/

	if (strlen(token) == i && a > 4294967295) {
	    *file_erreur =
		enfiler("Depassement capacite", token, nlines,
			*file_erreur);
	    return;
	}

/** verif hexa **/

	if ((token[i] == 'x' || token[i] == 'X') && strlen(token) < 11
	    && strlen(token) >= 3 && i == 1) {
	    i = i + 1;
	    while (isxdigit(token[i])) {
		i = i + 1;
	    }
	}
	if (strlen(token) == i) {
	    int             b = (int) strtol(token, NULL, 0);
	    char            s[11];
	    sprintf(s, "%d", b);
	    *file = enfiler("Valeur Hexadécimale", s, nlines, *file);
	} else {
	    *file_erreur =
		enfiler("Erreur valeur numérique", token, nlines,
			*file_erreur);
	}

    }
}

/** Verifie l'orthographe des chaines de caractères **/
void verif_chaine_de_caractere(char *token, char *current_address,char *delimiteur,file_jeu_instruction * file, unsigned int nlines, file_jeu_instruction * file_erreur, int *compteur,char*mot2)
{
	int i=0,compteur1=0;
	char mot[longeur_ligne];
	int longueur;
	int decompteur;
	int attente;
	strcpy(mot,"");
	strcat(mot,token);
	if(strcasecmp(mot2,"€")!=0){
		strcat(mot,mot2);
	}
	token=getNextToken(current_address, delimiteur);
	if(token!=NULL){
		*compteur = *compteur + strlen(token);
		if (*compteur > longeur_ligne) {
			*file_erreur =enfiler("Ligne trop longue","Doit etre inférieure à 200 caractères sans espaces",nlines, *file_erreur);
			token = NULL;
		}
	}
		
	while (token!=NULL) {
		strcat(mot," ");
		strcat(mot,token);
		token=getNextToken(current_address, delimiteur);
		
		if(token!=NULL){
			*compteur = *compteur + strlen(token);
			if (*compteur > longeur_ligne) {
				*file_erreur =enfiler("Ligne trop longue","Doit etre inférieure à 200 caractères sans espaces",nlines, *file_erreur);
				token = NULL;
			}
		}
	}
	/*printf("mot %s\n",mot);*/
	for(i=0;i<strlen(mot)-1;i++){
		if(mot[i+1]=='"'&&mot[i]!='\\'){
			compteur1=compteur1+1;
		}
	}
	if(compteur1%2!=1){
		*file_erreur = enfiler("Erreur de fin de chaine", "\" de fin manquant",nlines, *file_erreur);
	}
	else{
		i=0;
		longueur=strlen(mot);
		decompteur=0;
		while(i<longueur){
				char modif[longeur_ligne];
				if(mot[i-decompteur+1]=='"' &&mot[i-decompteur]!='\\'){
					strncat(modif,mot, i+2);
					*file = enfiler("Chaine de caractère", modif, nlines, *file);
					attente=strlen(modif);
					strcpy(modif,"");
					if(i!=longueur-2){
						i=i+2;
						if(mot[i-decompteur]=='#'){
							strcat(modif,&mot[i-decompteur]);
							i=strlen(mot)-1;
							*file = enfiler("Commentaire", modif, nlines, *file);
						}
						else if(mot[i-decompteur]==','){
							*file = enfiler("Délimiteur", ",", nlines, *file);
							strcat(modif,&mot[i+1]);
							strcpy(mot,"");
							decompteur=decompteur+2;
							strcpy(mot,modif);
						}
						else if(mot[i-decompteur]=='+'||mot[i-decompteur]=='-'||mot[i-decompteur]=='/'||mot[i-decompteur]==':'||mot[i-decompteur]==':'||mot[i-decompteur]=='('||mot[i-decompteur]=='\''||mot[i-decompteur]==')'){
								*file_erreur = enfiler("Erreur de caractère", "dans un .asciiz",nlines, *file_erreur);
						}
					}
					else{
						i=i+1;
					}
					decompteur=decompteur+attente;
				}
				if(mot[0]==' '){
					strcpy(modif,&mot[1]);
					strcpy(mot,modif);
					decompteur=decompteur+2;
				}
				strcpy(modif,"");
				i=i+1;
			}
	}
}

/**Sépare les tokens en plusieurs chaine suivant les délimiteurs **/
int
verif_delimitateur(char *token, char *mot1, char *mot2)
{
    int             i;
    int             j = 0;
    for (i = 0; i < strlen(token); i++) {

/** test de tous les delimiteurs**/

	if ((token[i] == '/') || (token[i] == '(') || (token[i] == ',')
	    || (token[i] == ')') || (token[i] == ':') || (token[i] == '-')
	    || (token[i] == '+') || (token[i] == 9) || (token[i] == '#')) {
	    if (strlen(token) > 1) {
		j = 1;
	    }
	    if (i == 0) {
		strncpy(mot1, token, 1);
		strncpy(mot2, token + 1, strlen(token) - 1);
		return j;
	    } else {
		strncpy(mot1, token, i);
		strncpy(mot2, token + i, strlen(token) - i);
		return j;
	    }
	}
    }
    strcpy(mot1, token);
    strcpy(mot2, "€");
    return j;
}
