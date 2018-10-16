Le répertoire est organisé ainsi : 
.
├── tests
│   ├── miam_sujet.s
│   ├── miam_sujet_avec_erreurs.s
│   ├── test_chaine_de_caractere.s
│   ├── test_commentaire.s
│   ├── test_directive.s
│   ├── test_nombre.s
│   └── test_registre.s
├── src
│   ├── main.c
│   ├── lex.c
│   ├── analyse_lexicale.c
│   ├── file.c
│   └── hachage.c
├── include
│   ├── notify.h
│   ├── lex.h
│   ├── global.h
│   ├── analyse_lexicale.h
│   ├── file.h
│   └── hachage.h
├── documentation
│   ├── Sujet.pdf
│   └── Livrable1.pdf
├── doc_tab_hachage
│   ├── instructions.txt
│   └── registres.txt
├── recapitulatif
│   ├── Analyse_Lexicale.txt
│   └── Recapitulatif_Erreur.txt
├── README.txt
└── Makefile

- tests : contient les fichiers pour tester le programme 
- src : qui contient le code C du programme.      
- include : qui contient toutes les définitions de types et prototypes du programme.
- documentation : contient les rapports des différents livrables et le sujet du projet
- doc_tab_hachage : contient les deux fichiers pour créer les tables de hachage instruction et registre
- recapitulatif : contient les résultats de l'analyse lexicale
- README.txt : ce document
- Makefile pour compiler soit en mode debug (afficher les traces du programme) soit en mode release (produit final)


--- pour compiler le code en mode debug (il créé l'exécutable 'as-mips' qui affiche les traces)
$ make debug 



--- pour l'executer 
$ ./as-mips tests/FICHIER.S



--- pour compiler le code en mode release (il créé l'exécutable 'as-mips' qui n'affiche pas les traces)
$ make release 