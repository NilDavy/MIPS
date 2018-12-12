#include "file_text.h"

/** Creer file **/
file_text creerfile_text(void)
{
    return NULL;
}

/** Verif file vide **/
int file_vide_text(file_text f)
{
    return !f;
}

/** visaliser une file **/
void visualiser_file_text(file_text f)
{
    file_text g = NULL;
    if (file_vide_text(f)) {
	printf("File vide\n");
	return;
    }

    g = f->suiv;
    while (g != f) {
	printf("Nom instruction : %s\t", g->nomInst);
	printf("Ligne : %d\t", g->ligne);
	printf("Decalage : %d\n", g->decalage);
	visualiser_file(g->op);
	g = g->suiv;
    }
    printf("Nom instruction : %s\t", g->nomInst);
    printf("Ligne : %d\t", g->ligne);
    printf("Decalage : %d\n", g->decalage);
    visualiser_file(g->op);
}


/** ajout d'un maillon a la file **/
file_text ajout_text(char *nom, int nbop, int line, unsigned int dec,
		     file_text f, file_jeu_instruction g)
{
    file_text e = calloc(1, sizeof(*e));
    if (e == NULL) {
	return NULL;
    }
    strcpy(e->nomInst, nom);
    e->ligne = line;
    e->decalage = dec;
    e->nbOp = nbop;
    e->op = creer_file();
    file_jeu_instruction h = g;
    h = h->suiv;
    file_jeu_instruction i = g;
    do {
	e->op = enfiler(h->identifiant, h->caractere, h->ligne, e->op);
	h = h->suiv;
    }
    while (h != i->suiv);
    if (file_vide_text(f)) {
	e->suiv = e;
	return e;
    }
    e->suiv = f->suiv;
    f->suiv = e;
    return e;
}

/** liberation de la memoire de la file **/

void liberer_text(file_text f)
{
    if (file_vide_text(f)) {
	return;
    }
    file_text g = f->suiv;
    f->suiv = NULL;
    while (g != NULL) {
	g = supprimer_tete_text(g);
    }

}

/** liberation de la memoire d'un maillon de la file **/
file_text supprimer_tete_text(file_text g)
{
    file_text c;
    if (g->suiv == g) {
	liberer_file(g->op);
	free(g);
	return NULL;
    }
    c = g;
    g = c->suiv;
    c->suiv = NULL;
    liberer_file(c->op);
    free(c);
    return g;
}

void ecrire_file_text(file_text f, FILE * a)
{
    if (file_vide_text(f)) {
	return;
    }
    file_text g = NULL;
    g = f->suiv;
    while (g != f) {
	fprintf(a,
		"Nom instruction : %s\t Ligne : %d\t Nombre d'opérandes : %d\t Decalage : %d\n",
		g->nomInst, g->ligne, g->nbOp, g->decalage);
	ecrire_file(g->op, a);
	g = g->suiv;
    }
    fprintf(a,
	    "Nom instruction : %s\t Ligne : %d\t Nombre d'opérandes : %d\t Decalage : %d\n",
	    g->nomInst, g->ligne, g->nbOp, g->decalage);
    ecrire_file(g->op, a);
}

file_text modif_etiquette(file_text co_text)
{
    file_text a = co_text->suiv;
    do {
	file_jeu_instruction f = a->op->suiv;
	do {
	    if (strcasecmp(f->identifiant, "Renvoie vers une étiquette")
		== 0 || strcasecmp(f->identifiant, "EtiquettePFort") == 0
		|| strcasecmp(f->identifiant, "EtiquettePFaible") == 0) {
		strcpy(f->caractere, "0");
	    }
	    f = f->suiv;
	} while (f != a->op->suiv);
	a = a->suiv;
    } while (a != co_text->suiv);
    return co_text;
}
