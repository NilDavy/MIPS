.set noreorder

.bss
.space 12
lunch :

.text
add :
add $1, $2, $3
J add

.data
.byte -2, 0x4
lunch : .word 12
.asciiz "test \" restest"
.space 1

.bss
lunch : .space 0x12
