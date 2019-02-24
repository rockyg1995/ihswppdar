;
; DA1B - Rocky Gonzalez.asm
;
; Created: 2/17/2019 8:17:42 PM
; Author : rocky
;

.include <m328pdef.inc>			; Include library for .SET and .ORG directives

.SET STARTADDS = 0x0200			; Create 'STARTADDS' to begin at address '0x0200'
.SET DIV = 3					; Create 'DIV' to be a value of '3'

		LDI R23, 99				; Store the value '99' into Counter (register R19)
		LDI XL, LOW(STARTADDS)	; Store address in Lower X-pointer
		LDI XH, HIGH(STARTADDS)	; Store address in Upper X-pointer
		LDI YL, LOW(0x0400)		; Store address in Lower Y-pointer
		LDI YH, HIGH(0x0400)	; Store address in Upper Y-pointer
		LDI ZL, LOW(0x0600)		; Store address in Lower Z-pointer
		LDI ZH, HIGH(0x0600)	; Store address in Upper Z-pointer
		LDI R20, 0x07			; Arbitrary value, consecutive addition executed
		MOV R21, R20			; Retain Arbitrary Value (Since 'R16' will Change)

; ------------------------------------------------------------------------------------
; Loop to Store '7, 14, ..., n' Values in Data Starting at Address 0x0200 (Using X-Pointer) 
Cont1:	CPI R20, 11		; Check if 'R20 > 10'
		BRLO L1			; If 'R20 <= 10', Add '7' More to 'R20'
		RJMP LoadY		; Load Value into Y or Z Address depending on Divisibility
Cont2:	ST  X+, R20		; Store 'R20' into 'X', then increment address location
		ADD R20, R21	; Increment 'R20' with itself (consecutive addition)
		DEC R23			; Else, Continue to Decrement Counter Value (register R23)
		BRNE Cont1		; Repeat 'Cont1' until all values are stored into address starting at 0x0200
		RJMP end		; End Program

L1:		ADD R20, R21	; Add '7' more so that 'R20 > 10'
		RJMP Cont1		; Recheck
; ------------------------------------------------------------------------------------
; Store Divisible Values by 3 into 'Y', and Non-Divisible Values into 'Z'
LoadY:	MOV R22, R20	; Store 'R20' into temporary register 'R22'
Repeat:	SUBI R22, DIV	; Subtract the value '3' from 'R22'
		CPI R22, DIV+1	; Check if 'R22 > 3', Set 'Carry' for SREG when 'R22 <= 3'
		BRSH Repeat		; If 'R22 > 3', Repeat subtracting '3'
		SUBI R22, DIV	; Otherwise, Subtract '3' Once More to Check if Divisible by '3'
		BRNE LoadZ		; If Not Divisible, Load Value into Z Address

		ST Y+, R20		; Else, Load Divisible Value into Y Address
		Add R16, R20	; Add value into R17:R16
		BRCS ext1		; If Overflow in R16, increment value into R17
		RJMP Cont2		; Continue back to Original Operation

LoadZ:	ST Z+, R20		; Load Non-Divisible Value into Z-Address
		Add R18, R20	; Add value into R19:R18
		BRCS ext2		; If Overflow in R18, increment value into R19
		RJMP Cont2		; Continue back to Original Operation
; ------------------------------------------------------------------------------------
; Incrementing R17:R16 (High Register)
ext1:	CLC				; Clear Carry in Status Register
		INC R17			; Increment R!7:R16 (High Register)
		RJMP Cont2		; Continue Original Loop
; ------------------------------------------------------------------------------------
; Incrementing R19:R18 (High Register)
ext2:	CLC				; Clear Carry in Status Register
		INC R19			; Increment R!9:R18 (High Register)
		RJMP Cont2		; Continue Original Loop
; ------------------------------------------------------------------------------------
; End of Program
end:	RJMP end		; End Program/Loop Forever

