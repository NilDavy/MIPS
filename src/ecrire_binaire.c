/**
 * @file section.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Sep 20 11:15:28 2017
 * @author François Portet
 * @date Nov 20, 2018
 * @brief Example file to generate ELF object file.
 *
 */

#include "ecrire_binaire.h"

/**
 * Looks for a specific name in strtab and symtab and return the index of the symbol corresponding to the name
 *
 * @param symtab the symbole table
 * @param strtab the string table
 * @param sym_name the name for which a symbol is searched for
 *
 * @return the index of the symbol in symtab or -1 if not found
 */
int
elf_get_sym_index_from_name(section symtab, section strtab, char *sym_name)
{
    int             i = 0;
    Elf32_Sym      *symboles = (Elf32_Sym *) symtab->start;

    int             offset = -1;
    if ((offset =
	 elf_get_string_offset(strtab->start, strtab->sz, sym_name)) > 0) {
	for (i = 0; i < (symtab->sz) / sizeof(Elf32_Sym); i++) {
	    if (symboles[i].st_name == offset)
		return i;
	}
    }
    return -1;

}

int
elf_get_sym_index_from_name2(section symtab, section shstrtab,
			     section strtab, char *sym_name)
{
    int             i = 0;
    Elf32_Sym      *symboles = (Elf32_Sym *) symtab->start;
    int             section_offset =
	elf_get_string_index(shstrtab->start, shstrtab->sz, sym_name);
    int             offset =
	elf_get_string_offset(strtab->start, strtab->sz, sym_name);

    if (offset > 0 || section_offset > 0) {
	for (i = 0; i < (symtab->sz) / sizeof(Elf32_Sym); i++) {
	    if (ELF32_R_TYPE(symboles[i].st_info) == STT_SECTION) {
		if (symboles[i].st_shndx == section_offset) {
		    return i;
		}
	    } else if (symboles[i].st_name == offset)
		return i;
	}
    }
    return -1;

}


#define add_string_to_table( section, symbol ) /* Explicitly add terminal '\0' */ \
write_section( section, (unsigned char *)symbol, 1+strlen(symbol), section->sz)
/**
 * @brief Generates a predefined shstrtab section containing the basic sections names
 * @return the completed shstrtab section
 */
section
make_shstrtab_section(void)
{
    section         shstrtab = new_section(".shstrtab", SECTION_CHUNK_SZ);
    add_string_to_table(shstrtab, "");	/* ELF string tables start with a
					 * '0' */
    add_string_to_table(shstrtab, ".text");
    add_string_to_table(shstrtab, ".rel.text");
    add_string_to_table(shstrtab, ".data");
    add_string_to_table(shstrtab, ".rel.data");
    add_string_to_table(shstrtab, ".bss");
    add_string_to_table(shstrtab, ".symtab");
    add_string_to_table(shstrtab, ".strtab");
    add_string_to_table(shstrtab, ".shstrtab");

    return shstrtab;
}


/**
 * @brief Generates the strtab section from an array of char *
 * @param symbols the a sorted array or char *
 * @param nb_sym the number of symbol to include in the section
 *
 * @return the completed strtab section
 */

section
make_strtab_section(file_symb co_symb)
{
    section         strtab = new_section(".strtab", SECTION_CHUNK_SZ);

    add_string_to_table(strtab, "");	/* ELF string tables start with a
					 * '0' */

    if (file_vide_symb(co_symb)) {
	return strtab;
    }

    file_symb       a = co_symb->suiv;
    do {
	if (!
	    (strcasecmp(a->nom, ".text") == 0
	     || strcasecmp(a->nom, ".data") == 0
	     || strcasecmp(a->nom, ".bss") == 0
	     || strcasecmp(a->section, "none") == 0)) {
	    add_string_to_table(strtab, a->nom);
	}

	a = a->suiv;
    }
    while (a != co_symb->suiv);
    do {
	if (!strcasecmp(a->section, "none")) {
	    add_string_to_table(strtab, a->nom);
	}

	a = a->suiv;
    }
    while (a != co_symb->suiv);
    return strtab;
}



/**
 * @brief Generates the data section from an array of int already in big endian
 *
 * @param  data_prog array of binary value
 * @param  nb_data size of the array
 *
 * @return the completed section
 *
 * the function writes the whole data in one shot to the section.
 */
section
make_data_section(int data_prog[], int nb_data)
{
    section         data = new_section(".data", SECTION_CHUNK_SZ);

    if (nb_data > 0) {
	write_section(data, (unsigned char *) (data_prog),
		      sizeof(*data_prog) * nb_data, 0);
    }
    return data;
}

/**
 * @brief Generates the text section from an array of int already in big endian
 *
 * @param  instructions array of binary value
 * @param  nb_inst size of the array
 *
 * @return the completed section
 *
 * the function writes the whole data in one shot to the section.
 */
section
make_text_section(int instructions[], int nb_inst)
{
    section         text = new_section(".text", SECTION_CHUNK_SZ);

    if (nb_inst > 0) {
	write_section(text, (unsigned char *) (instructions),
		      sizeof(*instructions) * nb_inst, 0);
    }
    return text;
}

