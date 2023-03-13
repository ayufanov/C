.text
    .globl f
    .type f, @function
    f:
        pushq %rdi
        pushq %rsi
        pushq %rbx

        movl (%rdi), %eax 
        movl (%rdi), %ebx

        cyc:
            check_if_greater_than_max:
            cmp (%rdi), %eax
            jg check_if_less_than_min
            movl (%rdi), %eax
            check_if_less_than_min:
            cmp (%rdi), %ebx
            jng check_end_loop
            movl (%rdi), %ebx
            check_end_loop:
            addq $4, %rdi
            dec %rsi
            cmp $0, %rsi
            jg cyc
        final:
        addl %ebx, %eax
        popq %rbx
        popq %rsi
        popq %rdi
        ret
