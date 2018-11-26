# TEST_RETURN_CODE = PASS
#allons au ru
.set noeorder
.text
add $8,$9,$10
Lw $t0 , lunchtime 
Lw $12, -200($7)
ADDI $t1,$t2,X
Lw $t0 , test
boucle :
add :
BeQ  $t0 , $t1 , byebye # test 
J boucle
    NOP
BGTZ $t1, menu

byebye:
JAL boucl

.data
lunchtime:
test:
    .word 12
X:
.asciiz "ils disent : \"au ruuu!\""
    .word menu
    .word Y

.bss

menu:
    .space 24

Y:
    .space 32
