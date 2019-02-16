;
; DA1A - Verification - Rocky Gonzalez.asm
;
; Created: 2/10/2019 5:11:38 PM
; Author : rocky
;
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
		clr r18			; Clear bits in Lower  24-bit Product
		clr r19			; Clear bits in Middle 24-bit Product
		clr r20			; Clear bits in Upper  24-bit Product

		; ------------------------------------------------------------------------------------
		; Utilize Multiplication Instruction and Store Value Into 24-bit Product 'R20:R19:R18'
		mul r24, r22	; Multiply Lower 16-bit Multiplicand with 8-bit Multiplicand -> R1:R0
		add r18, r0		; Store the lower data into the Lower  24-bit Product
		add r19, r1		; Store the upper data into the Middle 24-bit Product

		mul r25, r22	; Multiply Upper 16-bit Multiplicand with 8-bit Mulitplicand -> R1:R0
		add r19, r0		; Add the lower data into the Middle 24-bit Product
		brcs addc		; If 'Carry' in SREG set, Branch to add 'Carry' appropriately
		add r20, r1		; Otherwise, Add upper data into Upper 24-bit Product Without 'Carry'
		rjmp end		; Finish Program
addc:	adc r20, r1		; Add upper data into the Upper 24-bit Product With the 'Carry'

		; ------------------------------------------------------------------------------------
end:	rjmp end			; Loop End of Program