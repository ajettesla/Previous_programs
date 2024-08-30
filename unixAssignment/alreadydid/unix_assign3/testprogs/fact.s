 
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
	pushq	$0
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$1
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$1
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$1
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$2
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$2
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$3
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$6
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$4
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$24
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$5
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$120
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$6
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$720
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$7
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$5040
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$8
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$40320
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$9
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$362880
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$10
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$3628800
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	pushq	$11
	popq	%rdi
	call	fact
	pushq	%rbp
	pushq	$39916800
	popq	%rbx
	popq	%rax
	sub	%rbx,	%rax
	pushq	%rax
	movq	$format,	%rdi
	popq	%rsi
	pushq	$0
	call	printf
	popq	%rsi
	movq    $1,      %rdi   
        call    exit 