/**
 * @brief Generates the bss section from the size of the section
 *
 * @param  bss_prog the size of the bss section in number of bytes
 *
 * @return the completed section
 *
 * Since bss is only composed of zero, only the size of the section is necessary in the elf file
 */
section
make_bss_section(int bss_prog)
{
    section         bss = new_section(".bss", SECTION_CHUNK_SZ);
    bss->sz = bss_prog;
    return bss;
}


/**
 * @brief Generates the symtab section from an array of symbols
 *
 * @param  shstrtab the section of the string table of sections
 * @param  strtab the string table section
 * @param  symbols array of Elf32_Sym. The symbols are supposed to be already SORTED.
 * @param  nb_syms size of the array
 *
 *
 * @return the completed section
 *
 * shstrtab and strtab are necessary to bind the symbol names to the symbol entries.
 * Every symtab starts with an empty entry followed by the 3 standard sections :
 *     - text of type SECTION
 *     - data of type SECTION
 *     - bss of type SECTION
 *
 * These entries are present even if they are not in the original text code
 *
 */
section
make_symtab_section(section shstrtab, section strtab,
		    file_symb co_symb, int nb_ligne)
{

    section         symtab = new_section(".symtab", SECTION_CHUNK_SZ);
    unsigned int    offset = 0;

    Elf32_Sym       entry = { 0 };
    entry.st_name = 0;
    entry.st_value = 0;
    entry.st_size = 0;
    entry.st_info = 0;
    entry.st_other = 0;
    entry.st_shndx = SHN_UNDEF;

    write_section(symtab, (unsigned char *) &entry, sizeof(entry), offset);


    entry.st_name = 0;
    entry.st_value = 0;
    entry.st_size = 0;
    entry.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    entry.st_other = 0;
    entry.st_shndx =
	elf_get_string_index(shstrtab->start, shstrtab->sz, ".text");

    write_section(symtab, (unsigned char *) &entry, sizeof(entry),
		  symtab->sz);

    entry.st_name = 0;
    entry.st_value = 0;
    entry.st_size = 0;
    entry.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    entry.st_other = 0;
    entry.st_shndx =
	elf_get_string_index(shstrtab->start, shstrtab->sz, ".data");

    write_section(symtab, (unsigned char *) &entry, sizeof(entry),
		  symtab->sz);

    entry.st_name = 0;
    entry.st_value = 0;
    entry.st_size = 0;
    entry.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    entry.st_other = 0;
    entry.st_shndx =
	elf_get_string_index(shstrtab->start, shstrtab->sz, ".bss");

    write_section(symtab, (unsigned char *) &entry, sizeof(entry),
		  symtab->sz);
    if (file_vide_symb(co_symb))
	return symtab;
    file_symb       a = co_symb->suiv;
    int             i;
    if (file_vide_symb(a))
	return symtab;
    for (i = 1; i <= nb_ligne; i++) {
	do {
	    if (strcasecmp(a->section, "none") && a->ligne == i) {
		/*
		 * printf("Symbole %s à la ligne %d", a->nom, a->ligne); 
		 */
		Elf32_Sym       symb;
		symb.st_name =
		    elf_get_string_offset(strtab->start, strtab->sz,
					  a->nom);
		symb.st_size = 0;
		symb.st_value = a->decalage;
		symb.st_info = ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE);
		symb.st_other = 0;
		if (!strcmp(a->section, "TEXT"))
		    symb.st_shndx =
			elf_get_string_index(shstrtab->start, shstrtab->sz,
					     ".text");
		if (!strcmp(a->section, "DATA"))
		    symb.st_shndx =
			elf_get_string_index(shstrtab->start, shstrtab->sz,
					     ".data");
		if (!strcmp(a->section, "BSS"))
		    symb.st_shndx =
			elf_get_string_index(shstrtab->start, shstrtab->sz,
					     ".bss");
		/*
		 * printf("Ajout du symbole : %s qui est utilisé à la
		 * ligne : %d\n", a->nom, a->ligne); 
		 */
		write_section(symtab, (unsigned char *) &symb,
			      sizeof(symb), symtab->sz);
	    }
	    a = a->suiv;
	}
	while (a != co_symb->suiv);
    }
    for (i = 1; i <= nb_ligne; i++) {
	do {
	    /*
	     * Cas des symboles inconnus 
	     */
	    if (!strcasecmp(a->section, "none") && a->ligne == i) {
		Elf32_Sym       symb;
		symb.st_name =
		    elf_get_string_offset(strtab->start, strtab->sz,
					  a->nom);
		symb.st_size = 0;
		symb.st_value = 0;
		symb.st_info = ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE);
		symb.st_other = 0;
		symb.st_shndx = SHN_UNDEF;
		write_section(symtab, (unsigned char *) &symb,
			      sizeof(symb), symtab->sz);
	    }
	    a = a->suiv;
	}
	while (a != co_symb->suiv);
    }


    return symtab;
}


