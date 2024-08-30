.bss
	data resb 100
	datapos resb 8
	a:	.quad 0
	b:	.quad 0
	c:	.quad 0
	d:	.quad 0
	e:	.quad 0
	f:	.quad 0
	g:	.quad 0
	h:	.quad 0
	i:	.quad 0
	j:	.quad 0
	k:	.quad 0
	l:	.quad 0
	m:	.quad 0
	n:	.quad 0
	o:	.quad 0
	p:	.quad 0
	q:	.quad 0
	r:	.quad 0
	s:	.quad 0
	t:	.quad 0
	u:	.quad 0
	v:	.quad 0
	w:	.quad 0
	x:	.quad 0
	y:	.quad 0
	z:	.quad 0
.text
	global _start
_start:
	push	732
	pop	a
	push	2684
	pop	b
L000:
	push	a
	push	b
	pop	 rbx
	pop	 rax
	cmp	 rax  rbx
	jne
	jz	L001
	push	a
	push	b
	pop	 rbx
	pop	 rax
	cmp	 rax rbx
	jg
	jz	L002
	push	a
	push	b
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop	a
	jmp	L003
L002:
	push	b
	push	a
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop	b
L003:
	jmp	L000
L001:
	push	a
	pop rax
	call printdigit
	push	a
	push	b
	pop rbx
	pop rax
	call gcd
	pop rax
	call printdigit
	mov rax, 60
	mov rdi, 0
	syscall
