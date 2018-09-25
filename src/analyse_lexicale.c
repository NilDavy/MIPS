#include "lex.h"

/** Fonction principale qui distingue les cas **/
char* analyse_lexicale(char*token,char*current_address,char*delimiteur,Liste_hach*tab_registre,Liste_hach*tab_instruction,Liste_jeu_instruction *Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur){
	char commentaire[longueur_max]="";
	char*mot1=calloc(strlen(token),sizeof(token));
	if(mot1==NULL){
		fprintf( stderr, "Memory error : mot1\n" );
		exit(EXIT_FAILURE);
	}
	char*mot2=calloc(strlen(token),sizeof(token));
	if(mot2==NULL){
		fprintf( stderr, "Memory error : mot2\n" );
		exit(EXIT_FAILURE);
	}
	int j=0;
	
	j=verif_delimitateur(token,mot1,mot2);
	
	/*printf("1: %s\n",mot1);
	printf("2: %s\n",mot2);*/
	
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
			verif_directive(mot1,current_address,delimiteur,Liste,nlines,Liste_erreur);
			break;

/** registre **/
		case '$':
			verif_registre(mot1,tab_registre,Liste,nlines,Liste_erreur);
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
			verif_chaine_de_caractere(mot1,current_address,delimiteur,Liste,nlines,Liste_erreur);
			break;
		case 9 :
			break;
		default:
/** Caractere alphabetique **/
			if (isalpha(mot1[0])){
				int c;
				for (c = 1; c < strlen(mot1); c++)
				{
					if( !(isalpha(mot1[c]) || isdigit(mot1[c]) || mot1[c] == '_'))
					{
						*Liste_erreur=ajout_queue("Mauvais caractere dans la chaine", mot1, nlines, *Liste_erreur);
					}
					
				}
				
				if(strlen(mot2)==0){
					mot2=getNextToken(current_address, delimiteur);
					verif_caractere(mot1,mot2,current_address,delimiteur,tab_instruction,Liste,nlines,Liste_erreur);
					return mot2;
				}
				else{
					verif_caractere(mot1,mot2,current_address,delimiteur,tab_instruction,Liste,nlines,Liste_erreur);
				}
				
			}
			
/** caractere numérique **/
			else if (isdigit(mot1[0])){
				verif_numero(mot1,Liste,nlines,Liste_erreur);
			}
			else
			{
				printf("%s\n", mot1);
				*Liste_erreur=ajout_queue("Mauvais caractere au debut de la chaine", mot1, nlines, *Liste_erreur);
			}
	}
	
	if(j){
		token=analyse_lexicale(mot2,current_address,delimiteur,tab_registre,tab_instruction,Liste,nlines,Liste_erreur);
	}
	else{
		token=getNextToken(current_address,delimiteur);
	}
	free(mot1);
	free(mot2);
	return token;
}

/** Verifie l'orthographe des directives **/
void verif_directive(char*token,char*current_address,char*delimiteur,Liste_jeu_instruction*Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur){
	int i;
	int compteur=0;
	char*not="noreorder";
	char*tab[8]={".word",".bss",".data",".text",".byte",".asciiz",".space"};
	
	/** Verification qu'apres .set il y ait bien noreorder **/
	if(strcmp(token,".set")==0){
		*Liste=ajout_queue("Directive", token, nlines, *Liste);
		token=getNextToken(current_address,delimiteur);
		if(strcmp(token,not) ==0){
			*Liste=ajout_queue("Complement Directive .set", token, nlines, *Liste);
		}
		return;
	}
		

	for(i=0;i<7;i++){
		if(strcmp(token,tab[i])!=0){
			compteur=compteur+1;
		}
	}
	if(compteur==7){
		*Liste_erreur=ajout_queue("Erreur directive : ", token, nlines, *Liste_erreur);
	}else{	
	*Liste=ajout_queue("Directive", token, nlines, *Liste);
	}
}

