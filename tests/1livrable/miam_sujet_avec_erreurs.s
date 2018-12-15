# TEST_RETURN_CODE = PASS
#allons au ru
.set noeorder
.text
Lw $t0 , lunchtime 
    Lw $12, -200($7)
    ADDI $t1,$zero,8
boucle :
BeQ  $t0 , $t1 , byebye # test d'une chaine de caractère bien trop longue pour pouvoir voir comment réagit le programme car c'est important. voilà donc je rajoute des mots, parfois trop de mots pour etre sur d'avoir une chaine de caractères bien trop
    NOP
addi $t1 , $t1 , 1
J boucle
    NOP
byebye:
JAL viteviteauru

"test d'une chaine de caractère bien trop longue pour pouvoir voir comment réagit le programme car c'est important. voilà donc je rajoute des mots, parfois trop de mots pour etre sur d'avoir une chaine de caractère bien trop lonnnnnnnnnnngujklme"

.data
lunchtime:
    .word 12
    .word menu
    .asciiz " test d'une chaine de caractère bien trop longue pour pouvoir voir comment réagit le programme car c'est important. voilà donc je rajoute des mots, parfois trop de mots pour etre sur d'avoir une chaine de caractère bien trop longue, et on co"
.bss

menu:
    .space 24
