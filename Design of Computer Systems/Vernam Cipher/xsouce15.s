; Vernamova sifra na architekture DLX
; Tomáš Souèek xsouce15

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xsouce15"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 

main:   ; sem doplnte reseni Vernamovy sifry dle specifikace v zadani
	
	;xsouce15-r5-r7-r9-r21-r27-r0

	;S... 19 vpøed 
	;O... 15 vzad
	

	;r5.....pøíznakový registr
	;r7.....liché/sudé
	;r9.....index
	;r21....underflow/overflow
	;r27....bit se kterým pracuju
	
	;inicializace indexu a maximální délky (zarážky)
	addi r5, r0, 0
	
	addi r7, r0, 0
	addi r9, r0, 0 		
	;addi r21, r0, 5		
				
	Loop:
		nop
		nop

		seqi r5,r7, 1
		bnez r5, Liche

		seqi r5, r7, 0
		bnez r5, Sude

		

		
	Liche:
		nop
		nop
		
		lb r27, login(r9) ;naèti bit
		slti r5, r27, 97 ;break když narazí na èíslo...viz zadání
		bnez r5, Finish

		subi r27,r27, 15

		slti r5, r27, 97 ;když je menší než znak "a"
		bnez r5, Underflow

		sb cipher(r9), r27 ;ulož bit
		addi r9,r9,1 ;inkrementuj index
		addi r7, r0, 0 ;následnì chci jít do sudého èísla
		j Loop



	Sude:
		nop
		nop
	
		lb r27, login(r9) ;naèti bit
		slti r5, r27, 97 ;break když narazí na èíslo...viz zadání
		bnez r5, Finish
		
		addi r27,r27, 19

		sgti r5, r27, 122 ;když je vìtší než znak "z"
		bnez r5, Overflow

		sb cipher(r9), r27 ;ulož bit
		addi r9,r9,1 ;inkrementuj index
		addi r7, r0, 1 ;následnì chci jít do lichého èísla
		j Loop



	Underflow:
		;+26
		addi r27,r27, 26

		
		sb cipher(r9), r27 ;ulož bit
		addi r9,r9,1 ;inkrementuj index
		
		seqi r5, r7, 0 ;pokud se do underflow dostal ze sudého, pùjde následnì do lichého a naopak
		bnez r5, Liche

		seqi r5, r7, 1
		bnez r5, Sude



	Overflow:
		;-26
		subi r27,r27, 26

		
		sb cipher(r9), r27 ;ulož bit
		addi r9,r9,1 ;inkrementuj index
		
		seqi r5, r7, 0 ;pokud se do overflow dostal ze sudého, pùjde následnì do lichého a naopak
		bnez r5, Liche

		seqi r5, r7, 1
		bnez r5, Sude

	
	Finish:
		sb cipher(r9), r0


end:    addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace

