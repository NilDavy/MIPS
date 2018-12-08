.text
addi $t1,$zero,8
lui $t0,0x0
lw $t0,4($t0)
beq $t0,$t1,20
nop
addi $t1,$t1,1
j boucle
nop