/**
 * @brief Generates a relocation section from an array of Elf32_Rel
 *
 * @param relname the name of the relocation section (e.g., rel.text, rel.data)
 * @param relocations relocation entries already ready to be writen and SORTED
 * @param nb_reloc size of the array
 *
 * @return the completed section
 *
 */
section
make_rel32_section(char *relname, table_reloc reloc,
		   section symtab, section shstrtab, section strtab)
{

    section         reltab = new_section(relname, SECTION_CHUNK_SZ);
    if (table_vide(reloc))
	return reltab;
    table_reloc     r = reloc->suiv;
    if (table_vide(r)) {
	return reltab;
    }
    do {
	Elf32_Rel       rel;
	if (r->symb == NULL) {

	} else {
	    rel.r_offset = r->offset;


	    if (!strcasecmp(r->symb->section, "TEXT")) {
		rel.r_info =
		    ELF32_R_INFO(elf_get_sym_index_from_name2
				 (symtab, shstrtab, strtab, ".text"),
				 r->type);
	    } else if (!strcasecmp(r->symb->section, "DATA")) {
		rel.r_info =
		    ELF32_R_INFO(elf_get_sym_index_from_name2
				 (symtab, shstrtab, strtab, ".data"),
				 r->type);
	    } else if (!strcasecmp(r->symb->section, "BSS")) {
		rel.r_info =
		    ELF32_R_INFO(elf_get_sym_index_from_name2
				 (symtab, shstrtab, strtab, ".bss"),
				 r->type);
	    } else {
		/*
		 * rel.r_offset = 0; 
		 */
		rel.r_info =
		    ELF32_R_INFO(elf_get_sym_index_from_name2
				 (symtab, shstrtab, strtab, r->symb->nom),
				 r->type);
	    }
	}
	write_section(reltab, (unsigned char *) &rel, sizeof(rel),
		      reltab->sz);
	r = r->suiv;
    }
    while (r != reloc->suiv);
    return reltab;
}

void
swap(char *s2)
{
    char            attente[100] = "";
    strncpy(attente, s2 + 2, 1);
    strncpy(s2 + 2, s2 + 8, 1);
    strncpy(s2 + 8, attente, 1);

    strncpy(attente, s2 + 3, 1);
    strncpy(s2 + 3, s2 + 9, 1);
    strncpy(s2 + 9, attente, 1);

    strncpy(attente, s2 + 4, 1);
    strncpy(s2 + 4, s2 + 6, 1);
    strncpy(s2 + 6, attente, 1);

    strncpy(attente, s2 + 5, 1);
    strncpy(s2 + 5, s2 + 7, 1);
    strncpy(s2 + 7, attente, 1);

    return;
}


void
creer_data_value(int *data_value, int *data_type, int nbdata,
		 file_data co_data, file_symb co_symb)
{
    file_data       e = co_data->suiv;
    file_data       h = co_data;
    h->suiv = NULL;
    int             a = 0;
    int             b;
    int             t = 0;
    while (e != NULL) {
	/*printf("avant word %d\n",a);*/
	if (!strcasecmp(e->nomInst, ".word")) {
	    if (a % 4 != 0) {
		a += 4 - a % 4;
		/*printf("if word %d\n",a);*/
	    }
	}
	switch (e->type) {
	case 0:
	    data_value[a] = (int) (e->op).c;
	    data_type[a] = 0;
	    a = a + 1;
	    data_value[a] = 0;
	    data_type[a] = 3;
	    a = a + 1;
	    break;
	case 1:
	    data_value[a] = (int) (e->op).uc;
	    data_type[a] = 1;
	    a = a + 1;
	    data_value[a] = 0;
	    data_type[a] = 3;
	    a = a + 1;
	    break;
	case 2:
		if(!strcasecmp(e->nomInst, ".space")){
			for(b=0;b<(e->op).i;b++){
				data_value[a] = 0;
				data_type[a] = 3;
				a = a + 1;
			}
		}
		else{
			data_value[a] = (e->op).i;
			data_type[a] = 2;
			a = a + 1;
			data_value[a] = 0;
			data_type[a] = 3;
			a = a + 1;
		}
	    break;
	case 3:
	    data_value[a] = (e->op).ui;
	    data_type[a] = 3;
	    a = a + 1;
	    data_value[a] = 0;
	    data_type[a] = 3;
	    a = a + 1;
	    break;
	case 4:
	    if (!strcasecmp(e->nomInst, ".asciiz")) {
		for (b = 0; b < strlen((e->op).s); b++) {
		    /*
		     * printf("%c\n",(e->op).s[b]); 
		     */
		    t = 0;
		    if (e->op.s[b] == '\\' && e->op.s[b + 1] == 'n') {
			data_value[a + b] = 10;
			data_type[a + b] = 4;
			b = b + 2;
			t = 1;
		    } else {
			if (e->op.s[b] == '\\' && e->op.s[b + 1] == 't') {
			    data_value[a + b] = 9;
			    data_type[a + b] = 4;
			    b = b + 2;
			    t = 1;
			} else {
			    data_value[a + b] = (int) ((unsigned char)(e->op).s[b]);
				/*printf("%c %d\n",(e->op).s[b],data_value[a + b]);*/
			    data_type[a + b] = 4;
			}
		    }
		}
		a = a + strlen((e->op).s) - t;
		data_value[a] = 0;
		data_type[a] = 3;
		a = a + 1;
	    } else {
		/*
		 * forcement .word avec renvoie vers une éiquette 
		 */
		
		struct cellulesymb *ptr_symb =recuperer_cellule_symb(e->op.s, co_symb);
		if (!strcasecmp(ptr_symb->section, "none")|| strcasecmp(ptr_symb->section, "DATA")) {
		    for (b = 0; b < 4; b++) {
				data_type[a + b] = 5;
				data_value[a + b] = 0;
		    }
		    a = a + 1;
		} else {
		    data_type[a] = 5;
		    data_value[a] = ptr_symb->decalage;
		    a = a + 4;
		    data_value[a] = 0;
		    data_type[a] = 3;
		    a = a + 1;
		}
	    }
	    break;
		case 5:
			data_value[a] = (e->op).wi;
			data_type[a] = 5;
			a = a + 1;
		break;
	}

	e = e->suiv;
    }
}

