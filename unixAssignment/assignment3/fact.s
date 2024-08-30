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
	push	0
	pop rax
	call fact
	push	1
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	1
	pop rax
	call fact
	push	1
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	2
	pop rax
	call fact
	push	2
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	3
	pop rax
	call fact
	push	6
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	4
	pop rax
	call fact
	push	24
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	5
	pop rax
	call fact
	push	120
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	6
	pop rax
	call fact
	push	720
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	7
	pop rax
	call fact
	push	5040
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	8
	pop rax
	call fact
	push	40320
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	9
	pop rax
	call fact
	push	362880
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	10
	pop rax
	call fact
	push	3628800
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	push	11
	pop rax
	call fact
	push	39916800
	pop	 rbx
	pop	 rax
	sub	 rax rbx
	push	 rax
	pop rax
	call printdigit
	mov rax, 60
	mov rdi, 0
	syscall
