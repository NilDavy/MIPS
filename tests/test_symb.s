.set noreorder
.text
Lw $t0 , lunchtime
LW $6, -200($7)
ADDI $t1,$t2,8
boucle:
BEQ $t0 , $t1 , byebye
NOP
J boucle
NOP
byebye:
JAL viteviteauru

.data
lunchtime:
.word menu
.asciiz "ils disent : \"au ru!\""

.text
ADDI $t1,$t2,menu1
ADDI $t1,$t2,menu2
ADDI $t1,$t2,menu3
JAL byebye
NOP
J boucle
NOP

.data
.word menu1, menu2, menu3
.byte 20
.byte 30, 40, 20, 12

.bss
menu:
.space 24
menu1:
.space 24
menu2:
.space 240
menu3:
.space 12
