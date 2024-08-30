.data
	format: .asciz  "%d\n"
.text

.global fact
.global gcd
.global one
.global	zero
.global else
.global while
.global two
.global back


fact: 
	cmp 	$1,	%rdi
	jl	one
	pushq	%rdi
	pushq	$1
	popq	%rbx
	sub	%rbx,	%rdi 
	call	fact
	popq	%rdi
	imulq	%rdi,	%rbp
	ret
one:
	movq    $1,	%rbp                  
        ret


gcd:
	cmp	%rdi,	%rsi
	je	back
	cmp	%rsi,	%rdi
	jg	else
	sub	%rdi,	%rsi
	jmp	while
else:
	sub	%rsi,	%rdi
while:
	jmp	gcd
back:
	pushq %rdi
	popq  %rbp
	ret


two:
	cmp 	$1,	%rdi
	jle	zero
	pushq	%rdi
	pushq	$2
	xor	%rbx,	%rbx
	xor	%rdx,	%rdx
	xor	%rax,	%rax
	popq	%rbx
	popq	%rax
	idivq	%rbx
	pushq	%rax 
	popq	%rdi
	call 	two
	add	$1,	%rdi   
	ret    
	
zero:
	movq    $0,	%rdi 	          
        ret
