global wordsNasm

left_shift:
    push rbx
    cmp rbx, 16
    jl left_shift_8
    pslldq xmm0, 16
    sub rbx, 16
left_shift_8:
    cmp rbx, 8
    jl left_shift_4
    pslldq xmm0, 8
    sub rbx, 8
left_shift_4:
    cmp rbx, 4
    jl left_shift_2
    pslldq xmm0, 4
    sub rbx, 4
left_shift_2:
    cmp rbx, 2
    jl left_shift_1
    pslldq xmm0, 2
    sub rbx, 2
left_shift_1:
    cmp rbx, 1
    jl left_shift_0
    pslldq xmm0, 1
    sub rbx, 1
left_shift_0:
    pop rbx
    ret

right_shift:
    push rbx
    cmp rbx, 16
    jl right_shift_8
    psrldq xmm0, 16
    sub rbx, 16
right_shift_8:
    cmp rbx, 8
    jl right_shift_4
    psrldq xmm0, 8
    sub rbx, 8
right_shift_4:
    cmp rbx, 4
    jl right_shift_2
    psrldq xmm0, 4
    sub rbx, 4
right_shift_2:
    cmp rbx, 2
    jl right_shift_1
    psrldq xmm0, 2
    sub rbx, 2
right_shift_1:
    cmp rbx, 1
    jl right_shift_0
    psrldq xmm0, 1
    sub rbx, 1
right_shift_0:
    pop rbx
    ret


; rdi - data
; rsi - length
; rax - answer

wordsNasm:
    push rbx

    call precycle
    call cycle
    call postcycle

    pop rbx
    ret


; inverts xmm0
; use xmm7

invert_bits:
    pcmpeqb xmm7, xmm7
    xorpd xmm0, xmm7
    ret

invert_bits2:
    pcmpeqb xmm7, xmm7
    xorpd xmm2, xmm7
    ret


read_xmm0:
    movdqu xmm0, [rdi]
    ret


; add to rax number of 1's bytes in xmm0

add_bits:
    push rbx
    push rcx

    pmovmskb ebx, xmm0
    popcnt ecx, ebx
    add rax, rcx

    pop rcx
    pop rbx
    ret

precycle:
    push rdi

    xor rax, rax

    and rdi, 15
    mov rbx, 16
    sub rbx, rdi
    and rbx, 15

    mov rdi, rbx
    mov rbx, 16
    sub rbx, rdi

    xorpd xmm0, xmm0
    xorpd xmm5, xmm5

    mov rdi, 0x2020202020202020
    movq xmm5, rdi
    movdqa xmm0, xmm5
    pslldq xmm5, 8
    xorpd xmm5, xmm0

    pop rdi

    pcmpeqb xmm1, xmm1

    call read_xmm0
    pcmpeqb xmm0, xmm5

    call invert_bits
    call left_shift
    call invert_bits

    add rdi, 16
    sub rdi, rbx
    add rsi, rbx
    sub rsi, 16

    ret

process:
    movdqa xmm2, xmm0
    palignr xmm0, xmm1, 15
    movdqa xmm1, xmm2
    call invert_bits2
    andpd xmm0, xmm2
    call add_bits
    ret

cycle:
    call process

    cmp rsi, 16
    jl end_cycle

    movntdqa xmm0, [rdi]
    pcmpeqb xmm0, xmm5

    add rdi, 16
    sub rsi, 16
    jmp cycle

end_cycle:
    ret

postcycle:
    add rdi, rsi
    sub rdi, 16
    call read_xmm0
    pcmpeqb xmm0, xmm5

    call invert_bits
    mov rbx, 16
    sub rbx, rsi
    call right_shift
    call invert_bits

    call process
    ret