/** Verifie l'orthographe des registres **/
void verif_registre(char*token,Liste_hach*tab_registre,Liste_jeu_instruction*Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur){
	int n=hachage(token, 32);
	n=rech_hachage(token, tab_registre[n]);
	if (n==-1){
		*Liste_erreur=ajout_queue("Erreur nom registre", token, nlines, *Liste_erreur);
	}
	else{
	*Liste=ajout_queue("Registre", token, nlines, *Liste);
	}
}

/** Verifie l'orthographe des caractères **/
void verif_caractere(char*token,char*mot2,char*current_address,char*delimiteur,Liste_hach*tab_instruction,Liste_jeu_instruction *Liste,unsigned int nlines,Liste_jeu_instruction *Liste_erreur){
	int i;
	int n;
	int a;
	
	for(i=0;i<strlen(token);i++){
		token[i]=toupper(token[i]);
	}
	n=hachage(token, dim_tab_instruction);
	
	a=rech_hachage(token, tab_instruction[n]);
	
	
	if (a==1){
		*Liste=ajout_queue("Instruction", token, nlines, *Liste);
	}
	
	else{
		if((mot2!=NULL && strcmp(mot2, ":")==0) || (mot2!=NULL && mot2[0]==':')){
			*Liste=ajout_queue("Etiquette", token, nlines, *Liste);
		}
		else{
			if(mot2==NULL || (mot2!=NULL && mot2[0]=='#')){
				*Liste=ajout_queue("Renvoie vers une étiquette", token, nlines, *Liste);
			}
			else{
				*Liste_erreur=ajout_queue("Erreur chaine de caractère ", token, nlines, *Liste_erreur);
			}
				
		}
	}
}

/** Verifie l'orthographe des numéros **/
void verif_numero(char*token,Liste_jeu_instruction*Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur){
	int i=0;
	while (isdigit(token[i])){
		i=i+1;
	}
	
	if(strlen(token)==i){
		*Liste=ajout_queue("Valeur Décimale", token, nlines, *Liste);
	}
	else{
		if(strlen(token)>=11){
			*Liste_erreur=ajout_queue("Erreur Héxadecimale", token, nlines, *Liste_erreur);
			return;
		}
		
		else{
			if((token[i]=='x' || token[i]=='X') && strlen(token)<11){
				i=i+1;
				while(isxdigit(token[i])){
					i=i+1;
				}
				if(strlen(token)==i){
					*Liste=ajout_queue("Valeur Hexadécimale", token, nlines, *Liste);
				}
				else
				{
				*Liste_erreur=ajout_queue("Erreur valeur numérique", token, nlines, *Liste_erreur);
				}	
				
			}
		}
	}
}

/** Verifie l'orthographe des chaines de caractères **/
void verif_chaine_de_caractere(char*token,char*current_address,char*delimiteur,Liste_jeu_instruction*Liste,unsigned int nlines,Liste_jeu_instruction*Liste_erreur){
	char mot[longueur_max]="";

	while ((token!=NULL)&&(token[strlen(token)-1]!='"' || (strlen(token)==1) || (token[strlen(token)-1]!='"') || (token[strlen(token)-1]=='"'&& token[strlen(token)-2]=='\\'))){
			if((strlen(token)+strlen(mot)+strlen(" "))<longueur_max){
				strcat(mot, token);
				strcat(mot, " ");
			}
			else{
				fprintf( stderr, "La chaine de caractère est trop longue\n" );
				exit(EXIT_FAILURE);
			}
		   token=getNextToken(current_address,delimiteur);
		}
	
	if(token==NULL){
		*Liste_erreur=ajout_queue("Erreur de fin de chaine", "\" de fin manquant", nlines, *Liste_erreur);
		return;
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
	/*printf("%s\n",token);*/
	for(i=0;i<strlen(token);i++){
		if((token[i]=='/') || (token[i]=='(')|| (token[i]==',')|| (token[i]==')')|| (token[i]==':')|| (token[i]=='-')|| (token[i]=='+') || (token[i] == 9)){
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
