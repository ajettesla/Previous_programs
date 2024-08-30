.data
	format: .asciz  "%d\n"

.text
.global fact
.global gcd
.global fact_end
.global	zero
.global gcd_label1
.global gcd_label2
.global two_label1
.global gcd_loop


fact:
	cmp 	$1,	%rdi
	jl	fact_end
	pushq	%rdi
	pushq	$1
	popq	%rbx
	sub	%rbx,	%rdi
	call	fact
	popq	%rdi
	imulq	%rdi,	%rbp
	ret
fact_end:
	movq    $1,	%rbp
    ret
	
gcd:
	cmp	%rdi,	%rsi
	je	gcd_loop
	cmp	%rsi,	%rdi
	jg	gcd_label1
	sub	%rdi,	%rsi
	jmp	gcd_label2
gcd_label1:
	sub	%rsi,	%rdi
gcd_label2:
	jmp	gcd
gcd_loop:
	pushq %rdi
	popq  %rbp
	ret


two:
	cmp 	$1,	%rdi
	jle	two_label1
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
	call 	two_label1
	add	$1,	%rdi
	ret

two_label1:
	movq    $0,	%rdi
    ret
