.text
add $3,$4,$5
addi $3,$4,12
sub $4,$5,$3
mult $3,$4
div $4,$5
AND $4,$7,$23
or $3,$7,$3
xor $7,$8,$9
sll $8,$9,4
srl $8,$9,4
slt $7,$8,$9
lw $4, 200($3)
sw $4, 200($3)
lui $4, 200
mfhi $9
mflo $8
beq $4,$6, 200
bne $4,$6, 200
bgtz $4,200
blez $4,200
j 16
jal 20
jr $6
NOP
move $5,$6
NEG $5,$6
li $9,12
BLT $4,$3,256
