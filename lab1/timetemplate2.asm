  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #

hexasc:
	li $t0, 9 # load 9 into reg t0
	slt $t1, $t0, $a0 # if value in t0 < value in a0, t1 is 1. otherwise, t1 is 0.
	bne $t1, $zero, overten # if value in t1 is not 0, branch to overten.
	# so if a0 has value 5, then line 26 makes t1 contain a 0. so then no branching to overten occurs.
	addi $v0, $a0, 0x30
	jr $ra
	nop
	
	overten:
		addi $v0, $a0, 0x37
		jr $ra
		nop
		
delay:
	PUSH($ra)
	PUSH($s0) # use s0 to store input parameter
	move	$s0, $a0 # store input param in s0. ms = $s0
	
	# prep for-loop
	PUSH($s1) # we will store our for-loop variable i in $s1, so save $s1
	PUSH($s2) # 4711 will be stored here
	
	while:
		beq	$s0, $0, done # while ms != 0
		nop
		addi	$s0, $s0, -1 # ms = ms - 1
		add	$s1, $0, $0 # i = 0
		addi	$s2, $0, 4400 # $s2 = 4711 [i have since changed this to $s2 = 1. Why 4711? ms is millisecond delay?]
		
		# now we need a for loop
		for:
			beq	$s1, $s2, while # while i != 4711, do the below. if i == 4711, go to while again.
			nop
			addi	$s1, $s1, 1 # i += 1
			j	for # repeat for
			nop
	done:
		POP($s2)
		POP($s1)
		POP($s0)
		POP($ra)
		jr $ra
		nop

time2string:
	PUSH($ra) # save return address, because it will be changed
	PUSH($s1) # save register s1 because we will be using it for storing the memory address where we put the NBCD ASCII chars
	PUSH($s0) # save reg s0 because we will be using it for storing the NBCD byte
	move	$s1, $a0 # save mem. address in $s1
	move	$s0, $a1 # save NBCD byte in $s0
	
	andi	$t0, $s0, 0xf000 # perform bit mask on NBCD to extract first minute digit
	#PUSH($s2) # ta bort parentes senare i labwork.S
	#add	$s2, $t0, $0
	#addi	$t5, $0, 57
	#beq	$t5, $s2, fixnine
	#fixnine:
	#	
	srl	$a0, $t0, 12 # make the digit be in the first 4 bits (least significant nibble) because that's what hexasc can handle
	jal	hexasc # call hexasc. in previous line, we placed our input parameter in $a0 directly.
	nop
	move	$t1, $v0 # move return value from hexasc (found in $v0) to $t1. This is the ASCII representation of first digit
	sb	$t1, 0($s1) # store the ASCII char in the given mem. address
	
	andi	$t0, $s0, 0xf00 # perform bit mask on NBCD to extract first minute digit
	srl	$a0, $t0, 8 # make the digit be in the first 4 bits (least significant nibble) because that's what hexasc can handle
	jal	hexasc # call hexasc. in previous line, we placed our input parameter in $a0 directly.
	nop
	move	$t1, $v0 # move return value from hexasc (found in $v0) to $t1. This is the ASCII representation of first digit
	sb	$t1, 1($s1) # store the ASCII char in the given mem. address

	# Now let's write a colon char
	addi	$t5, $0, 0x3a # colon ":"
	sb	$t5, 2($s1)
	
	andi	$t0, $s0, 0xf0 # perform bit mask on NBCD to extract first minute digit
	srl	$a0, $t0, 4 # make the digit be in the first 4 bits (least significant nibble) because that's what hexasc can handle
	jal	hexasc # call hexasc. in previous line, we placed our input parameter in $a0 directly.
	nop
	move	$t1, $v0 # move return value from hexasc (found in $v0) to $t1. This is the ASCII representation of first digit
	sb	$t1, 3($s1) # store the ASCII char in the given mem. address
	
	andi	$t0, $s0, 0xf # perform bit mask on NBCD to extract first minute digit
	srl	$a0, $t0, 0 # no need for shift here, because last second digit is in least significant nibble
	jal	hexasc # call hexasc. in previous line, we placed our input parameter in $a0 directly.
	nop
	
	move	$t1, $v0 # move return value from hexasc (found in $v0) to $t1. This is the ASCII representation of first digit
	#move $s2, $v0
	addi $t5, $0, 57
	# PUSH $ra BEFORE CALL TO FIXNINE?
	sb $0, 5($s1)
	sb	$t1, 4($s1) # store the ASCII char in the given mem. address
	bne $t1, $t5, skip
		
	addi $t4, $0, 78
	addi $t5, $0, 73
	addi $t6, $0, 69
	sb $t4, 4($s1)
	sb $t5, 5($s1)
	sb $t4, 6($s1)
	sb $t6, 7($s1)
	sb $0, 8($s1)
	
	skip:

	POP($s0)
	POP($s1)
	POP($ra)
	
	jr $ra
	nop