void
creer_hexa_data(file_jeu_instruction * f_data_bin, int *data_type,
		int *data_value, int nbdata)
{
    int             i;
    char            hexadecimal[20];
    for (i = 0; i < nbdata; i++) {
	sprintf(hexadecimal, "%x", data_value[i]);
	/*
	 * printf("%d %s\n",data_value[i],hexadecimal); 
	 */
	*f_data_bin =
	    enfiler("Binaire", hexadecimal, data_type[i], *f_data_bin);
    }
    for (i = 0; i < (4 - (nbdata % 4)); i++) {
	*f_data_bin = enfiler("Binaire", "0", 0, *f_data_bin);
    }
}

int
nombre_mot_data(int nbdata)
{
    int             a;
    if ((nbdata) % 4 == 0) {
	a = nbdata / 4;
    } else {
	a = nbdata / 4 + 1;
    }
    return a;
}

void
ordre_data(file_jeu_instruction f_data_bin, int *data_prog, int nbdata)
{
    file_jeu_instruction f = f_data_bin->suiv;
    int             i,j;
    char            s2[200];
    char            s1[100];
    for (i = 0; i < nbdata; i++) {
	strcpy(s1, "");
	strcpy(s2, "0x");

	if(f->ligne==5){
	/*C'est un point word*/
		int a=strlen(f->caractere);
		for(j=0;j<(8-a);j++){
			/*on complete avec des zeros*/
			strcat(s1,"0");
		}
		strcat(s1,f->caractere);
		f=f->suiv;
	}
	else{
		if (strlen(f->caractere) == 1) {
			strcat(s1, "0");
			}
			strcat(s1, f->caractere);
			if (strlen(f->suiv->caractere) == 1) {
			strcat(s1, "0");
			}
			strcat(s1, f->suiv->caractere);
			if (strlen(f->suiv->suiv->caractere) == 1) {
			strcat(s1, "0");
			}
			strcat(s1, f->suiv->suiv->caractere);
			if (strlen(f->suiv->suiv->suiv->caractere) == 1) {
			strcat(s1, "0");
			}
			strcat(s1, f->suiv->suiv->suiv->caractere);
			f = f->suiv->suiv->suiv->suiv;
		}

	strcat(s2, s1);
	swap(s2);
	data_prog[i] = (int)strtol(s2, NULL, 0);
	/*printf("%d %s %s %d\n",i,s1,s2,data_prog[i]);*/
    }
}

void
creer_section_data(section * data, int nbdata, file_data co_data,
		   file_symb co_symb)
{
    file_jeu_instruction f_data_bin = creer_file();
    int             nombre_mot = nombre_mot_data(nbdata);
    int            *data_value = calloc(nombre_mot * 4, sizeof(int));
    int            *data_type = calloc(nombre_mot * 4, sizeof(int));
    int            *data_prog = calloc(nombre_mot, sizeof(int));

    /*
     * Permet de creer la valeur de chaque data en int 
     */
    creer_data_value(data_value, data_type, nbdata, co_data, co_symb);
    /*printf("nb %d\n",nbdata);
	int i;
	for(i=0;i<nbdata;i++){ printf("%d %d %d\n",i,data_value[i],data_type[i]); }*/

    /*
     * Transformation en hexa 
     */

    creer_hexa_data(&f_data_bin, data_type, data_value,nombre_mot*4);
	/*visualiser_file(f_data_bin);*/

    /*
     * Mise en place 
     */

    ordre_data(f_data_bin, data_prog, nombre_mot);
	/*int i;*/
	/*printf("\n \n %d\n",nombre_mot);
    for(i=0;i<nombre_mot;i++){ printf("%d\n",data_prog[i]); }*/
    *data = make_data_section(data_prog, nombre_mot);

    if (!data) {
	fprintf(stderr,"Unable to write .data section (missing information).\n");
	exit(EXIT_FAILURE);}
	free(data_value);
	free(data_type);
	free(data_prog );
	liberer_file(f_data_bin);
}

