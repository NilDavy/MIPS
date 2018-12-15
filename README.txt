Le répertoire est organisé ainsi : 
.
├── doc_tab_hachage
│   ├── instructions.txt
│   └── registres.txt
│
├── documentation
│   ├──	Livrable1.pdf
│   ├── Livrable2.pdf
│   ├── Livrable3.pdf
│   ├── Livrable4.pdf
│   ├── Présentation1.pdf
│   ├── Présentation2.pdf
│   ├── Présentation3.pdf
│   ├── Présentation4.pdf
│   └── Sujet.pdf
│
├── include
│   ├── analyse_lexicale.h
│   ├── analyse_syntaxique.h
│   ├── ecrire_binaire.h
│   ├── file_bss.h
│   ├── file_data.h
│   ├── file_symb.h
│   ├── file_text.h
│   ├── file.h
│   ├── global.h
│   ├── hachage.h
│   ├── lex.h
│   ├── notify.h
│   ├── pelf
│   │     ├── elf-mips32.h
│   │     ├── elf.h
│   │     ├── pelf.h
│   └     └── section.h
│   └── table_relocation.h
│
├── Makefile
├── README.txt
│
├── recapitulatif
│   ├── Analyse_Lexicale.txt
│   ├── Recapitulatif_BSS.txt
│   ├── Recapitulatif_DATA.txt
│   ├── Recapitulatif_Erreur.txt
│   ├── Recapitulatif_Relocation.txt
│   ├── Recapitulatif_SYMB.txt
│   └── Recapitulatif_TEXT.txt
│
├── src
│   ├── analyse_lexicale.c
│   ├── analyse_syntaxique.c
│   ├── ecrire_binaire.c
│   ├── elf-mips32.c
│   ├── elf.c
│   ├── endian.c
│   ├── file_bss.c
│   ├── file_data.c
│   ├── file_symb.c
│   ├── file_text.c
│   ├── file.c
│   ├── hachage.c
│   ├── lex.c
│   ├── main.c
│   ├── read.c
│   ├── section.c
│   ├── table_relocation.c
│   └── write.c
│
├── tests
│   ├── 1livrable
│   │     ├── miam_sujet_avec_erreurs.s
│   │     ├── test_chaine_de_caractere.s
│   │     ├── test_commentaire.s
│   │     ├── test_directive.s
│   │     ├── test_instruction.s
│   │     ├── test_nombre.s
│   │     └── test_registre.s
│   ├── 2livrable
│   │     ├── test_bss_erreur.s
│   │     ├── test_bss.s
│   │     ├── test_data_erreur.s
│   │     ├── test_data.s
│   │     ├── test_global.s
│   │     ├── test_text_erreur.s
│   │     └── test_text.s
│   ├── 3livrable
│   │     ├── test_base_offset.s
│   │     ├── test_nombre_operande.s
│   │     ├── test_pseudo_instruction.s
│   │     ├── test_relocation.s
│   │     ├── test_verif_type_operande_erreur.s
│   │     └── test_verif_type_operande.s
│   ├── 4livrable
│   │     ├── copie_chaine.o
│   │     ├── copie_chaine.o.ref
│   │     ├── copie_chaine.s
│   │     ├── donnees_simples.o
│   │     ├── donnees_simples.o.ref
│   │     ├── donnees_simples.s
│   │     ├── miam_sujet.o
│   │     ├── miam_sujet.o.ref
│   │     ├── miam_sujet.s
│   │     ├── test_data.o
│   │     ├── test_data.o.ref
│   │     ├── test_data.s
│   │     ├── test_fail_prof.s
│   │     ├── test_text.o
│   │     ├── test_text.o.ref
│   │     ├── test_text.s
│   │     ├── test1.o
│   └     └── test1.s
│
├── verif
│   ├── README.txt
└   └── simpleUnitTest.sh



- doc_tab_hachage : contient les deux fichiers pour créer les tables de hachage instruction et registre
- documentation : contient les rapports des différents livrables et le sujet du projet
- include : qui contient toutes les définitions de types et prototypes du programme.
- Makefile pour compiler soit en mode debug (afficher les traces du programme) soit en mode release (produit final)
- README.txt : ce document
- recapitulatif : contient les résultats de l'analyse lexicale et syntaxique
- src : qui contient le code C du programme.      
- tests : contient les fichiers pour tester le programme 
- verif : contient le programme de test unitaire pour verifier le binaire sortant


--- pour compiler le code en mode debug (il créé l'exécutable 'as-mips' qui affiche les traces)
$ make debug 



--- pour l'executer 
$ ./as-mips tests/FICHIER.S



--- pour compiler le code en mode release (il créé l'exécutable 'as-mips' qui n'affiche pas les traces)
$ make release 


--- pour faire un test unitaire
$ verif/simpleUnitTest.sh -e ./as-mips tests/FICHIERS.S