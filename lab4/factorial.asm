#	input=a0(#4) output =v0 (#2)

addi $a0, $a0, 5 # input N
addi $a1, $a0, -1 # counter F
addi $t0, $0, 1 # constant 1
add $a3, $0, $a1 # counter max
beq $0, $a0, special0
bigloop:
		beq $0, $0, mult2 #dont run first time
		multiply:
		add $a3, $0, $a1 # counter max
		addi $a3, $a3, -1
		mult2:
		beq $0, $a3, stopmultiply #stop when a3 counted down to 0
		add $v0, $v0, $a0
		addi $a3, $a3, -1
		beq $0, $0, mult2
		add $0, $0, $0 #nop
	stopmultiply:
		addi $a1, $a1, -1
		beq $a1, $0, stop
		add $a0, $0, $v0
		beq $0, $0, multiply
	
	#F=F*N


stop:
	beq $0, $0, stop
	add $t1, $0, $0
	
special0:
	addi $v0, $0, 1
	beq $0, $0, stop
