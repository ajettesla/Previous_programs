 
.data
	format: .asciz  "%d\n"
.bss
	a:  .quad   0
	b:  .quad   0
	c:  .quad   0
	d:  .quad   0
	e:  .quad   0
	f:  .quad   0
	g:  .quad   0
	h:  .quad   0
	i:  .quad   0
	j:  .quad   0
	k:  .quad   0
	l:  .quad   0
	m:  .quad   0
	n:  .quad   0
	o:  .quad   0
	p:  .quad   0
	q:  .quad   0
	r:  .quad   0
	s:  .quad   0
	t:  .quad   0
	u:  .quad   0
	v:  .quad   0
	w:  .quad   0
	x:  .quad   0
	y:  .quad   0
	z:  .quad   0
.text
.globl main            


	
main:
	pushq	$732
	popq	a
	pushq	$2684
	popq	b
L000:
	pushq	a
	pushq	b
	popq	%rbx
	popq	%rax
	cmp	%rax,	%rbx
	je	L001
	pushq	a
	pushq	b
	popq	%rbx
	popq	%rax
	cmp	%rax,	%rbx
	jge	L002
	pushq	a
	pushq	b
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	popq	a
	jmp	L003
L002:
	pushq	b
	pushq	a
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	popq	b
L003:
	jmp	L000
L001:
	pushq	a
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	a
	pushq	b
	popq	%rdi
	popq	%rsi
	call	gcd
	pushq	%rbp
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	movq    $1,      %rdi   
        call    exit 
