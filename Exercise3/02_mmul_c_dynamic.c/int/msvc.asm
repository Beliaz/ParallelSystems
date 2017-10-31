        test     r11, r11
        jle      SHORT $LN15@main
        mov      QWORD PTR [rsp+32], r14
        mov      rdi, r15
        mov      r14, r13
        mov      rsi, r11
        sub      r14, r15
        npad     10
$LL16@main:
        xor      r10d, r10d
        mov      rbx, r12
        npad     10
$LL19@main:
        xor      eax, eax
$LL22@main:
        mov      rcx, QWORD PTR [r14+rdi]
        mov      rdx, QWORD PTR [rbx]
        lea      r9, QWORD PTR [rcx+rax*4]
        mov      rcx, QWORD PTR [rdi]
        mov      r8d, DWORD PTR [rcx+r10*4]
        imul     r8d, DWORD PTR [rdx+rax*4]
        inc      rax
        add      DWORD PTR [r9], r8d
        cmp      rax, r11
        jl       SHORT $LL22@main
        inc      r10
        add      rbx, 8
        cmp      r10, r11
        jl       SHORT $LL19@main
        add      rdi, 8
        sub      rsi, 1
        jne      SHORT $LL16@main
        mov      r14, QWORD PTR [rsp+32]