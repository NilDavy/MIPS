.text
ADD $t0,$t1,$t2
addi $t0,$t1,8
J 8
LW $t0,lunchtime
LW $t0,8($8)
BEQ $t0 , $t1 , byebye
