main:
addi $t0, $zero, 0 //initialize to & give space for the clock
keyboardcheck:
lw $t0, 65280($zero) //check the status for keyboardControl
beq $t0, $zero, updatepos
lw $s0, 65281($zero) //0xFF01 - data in keyboard loaded to $s0
sw $zero, 65280($zero) //finished reading, run the clock at Keyboard
addi $t0, $zero, 119
beq $s0, $t0, speedyminus //s1 is speedx, s2 is speedy 
addi $t0, $zero, 115
beq $s0, $t0, speedyplus
addi $t0, $zero, 97
beq $s0, $t0, speedxminus
addi $t0, $zero, 100
beq $s0, $t0, speedxplus
updatepos:
addi $t1, $s3, 0 //store s3, s4 for turning black
addi $t2, $s4, 0 
add $s3, $s3, $s1 //update pos with current speed
add $s4, $s4, $s2 //s3 is posx, s4 is posy
lw $t3, 65296($zero) //check if RGB monitor is ready for input
beq $t3, $zero, oldposblack
oldposblack:
sw $t1, 65297($zero) //input x-coordinate
sw $t2, 65298($zero) //input y-coordinate
addi $t9, $zero, 0 //store rgb black
sw $t9, 65299($zero)
addi $t4, $zero, 1
sw $t4, 65296($zero)
addi $t0, $zero, 1 //wait one instruction
lw $t3, 65296($zero)
beq $t3, $zero, newposblue
newposblue:
sw $s3, 65297($zero)
sw $s4, 65298($zero)
addi $t9, $zero, 255 //new pos blue
sw $t9, 65299($zero) //store blue into color register in video control
sw $t4, 65296($zero) //store 1 to RGB status, being ready to draw
addi $t0, $zero, 128 
beq $s3, $t0, success1 //if posx statisfy target position, go to success1
notyetsuccess:
addi $t9, $zero, 256
addi $t0, $zero, 1
slt $t1, $s3, $zero //posx < 0 then t1=1 out of bound
beq $t1, $t0, failed //if posx went out of bound, fail
slt $t2, $t9, $s3 //posx>256 then t1=1 out of bound
beq $t2, $t0, failed //if posx went out of bound, fail
slt $t3, $s4, $zero
beq $t3, $t0, failed //if posx went out of bound, fail
slt $t4, $t9, $s4
beq $t4, $t0, failed //same logic for posy
j main //loop back
failed: //Turn whole screen red
addi $t0, $zero, -1
sw $t0, 65299($zero)
j end
success1:
addi $t0, $zero, 128
beq $s4, $t0, success2
j notyetsuccess
success2:
addi $t0, $zero, -1
sw $t0, 65299($zero) //GAME WON if pox also statisfy
j end
speedyminus:
addi $s2, $s2, -1
j keyboardcheck
speedyplus:
addi $s2, $s2, 1
j keyboardcheck
speedxminus:
addi $s1, $s1, -1
j keyboardcheck
speedxplus:
addi $s1, $s1, 1
j keyboardcheck
end:
