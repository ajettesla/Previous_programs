.data
	format: .asciz  "%d\n"


.text
.global lntwo
.global startln
.global endloopln
.global gcd
.global aisless
.global tl
.global gcddone
.global printdigit
.global printdigitloop1
.global printdigitloop2
.global fact
.global factstart
.global fact_end
.global settoone



lntwo:
    mov rcx, 0
startln:
    cmp rax, $1
    je endloopln
    mov rdx, 0
    mov rbx, $2
    idiv rbx
    inc rcx
    jmp startln
endloopln:
    mov rax, rcx
    ret

gcd:
    cmp rax, rbx
    je gcddone
    jl aisless
    sub rax,rbx
    jmp tl
aisless:
    sub rbx, rax
tl:
    jmp gcd
gcddone:
    ret

printdigit:
    push rax
    push rbx
    mov rcx, data
    mov rbx, 10
    mov [rcx], rbx
    inc rcx
    mov [datapos], rcx
    
printdigitloop1:
    mov rdx, 0
    mov rbx, 10
    div rbx
    push rax
    add rdx, 48

    mov rcx,[datapos]
    mov [rcx], dl
    inc rcx
    mov [datapos], rcx

    pop rax
    cmp rax, 0
    jne printdigitloop1

printdigitloop2:

    mov rcx,[datapos]
    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [datapos]
    dec rcx
    mov [datapos], rcx
    cmp rcx, data
    jge printdigitloop2
    pop rbx
    pop rax
    ret


fact:
    mov rbx, rax
factstart:
    cmp rax, $0
    je endfact
    cmp rax, $1
    je endfact
    sub rax, 1
    imul rbx, rax
    jmp factstart
settoone:
    mov rbx, 1
    jmp endfact
endfact:
    mov rax, rbx
    ret