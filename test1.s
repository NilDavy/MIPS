# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -200($7)
    ADDI $t1,$zero,8
boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle 
	0x12323
	0X123123123123
	0xG
    NOP
byebye:
    JAL vitevite auru
. word
.data
_lunchtime: 
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru\\\""
    .wo rd 12
aa: a:
#aa a:
.bss 
%menu:
    .space 24
