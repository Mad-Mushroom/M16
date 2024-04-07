nop
hlt
jmp otherRegStuff

aluStuff:
	add &a, 0xffff
	adr &a, &b
	dec &b, 0xffff
	der &c, &b

otherRegStuff:
	set &d, 0xffff

jmp otherRegStuff

jumpStuff:
	jmp jumpStuff
	jmr &a
	jmz aluStuff
	jmo otherRegStuff

compareStuff:
	cmp &a, 0xffff
	cmr &a, &c
	je jumpStuff

ioStuff:
	inb &a
	oub 0xffff
	our &b
	sin &c
	sou 0xffff
	sor &d

memStuff:
	; general purpose memory
	mem &a, 0xffff
	mer &a, &b
	lem &a, 0xffff
	ler &a, &b
	; video memory
	ver 0xffff, &a
	vrm &a, 0xffff
	vrr &a, &b

ramrom:
	xra 0x0001
	xro 0x0001

other:
	ret
