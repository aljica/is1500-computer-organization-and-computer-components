  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0, -17		# change this to test different values
	
	li	$t2, 0xf 	# for bit masking purposes (masking $a0)
	and	$a0, $t2, $a0   # bit mask, get only 4 least significant bits

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)
	
	li 	$t3, 0x7f	# for bit masking purposes (masking the result in $v0)
	and 	$v0, $t3, $v0	# get 7 least significant bits	

	li	$ra, 0		# resetting $ra to 0 (what it was before we call hexasc
	# altho perhaps we should copy the value in $ra in a $t-register, then do this reset INSIDE hexasc-function
	# and simply do the jr instruction (in hexasc) with the $t-register. read the lab spec.

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
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
