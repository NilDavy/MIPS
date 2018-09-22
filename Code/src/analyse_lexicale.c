#include "lex.h"

/** Fonction principale qui distingue les cas **/
char* analyse_lexicale(char*token,char*current_address,char*delimiteur,Liste_hach*tab_registre,Liste_hach*tab_instruction,Liste_jeu_instruction *Liste,unsigned int nlines,FILE*f_erreur){
	char commentaire[longueur_max]="";
	char*mot1=calloc(strlen(token),sizeof(token));
	char*mot2=calloc(strlen(token),sizeof(token));
	int j=0;
	
	j=verif_delimitateur(token,mot1,mot2);
	
	switch (mot1[0]) {
			
/** Commentaire **/
		case '#':
			
			/** Vérification si la chaine n'est pas trop longue puis concaténation en une seule chaine**/
			if((strlen(token)+strlen(commentaire))<longueur_max){
				strcat(commentaire, token);
			}
			else{
				fprintf( stderr, "La chaine de caractère est trop longue\n" );
				exit(EXIT_FAILURE);
			}
			token=getNextToken(current_address,delimiteur);
			
			while( token != NULL){
				
				if((strlen(token)+strlen(commentaire)+strlen(" "))<longueur_max){
					strcat(commentaire, " ");
					strcat(commentaire, token);
				}
				else{
					fprintf( stderr, "La chaine de caractère est trop longue\n" );
					exit(EXIT_FAILURE);
				}

				token=getNextToken(current_address,delimiteur);
			}
			 *Liste=ajout_queue("Commentaire", commentaire,nlines, *Liste);
			break;
			
/** directive **/
		case '.':
			verif_directive(mot1,current_address,delimiteur,Liste,nlines,f_erreur);
			break;

/** registre **/
		case '$':
			verif_registre(mot1,tab_registre,Liste,nlines,f_erreur);
			break;
			
/** Delimiteur **/
		case '(':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case ')':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case '-':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case '+':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case '/':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case ':':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
			
		case ',':
			*Liste=ajout_queue("Délimiteur", mot1, nlines, *Liste);
			break;
		
/** Chaine de caractere **/
		case '"':
			verif_chaine_de_caractère(mot1,current_address,delimiteur,Liste,nlines,f_erreur);
			break;
			
		default:
/** Caractere alphabetique **/
			if (isalpha(token[0])){
				verif_caractere(mot1,mot2,current_address,delimiteur,tab_instruction,Liste,nlines,f_erreur);
			}
			
/** caractere numérique **/
			if (isdigit(token[0])){
				verif_numero(mot1,Liste,nlines,f_erreur);
			}
	}
	
	if(j){
		token=analyse_lexicale(mot2,current_address,delimiteur,tab_registre,tab_instruction,Liste,nlines,f_erreur);
	}
	else{
		token=getNextToken(current_address,delimiteur);
	}
	free(mot1);
	free(mot2);
	return token;
}

/** Verifie l'orthographe des directives **/
void verif_directive(char*token,char*current_address,char*delimiteur,Liste_jeu_instruction*Liste,unsigned int nlines,FILE*f_erreur){
	int i;
	int compteur=0;
	char*not="noreorder";
	char*tab[8]={".word",".bss",".data",".text",".byte",".asciiz",".space"};
	for(i=0;i<7;i++){
		if(strcmp(token,tab[i])!=0){
			compteur=compteur+1;
		}
	}
	
/** Verification qu'apres .set il y ait bien noreorder **/
	if(strcmp(token,".set")==0){
		*Liste=ajout_queue("Directive", token, nlines, *Liste);
		token=getNextToken(current_address,delimiteur);
		if(strcmp(token,not) ==0){
			*Liste=ajout_queue("Complement Directive .set", token, nlines, *Liste);
		}
		else{
			compteur=compteur+1;
		}
	}
	if(compteur==8){
		fprintf(f_erreur,"Erreur sur la directive %s, ligne %d\n\n",token,nlines);
	}
	else{
		*Liste=ajout_queue("Directive", token, nlines, *Liste);
	}
}

