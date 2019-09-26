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
	
	overten:
		addi $v0, $a0, 0x37
		jr $ra
		
delay:
	jr $ra
	nop

time2string:
	addi	$t0, $0, 0xf000 # 1111 0000 0000 0000 for bit masking purposes (get first NBCD digit)
	and	$t1, $t0, $a1 # perform bit mask on NBCD to extract first time digit
	PUSH($a0)	# saving the memory address, because we now have to call hexasc (which needs $a0 as input register)
	PUSH($t0)	# saving bit mask value so we can use it in the next iteration as well
	add	$a0, $t1, $0 # place first NBCD nibble into $a0 as input register prior to calling hexasc
	jal	hexasc # call hexasc
	nop	# delay slot filler
	POP($t0)	# restore bit mask value 0xf000
	POP($a0)	# restore memory address
	add	$t2, $v0, $0 # save return value of hexasc in $t2
	
	srl	$t0, $t0, 4 # 0000 1111 0000 0000, next bit mask
	and	$t1, $t0, $a1 # bit mask
	PUSH($a0)
	PUSH($t0)
	add	$a0, $t1, $0
	jal	hexasc
	nop
	POP($t0)
	POP($a0)
	add $t3, $v0, $0
	
	
	# We now have the first NBCD byte (most significant nibble in $t2, one less significant in $t3)
	add	$t4, $t3, $t2 # merging the two nibbles into a byte in one register. For simplifying store byte (sb) instruction
	sb	$t4, 0($a0) # offset is measured in bytes (yey!)
	
	# Now let's write a colon char
	add	$t5, $0, 0x3A # colon ":"
	sb	$t5, 1($a0)
	
	# Now let's do the same thing for the NBCD seconds
	
	srl	$t0, $t0, 4
	and	$t1, $t0, $a1
	PUSH($a0)
	PUSH($t0)
	add	$a0, $t1, $0
	jal	hexasc
	nop
	POP($t0)
	POP($a0)
	add	$t2, $v0, $0
	
	srl	$t0, $t0, 4
	and	$t1, $t0, $a1
	PUSH($a0)
	PUSH($t0)
	add	$a0, $t1, $0
	jal	hexasc
	nop
	POP($t0)
	POP($a0)
	add	$t3, $v0, $0
	
	add	$t4, $t3, $t2 # merge nibbles
	sb	$t5, 2($a0)
	
	sb	$0, 3($a0) # store end of string byte code