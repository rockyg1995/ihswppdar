;
; DA1A - Rocky Gonzalez.asm
;
; Created: 2/5/2019 6:54:39 PM
; Author : rocky
; Summary: The following program executes the function for a Multiplication
; by utilizing a 16-bit Multiplicand and 8-bit Multiplicand, then storing the
; product into 3 registers (24-bit Product) 

.include <m328pdef.inc>	; Include library for .SET and .ORG directives

.SET	MULT1l = 0xff	; Set value for Lower 16-bit Multiplicand
.SET	MULT1u = 0xff	; Set value for Upper 16-bit Multiplicand
.SET	MULT2  = 0xff	; Set value for 8-bit Multiplicand

.ORG 0					; Start Data Collecting from the Origin '0x00'

		; Register Assignments
		ldi r24, MULT1l	; Load in value for Lower 16-bit 1st Multiplicand
		ldi r25, MULT1u	; Load in value for Upper 16-bit 1st Multiplicand
		ldi r22, MULT2	; Load in value for 8-bit 2nd Multiplicand
		push r22		; Save 8-bit Multiplicand Value into Stack
		push r24		; Save 16-bit Lower Multiplicand Value into Stack
		push r25		; Save 16-bit Upper Multiplicand Value into Stack
		ldi r17, 0x01	; Load in value representing increment (For Carry SReg)
		clr r18			; Clear bits in Lower  24-bit Product
		clr r19			; Clear bits in Middle 24-bit Product
		clr r20			; Clear bits in Upper  24-bit Product

		; ------------------------------------------------------------------------------------
		; Check to see if Iterative Addition occurs
chckif:	subi r24, 0		; Check if 'R24 > 0'
		breq else		; Go to 'else' if 'R24 == 0'
		rjmp repeat		; Execute Iterative Addition Loop

else:	subi r25, 0		; Check if 'R25 > 0'
		breq end		; End if '1st Multiplicand' itration is complete (R25 == 0 and R24 == 0)
		dec r25			; Otherwise Decrement 'R25'
		rjmp repeat		; Execute Iterative Addition Loop

		; ------------------------------------------------------------------------------------
		; Iterative Addition Loop
repeat:	add r18, r22	; Iterate Adding '2nd Multiplicand' by '1st Multiplicand' times
		brcs prod1		; If Overflow in R18, increment value into R19
cont:	dec r24			; Decrement R24
		brne repeat		; If 'R24 > 0', repeat Iterative Addition
		rjmp chckif		; Otherwise, Check to see if '1st Multiplicand' iteration is complete

		; ------------------------------------------------------------------------------------
		; Incrementing Middle 24-bit Product
prod1:	clc				; Clear Carry in Status Register
		add r19, r17	; Increment Middle 24-bit Product (R19)
		brcs prod2		; If Overflow is set, Increment Upper 24-bit Product 
		rjmp cont		; Continue Original Loop

		; Incrementing Upper 24-bit Product
prod2:	clc				; Clear Carry in Status Register
		add r20, r17	; Increment Upper 24-bit Product (R20)
		rjmp cont		; Continue Original Loop

		; ------------------------------------------------------------------------------------
end:	pop r25			; Restore 16-bit Upper Multiplicand Value into Stack
		pop r24			; Restore 16-bit Lower Multiplicand Value into Stack
		pop r25			; Restore 8-bit Multiplicand Value from Stack / Stack Empty
endf:	rjmp endf		; Loop End of Program