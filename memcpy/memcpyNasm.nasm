global memcpyNasm

; rdi - destination
; rsi - source
; rdx - number of bytes

memcpyNasm:
     push rax
     push rbx

     call precopy

     push rdx
     mov rbx, rdi
     call get_bytes_to_align
     mov rdx, rax
     mov rbx, rsi
     call get_bytes_to_align
     mov rbx, rdx
     pop rdx

     sub rdx, rax  ; important!
     cmp rax, rbx
     jg source_margin_greater
     jmp source_margin_less_or_equal
after_margin_comparision:

     pop rbx
     pop rax
     ret


; rbx - address
; rax - number of bytes to align

get_bytes_to_align:
    and rbx, 15
    mov rax, 16
    sub rax, rbx
    and rax, 15
    ret


; rax - source margin
; rbx - destination margin

source_margin_greater:
    push rax
    mov rax, [rsi + 16]
    mov [rdi + 16], rax

    mov rax, [rsi + 24]
    mov [rdi + 24], rax
    pop rax

    add rdi, rbx
    add rdi, 16
    add rsi, rax
    add rbx, 16
    sub rbx, rax
    mov rax, rbx

    call precycle

    jmp after_margin_comparision


; rax - source margin
; rbx - destination margin

source_margin_less_or_equal:
    add rdi, rbx
    add rsi, rax

    sub rbx, rax
    mov rax, rbx

    call precycle

    jmp after_margin_comparision


; rax - shift for pair to left
; rsi - source
; rdi - destination
; rdx - rest of size

precycle:
    movdqa xmm0, [rsi]
    sub rdx, 16
    add rsi, 16
    call choose_function
    ret


; rdi - destination
; rsi - source
; rdx - size

precopy:
    push rax

    mov rax, [rsi]
    mov [rdi], rax

    mov rax, [rsi + 8]
    mov [rdi + 8], rax

    push rsi
    push rdi

    add rsi, rdx
    add rdi, rdx
    sub rsi, 8
    sub rdi, 8

    mov rax, [rsi]
    mov [rdi], rax

    mov rax, [rsi - 8]
    mov [rdi - 8], rax

    mov rax, [rsi - 16]
    mov [rdi - 16], rax

    mov rax, [rsi - 24]
    mov [rdi - 24], rax

    pop rdi
    pop rsi

    pop rax
    ret


choose_function:
    cmp rax, 7
    jle choose_function_0_7
    jmp choose_function_8_15
exit:
    ret

choose_function_0_7:
    cmp rax, 3
    jle choose_function_0_3
    jmp choose_function_4_7

choose_function_8_15:
    cmp rax, 11
    jle choose_function_8_11
    jmp choose_function_12_15

choose_function_0_3:
    cmp rax, 1
    jle choose_function_0_1
    jmp choose_function_2_3

choose_function_4_7:
    cmp rax, 5
    jle choose_function_4_5
    jmp choose_function_6_7

choose_function_8_11:
    cmp rax, 9
    jle choose_function_8_9
    jmp choose_function_10_11

choose_function_12_15:
    cmp rax, 13
    jle choose_function_12_13
    jmp choose_function_14_15

choose_function_0_1:
    cmp rax, 0
    je cycle_0
    jmp cycle_1

choose_function_2_3:
    cmp rax, 2
    je cycle_2
    jmp cycle_3

choose_function_4_5:
    cmp rax, 4
    je cycle_4
    jmp cycle_5

choose_function_6_7:
    cmp rax, 6
    je cycle_6
    jmp cycle_7

choose_function_8_9:
    cmp rax, 8
    je cycle_8
    jmp cycle_9

choose_function_10_11:
    cmp rax, 10
    je cycle_10
    jmp cycle_11

choose_function_12_13:
    cmp rax, 12
    je cycle_12
    jmp cycle_13

choose_function_14_15:
    cmp rax, 14
    je cycle_14
    jmp cycle_15


; rdi - destination
; rsi - source
; rdx - size
; xmm0 - first half

cycle_0:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 0
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_0

cycle_1:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 1
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_1

cycle_2:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 2
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_2

cycle_3:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 3
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_3

cycle_4:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 4
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_4

cycle_5:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 5
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_5

cycle_6:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 6
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_6

cycle_7:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 7
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_7

cycle_8:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 8
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_8

cycle_9:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 9
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_9

cycle_10:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 10
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_10

cycle_11:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 11
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_11

cycle_12:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 12
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_12

cycle_13:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 13
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_13

cycle_14:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 14
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_14

cycle_15:
    cmp rdx, 16
    jl exit

    movntdqa xmm1, [rsi]
    movdqa xmm2, xmm1

    palignr xmm1, xmm0, 15
    movdqa [rdi], xmm1

    movdqa xmm0, xmm2

    sub rdx, 16
    add rsi, 16
    add rdi, 16

    jmp cycle_15
