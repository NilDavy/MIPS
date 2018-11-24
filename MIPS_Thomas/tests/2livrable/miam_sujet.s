# TEST_RETURN_CODE = PASS
#allons au ru
.set noeorder
.text
Lw $t0 , lunchtime 
    Lw $12, -200($7)
    ADDI $t1,$zero,8
boucle :
add :
BeQ  $t0 , $t1 , byebye # test 
    NOP
addi $t1 , $t1 , 1
J boucle
    NOP

byebye:
JAL boucle

.data
lunchtime:
test:
    .word 12
.asciiz "ils disent : \"au ruuu!\""
    .word menu

.bss

menu:
    .space 24
