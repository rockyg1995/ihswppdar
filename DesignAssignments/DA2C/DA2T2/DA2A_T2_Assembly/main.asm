;
; DA2A_T2_Assembly.asm
;
; Created: 3/2/2019 9:11:02 AM
; Author: rocky
;

.ORG 0

	LDI R16, (1<<2)	; Load '0000 0100' into R16
	LDI R17, (0<<2) ; Load 'xxxx x0xx' into R17
	CLR R18
	OUT DDRB, R16	; Set PB2 as an Output
	OUT PORTB, R17	; Set PB2 as 'Low'
	OUT DDRC, R17	; Set PC2 as an Input
	Out PORTC, R16	; Activate Pull-up in PC2 (resistor connected to VCC)

check:				; Poll: Check if PC2 is 'High' or 'Low'
	IN R18, PINC	; Read in PC2 to 'R18'
	ANDI R18, 0x04	; Mask Out all bits except bit 2
	BRNE check		; If Bit 2 is 'Low', continue reading PC2
	SBI PORTB, 2	; Else, Toggle LED 'High' at PB2
	CAll delay1250ms; Subroutine to Delay program 1250ms
	CBI PORTB, 2	; Toggle LED 'Low' at PB2
	RJMP check		; Go back to Poll

delay1250ms:		; Delay Program '290ms'
	Push R16		; Store R16 into Stack
	Push R17		; Store R17 into Stack
	Push R18		; Store R18 into Stack
	LDI R18, 32		;  
L3:	LDI R17, 250	; 32
L2:	LDI R16, 250	; 250*32
L1:	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	NOP				; 250*250*32
	DEC R16			; 250*250*32
	BRNE L1			; 250*250*32
	DEC R17			; 250*32
	BRNE L2			; 250*32
	DEC R18			; 32
	BRNE L3			; 32
	Pop R18			; Restore R18 from Stack
	Pop R17			; Restore R17 from Stack
	Pop R16			; Restore R16 from Stack
	RET				; Complete Subroutine
