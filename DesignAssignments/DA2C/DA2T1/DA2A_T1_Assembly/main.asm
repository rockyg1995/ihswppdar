;
; DA2A_T1_Assembly.asm
;
; Created: 3/1/2019 10:08:49 PM
; Author: rocky
;

.ORG 0

	LDI R16, (1<<2)	; Load '0000 0100' into R16
	OUT DDRB, R16	; Set PB2 as an Output
	OUT PORTB, R16	; Set PB2 as 'High'

main:				; Loop LED Toggle, Period of 735ms (60% High, 40% Low)
	CAll delay435ms	; Subroutine to Delay program 435ms
	CBI PORTB, 2	; Toggle LED 'Low' at PORTB.2
	CAll delay290ms	; Subroutine to Delay program 290ms
	SBI PORTB, 2	; Toggle LED 'High' at PORTB.2
	RJMP main		; Repeat LED Squarewave Period (725ms)

delay435ms:			; Delay Program '290ms'
	Push R16		; Store R16 into Stack
	Push R17		; Store R17 into Stack
	Push R18		; Store R18 into Stack
	LDI R18, 15		;  
L3:	LDI R17, 200	; 15
L2:	LDI R16, 232	; 200*15
L1:	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	NOP				; 232*200*15
	DEC R16			; 232*200*15
	BRNE L1			; 232*200*15
	DEC R17			; 200*15
	BRNE L2			; 200*15
	DEC R18			; 15
	BRNE L3			; 15
	Pop R18			; Restore R18 from Stack
	Pop R17			; Restore R17 from Stack
	Pop R16			; Restore R16 from Stack
	RET				; Complete Subroutine

delay290ms:			; Delay Program '290ms'
	Push R16		; Store R16 into Stack
	Push R17		; Store R17 into Stack
	Push R18		; Store R18 into Stack
	LDI R18, 10		;  
M3:	LDI R17, 200	; 10 
M2:	LDI R16, 232	; 200*10
M1:	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	NOP				; 232*200*10
	DEC R16			; 232*200*10
	BRNE M1			; 232*200*10
	DEC R17			; 200*10
	BRNE M2			; 200*10
	DEC R18			; 10
	BRNE M3			; 10
	Pop R18			; Else, Restore R18 from Stack
	Pop R17			; Restore R17 from Stack
	Pop R16			; Restore R16 from Stack
	RET				; Complete Subroutine