/** Verifie l'orthographe des registres **/
void verif_registre(char*token,Liste_hach*tab_registre,Liste_jeu_instruction*Liste,unsigned int nlines,FILE*f_erreur){
	int n=hachage(token, 32);
	n=rech_hachage(token, tab_registre[n]);
	if (n==-1){
		fprintf(f_erreur,"Erreur sur le registre %s, ligne %d\n", token, nlines);
	}
	else{
		*Liste=ajout_queue("Registre", token, nlines, *Liste);
	}
}

/** Verifie l'orthographe des caractères **/
void verif_caractere(char*token,char*mot2,char*current_address,char*delimiteur,Liste_hach*tab_instruction,Liste_jeu_instruction *Liste,unsigned int nlines,FILE*f_erreur){
	int i;
	int n;
	int a;
	char*mot=calloc(strlen(token),sizeof(token));

	for(i=0;i<strlen(token);i++){
		token[i]=toupper(token[i]);
	}
	n=hachage(token, dim_tab_instruction);
	
	a=rech_hachage(token, tab_instruction[n]);
	
	
	if (a==1){
		*Liste=ajout_queue("Instruction", token, nlines, *Liste);
	}
	else{
		if(strcmp(mot2, ":")==0){
			*Liste=ajout_queue("Etiquette", token, nlines, *Liste);
		}
		else{
			strcpy(mot, token);
			token=getNextToken(current_address,delimiteur);
			if(token!=NULL && token[0]==':'){
				*Liste=ajout_queue("Etiquette", token, nlines, *Liste);
			}
			else{
				*Liste=ajout_queue("Renvoie vers une étiquette", mot, nlines, *Liste);
			}
		}
	}
	free(mot);
}

/** Verifie l'orthographe des numéros **/
void verif_numero(char*token,Liste_jeu_instruction*Liste,unsigned int nlines,FILE*f_erreur){
	int i=0;
	while (isdigit(token[i])){
		i=i+1;
	}
	
	if(strlen(token)==i){
		*Liste=ajout_queue("Valeur Décimale", token, nlines, *Liste);
	}
	else{
		if(token[i]=='x' && strlen(token)<11){
			i=i+1;
			while(isxdigit(token[i])){
				i=i+1;
			}
			if(strlen(token)==i){
				*Liste=ajout_queue("Valeur Hexadécimale", token, nlines, *Liste);
			}
			else{
				fprintf(f_erreur,"Erreur sur les caractères %s, ligne %d\n",token, nlines);
			}
		}
	}
}

/** Verifie l'orthographe des chaines de caractères **/
void verif_chaine_de_caractère(char*token,char*current_address,char*delimiteur,Liste_jeu_instruction*Liste,unsigned int nlines,FILE*f_erreur){
	char mot[longueur_max]="";
	while(strlen(token)==1 || token[strlen(token)-1]!='"' || (token[strlen(token)-1]=='"' && token[strlen(token)-2]=='\\')){
		if(token!=NULL){
			if((strlen(token)+strlen(mot)+strlen(" "))<longueur_max){
				strcat(mot, token);
				strcat(mot, " ");
			}
			else{
				fprintf( stderr, "La chaine de caractère est trop longue\n" );
				exit(EXIT_FAILURE);
			}
		}
		token=getNextToken(current_address,delimiteur);
	}
	
	if((strlen(token)+strlen(mot))<longueur_max){
		strcat(mot, token);
	}
	else{
		fprintf( stderr, "La chaine de caractère est trop longue\n" );
		exit(EXIT_FAILURE);
	}

	*Liste=ajout_queue("Chaine de caractère", mot, nlines, *Liste);
}

/**Sépare les tokens en plusieurs chaine suivant les délimiteurs **/
int verif_delimitateur(char*token,char*mot1,char*mot2){
	int i;
	int j=0;
	for(i=0;i<strlen(token);i++){
		if((token[i]=='/') || (token[i]=='(')|| (token[i]==',')|| (token[i]==')')|| (token[i]==':')|| (token[i]=='-')|| (token[i]=='+')){
			if (strlen(token)>1){
				j=1;
			}
			if(i==0){
				strncpy(mot1, token, 1);
				strncpy(mot2, token+1, strlen(token)-1);
				return j;
			}
			else{
				strncpy(mot1, token, i);
				strncpy(mot2, token+i, strlen(token)-i);
				return j;
			}
		}
	}
	strncpy(mot1, token, strlen(token));
	return j;
}
