#include "liste.h"

/* Creation d'une liste basique */
Liste_jeu_instruction creer_liste(void) {
	return NULL;
}

/* Verification si la liste basique est vide
 Renvoie 1 si la liste est vide, 0 sinon */
int liste_vide(Liste_jeu_instruction l) {
	return !l;
}

/* Visualisation de la liste basique */
void visualiser_liste(Liste_jeu_instruction l) {
	Liste_jeu_instruction c = NULL;
	if (liste_vide(l)) {
		puts("liste vide");
		return;
	}
	for (c = l; !liste_vide(c); c = c->suiv) {
		printf("%s\t %s\t %d\n",c->identifiant,c->caractere,c->ligne);
	}
	printf("\n");
}

/* Ajout d'un element en tete de la liste basique */
Liste_jeu_instruction ajout_tete(char*nom,char*carac,int line, Liste_jeu_instruction l) {
	Liste_jeu_instruction c = NULL;
	c = calloc(1,sizeof(*c));
	if (c == NULL) {
		return NULL;
	}
	strcpy(c->identifiant, nom);
	strcpy(c->caractere, carac);
	c->ligne=line;
	c->suiv = l;
	return c;
}

/* Suppression d'un element en tete de la liste basique */
Liste_jeu_instruction supprimer_tete(Liste_jeu_instruction l) {
	Liste_jeu_instruction c = NULL;
	if (liste_vide(l)) {
		return NULL;
	}
	c = l->suiv;
	free(l);
	return c;
}

/* Liberation de la liste basique */
void liberer(Liste_jeu_instruction  l) {
	Liste_jeu_instruction c = NULL;
	c = l;
	while (c  != NULL) {
		c = supprimer_tete(c);
	}
}

/* Ajout d'un element en queue de la liste basique */
Liste_jeu_instruction ajout_queue(char*nom,char*carac,int line, Liste_jeu_instruction l) {
	Liste_jeu_instruction c = NULL;
	Liste_jeu_instruction p = NULL;
	
	if (liste_vide(l)) {
		return ajout_tete(nom,carac,line,l);
	}
	c = calloc(1,sizeof(*c));
	if (liste_vide(c)) {
		return NULL;
	}
	strcpy(c->identifiant, nom);
	strcpy(c->caractere, carac);
	c->ligne=line;
	c->suiv = NULL;
	p = l;
	while (p->suiv != NULL) {
		p = p->suiv;
	}
	p->suiv = c;
	return l;
}
