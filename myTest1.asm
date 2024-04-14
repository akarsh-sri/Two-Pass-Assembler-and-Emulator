; NAME    : AKARSH SRIVASTAVA
; ROLL No : 2201CS10
; This program stores the larger out of two variables in the main memory.
; Declaration of Authorship
; This file, myTest1.asm, is part of the miniproject of CS210 at the
; Department of Computer Science and Engg, IIT Patna.

main: ldc 4
	ldc 2
	sub
	brlz else
if:	
    ldc 4
    ldc 0
    stnl 0
    HALT
else:
    ldc 2
    ldc 0
    stnl 0
	HALT