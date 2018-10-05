# TEST_RETURN_CODE = PASS
#allons au ru
.set noeorder
.text
Lw $t0 , lunchtime 
    Lw $12, -200($7) #test
    ADI $t1,$ero,8

0x12323
0X123123123123
0xG
boucle :#test
BeQ  $t0 , $t1 , byebye #test
    NOP
addi $t1 , $t1 , 1
J boucle #test
    NOP
byebye:
JAL viteviteauru # test

.data
lunchtime:
    .word 12
    .word menu
    .asciiz " ils disent : \"au ru!\"
.bss

menu:
    .space 24
