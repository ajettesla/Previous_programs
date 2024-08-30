.data
data: .space 100
datapos: .space 8

.text
.globl _start
.globl gcd
.globl aisless
.globl gcddone
.globl tl
.globl lntwo

_start:
    movq $2684, %rax
    movq $732, %rbx
    call gcd
    call printdigit
    movq $60, %rax
    movq $0, %rdi
    syscall

print_text:
    movq $0, %rcx
    pushq %rax
textloop1:
    cmpb $0, (%rax)
    jg textloop2
    dec %rcx
    movq $1, %rax
    movq $1, %rdi
    popq %rsi
    movq %rcx, %rdx
    syscall
    ret
textloop2:
    inc %rcx
    inc %rax
    jmp textloop1

printdigit:
    pushq %rax
    pushq %rbx
    movq $data, %rcx
    movq $10, %rbx
    movb %bl, (%rcx)
    inc %rcx
    movq %rcx, datapos

printdigitloop1:
    movq $0, %rdx
    movq $10, %rbx
    divq %rbx
    pushq %rax
    add $48, %dl

    movq datapos, %rcx
    movb %dl, (%rcx)
    inc %rcx
    movq %rcx, datapos

    popq %rax
    cmp $0, %rax
    jne printdigitloop1

printdigitloop2:
    movq datapos, %rcx
    movq $1, %rax
    movq $1, %rdi
    movq %rcx, %rsi
    movq $1, %rdx
    syscall

    movq datapos, %rcx
    dec %rcx
    movq %rcx, datapos
    cmp $data, %rcx
    jge printdigitloop2
    popq %rbx
    popq %rax
    ret

gcd:
    cmp %rax, %rbx
    je gcddone
    jl aisless
    sub %rax, %rbx
    jmp tl
aisless:
    sub %rbx, %rax
tl:
    jmp gcd
gcddone:
    ret

fact:
    movq %rax, %rbx
factstart:
    cmp $0, %rax
    je endfact
    cmp $1, %rax
    je endfact
    sub $1, %rax
    imul %rax, %rbx
    jmp factstart
settoone:
    movq $1, %rbx
    jmp endfact
endfact:
    movq %rbx, %rax
    ret

lntwo:
    movq $0, %rcx
startln:
    cmp $1, %rax
    je endloopln
    movq $0, %rdx
    movq $2, %rbx
    divq %rbx
    inc %rcx
    jmp startln
endloopln:
    movq %rcx, %rax
    ret
