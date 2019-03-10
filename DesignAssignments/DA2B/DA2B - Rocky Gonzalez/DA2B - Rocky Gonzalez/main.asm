;
; DA2B - Rocky Gonzalez.asm
;
; Created: 3/9/2019 8:39:31 AM
; Author : rocky
;

.ORG 0				; Location for reset
	JMP Main		; Label Address for Main
.ORG 0x02			; Location for external interrupt 0
	JMP EX0_ISR		; Label Address for External Interrupt 0

Main:
	; Interrupt Related Initialization
	LDI R20, HIGH(RAMEND)	; Load Upper RAM end address
	OUT SPH, R20			; Initialize Upper Stack Pointer
	LDI R20, LOW(RAMEND)	; Load Lower Ram end address
	OUT SPL, R20			; Initialize Lower Stack Pointer
	LDI R20, 0x2			; Make 'INT0' falling edge triggered
	STS EICRA, R20			; External Interrupt Control Register A 'xxxx 0010'
	SBI PORTD, 2			; Activate pull-up in 'Interrupt 0' (PD2)
	SBI DDRB, 5				; Set PB5 as an Output (Interrupt LED)
	CBI PORTB, 5			; Set PB2 as 'High'
	LDI R20, (1<<INT0)		; Enable 'INT0'
	OUT EIMSK, R20			; Store into 'EIMSK'
	SEI						; Enable Global Interrupts
	; Main Function Initialization
	SBI DDRB, 2				; Set PB2 as an Output
	SBI PORTB, 2			; Set PB2 as 'High'
Repeat:
	CAll delay435ms	; Subroutine to Delay program 435ms
	CBI PORTB, 2	; Toggle LED 'Low' at PORTB.2
	CAll delay290ms	; Subroutine to Delay program 290ms
	SBI PORTB, 2	; Toggle LED 'High' at PORTB.2
	RJMP Repeat		; Repeat LED Squarewave Period (725ms)

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

delay1s:			; Delay Program '290ms'
	Push R16		; Store R16 into Stack
	Push R17		; Store R17 into Stack
	Push R18		; Store R18 into Stack
	LDI R18, 32		;
N3:	LDI R17, 200	; 32
N2:	LDI R16, 250	; 200*32
N1:	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	NOP				; 250*200*32
	DEC R16			; 250*200*32
	BRNE N1			; 250*200*32
	DEC R17			; 200*32
	BRNE N2			; 200*32
	DEC R18			; 32
	BRNE N3			; 32
	Pop R18			; Restore R18 from Stack
	Pop R17			; Restore R17 from Stack
	Pop R16			; Restore R16 from Stack
	RET				; Complete Subroutine

EX0_ISR:
	IN R21, PORTB	; Read in Value of PB5 to R21
	LDI R22, (1<<5)	; '0010 0000' for toggling PB5
	EOR R21, R22	; XOR Current Status of Interrupt LED
	OUT PORTB, R21	; Output Interrupt LED
	CAll delay1s	; Subroutine to Delay program 1000ms
RETI				; Return to Original Placement in Code
