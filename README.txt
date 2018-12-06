Le répertoire est organisé ainsi : 
.
├── tests
│   ├── miam_sujet.s
│   ├── 1livrable
│   │   ├── miam_sujet_avec_erreurs.s
│   │   ├── test_chaine_de_caractere.s
│   │   ├── test_commentaire.s
│   │   ├── test_directive.s
│   │   ├── test_nombre.s
│   │   └── test_registre.s
│   ├── 2livrable
│   │   ├── test_bss.s
│   │   ├── test_bss_erreur.s
│   │   ├── test_data.s
│   │   ├── test_data_erreur.s
│   │   ├── test_global.s
│   │   ├── test_text.s
│   │   └── test_text_erreur.s
│   ├── 3livrable
│   │   ├── test_base_offset.s
│   │   ├── test_nombre_operande.s
│   │   ├── test_pseudo_instruction.s
│   │   ├── test_verif_type_operande.s
│   └   └── test_verif_type_operande_erreur.s

├── src
│   ├── main.c
│   ├── lex.c
│   ├── analyse_lexicale.c
│   ├── analyse_syntaxique.c
│   ├── file_bss.c
│   ├── file_data.c
│   ├── file_text.c
│   ├── file_symb.c
│   ├── file.c
│   ├── table_relocation.c
│   └── hachage.c
├── include
│   ├── notify.h
│   ├── lex.h
│   ├── global.h
│   ├── analyse_lexicale.h
│   ├── analyse_syntaxique.h
│   ├── file_bss.h
│   ├── file_data.h
│   ├── file_text.h
│   ├── file_symb.h
│   ├── file.h
│   ├── table_relocation.h
│   └── hachage.h
├── documentation
│   ├── Sujet.pdf
│   ├── Présentation1.pdf
│   ├── Présentation2.pdf
│   ├── Présentation3.pdf
│   ├──	Livrable1.pdf
│   ├── Livrable2.pdf
│   └── Livrable3.pdf
├── doc_tab_hachage
│   ├── instructions.txt
│   └── registres.txt
├── recapitulatif
│   ├── Analyse_Lexicale.txt
│   ├── Recapitulatif_BSS.txt
│   ├── Recapitulatif_DATA.txt
│   ├── Recapitulatif_SYMB.txt
│   ├── Recapitulatif_TEXT.txt
│   └── Recapitulatif_Erreur.txt
│   ├── Recapitulatif_Relocation.txt
├── README.txt
└── Makefile

- tests : contient les fichiers pour tester le programme 
- src : qui contient le code C du programme.      
- include : qui contient toutes les définitions de types et prototypes du programme.
- documentation : contient les rapports des différents livrables et le sujet du projet
- doc_tab_hachage : contient les deux fichiers pour créer les tables de hachage instruction et registre
- recapitulatif : contient les résultats de l'analyse lexicale et syntaxique
- README.txt : ce document
- Makefile pour compiler soit en mode debug (afficher les traces du programme) soit en mode release (produit final)


--- pour compiler le code en mode debug (il créé l'exécutable 'as-mips' qui affiche les traces)
$ make debug 



--- pour l'executer 
$ ./as-mips tests/FICHIER.S



--- pour compiler le code en mode release (il créé l'exécutable 'as-mips' qui n'affiche pas les traces)
$ make release 