void
creer_section_bss(section * bss, int cptbss)
{
    *bss = make_bss_section(cptbss);
    if (!bss) {
	fprintf(stderr,
		"Unable to write .bss section (missing information).\n");
	exit(EXIT_FAILURE);
    }
}

void
creer_section_text(section * text, int nbtext, file_text co_text,
		   Liste_hach * tab_instruction,
		   Liste_hach * tab_registre, file_symb co_symb)
{
    int             nbre = nbtext / 4;
    int             compteur = 0;
    int             n;
    int            *text_prog = calloc(nbre, sizeof(int));
    char           *type = NULL;
    int             ligne = 0;
    /*
     * remplir le tableau text_prog on regarde la file co_text 
     */

    file_text       f = co_text->suiv;
    /*
     * choix du type et envoie vers la fonction correspondante 
     */
    do {
	n = hachage(f->nomInst, dim_tab_instruction);
	type = rec_hachage_type(f->nomInst, tab_instruction[n]);
	/*
	 * printf("%s\n",f->nomInst); 
	 */
	if (strcasecmp(type, "R") == 0) {
	    construction_R(&compteur, text_prog, nbre, f, n,
			   tab_instruction, tab_registre, &ligne);
	} else {
	    if (strcasecmp(type, "I") == 0) {
		construction_I(&compteur, text_prog, nbre, f, n,
			       tab_instruction, tab_registre, &ligne,
			       co_symb);

	    } else {
		/*
		 * type J 
		 */
		construction_J(&compteur, text_prog, nbre, f, n,
			       tab_instruction, tab_registre, &ligne,
			       co_symb);
	    }
	}
	f = f->suiv;
    }
    while (f != co_text->suiv);


    *text = make_text_section(text_prog, nbre);
    if (!text) {
	fprintf(stderr,
		"Unable to write .text section (missing information).\n");
	exit(EXIT_FAILURE);
    }
	free(text_prog);
}

void
construction_R(int *compteur, int *text_prog, int nbre, file_text f,
	       int n, Liste_hach * tab_instruction,
	       Liste_hach * tab_registre, int *ligne)
{
    char            mot[100];
    char            bin[100];
    char            hexadecimal[20];
    strcpy(mot, "");
    char            oppcode[25];
    char            fonction[25];
    char            a1[12];
    char            opp1[25];
    char            a2[12];
    char            opp2[25];
    char            a3[12];
    char            opp3[25];
    char            chaine_fin[25];
    strcpy(chaine_fin, "0x");
    int             i;
    long int        a;
    file_jeu_instruction g = f->op;


    information(oppcode, fonction, a1, a2, a3, tab_instruction[n],
		f->nomInst);

    strcat(mot, oppcode);

    /*
     * on cacat d'abord rs, rt, rd, sa suivant les cas et l'ordre 
     */

    if (strcasecmp(a1, "rs") == 0) {
	strcpy(opp1,
	       rec_hachage_oppcode(g->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "rs") == 0) {
	strcpy(opp2,
	       rec_hachage_oppcode(g->suiv->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp2);

    } else if (strcasecmp(a3, "rs") == 0) {
	strcpy(opp3,
	       rec_hachage_oppcode(g->caractere,
				   tab_registre[hachage
						(g->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else if (strcasecmp(f->nomInst, "ROTR") == 0) {
	strcat(mot, "00001");
    } else {
	strcat(mot, "00000");
    }
    if (strcasecmp(a1, "rt") == 0) {
	strcpy(opp1,
	       rec_hachage_oppcode(g->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "rt") == 0) {
	strcpy(opp2,
	       rec_hachage_oppcode(g->suiv->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp2);
    } else if (strcasecmp(a3, "rt") == 0) {
	strcpy(opp3,
	       rec_hachage_oppcode(g->caractere,
				   tab_registre[hachage
						(g->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else {
	strcat(mot, "00000");
    }

    if (strcasecmp(a1, "rd") == 0) {
	strcpy(opp1,
	       rec_hachage_oppcode(g->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "rd") == 0) {
	strcpy(opp2,
	       rec_hachage_oppcode(g->suiv->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp2);
    } else if (strcasecmp(a3, "rd") == 0) {
	strcpy(opp3,
	       rec_hachage_oppcode(g->caractere,
				   tab_registre[hachage
						(g->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else {
	strcat(mot, "00000");
    }

    if (strcasecmp(a1, "sa") == 0) {
	convertir_dec_bin(atoi(g->suiv->caractere), bin, 5);
	strcat(mot, bin);
    } else if (strcasecmp(a2, "sa") == 0) {
	convertir_dec_bin(atoi(g->suiv->suiv->caractere), bin, 5);
	strcat(mot, bin);
    } else if (strcasecmp(a3, "sa") == 0) {
	convertir_dec_bin(atoi(g->caractere), bin, 5);
	strcat(mot, bin);
    } else {
	strcat(mot, "00000");
    }

    strcat(mot, fonction);
    a = convertir_bin_dec(mot, strlen(mot));
    sprintf(hexadecimal, "%lx", a);
    for (i = 0; i < (8 - strlen(hexadecimal)); i++) {
	strcat(chaine_fin, "0");
    }
    strcat(chaine_fin, hexadecimal);
    swap(chaine_fin);
    a = (int) strtol(chaine_fin, NULL, 0);
    text_prog[*compteur] = a;
    *compteur = *compteur + 1;
    *ligne = f->ligne;
}

void
construction_I(int *compteur, int *text_prog, int nbre, file_text f,
	       int n, Liste_hach * tab_instruction,
	       Liste_hach * tab_registre, int *ligne, file_symb co_symb)
{
    char            mot[100];
    char            bin[100];
    strcpy(bin, "");
    char            hexadecimal[20];
    strcpy(mot, "");
    char            offset[50];
    char            base1[50];
    char            base[50];
    strcpy(base, "");
    strcpy(base1, "");
    strcpy(offset, "");
    char            oppcode[25];
    char            fonction[25];
    char            a1[12];
    char            opp1[25];
    char            a2[12];
    char            opp2[25];
    char            a3[12];
    char            opp3[25];
    char            chaine_fin[25];
    strcpy(chaine_fin, "0x");
    long int        a;
    int             i,
                    z;

    file_jeu_instruction g = f->op;
    information(oppcode, fonction, a1, a2, a3, tab_instruction[n],
		f->nomInst);

    strcat(mot, oppcode);

    /*
     * Si pseudo-instruction alors rt est devant
     */
    if (*ligne == f->ligne) {
	if (strcasecmp(a1, "rt") == 0) {
	    strcpy(opp1,
		   rec_hachage_oppcode(g->suiv->caractere,
				       tab_registre[hachage
						    (g->suiv->caractere,
						     dim_tab_registre)]));
	    strcat(mot, opp1);
	} else if (strcasecmp(a2, "rt") == 0) {
	    strcpy(opp2,
		   rec_hachage_oppcode(g->suiv->suiv->caractere,
				       tab_registre[hachage
						    (g->suiv->suiv->
						     caractere,
						     dim_tab_registre)]));
	    strcat(mot, opp2);
	} else if (strcasecmp(a3, "rt") == 0) {
	    strcpy(opp3,
		   rec_hachage_oppcode(g->caractere,
				       tab_registre[hachage
						    (g->caractere,
						     dim_tab_registre)]));
	    strcat(mot, opp3);
	} else {
	    strcat(mot, "00000");
	}
    }
    /*
     * sinon base offset ou rs est devant 
     */
    else if (strcasecmp(a1, "rs") == 0) {
	strcpy(opp1,
	       rec_hachage_oppcode(g->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "rs") == 0) {
	strcpy(opp2,
	       rec_hachage_oppcode(g->suiv->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp2);
    } else if (strcasecmp(a3, "rs") == 0) {
	strcpy(opp3,
	       rec_hachage_oppcode(g->caractere,
				   tab_registre[hachage
						(g->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else if (strcasecmp(a1, "bo") == 0) {
	i = 0;
	while (g->suiv->caractere[i] != '(') {
	    i = i + 1;
	}
	strncat(offset, g->suiv->caractere, i);
	i = i + 1;
	z = i;
	while (g->suiv->caractere[i] != ')') {
	    strcat(base1, &g->suiv->caractere[i]);
	    i = i + 1;
	}
	strncat(base, base1, i - z);
	strcpy(opp1,
	       rec_hachage_oppcode(base,
				   tab_registre[hachage
						(base,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "bo") == 0) {
	i = 0;
	while (g->suiv->suiv->caractere[i] != '(') {
	    i = i + 1;
	}
	strncat(offset, g->suiv->suiv->caractere, i);
	i = i + 1;
	z = i;
	while (g->suiv->suiv->caractere[i] != ')') {
	    strcat(base1, &g->suiv->suiv->caractere[i]);
	    i = i + 1;
	}
	strncat(base, base1, i - z);
	strcpy(opp2,
	       rec_hachage_oppcode(base,
				   tab_registre[hachage
						(base,
						 dim_tab_registre)]));
	strcat(mot, opp2);
    } else if (strcasecmp(a3, "bo") == 0) {
	i = 0;
	while (g->caractere[i] != '(') {
	    i = i + 1;
	}
	strncat(offset, g->caractere, i);
	i = i + 1;
	z = i;
	while (g->caractere[i] != ')') {
	    strcat(base1, &g->caractere[i]);
	    i = i + 1;
	}
	strncat(base, base1, i - z);
	strcpy(opp3,
	       rec_hachage_oppcode(base,
				   tab_registre[hachage
						(base,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else {
	strcat(mot, "00000");
    }

    /*
     * puis rt s'il existe 
     */
    if (strcasecmp(a1, "rt") == 0) {
	strcpy(opp1,
	       rec_hachage_oppcode(g->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp1);
    } else if (strcasecmp(a2, "rt") == 0) {
	strcpy(opp2,
	       rec_hachage_oppcode(g->suiv->suiv->caractere,
				   tab_registre[hachage
						(g->suiv->suiv->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp2);
    } else if (strcasecmp(a3, "rt") == 0) {
	strcpy(opp3,
	       rec_hachage_oppcode(g->caractere,
				   tab_registre[hachage
						(g->caractere,
						 dim_tab_registre)]));
	strcat(mot, opp3);
    } else {
	strcat(mot, "00000");
    }

    /*
     * ou un immediat 
     */
    if (strcasecmp(a1, "i") == 0) {
	if (!strcasecmp
	    (g->suiv->identifiant, "renvoie vers une étiquette")) {
	    struct cellulesymb *ptr_symb =
		recuperer_cellule_symb(g->suiv->caractere, co_symb);
	    if (!strcasecmp(ptr_symb->section, "none")
		|| strcasecmp(ptr_symb->section, "TEXT")) {
		strcat(mot, "0000000000000000");
	    } else {
		printf("Symbole trouvé :%d\n", ptr_symb->decalage);
		convertir_dec_bin(ptr_symb->decalage, bin, 16);
		strcat(mot, bin);
	    }
	} else {
	    convertir_dec_bin(atoi(g->suiv->caractere), bin, 16);
	    strcat(mot, bin);
	}
    } else {
	if (strcasecmp(a2, "i") == 0) {
	    if (!strcasecmp
		(g->suiv->suiv->identifiant,
		 "renvoie vers une étiquette")) {
		/*
		 * Verif renvoie vers une etiquette pour remplacer 
		 */
		struct cellulesymb *ptr_symb =
		    recuperer_cellule_symb(g->suiv->suiv->caractere,
					   co_symb);
		if (!strcasecmp(ptr_symb->section, "none")
		    || strcasecmp(ptr_symb->section, "TEXT")) {
		    strcat(mot, "0000000000000000");
		} else {
		    printf("Symbole trouvé :%d\n", ptr_symb->decalage);
		    convertir_dec_bin(ptr_symb->decalage, bin, 16);
		    strcat(mot, bin);
		}
	    } else {
		convertir_dec_bin(atoi(g->suiv->suiv->caractere), bin, 16);
		strcat(mot, bin);
	    }
	} else if (strcasecmp(a3, "i") == 0) {
	    if (!strcasecmp(g->identifiant, "renvoie vers une étiquette")) {
		struct cellulesymb *ptr_symb =
		    recuperer_cellule_symb(g->caractere, co_symb);
		if (!strcasecmp(ptr_symb->section, "none")
		    || strcasecmp(ptr_symb->section, "TEXT")) {
		    strcat(mot, "0000000000000000");
		} else {
		    printf("Symbole trouvé :%d\n", ptr_symb->decalage);
		    convertir_dec_bin(ptr_symb->decalage, bin, 16);
		    strcat(mot, bin);
		}
	    } else {
		convertir_dec_bin(atoi(g->caractere), bin, 16);
		strcat(mot, bin);
	    }
	} else if (strcasecmp(a1, "bo") == 0) {
	    /*
	     * Verif renvoie vers une etiquette pour remplacer 
	     */
	    if (strcasecmp(g->suiv->identifiant, "EtiquettePFaible") == 0) {
		struct cellulesymb *ptr_symb =
		    recuperer_cellule_symb(g->suiv->caractere,
					   co_symb);
		convertir_dec_bin(ptr_symb->decalage, bin, 16);
		strcat(mot, bin);
	    } else {
		convertir_dec_bin(atoi(offset), bin, 16);
		strcat(mot, bin);
	    }
	} else if (strcasecmp(a2, "bo") == 0) {
	    if (strcasecmp(g->suiv->suiv->identifiant, "EtiquettePFaible")
		== 0) {
		struct cellulesymb *ptr_symb =
		    recuperer_cellule_symb(g->suiv->suiv->caractere,
					   co_symb);
		convertir_dec_bin(ptr_symb->decalage, bin, 16);
		strcat(mot, bin);
	    } else {
		convertir_dec_bin(atoi(offset), bin, 16);
		strcat(mot, bin);
	    }
	} else if (strcasecmp(a3, "bo") == 0) {
	    if (strcasecmp(g->suiv->identifiant, "EtiquettePFaible") == 0) {
		struct cellulesymb *ptr_symb =
		    recuperer_cellule_symb(g->caractere,
					   co_symb);
		convertir_dec_bin(ptr_symb->decalage, bin, 16);
		strcat(mot, bin);
	    } else {
		convertir_dec_bin(atoi(offset), bin, 16);
		strcat(mot, bin);
	    }
	} else if (strcasecmp(a1, "of") == 0) {
	    /*
	     * ou bien on a un offset 
	     */
	    convertir_dec_bin(atoi(g->suiv->caractere), bin, 16);
	    strcat(mot, bin);
	} else if (strcasecmp(a2, "of") == 0) {
	    convertir_dec_bin(atoi(g->suiv->suiv->caractere), bin, 16);
	    strcat(mot, bin);
	} else if (strcasecmp(a3, "of") == 0) {
	    convertir_dec_bin(atoi(g->caractere), bin, 16);
	    strcat(mot, bin);

	} else {
	    strcat(mot, "0000000000000000");
	}
    }
    a = convertir_bin_dec(mot, strlen(mot));
    sprintf(hexadecimal, "%lx", a);
    for (i = 0; i < (8 - strlen(hexadecimal)); i++) {
	strcat(chaine_fin, "0");
    }
    strcat(chaine_fin, hexadecimal);

    swap(chaine_fin);
    a = (int) strtol(chaine_fin, NULL, 0);
    text_prog[*compteur] = a;
    *compteur = *compteur + 1;
    *ligne = f->ligne;
}

void
construction_J(int *compteur, int *text_prog, int nbre, file_text f,
	       int n, Liste_hach * tab_instruction,
	       Liste_hach * tab_registre, int *ligne, file_symb co_symb)
{
    char            mot[100];
    char            bin[100];
    strcpy(bin, "");
    char            hexadecimal[20];
    strcpy(mot, "");
    char            oppcode[25];
    char            fonction[25];
    char            a1[12];
    char            a2[12];
    char            a3[12];
    char            chaine_fin[25];
    strcpy(chaine_fin, "0x");
    int             i;
    long int        a;
    file_jeu_instruction g = f->op;
    information(oppcode, fonction, a1, a2, a3, tab_instruction[n],
		f->nomInst);

    strcat(mot, oppcode);

    if (strcasecmp(a1, "t") == 0) {
	if (!strcasecmp(g->identifiant, "renvoie vers une étiquette")) {
	    /*
	     * printf("Rcherche du symbole : %s\n", g->caractere); 
	     */
	    struct cellulesymb *ptr_symb =
		recuperer_cellule_symb(g->caractere, co_symb);
	    if (!strcasecmp(ptr_symb->section, "none")
		|| strcasecmp(ptr_symb->section, "TEXT")) {
		/*
		 * printf("Symbole non trouvé : %s\n", g->caractere); 
		 */
		strcat(mot, "00000000000000000000000000");
	    } else {
		/*
		 * printf("Symbole trouvé : %s %d\n",
		 * g->caractere,ptr_symb->decalage); 
		 */
		convertir_dec_bin(ptr_symb->decalage / 4, bin, 26);
		strcat(mot, bin);
	    }
	} else {
	    convertir_dec_bin(atoi(g->caractere), bin, 26);
	    strcat(mot, bin);
	}

    }

    a = convertir_bin_dec(mot, strlen(mot));
    sprintf(hexadecimal, "%lx", a);
    for (i = 0; i < (8 - strlen(hexadecimal)); i++) {
	strcat(chaine_fin, "0");
    }
    strcat(chaine_fin, hexadecimal);

    swap(chaine_fin);
    a = (int) strtol(chaine_fin, NULL, 0);
    text_prog[*compteur] = a;
    *compteur = *compteur + 1;
    *ligne = f->ligne;
}

long int
convertir_bin_dec(char *mot, int n)
{
    long int        nombre = 0,
	a = 1;
    int             i;
    for (i = (n - 1); i >= 0; i--) {
	/*
	 * printf("%d %c %ld\n",i,mot[i],a); 
	 */
	if (mot[i] == '1') {
	    nombre = nombre + a;
	}
	a = a * 2;
    }
    return nombre;
}

void
convertir_dec_bin(long int n, char *mot, int taille)
{
    char            nombre[100];
    char            nombre_neg[100];
    strcpy(nombre, "");
    strcpy(nombre_neg, "");
    long int        b = n;
    int             i,
                    retenue = 0;
    long int        a = 1;
    for (i = 0; i < taille - 1; i++) {
	a = a * 2;
    }
    while (a != 1) {
	if (b / a == 0) {
	    strcat(nombre, "0");
	} else {
	    strcat(nombre, "1");
	    b = b % a;
	}
	a = a / 2;
    }
    if (b / a == 0) {
	strcat(nombre, "0");
    } else {
	strcat(nombre, "1");
    }
    if (n < 0) {
	for (i = 0; i < taille; i++) {
	    if (nombre[i] == '0') {
		strcat(nombre_neg, "1");
	    } else {
		strcat(nombre_neg, "0");
	    }
	}
	if (nombre_neg[taille - 1] == '0') {
	    nombre_neg[taille - 1] = '1';
	} else {
	    nombre_neg[taille - 1] = '0';
	    retenue = 1;
	    i = taille - 2;
	    while (retenue == 1) {
		if (nombre_neg[i] == '1') {
		    nombre_neg[i] = '0';
		} else {
		    nombre_neg[i] = '1';
		    retenue = 0;
		}
		i = i - 1;
	    }
	}
	strcpy(mot, nombre_neg);
    } else {
	strcpy(mot, nombre);
    }
    /*
     * printf("nombre %s nombre neg %s\n",nombre, nombre_neg); 
     */

    /*
     * int i = 0; printf("mot %s %d\n",mot,atoi(mot)); for(i = taille; i
     * >= 0; i--){ if((atoi(mot) & (1 << i)) != 0){ printf("1"); }else{
     * printf("0"); } } printf("\n"); 
     */
    return;
}
